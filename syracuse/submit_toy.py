import os, argparse, re

import ROOT as r


def parse_args():
    parser = argparse.ArgumentParser(description="Submit toy fits.")
    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument(
        "--fitresult",
        type=str,
        help="Paths to RooFitResult file to be used as initial parameter values for toy fits.",
    )
    group.add_argument(
        "--func",
        type=str,
        help="Path to func file to be used as initial parameter values for toy fits.",
    )
    parser.add_argument("--workdir", required=True, type=str, help="Folder run fits.")
    parser.add_argument(
        "--sub_name", type=str, required=True, help="HTCondor submit file name."
    )
    parser.add_argument(
        "--start_index", type=int, default=0, help="Index of first toy."
    )
    parser.add_argument(
        "--nToys", type=int, required=True, help="Number of toys to run."
    )
    parser.add_argument(
        "--tag_prefix", type=str, required=True, help="Prefix of toy run tag."
    )
    parser.add_argument(
        "--toy_data",
        type=str,
        required=True,
        help="Pattern to path of toy data, will format <{i}> with index.",
    )
    parser.add_argument(
        "--toy_mcsw",
        type=str,
        required=True,
        help="Pattern to path of toy mcsw, will format <{i}> with index.",
    )
    parser.add_argument(
        "--constraint",
        type=str,
        required=True,
        help="Pattern to path of toy mcsw, will format <{i}> with index.",
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


def write_sub_file(workdir, spin_4440, spin_4457, sub_name, datasets, constraints, func_path, tag):
    syracuse_script_dir = os.path.dirname(os.path.abspath(__file__))
    with open(os.path.join(workdir, sub_name), "w") as file:
        file.write(f"executable = {syracuse_script_dir}/submit_toy.sh\n")
        file.write(f"initialdir = {workdir}\n")
        file.write(f"output     = log/{tag}_$(Index).log\n")
        file.write(f"error      = error/{tag}_$(Index).log\n")
        file.write(f"log        = condor_log/{tag}_$(Index).log\n")
        file.write("\n")
        # file.write(
        #     'Requirements  = ( TARGET.vm_name == "its-u18-nfs-20191029_gpu" ) && \\\n'
        # ) There is something new by orange grid team
        file.write("requirements  = (TotalGPUs > 0) && \\\n")
        file.write("                ( TARGET.Name =!= LastRemoteHost ) && \\\n")
        file.write(
            '                ( TARGET.Machine != "SURGE-OG-10-5-137-119" ) && \\\n'
        )
        file.write('                ( TARGET.Machine != "CRUSH-NODE-10-5-37-5" ) && \\\n')
        file.write('                ( TARGET.Machine != "SURGE-OG-10-5-136-236" )\n')
        file.write("+request_gpus = 1\n")
        file.write("\n")
        file.write(
            f"arguments  = {spin_4440} {spin_4457} $(ToyData) $(ToyMCSW) {tag}_$(Index) {func_path} $(Constraint)\n"
        )
        file.write("queue Index, ToyData, ToyMCSW, Constraint from (\n")
        for (index, data, mcsw), constraint in zip(datasets, constraints):
            file.write(f"    {index}, {data}, {mcsw}, {constraint}\n")
        file.write(")\n")
        file.close()


def write_submission_file(
    workdir,
    sub_name,
    spin_parity_pattern,
    datasets,
    constraints,
    func_path,
    tag_prefix,
):
    tag = f"{tag_prefix}_{spin_parity_pattern}"
    spin_4440, spin_4457 = get_Pc_spins(spin_parity_pattern)
    write_sub_file(workdir, spin_4440, spin_4457, sub_name, datasets, constraints, func_path, tag)


def get_datasets(start_index, nToys, toy_data, toy_mcsw):
    indices = range(start_index, start_index + nToys)

    toy_data_paths = [os.path.abspath(toy_data.format(i=i)) for i in indices]
    toy_mcsw_paths = [os.path.abspath(toy_mcsw.format(i=i)) for i in indices]

    for path in toy_data_paths:
        if not os.path.exists(path):
            raise ValueError(f"Toy data does not exists: {path}.")

    for path in toy_mcsw_paths:
        if not os.path.exists(path):
            raise ValueError(f"Toy mcsw does not exists: {path}.")

    datasets = [
        (i, data, mcsw)
        for i, data, mcsw in zip(indices, toy_data_paths, toy_mcsw_paths)
    ]

    return datasets


def check_fitresult(filepath):
    root_file = r.TFile.Open(filepath)
    fitresult = root_file.Get("nll")
    if not valid_fitresult(fitresult):
        raise ValueError(f"{path} does not have a valid RooFitResult!")


def get_constraints(start_index, nToys, constraint):
    indices = range(start_index, start_index + nToys)

    constraint_paths = [os.path.abspath(constraint.format(i=i)) for i in indices]

    for path in constraint_paths:
        if not os.path.exists(path):
            raise ValueError(f"Constraint does not exists: {path}.")

    return constraint_paths

if __name__ == "__main__":
    args = parse_args()
    workdir = os.path.abspath(args.workdir)

    prepare_workdir(workdir)
    spin_parity_pattern = re.compile(r".*_(4440[mp][1-5]-4457[mp][1-5])")
    if args.fitresult:
        check_fitresult(args.fitresult)
        spin_parity = spin_parity_pattern.match(args.fitresult).group(1)
        func_path = os.path.join(
            args.workdir, "func", f"initial_{args.tag_prefix}_{spin_parity}.func"
        )
        write_func_file(args.fitresult, func_path)
        func_path = os.path.abspath(func_path)
    else:
        spin_parity = spin_parity_pattern.match(args.func).group(1)
        func_path = os.path.abspath(args.func)

    datasets = get_datasets(args.start_index, args.nToys, args.toy_data, args.toy_mcsw)
    constraints = get_constraints(args.start_index, args.nToys, args.constraint)

    write_submission_file(
        workdir,
        args.sub_name,
        spin_parity,
        datasets,
        constraints,
        func_path,
        args.tag_prefix,
    )
