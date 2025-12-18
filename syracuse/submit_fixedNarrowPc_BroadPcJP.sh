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

S4440=$1
S4457=$2
SBROAD=$3
PBROAD=$4
SEED=$5
TAG=$6
INITIAL_FUNC=$7

PC_ARGS="--Pc4440 ${S4440},-1 --Pc4457 ${S4457},-1 --Pc4312 1,-1 --Pc4500 ${SBROAD},${PBROAD}"
INPUT_ARGS="-d sPlot.root -m mcsw.root -c err/PRC_Zcs4000Cons.err -i ${INITIAL_FUNC}"
OUTPUT_ARGS="-f func/${TAG}.func -r fitresults/${TAG}.root -h history/${TAG}.root"
RANDOM_ARGS="--seed ${SEED} --randomLs --largeRandom"

/home/dtou01/Lb2JpsipK/lb2jpsipk_run12_aman/bin/systematic_BroadPcJP0 ${PC_ARGS} ${INPUT_ARGS} ${OUTPUT_ARGS} ${RANDOM_ARGS}
