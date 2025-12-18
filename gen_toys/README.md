# Toy Generation for Pc
- Setup the environment by sourcing `lb2jpsipk_run12_aman/readme`.
- The snakemake workflow was tested on Tsinghua's cluster. 
- The workflow was written assuming there is ample RAM available (intermediate workflow outputs are cached into `\tmp`) and there are 4 GPUs.

## Things to to before generating toys:
1. Put the amplitude model (amplitude fit result nTuple) into `fitresults/default_<jp_config>4312m1-4600p3.root`.
2. Compile the efficiency executable (this is only used as a comparison to `GBreweight`)

        cd lb2jpsipk_run12_aman/moments/For_eff/
        rm bin/*o
        make foo=Addeff_forJPtoy -j 8

3. Compile the amplitude weight attachment executable.

        cd lb2jpsipk_run12_aman
        for id in 0 1 2 3
        do
            make clean
            make foo=StoreAmp2s iDv=${id} -j 8
        done
4. Link `toyr_fine_bin.dat`

        ln -s lb2jpsipk_run12_aman/Fit_fullPHSP/toyr_fine_bin.dat .

## How to run snakemake workflow
This `snakemake` workflow makes heavy use of `\tmp` to cache intermediate output files. `\tmp` resides in system memory and not disk space so it's fast for temporary files that would be deleted anyways. However, `snakemake` cannot limit the amount of `\tmp` space available (or I don't know to limit it). 

To avoid blowing up this cache, we first run up till `GBreweight` are trained, which are the longest rules to run. Once we have the reweighter, we run the workflow normally but prioritise the rules that calculate maximum weight for the hit-and-reject steps.

    snakemake --cores 64 --scheduler greedy --until train_reweighter
    snakemake --cores 64 --scheduler greedy -P get_maximum_efficiency get_maximum_GBweight get_maximum_amp2s get_maximum_toymc_efficiency get_maximum_toymc_GBweight

Once you have run the workflow, the toys can be found in into `lb2jpsipk_run12_aman/gen_toys/output`.

### Reason for workflow setup
- Our workflow has a lot of temporary outputs.
- If we don't run `train_reweighter` first, our workflow will keep producing temporary files into `/tmp` while the reweighter is trained. This will blow up `/tmp` since those temporary files are waiting for trained 'GBreweighter' to run the next step.
- Prioritising `get_maximum_<weight>` in the second step will ask snakemake to only run jobs that will produce outputs for those jobs first and get the maximum values for hit-and-reject. This will avoid temporary outputs filling up `/tmp` whilst `snakemake` waits for the inputs to calculate hit-and-reject maximum values.
- We use `greedy` scheduler since the default `ilp` scheduler has some problems with our workflow (probably because it has lots of temporary outputs).
- Based on my observation, the files in `/tmp` never exceed 40Gb in total size during the `snakemake` workflow.
