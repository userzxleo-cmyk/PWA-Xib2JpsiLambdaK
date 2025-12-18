#!/bin/bash

export OMPI_MCA_opal_cuda_support=true
cuda_ver="/home/dtou01/Software/cuda/cuda-11.6"
export CUDA_PATH="$cuda_ver"
export CUDA_HOME="$cuda_ver"
export PATH="$cuda_ver/bin:${PATH}"
export LD_LIBRARY_PATH="$cuda_ver/lib64:${LD_LIBRARY_PATH}"

export LD_LIBRARY_PATH=/home/dtou01/Software/miniconda3/envs/root_24/lib:${LD_LIBRARY_PATH}
source /home/dtou01/Software/miniconda3/etc/profile.d/conda.sh
conda activate root_24

scale_number=1.32
lowerlimit=0.5

J4440=$1
J4457=$2
TAG=$3

PC_ARGS="--Pc4440 ${J4440},-1 --Pc4457 ${J4457},-1 --Pc4312 1,-1 --Pc4500 3,1"
INPUT_ARGS="-d sPlot.root -m mcsw.root -c err/PRC_Zcs4000Cons.err -i func/initial_${TAG}.func"
OUTPUT_ARGS="-f func/${TAG}.func -r fitresults/${TAG}.root -h history/${TAG}.root"

/home/dtou01/Lb2JpsipK/lb2jpsipk_run12_aman/bin/FitAlternateCouplings0 ${PC_ARGS} ${INPUT_ARGS} ${OUTPUT_ARGS}
