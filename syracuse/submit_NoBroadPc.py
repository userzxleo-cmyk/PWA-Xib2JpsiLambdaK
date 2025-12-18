import os, argparse, shutil, re, sys
import ROOT as r


def parse_args():
    parser = argparse.ArgumentParser(
        description="Submit NoBroadPc fits with randomised initial parameters."
    )
    parser.add_argument(
        "--func",
        type=str,
        help="Path to func file to be used as seed for randomised parameters.",
    )
    parser.add_argument("--workdir", required=True, type=str, help="Folder run fits.")
    parser.add_argument(
        "--sub_name", type=str, required=True, help="HTCondor submit file name."
    )
    parser.add_argument(
        "--seed_offset", type=int, default=5000, help="Default offset of random seed."
    )  # 5000
    parser.add_argument("--n_fits", type=int, default=20, help="Number of fits.")
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


def get_Pc_spins(tag):
    pattern = re.compile(r".*4440m([1-3])-4457m([1-3])")
    result = pattern.match(tag)

    if result is None:
        raise ValueError(f"Cannot get spin of Pc(4440) and Pc(4457) from tag: <{tag}>.")

    spin_4440 = result.group(1)
    spin_4457 = result.group(2)

    return spin_4440, spin_4457


def write_sub_file(
    workdir,
    spin_4440,
    spin_4457,
    sub_name,
    func_path,
    tag_prefix,
    seed_offset,
    n_fits,
):
    syracuse_script_dir = os.path.dirname(os.path.abspath(__file__))
    with open(os.path.join(workdir, sub_name), "w") as file:
        file.write(f"executable = {syracuse_script_dir}/submit_NoBroadPc.sh\n")
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
            "                ( TARGET.Machine != \"SURGE-OG-10-5-137-119\" ) && \\\n"
        )
        file.write(
            "                ( TARGET.Machine != \"CRUSH-NODE-10-5-37-5\" ) && \\\n"
        )
        file.write("                ( TARGET.Machine != \"SURGE-OG-10-5-136-236\" )\n")
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
    narrow_Pc_spin_parity,
    func_path,
    seed_offset,
    n_fits,
):
    tag_prefix = f"NoBroadPc-{narrow_Pc_spin_parity}"
    spin_4440, spin_4457 = get_Pc_spins(narrow_Pc_spin_parity)
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
    spin_parity_pattern = re.compile(r".*(4440[mp][1-5]-4457[mp][1-5])")
    narrow_Pc_spin_parity = spin_parity_pattern.match(args.func).group(1)
    func_path = os.path.abspath(args.func)

    write_submission_file(
        workdir,
        args.sub_name,
        narrow_Pc_spin_parity,
        func_path,
        args.seed_offset,
        args.n_fits,
    )
