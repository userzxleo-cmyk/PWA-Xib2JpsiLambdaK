import os, argparse, shutil, re, glob, itertools

import ROOT as r

def parse_args():
    parser = argparse.ArgumentParser(description="Submit random fits.")
    parser.add_argument("--fitresults", type=str, required=True, help="Glob pattern to find RooFitResult.")
    parser.add_argument("--regex", type=str, required=True, help="Regex to get index.")
    parser.add_argument("--toy_data", required=True, type=str, help="Substitution pattern for toy data.")
    parser.add_argument("--toy_mcsw", required=True, type=str, help="Substitution pattern for toy data.")
    parser.add_argument("--workdir", required=True, type=str, help="Folder run fits.")
    parser.add_argument("--sub_name", type=str, required=True, help="HTCondor submit file name.")
    parser.add_argument("--jp4440", required=True, type=int, nargs=2, help="JP config of Pc(4440).")
    parser.add_argument("--jp4457", required=True, type=int, nargs=2, help="JP config of Pc(4457).")
    parser.add_argument("--tag", required=True, type=str, help="Tag of toy fit.")
    parser.add_argument("--filter_converged", action="store_true", help="Filter out converged RooFitResult.")
    args = parser.parse_args()

    assert(args.jp4440[1] == 1 or args.jp4440[1] == -1)
    assert(args.jp4457[1] == 1 or args.jp4457[1] == -1)

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
            os.symlink(os.path.join("/home/dtou01/Lb2JpsipK/lb2jpsipk_run12_aman/Fit_fullPHSP", symlink), os.path.join(workdir, symlink))

def write_sub_file(workdir, indices, fitresults, toy_data, toy_mcsw, jp4440, jp4457, sub_name, tag):
    with open(os.path.join(workdir, sub_name), "w") as file:
        file.write("executable = /home/dtou01/Lb2JpsipK/lb2jpsipk_run12_aman/syracuse/submit_toy.sh\n")
        file.write(f"initialdir = {workdir}\n")
        file.write(f"output     = log/{tag}_$(Index).log\n")
        file.write(f"error      = error/{tag}_$(Index).log\n")
        file.write(f"log        = condor_log/{tag}_$(Index).log\n")
        file.write("\n")
        file.write("Requirements  = ( TARGET.vm_name == \"its-u18-nfs-20191029_gpu\" ) && \\\n")
        file.write("                ( TARGET.Name =!= LastRemoteHost ) && \\\n")
        file.write("                ( TARGET.Machine != \"SURGE-OG-10-5-137-119\" )\n")
        file.write("+request_gpus = 1\n")
        file.write("\n")
        file.write(f"arguments  = {jp4440[0]} {jp4440[1]} {jp4457[0]} {jp4457[1]} $(InitialFitResult) $(ToyData) $(ToyMCSW) {tag}_$(Index)\n")
        file.write("\n")
        file.write("queue Index, InitialFitResult, ToyData, ToyMCSW from (\n")
        for index, fitresult, data, mcsw in zip(indices, fitresults, toy_data, toy_mcsw):
            file.write(f"    {index}, {fitresult}, {data}, {mcsw}\n")
        file.write(")\n")
        file.close()

def get_index_function(pattern):
    function = lambda string : int(pattern.match(string).group(1))
    return function

def substitute(pattern, indices):
    if "{index}" in pattern:
        paths = [ pattern.format(index=index) for index in indices ]
    else:
        paths = [ pattern ] * len(indices)
    return paths

def converged(path):
    file = r.TFile.Open(path)
    fitresult = file.Get("nll")
    converged = fitresult and fitresult.status() == 0
    file.Close()
    return  converged

def filter_converged(fitresults):
    fitresults = [ path for path in fitresults if not converged(path) ]
    return fitresults

def match_index(fitresults, regex):
    pattern = re.compile(regex)
    indices = [ int(pattern.match(path).group(1)) for path in fitresults ]
    return indices

if __name__ == "__main__":
    args = parse_args()

    prepare_workdir(args.workdir)
    fitresults = glob.glob(args.fitresults)
    if args.filter_converged:
        fitresults = filter_converged(fitresults)
    indices = match_index(fitresults, args.regex)
    toy_data = substitute(args.toy_data, indices)
    toy_mcsw = substitute(args.toy_mcsw, indices)

    workdir = os.path.abspath(args.workdir)
    fitresults = [ os.path.relpath(path, args.workdir) for path in fitresults ]
    toy_data = [ os.path.relpath(path, args.workdir) for path in toy_data ]
    toy_mcsw = [ os.path.relpath(path, args.workdir) for path in toy_mcsw ]

    write_sub_file(workdir, indices, fitresults, toy_data, toy_mcsw, args.jp4440, args.jp4457, args.sub_name, args.tag)
