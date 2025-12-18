import os, argparse, yaml, datetime
from typing import Union, Tuple

import ROOT as r
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

def parse_args():
    parser = argparse.ArgumentParser(description="Compare distribution between efficiency and GBreweight.")
    parser.add_argument("--mcsw", type=str, required=True, help="Path to mcsw.root.")
    parser.add_argument("--nTuples", type=str, required=True, nargs="+", help="Path toy nTuples.")
    parser.add_argument("--efficiency", type=str, required=True, nargs="+", help="Path nTuples with toy efficiencies.")
    parser.add_argument("--GBweight", type=str, required=True, nargs="+", help="Path to nTuples with toy GB weights.")
    parser.add_argument("--yaml", type=str, required=True, help="Path to YAML with options for plotting.")
    parser.add_argument("--output_path", type=str, required=True, help="Path to store plots.")
    args = parser.parse_args()

    if len(args.nTuples) != len(args.efficiency):
        error_string = f"Length of --nTuples ({len(args.nTuples)}) "+ \
                       f"does not match --efficiency ({len(args.efficiency)})"
        raise ValueError(error_string)
    if len(args.nTuples) != len(args.GBweight):
        error_string = f"Length of --nTuples ({len(args.nTuples)}) "+ \
                       f"does not match --GBweight ({len(args.GBweight)})"
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
    efficiency_chain = r.TChain("tree", "")
    GBweight_chain = r.TChain("tree", "")
    for path in args.nTuples:
        toy_chain.AddFile(path)
    for path in args.efficiency:
        efficiency_chain.AddFile(path)
    for path in args.GBweight:
        GBweight_chain.AddFile(path)

    MC_dataframe = r.RDataFrame("h1", args.mcsw)
    toy_dataframe = r.RDataFrame(toy_chain)
    MC_weight = MC_dataframe.AsNumpy(["sw"])["sw"]
    toy_weight = toy_dataframe.AsNumpy(["sw"])["sw"]

    efficiencies = r.RDataFrame(efficiency_chain).AsNumpy(["eff"])["eff"]
    GBweights = r.RDataFrame(GBweight_chain).AsNumpy(["GBweight"])["GBweight"]
    efficiencies = toy_weight * efficiencies
    GBweights = toy_weight * GBweights

    variables = [ variable for variable in options.keys() ]
    MC_variables = MC_dataframe.AsNumpy(variables)
    toy_variables = toy_dataframe.AsNumpy(variables)

    plt.rcParams["font.size"] = 16

    with PdfPages(args.output_path) as pdf:
        for key, config in options.items():
            nBins = config["nBins"]
            xmin = config["xmin"]
            xmax = config["xmax"]
            xlabel = config["xlabel"]

            figure, axes = plt.subplots(ncols=2, figsize=(24, 9))
            x, xerr, y, yerr = histogram_to_scatter_points(MC_variables[key],
                                                           nBins,
                                                           MC_weight,
                                                           True,
                                                           xmin,
                                                           xmax)
            axes[0].errorbar(x, y, xerr=xerr, yerr=yerr,
                             fmt="o", linewidth=1.0, color="tab:blue",
                             label="MC", ls="none", markersize=2.5)
            axes[1].errorbar(x, y, xerr=xerr, yerr=yerr,
                             fmt="o", linewidth=1.0, color="tab:blue",
                             label="MC", ls="none", markersize=2.5)

            x, xerr, y, yerr = histogram_to_scatter_points(toy_variables[key],
                                                           nBins,
                                                           efficiencies,
                                                           True,
                                                           xmin,
                                                           xmax)
            axes[0].errorbar(x, y, xerr=xerr, yerr=yerr,
                             fmt="o", linewidth=1.0, color="tab:orange",
                             label="Toys with efficiency", ls="none", markersize=2.5)
            x, xerr, y, yerr = histogram_to_scatter_points(toy_variables[key],
                                                           nBins,
                                                           GBweights,
                                                           True,
                                                           xmin,
                                                           xmax)
            axes[1].errorbar(x, y, xerr=xerr, yerr=yerr,
                             fmt="o", linewidth=1.0, color="tab:orange",
                             label="Toys with GB weights", ls="none", markersize=2.5)
            axes[0].legend()
            axes[1].legend()
            axes[0].set_xlabel(xlabel)
            axes[0].set_ylim(bottom=0)
            axes[1].set_ylim(bottom=0)
            figure.suptitle(key)
            plt.tight_layout()
            pdf.savefig(figure)
            plt.clf()
            plt.close()

        infodict = pdf.infodict()
        infodict['Title'] = 'MC vs Toy Distributions'
        infodict['Author'] = os.getlogin()
        infodict['CreationDate'] = datetime.datetime.today()
        infodict['ModDate'] = datetime.datetime.today()
