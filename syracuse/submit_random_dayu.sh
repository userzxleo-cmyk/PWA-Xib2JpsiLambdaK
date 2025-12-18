#!/bin/bash

export OMPI_MCA_opal_cuda_support=true
cuda_ver="/home/dtou01/Software/cuda/cuda-11.6"
export CUDA_PATH="$cuda_ver"
export CUDA_HOME="$cuda_ver"
export PATH="$cuda_ver/bin:${PATH}"
export LD_LIBRARY_PATH="$cuda_ver/lib64:${LD_LIBRARY_PATH}"

export LD_LIBRARY_PATH==/home/dtou01/Software/miniconda3/envs/root_22/lib:${LD_LIBRARY_PATH}
source =/home/dtou01/Software/miniconda3/envs/root_22/lib
conda activate root_22


scale_number=1.32
lowerlimit=0.5

S4440=$1
S4457=$2
SEED=$3
TAG=$4

#echo "${P4440} ${P4457} ${INFUNC} ${OUTFUNC} ${OUTRESULT}"
/home/dtou01/Lb2JpsipK/lb2jpsipk_run12_aman/bin/FitKM4Pc_fullPHSP_fineBinning_ExtLst_openCoupleChannel_ImMass_smartSmearing_smartbinning_rndLS_rndmass_largernd0 ${S4440} -1 ${S4457} -1 1 -1 3 1 initial_${TAG} final_${TAG}_seed${SEED} ${TAG}_seed${SEED} ${scale_number} ${lowerlimit} 100. ${SEED}
