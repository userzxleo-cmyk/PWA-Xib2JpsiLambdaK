import os, argparse, shutil, re, sys
import ROOT as r


def parse_args():
    parser = argparse.ArgumentParser(description="Submit random fits.")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "--fitresult",
        type=str,
        help="Paths to RooFitResult file to be used as seed for random fits.",
    )
    group.add_argument(
        "--func",
        type=str,
        help="Path to func file to be used as seed for random fits.",
    )
    parser.add_argument("--workdir", required=True, type=str, help="Folder run fits.")
    parser.add_argument(
        "--sub_name", type=str, required=True, help="HTCondor submit file name."
    )
    parser.add_argument("--run_index", type=int, required=True, help="Index of run.")
    parser.add_argument(
        "--seed_offset", type=int, required=True, help="Default offset of random seed."
    )  # 1000 For 4440m1, 2000 For 4440m3
    parser.add_argument(
        "--n_fits", type=int, default=200, help="Number of random fits."
    )
    args = parser.parse_args()

    return args


def prepare_workdir(workdir):
    os.makedirs(workdir, exist_ok=True)
    os.makedirs(os.path.join(workdir, "log"), exist_ok=True)
    os.makedirs(os.path.join(workdir, "error"), exist_ok=True)
    os.makedirs(os.path.join(workdir, "condor_log"), exist_ok=True)
    os.makedirs(os.path.join(workdir, "func"), exist_ok=True)
    os.makedirs(os.path.join(workdir, "fitresults"), exist_ok=True)
    os.makedirs(os.path.join(workdir, "history"), exist_ok=True)
    for symlink in ["mcsw.root", "sPlot.root", "toyr_fine_bin.dat", "err"]:
        if not os.path.exists(os.path.join(workdir, symlink)):
            os.symlink(
                os.path.join(
                    "/home/dtou01/Lb2JpsipK/lb2jpsipk_run12_aman/Fit_fullPHSP", symlink
                ),
                os.path.join(workdir, symlink),
            )


def valid_fitresult(fitresult):
    return fitresult and fitresult.status() == 0 and fitresult.minNll() > -1e6


def write_func_file(result_path, output_path):
    result_file = r.TFile.Open(result_path)
    fitresult = result_file.Get("nll")
    with open(output_path, "w") as file:
        for par in fitresult.constPars():
            error_string = f"+/- {par.getError():.5} " if par.hasError() else ""
            lower_limit_string = f"{par.getMin():.7}" if par.hasMin() else "-INF"
            upper_limit_string = f"{par.getMax():.7}" if par.hasMax() else "+INF"
            limit_string = f"L({lower_limit_string} - {upper_limit_string})"
            file.write(
                f"{par.GetName()} = {par.getVal():.7} {error_string}C {limit_string}\n"
            )

        for par in fitresult.floatParsFinal():
            lower_limit_string = f"{par.getMin():.7}" if par.hasMin() else "-INF"
            upper_limit_string = f"{par.getMax():.7}" if par.hasMax() else "+INF"
            limit_string = f"L({lower_limit_string} - {upper_limit_string})"
            file.write(
                f"{par.GetName()} = {par.getVal():.7} +/- {par.getError():.5} {limit_string}\n"
            )

        file.close()


def get_Pc_spins(tag):
    pattern = re.compile(r".*4440m([1-3])-4457m([1-3])")
    result = pattern.match(tag)

    if result is None:
        raise ValueError(f"Cannot get spin of Pc(4440) and Pc(4457) from tag: <{tag}>.")

    spin_4440 = result.group(1)
    spin_4457 = result.group(2)

    return spin_4440, spin_4457


def write_sub_file(
    workdir, spin_4440, spin_4457, sub_name, func_path, tag_prefix, seed_offset, n_fits
):
    syracuse_script_dir = os.path.dirname(os.path.abspath(__file__))
    with open(os.path.join(workdir, sub_name), "w") as file:
        file.write(f"executable = {syracuse_script_dir}/submit_random.sh\n")
        file.write(f"initialdir = {workdir}\n")
        file.write(f"output     = log/{tag_prefix}_seed$(Seed).log\n")
        file.write(f"error      = error/{tag_prefix}_seed$(Seed).log\n")
        file.write(f"log        = condor_log/{tag_prefix}_seed$(Seed).log\n")
        file.write("\n")
        # file.write(
        #     'Requirements  = ( TARGET.vm_name == "its-u18-nfs-20191029_gpu" ) && \\\n'
        # ) There is something new by orange grid team
        file.write("requirements  = (TotalGPUs > 0) && \\\n")
        file.write("                ( TARGET.Name =!= LastRemoteHost ) && \\\n")
        file.write(
            '                ( TARGET.Machine != "SURGE-OG-10-5-137-119" ) && \\\n'
        )
        file.write('                ( TARGET.Machine != "SURGE-OG-10-5-136-236" )\n')
        file.write("+request_gpus = 1\n")
        file.write("\n")
        file.write(
            f"arguments  = {spin_4440} {spin_4457} $(Seed) {tag_prefix}_seed$(Seed) {func_path}\n"
        )
        file.write("queue Seed from (\n")
        for i in range(n_fits):
            file.write(f"    {seed_offset + i:d}\n")
        file.write(")\n")
        file.close()


def write_submission_file(
    workdir,
    sub_name,
    spin_parity_pattern,
    func_path,
    run_index,
    seed_offset,
    n_fits,
):
    tag_prefix = f"randomFit_{spin_parity_pattern}_R{run_index}"
    spin_4440, spin_4457 = get_Pc_spins(spin_parity_pattern)
    write_sub_file(
        workdir,
        spin_4440,
        spin_4457,
        sub_name,
        func_path,
        tag_prefix,
        seed_offset,
        n_fits,
    )


def check_fitresult(filepath):
   root_file = r.TFile.Open(filepath)
   fitresult = root_file.Get("nll")
   if not valid_fitresult(fitresult):
       raise ValueError(f"{path} does not have a valid RooFitResult!")


if __name__ == "__main__":
    args = parse_args()
    workdir = os.path.abspath(args.workdir)

    prepare_workdir(workdir)
    spin_parity_pattern = re.compile(r".*_(4440[mp][1-5]-4457[mp][1-5])")
    if args.fitresult:
        check_fitresult(args.fitresult)
        spin_parity = spin_parity_pattern.match(args.fitresult).group(1)
        func_path = os.path.join(
            args.workdir, "func", f"initial_{spin_parity}.func"
        )
        write_func_file(args.fitresult, func_path)
        func_path = os.path.abspath(func_path)
    else:
        spin_parity = spin_parity_pattern.match(args.func).group(1)
        func_path = os.path.abspath(args.func)

    write_submission_file(
        workdir,
        args.sub_name,
        spin_parity,
        func_path,
        args.run_index,
        args.seed_offset,
        args.n_fits,
    )

# def find_roofitresult_files(folder, regex):
#     pattern = re.compile(regex)
#     files = os.listdir(folder)
#     files = [file for file in files if os.path.isfile(os.path.join(folder, file))]
#     files = [file for file in files if pattern.match(file)]
#     files = [os.path.join(folder, file) for file in files]

#     return files


# def get_best_results(fitresults):
#     spin_parity_pattern = re.compile(
#         ".*_(4440[mp][1-5]-4457[mp][1-5]-4312[mp][1-5]-4600[mp][1-5])_"
#     )
#     best_nlls = dict()
#     best_results = dict()
#     for filepath in fit_results:
#         root_file = r.TFile.Open(filepath)
#         fitresult = root_file.Get("nll")
#         # Only proceed if RooFitResult is found and fit converged
#         # Also removes fits with bad NLL (NLLs are typically in the -175k range)
#         if valid_fitresult(fitresult):
#             spin_parity_config = spin_parity_pattern.match(filepath).group(1)
#             nll = fitresult.minNll()
#             if spin_parity_config in best_nlls:
#                 if nll < best_nlls[spin_parity_config]:
#                     best_nlls[spin_parity_config] = nll
#                     best_results[spin_parity_config] = filepath
#             else:
#                 best_nlls[spin_parity_config] = nll
#                 best_results[spin_parity_config] = filepath
#     return best_results


# def get_tags(best_results, new_run_index):
#     old_tags = {
#         key: os.path.splitext(os.path.basename(filepath))[0]
#         for key, filepath in best_results.items()
#     }
#     tag_pattern = re.compile("(.*)_R([0-9]+)_")
#     for tag in old_tags.values():
#         if int(tag_pattern.match(tag).group(2)) == new_run_index:
#             raise ValueError(f"Run index {new_run_index} is repeated from input {tag}")
#     tags = {key: tag_pattern.match(tag).group(1) for key, tag in old_tags.items()}
#     tags = {key: f"{tag}_R{new_run_index}" for key, tag in tags.items()}
#     return tags

# def write_func_files(workdir, tags, best_results):
#     func_directory = os.path.join(workdir, "func")
#     func_names = [f"initial_{tag}.func" for tag, result_path in zip(tags, best_results)]
#     func_paths = [os.path.join(func_directory, func_name) for func_name in func_names]
#     for result_path, outputpath in zip(best_results, func_paths):
#         write_func_file(result_path, outputpath)
