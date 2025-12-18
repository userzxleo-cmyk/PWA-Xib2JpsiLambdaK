import os, argparse, yaml, datetime
from typing import Union, Tuple

import ROOT as r
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages

def parse_args():
    parser = argparse.ArgumentParser(description="Compare distribution between toy and data (or MC).")

    group = parser.add_mutually_exclusive_group(required=True)
    group.add_argument("--mcsw", type=str, help="Path to mcsw.root.")
    group.add_argument("--data", type=str, help="Path to sPlot.root.")

    parser.add_argument("--toy", type=str, required=True, help="Path to toy nTuple.")
    parser.add_argument("--treename", type=str, choices=["DTree", "h1"], help="Name of TTree in input files.")
    parser.add_argument("--weight", type=str, default=None, help="Branch name of weights to use in both toy and real data (or MC).")
    parser.add_argument("--yaml", type=str, required=True, help="Path to YAML with options for plotting.")
    parser.add_argument("--output_path", type=str, required=True, help="Path to store plots.")

    args = parser.parse_args()

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

    nominal_rootfile_path = args.mcsw if args.mcsw else args.data
    toy_dataframe = r.RDataFrame(args.treename, args.toy)
    nominal_dataframe = r.RDataFrame(args.treename, nominal_rootfile_path)

    toy_weights = toy_dataframe.AsNumpy([args.weight])[args.weight]
    nominal_weights = nominal_dataframe.AsNumpy([args.weight])[args.weight]

    variables = [ variable for variable in options.keys() ]
    toy_variables = toy_dataframe.AsNumpy(variables)
    nominal_variables = nominal_dataframe.AsNumpy(variables)

    plt.rcParams["font.size"] = 16
    nominal_label = "MC" if args.mcsw else "Data"

    with PdfPages(args.output_path) as pdf:
        # Plot with mjpsip enchanced region
        nBins = 100
        xmin = 4.2
        xmax = 4.6
        title = "$m(\\psi p)$ Enhanced"
        xlabel = "$m(\\psi p)$ [GeV]"

        toy_indices = np.logical_and( toy_variables["mkp"] > 1.6, np.logical_and( toy_variables["mjpsip"] >= 4.2, toy_variables["mjpsip"] < 4.6 ) )
        nominal_indices = np.logical_and( nominal_variables["mkp"] > 1.6, np.logical_and( nominal_variables["mjpsip"] >= 4.2, nominal_variables["mjpsip"] < 4.6 ) )

        figure = plt.figure(figsize=(16, 9))
        x, xerr, y, yerr = histogram_to_scatter_points(nominal_variables["mjpsip"][nominal_indices],
                                                       nBins,
                                                       nominal_weights[nominal_indices],
                                                       True,
                                                       xmin,
                                                       xmax)
        plt.errorbar(x, y, xerr=xerr, yerr=yerr,
                     fmt="o", linewidth=1.0, color="tab:blue",
                     label=nominal_label, ls="none", markersize=2.5)

        x, xerr, y, yerr = histogram_to_scatter_points(toy_variables["mjpsip"][toy_indices],
                                                       nBins,
                                                       toy_weights[toy_indices],
                                                       True,
                                                       xmin,
                                                       xmax)
        plt.errorbar(x, y, xerr=xerr, yerr=yerr,
                     fmt="o", linewidth=1.0, color="tab:orange",
                     label="Toys", ls="none", markersize=2.5)
        plt.legend()
        plt.xlabel(xlabel)
        plt.ylim(bottom=0)
        plt.title(title)
        plt.tight_layout()
        pdf.savefig(figure)
        plt.clf()
        plt.close()

        for key, config in options.items():
            nBins = config["nBins"]
            xmin = config["xmin"]
            xmax = config["xmax"]
            xlabel = config["xlabel"]

            figure = plt.figure(figsize=(16, 9))
            x, xerr, y, yerr = histogram_to_scatter_points(nominal_variables[key],
                                                           nBins,
                                                           nominal_weights,
                                                           True,
                                                           xmin,
                                                           xmax)
            plt.errorbar(x, y, xerr=xerr, yerr=yerr,
                         fmt="o", linewidth=1.0, color="tab:blue",
                         label=nominal_label, ls="none", markersize=2.5)

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
        infodict['Title'] = f'{nominal_label} vs Toy Distributions'
        infodict['Author'] = os.getlogin()
        infodict['CreationDate'] = datetime.datetime.today()
        infodict['ModDate'] = datetime.datetime.today()
