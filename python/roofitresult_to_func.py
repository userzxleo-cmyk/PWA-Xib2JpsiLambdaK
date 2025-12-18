import ROOT as r
import argparse, os

def parse_args():
    parser = argparse.ArgumentParser(description="Convert RooFitResult to func file.")
    parser.add_argument("infile", type=str, help="Input ROOT file with RooFitResult.")
    parser.add_argument("outfile", type=str, help="Output func file.")
    parser.add_argument("--initial", action="store_true", help="Create func from initial values.")
    parser.add_argument("--refit", action="store_true", help="Create func for refit with smaller error to help initial finite difference calculation.")

    args = parser.parse_args()
    return args

def get_fit_result(filepath):
    if not os.path.exists(filepath):
        raise ValueError(f"File {filepath} does not exist!")

    file = r.TFile.Open(filepath)
    if not file:
        raise ValueError(f"Cannot open {filepath} as a ROOT file!")

    fit_result = file.Get("nll")
    if not fit_result:
        raise ValueError(f"{filepath} does not have a RooFitResult named \"nll\".")

    return fit_result

def write_func_file( outfile, fit_result, initial, refit ):
    with open( outfile, "w" ) as file:
        for par in fit_result.constPars():
            error_string = f"+/- {par.getError():.6} " if par.hasError() else ""
            lower_limit_string = f"{par.getMin():.7}" if par.hasMin() else "-INF"
            upper_limit_string = f"{par.getMax():.7}" if par.hasMax() else "+INF"       
            limit_string = f"L({lower_limit_string} - {upper_limit_string})"
            file.write(f"{par.GetName()} = {par.getVal():.7} {error_string}C {limit_string}\n")

        parameters = fit_result.floatParsInit() if initial else fit_result.floatParsFinal()
        for par in parameters:
            lower_limit_string = f"{par.getMin():.7}" if par.hasMin() else "-INF"
            upper_limit_string = f"{par.getMax():.7}" if par.hasMax() else "+INF"       
            limit_string = f"L({lower_limit_string} - {upper_limit_string})"
            error_string = f"{0.1*par.getError():.6}" if refit else f"{par.getError():.6}"
            file.write(f"{par.GetName()} = {par.getVal():.7} +/- {error_string} {limit_string}\n")

        file.close()

def main():
    args = parse_args()
    fit_result = get_fit_result(args.infile)
    write_func_file(args.outfile, fit_result, args.initial, args.refit)

if __name__ == "__main__":
    main()
