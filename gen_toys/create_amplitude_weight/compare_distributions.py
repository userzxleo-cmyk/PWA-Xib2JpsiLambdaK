import os, argparse, yaml, datetime
from typing import Union, Tuple

import ROOT as r
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

def parse_args():
    parser = argparse.ArgumentParser(description="Compare distribution of data with toy.")
    parser.add_argument("--data", type=str, required=True, help="Path to real data sPlot.root.")
    parser.add_argument("--nTuples", type=str, required=True, nargs="+", help="Path toy nTuples.")
    parser.add_argument("--amp2s", type=str, required=True, nargs="+", help="Path to nTuples with per-event amp^2.")
    parser.add_argument("--yaml", type=str, required=True, help="Path to YAML with options for plotting.")
    parser.add_argument("--output_path", type=str, required=True, help="Path to store plots.")
    args = parser.parse_args()

    if len(args.nTuples) != len(args.amp2s):
        error_string = f"Length of --nTuples ({len(args.nTuples)}) "+ \
                       f"does not match --amp2s ({len(args.amp2s)})"
        raise ValueError(error_string)

    return args

def histogram_to_scatter_points( x: np.array, 
                                 nBins: int, 
                                 weights: np.array = None, 
                                 density: bool = False,
                                 minimum: Union[float, None] = None, 
                                 maximum: Union[float, None] = None
                               ) -> Tuple[np.array, np.array, np.array, np.array]:
    if weights is not None and x.size != weights.size:
        raise ValueError("histogram_to_scatter_points: x and weights have different dimensions!")

    if weights is None:
        weights = np.ones_like(x)
    if minimum == None:
        minimum = np.amin(x)
    if maximum == None:
        maximum = np.amax(x)

    weights_squared = np.square(weights)
    bin_edges = np.linspace( minimum, maximum, nBins + 1 )
    bin_centres = ( bin_edges[:-1] + bin_edges[1:] ) / 2
    bin_values = np.histogram(x, bins=bin_edges, weights=weights)[0]
    bin_errors = np.sqrt( np.histogram(x, bins=bin_edges, weights=weights_squared)[0] )

    if density:
        normalisation = 1./np.sum(bin_values)
        bin_values = normalisation * bin_values
        bin_errors = normalisation * bin_errors

    bin_width = ( maximum - minimum ) / nBins
    xerr = 0.5 * bin_width * np.ones_like(bin_centres)

    return bin_centres, xerr, bin_values, bin_errors

def parse_yaml(yaml_file):
    with open(yaml_file, "r") as file:
        options = yaml.load(file, Loader=yaml.Loader)
        file.close()
    return options

if __name__ == "__main__":
    args = parse_args()
    options = parse_yaml(args.yaml)
    os.makedirs(os.path.dirname(args.output_path), exist_ok=True)

    toy_chain = r.TChain("tree", "")
    amp2s_chain = r.TChain("tree", "")
    for path in args.nTuples:
        toy_chain.AddFile(path)
    for path in args.amp2s:
        amp2s_chain.AddFile(path)

    data_dataframe = r.RDataFrame("DTree", args.data)
    toy_dataframe = r.RDataFrame(toy_chain)
    data_weights = data_dataframe.AsNumpy(["nsig_sw"])["nsig_sw"]
    toy_weights = r.RDataFrame(amp2s_chain).AsNumpy(["amp2"])["amp2"]

    variables = [ variable for variable in options.keys() ]
    data_variables = data_dataframe.AsNumpy(variables)
    toy_variables = toy_dataframe.AsNumpy(variables)

    plt.rcParams["font.size"] = 16

    with PdfPages(args.output_path) as pdf:
        for key, config in options.items():
            nBins = config["nBins"]
            xmin = config["xmin"]
            xmax = config["xmax"]
            xlabel = config["xlabel"]

            figure = plt.figure(figsize=(16, 9))
            x, xerr, y, yerr = histogram_to_scatter_points(data_variables[key],
                                                           nBins,
                                                           data_weights,
                                                           True,
                                                           xmin,
                                                           xmax)
            plt.errorbar(x, y, xerr=xerr, yerr=yerr,
                         fmt="o", linewidth=1.0, color="tab:blue",
                         label="Real Data", ls="none", markersize=2.5)

            x, xerr, y, yerr = histogram_to_scatter_points(toy_variables[key],
                                                           nBins,
                                                           toy_weights,
                                                           True,
                                                           xmin,
                                                           xmax)
            plt.errorbar(x, y, xerr=xerr, yerr=yerr,
                         fmt="o", linewidth=1.0, color="tab:orange",
                         label="Toys", ls="none", markersize=2.5)
            plt.legend()
            plt.xlabel(xlabel)
            plt.ylim(bottom=0)
            plt.title(key)
            plt.tight_layout()
            pdf.savefig(figure)
            plt.clf()
            plt.close()

        infodict = pdf.infodict()
        infodict['Title'] = 'Real Data vs Toy Distributions'
        infodict['Author'] = os.getlogin()
        infodict['CreationDate'] = datetime.datetime.today()
        infodict['ModDate'] = datetime.datetime.today()
