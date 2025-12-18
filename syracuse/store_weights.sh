#!/bin/bash

export OMPI_MCA_opal_cuda_support=true
cuda_ver="/home/hmu105/software/cuda/cuda-11.6"
export CUDA_PATH="$cuda_ver"
export CUDA_HOME="$cuda_ver"
export PATH="$cuda_ver/bin:${PATH}"
export LD_LIBRARY_PATH="$cuda_ver/lib64:${LD_LIBRARY_PATH}"

export LD_LIBRARY_PATH=/home/dtou01/Software/miniconda3/envs/root_22/lib:${LD_LIBRARY_PATH}
source /home/dtou01/Software/miniconda3/etc/profile.d/conda.sh 
conda activate root_22 


scale_number=1.32
lowerlimit=0.5

P4440=$1
P4457=$2
FITRESULT=$3
DATAWEIGHTS=$4
MCWEIGHTS=$5

#echo "${P4440} ${P4457} ${INFUNC} ${OUTFUNC} ${OUTRESULT}"
/home/dtou01/Lb2JpsipK/lb2jpsipk_run12_aman/bin/StoreResonanceWeights0 ${P4440} -1 ${P4457} -1 1 -1 3 1 ${FITRESULT} ${scale_number} ${lowerlimit} 100.  /home/dtou01/Lb2JpsipK/lb2jpsipk_run12_aman/Fit_fullPHSP/mcsw.root ${DATAWEIGHTS} ${MCWEIGHTS}
