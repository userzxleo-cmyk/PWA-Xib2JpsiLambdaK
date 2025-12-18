#
# Makefile for programs that include ROOT and RooFit.
#
ROOTCFLAGS   := $(shell ${ROOTSYS}/bin/root-config --cflags)
ROOTLIBS     := $(shell ${ROOTSYS}/bin/root-config --libs)
CWD          := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))
HEADER_DIR   := "\"$(CWD)/RooTDalitz\""
MYLIB_DIR    := "\"$(CWD)/bin\""

MODE=GPU
iDv=$(iDv)
nDv=1
NLL_SHIFT=10000000

CXXFLAGS      = -O3 -Wall -fPIC -W

CXXFLAGS     += -I. $(ROOTCFLAGS) -std=c++17

LIBS       += $(ROOTLIBS) -lHtml -lThread -lMinuit -lRooFit -lRooFitCore -lMathCore -lMinuit2 -lRooStats -lMathMore

ifeq ($(MODE),GPU)
# LIBS	+=  -lcudart -L/usr/local/cuda/lib64 -lnvToolsExt -I/usr/local/cuda/include
LIBS	+=  -lcudart -L/usr/local/cuda/lib64 -I/usr/local/cuda/include
endif

# -lEve -lGeom -lGui

SRCDIR        = src/

BINDIR        = bin/

SCRDIR        = scripts/

Header        = RooTDalitz/

CC            = g++

# List of object files to compile
OBJS          = $(BINDIR)$(foo).o \
                $(BINDIR)RooDalitzAmplitude.o \
                $(BINDIR)EvtCGCoefSingle.o \
                $(BINDIR)EvtOrthogVector.o \
                $(BINDIR)RooSUMVar.o \
                $(BINDIR)RooFGaussian.o \
                $(CWD)/bin/libMinuit3Minimizer.so \
                $(BINDIR)config.o \
                $(BINDIR)execute.o \
                $(BINDIR)fit.o \
                $(BINDIR)timer.o \
                $(BINDIR)random.o \
                $(BINDIR)parameter.o

ifeq ($(MODE),GPU)
	OBJS+=$(BINDIR)gpuCode.o $(BINDIR)cu_DPFPropogator.o $(BINDIR)kernel_calEva.o
endif
#-------------------------------------------------------
all:				$(foo)
#-------------------------------------------------------

$(foo) :			$(OBJS)
				@echo "Linking executable ..."
				${CC} -o $(BINDIR)$(foo) $(OBJS)  -Wl,-rpath=$(CWD)/bin -L$(CWD)/bin $(LIBS) -lMinuit3Minimizer
				@mv $(BINDIR)$(foo) $(BINDIR)$(foo)$(iDv)
				@echo "Done."
#				cp $(BINDIR)$(foo) ../bin/$(foo)

#-------------------------------------------------------
$(BINDIR)$(foo).o : ${SCRDIR}$(foo).C
				@echo Compiling components...
				${CC} $(CXXFLAGS) $(LIBS) -c ${SCRDIR}$(foo).C -o $(BINDIR)$(foo).o

ifeq ($(MODE),GPU)
$(BINDIR)gpuCode.o:	$(BINDIR)kernel_calEva.o $(BINDIR)cu_DPFPropogator.o
	nvcc $^ -o $(BINDIR)gpuCode.o -dlink

$(BINDIR)cu_DPFPropogator.o:	${SRCDIR}cu_DPFPropogator.cu ${Header}cu_DPFPropogator.h ${Header}cuSolve4x4.h
	nvcc -dc $< #--maxrregcount 128
	mv cu_DPFPropogator.o $(BINDIR)

$(BINDIR)kernel_calEva.o:	${SRCDIR}kernel_calEva.cu ${Header}cu_DPFPropogator.h ${Header}kernel_calEva.h
	nvcc -dc $<  -D iDevice=$(iDv) -D DEVICE_NUM=$(nDv) -D NLL_SHIFT=$(NLL_SHIFT) #--maxrregcount 128
	mv kernel_calEva.o $(BINDIR)
endif

$(BINDIR)RooDalitzAmplitude.o:	${SRCDIR}RooDalitzAmplitude.cxx ${Header}RooDalitzAmplitude.h
				${CC} $(CXXFLAGS) $(LIBS) -c ${SRCDIR}RooDalitzAmplitude.cxx -o $(BINDIR)RooDalitzAmplitude.o -D $(MODE) -D DEVICE_NUM=$(nDv)

$(BINDIR)EvtCGCoefSingle.o:	${SRCDIR}EvtCGCoefSingle.cpp ${Header}EvtCGCoefSingle.hh
				${CC} $(CXXFLAGS) -c ${SRCDIR}EvtCGCoefSingle.cpp -o $(BINDIR)EvtCGCoefSingle.o

$(BINDIR)EvtOrthogVector.o:	${SRCDIR}EvtOrthogVector.cpp ${Header}EvtOrthogVector.hh
				${CC} $(CXXFLAGS) -c ${SRCDIR}EvtOrthogVector.cpp -o $(BINDIR)EvtOrthogVector.o

$(BINDIR)RooSUMVar.o:		${SRCDIR}RooSUMVar.cxx ${Header}RooSUMVar.h
				${CC} $(CXXFLAGS) -c ${SRCDIR}RooSUMVar.cxx -o $(BINDIR)RooSUMVar.o

$(BINDIR)RooFGaussian.o : 	${SRCDIR}RooFGaussian.cxx ${Header}RooFGaussian.h
				${CC} $(CXXFLAGS) -c ${SRCDIR}RooFGaussian.cxx -o $(BINDIR)RooFGaussian.o

$(CWD)/bin/libMinuit3Minimizer.so: ${SRCDIR}Minuit3Minimizer.cxx ${Header}Minuit3Minimizer.h
				${CC} $(CXXFLAGS) $(LIBS) ${SRCDIR}Minuit3Minimizer.cxx -shared -o $(BINDIR)libMinuit3Minimizer.so

$(BINDIR)config.o: ${SRCDIR}config.cpp ${Header}config.h
				${CC} $(CXXFLAGS) $(LIBS) -c ${SRCDIR}config.cpp -o $(BINDIR)config.o

$(BINDIR)random.o: ${SRCDIR}random.cpp ${Header}random.h
				${CC} $(CXXFLAGS) $(LIBS) -c ${SRCDIR}random.cpp -o $(BINDIR)random.o

$(BINDIR)timer.o: ${SRCDIR}timer.cpp ${Header}timer.h
				${CC} $(CXXFLAGS) $(LIBS) -c ${SRCDIR}timer.cpp -o $(BINDIR)timer.o

# Need to link the Minuit3Minimizer library during runtime
$(BINDIR)fit.o: $(CWD)/bin/libMinuit3Minimizer.so ${SRCDIR}fit.cpp ${Header}fit.h
				${CC} $(CXXFLAGS) $(LIBS) -c ${SRCDIR}fit.cpp -o $(BINDIR)fit.o -D HEADER_DIR=$(HEADER_DIR) -D MYLIB_DIR=$(MYLIB_DIR) -D NLL_SHIFT=$(NLL_SHIFT) -Wl,-rpath=$(CWD)/bin -L$(CWD)/bin -lMinuit3Minimizer

$(BINDIR)parameter.o: $(BINDIR)random.o ${SRCDIR}parameter.cpp ${Header}parameter.h
				${CC} $(CXXFLAGS) $(LIBS) -c ${SRCDIR}parameter.cpp -o $(BINDIR)parameter.o

$(BINDIR)execute.o: $(BINDIR)config.o $(BINDIR)fit.o $(BINDIR)parameter.o $(BINDIR)random.o ${SRCDIR}execute.cpp ${Header}execute.h
				${CC} $(CXXFLAGS) $(LIBS) -c ${SRCDIR}execute.cpp -o $(BINDIR)execute.o

#-------------------------------------------------------
clean:
		@rm -f bin/tpanal ${SRCDIR}._* ${SRCDIR}*~ core* $(BINDIR)*.o $(BINDIR)*.so ._*
		@echo cleaned
# DO NOT DELETE
