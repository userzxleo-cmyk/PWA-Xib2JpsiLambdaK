import os, argparse, shutil, re
import ROOT as r


def parse_args():
    parser = argparse.ArgumentParser(description="Resubmit failed random fits.")
    parser.add_argument(
        "--infolder",
        required=True,
        type=str,
        help="Folder to search for RooFitResult file.",
    )
    parser.add_argument(
        "--regex",
        required=True,
        type=str,
        help="Regular expression to match to file.",
    )
    parser.add_argument(
        "--workdir",
        required=True,
        type=str,
        help="Folder run fits.",
    )
    parser.add_argument(
        "--sub_name",
        type=str,
        required=True,
        help="HTCondor submit file name.",
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


def find_roofitresult_files(folder, regex):
    pattern = re.compile(regex)
    files = os.listdir(folder)
    files = [file for file in files if os.path.isfile(os.path.join(folder, file))]
    files = [file for file in files if pattern.match(file)]
    files = [os.path.join(folder, file) for file in files]

    return files


def get_failed_fits(fit_results):
    failed_results = list()
    for root_file in fit_results:
        file = r.TFile.Open(root_file)
        fitresult = file.Get("nll")
        if fitresult and fitresult.status() != 0:
            failed_results.append(root_file)

    return failed_results


def get_rerun_tag(fit_result):
    old_tag = os.path.splitext(os.path.basename(fit_result))[0]
    pattern = re.compile(r"(.*)_(seed[0-9]+)")
    result = pattern.match(old_tag)
    new_tag = f"{result.group(1)}_refit_{result.group(2)}"
    return new_tag


def write_func_file(fitresult_path, output_path):
    result_file = r.TFile.Open(fitresult_path)
    fitresult = result_file.Get("nll")
    with open(output_path, "w") as file:
        for par in fitresult.constPars():
            error_string = f"+/- {par.getError():.6} " if par.hasError() else ""
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
                f"{par.GetName()} = {par.getVal():.7} +/- {0.1*par.getError():.6} {limit_string}\n"
            )

        file.close()


def write_func_files(workdir, tags, fit_results):
    func_directory = os.path.join(workdir, "func")
    func_names = [f"initial_{tag}.func" for tag, fit_result in zip(tags, fit_results)]
    func_paths = [os.path.join(func_directory, func_name) for func_name in func_names]
    for fit_result, outputpath in zip(fit_results, func_paths):
        write_func_file(fit_result, outputpath)


def get_Pc_spin(tags):
    pattern_4440 = re.compile(r".*4440m([1-3])-")
    pattern_4457 = re.compile(r".*-4457m([1-3])")

    spins_4440 = [pattern_4440.match(tag).group(1) for tag in tags]
    spins_4457 = [pattern_4457.match(tag).group(1) for tag in tags]

    return spins_4440, spins_4457


def write_sub_file(workdir, spins_4440, spins_4457, sub_name, tags):
    syracuse_script_dir = os.path.dirname(os.path.abspath(__file__))
    workdir = os.path.abspath(workdir)
    with open(os.path.join(workdir, sub_name), "w") as file:
        file.write(f"executable = {syracuse_script_dir}/resubmit_random.sh\n")
        file.write(f"initialdir = {workdir}\n")
        file.write(f"output     = log/$(JobTag).log\n")
        file.write(f"error      = error/$(JobTag).log\n")
        file.write(f"log        = condor_log/$(JobTag).log\n")
        file.write("\n")
        file.write("requirements  = (TotalGPUs > 0) && \\\n")
        file.write("                ( TARGET.Name =!= LastRemoteHost ) && \\\n")
        file.write(
            '                ( TARGET.Machine != "SURGE-OG-10-5-137-119" ) && \\\n'
        )
        file.write('                ( TARGET.Machine != "SURGE-OG-10-5-136-236" )\n')
        file.write("+request_gpus = 1\n")
        file.write("\n")
        file.write(f"arguments  = $(J4440) $(J4457) $(JobTag)\n")
        file.write("queue J4440, J4457, JobTag from (\n")
        for J_4440, J_4457, tag in zip(spins_4440, spins_4457, tags):
            file.write(f"    {J_4440}, {J_4457}, {tag} \n")
        file.write(")\n")
        file.close()


def write_submission_files(workdir, sub_name, fit_results):
    tags = [get_rerun_tag(fit_result) for fit_result in fit_results]
    spins_4440, spins_4457 = get_Pc_spin(tags)
    write_func_files(workdir, tags, fit_results)
    write_sub_file(workdir, spins_4440, spins_4457, sub_name, tags)


if __name__ == "__main__":
    args = parse_args()
    prepare_workdir(args.workdir)
    fit_results = find_roofitresult_files(args.infolder, args.regex)
    fit_results_to_rerun = get_failed_fits(fit_results)
    write_submission_files(args.workdir, args.sub_name, fit_results_to_rerun)
