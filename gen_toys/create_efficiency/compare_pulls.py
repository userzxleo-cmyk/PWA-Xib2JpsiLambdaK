import argparse, os, yaml, json

import ROOT as r
import numpy as np
import matplotlib.pyplot as plt

from matplotlib.backends.backend_pdf import PdfPages

def parse_args():
    parser = argparse.ArgumentParser(description="Plot and analyse pulls.")
    parser.add_argument("--pulls_npz", type=str, help="Path to stored pulls.")
    parser.add_argument("--yaml", type=str, help="YAML for plot options.")
    parser.add_argument("--json_path", type=str, help="Path to JSON info dump.")
    parser.add_argument("--plot_path", type=str, help="Path to plot.")

    args = parser.parse_args()

    return args

def fit_pulls(pulls):
    limit = max(np.abs(np.amin(pulls)), np.amax(pulls))
    pull = r.RooRealVar("pull", "(Toy - Full)_{i}/#sigma_{i}", -1.1 * limit, 1.1 * limit)
    dataset = r.RooDataSet("dataset", "dataset", r.RooArgSet(pull))

    for pull_value in pulls:
        pull.setVal(pull_value)
        dataset.add(r.RooArgSet(pull))

    mu = r.RooRealVar("mu", "mu", np.mean(pulls), np.amin(pulls), np.amax(pulls))
    sigma = r.RooRealVar("sigma", "sigma", np.std(pulls), 0.1, 3 * np.std(pulls))
    mu.setError(0.01 * mu.getVal())
    sigma.setError(0.01 * sigma.getVal())

    model = r.RooGaussian("gauss", "gauss", pull, mu, sigma)
    result = model.fitTo(dataset, r.RooFit.Strategy(1), r.RooFit.Save(1), r.RooFit.Offset(1), r.RooFit.Minimizer("Minuit2", "MIGRAD"))

    return result, mu, sigma

def parse_yaml(yaml_file):
    with open(yaml_file, "r") as file:
        options = yaml.load(file, Loader=yaml.Loader)
        file.close()
    return options

def get_gaussian_shape(info, x):
    mu = info["fit_results"]["mu"]["value"]
    sigma = info["fit_results"]["sigma"]["value"]

    y = np.exp( -0.5 * np.square( ( x - mu ) / sigma ) )
    y = y / np.sum(y) # Normalise to 1

    return y

def get_text(info):
    mean = info["mean"]
    standard_deviation = info["standard_deviation"]
    skewness = info["fisher_skewness"]
    mu = info["fit_results"]["mu"]["value"]
    sigma = info["fit_results"]["sigma"]["value"]

    text = f"mean           : {mean:.3f}\n" +\
           f"std. deviation : {standard_deviation:.3f}\n" +\
           f"skewness       : {skewness:.3f}\n" +\
           f"fitted $\\mu$    : {mu:.3f}\n" +\
           f"fitted $\\sigma$    : {sigma:.3f}"
    return text

if __name__ == "__main__":
    args = parse_args()

    pulls_dict = np.load(args.pulls_npz)
    options = parse_yaml(args.yaml)
    variable_labels = [ label for label in options["variables"].values() ]

    info_dict = dict()

    # 2D to 6D
    for key, pulls in pulls_dict.items():
        result, mu, sigma = fit_pulls(pulls)

        mean = np.mean(pulls)
        standard_deviation = np.std(pulls)
        fisher_skewness = np.mean(np.power((pulls - mean)/standard_deviation, 3))

        info = { "mean" : mean,
                 "standard_deviation" : standard_deviation,
                 "fisher_skewness" : fisher_skewness,
                 "fit_results" : {
                    "status" : result.status(),
                    "covariance" : result.covQual(),
                    "mu" : {
                        "value" : mu.getVal(),
                        "error" : mu.getVal()
                    },
                    "sigma" : {
                        "value" : sigma.getVal(),
                        "error" : sigma.getVal()
                    }
                 }
               }

        info_dict[key] = info

    os.makedirs(os.path.dirname(args.json_path), exist_ok=True)
    os.makedirs(os.path.dirname(args.plot_path), exist_ok=True)
    with open(args.json_path, "w") as file:
        file.write(json.dumps(info_dict, indent=2))

    plt.rcParams["font.size"] = 18
    with PdfPages(args.plot_path) as pdf:
        for ndim in range(2, 7):
            variables_string = ", ".join(variable_labels[:ndim])
            title = f"{ndim}D Pulls of [{variables_string}]"

            efficiency_pulls = pulls_dict[f"efficiency_{ndim}D"]
            GBweight_pulls = pulls_dict[f"GBweight_{ndim}D"]
            efficiency_weights = np.ones_like(efficiency_pulls) / efficiency_pulls.size
            GBweight_weights = np.ones_like(GBweight_pulls) / GBweight_pulls.size
            xmin = min(np.amin(efficiency_pulls), np.amin(GBweight_pulls))
            xmax = max(np.amax(efficiency_pulls), np.amax(GBweight_pulls))
            delta = xmax - xmin
            xmin = xmin - 0.05 * delta
            xmax = xmax + 0.05 * delta

            x = np.linspace(xmin, xmax, 1000)
            efficiency_y = 10 * get_gaussian_shape(info_dict[f"efficiency_{ndim}D"], x)
            GBweight_y = 10 * get_gaussian_shape(info_dict[f"GBweight_{ndim}D"], x)

            figure, axes = plt.subplots(ncols=2, figsize=(24, 9), sharex=True, sharey=True)
            axes[0].hist(efficiency_pulls, bins=100, range=(xmin, xmax), weights=efficiency_weights,
                         color="tab:blue", alpha=0.5, label="pulls")
            axes[1].hist(GBweight_pulls, bins=100, range=(xmin, xmax), weights=GBweight_weights,
                         color="tab:blue", alpha=0.5, label="pulls")
            axes[0].plot(x, efficiency_y, color="tab:red", label="Gaussian fit")
            axes[1].plot(x, GBweight_y, color="tab:red", label="Gaussian fit")

            axes[0].set_title("Efficiency")
            axes[1].set_title("GBweight")
            axes[0].set_ylim(bottom=0)
            axes[1].set_ylim(bottom=0)
            axes[1].legend(loc="upper left")

            efficiency_text = get_text(info_dict[f"efficiency_{ndim}D"])
            GBweight_text = get_text(info_dict[f"GBweight_{ndim}D"])

            # Add a text box
            axes[0].text(0.95, 0.95, efficiency_text,
                         ha="right", va="top",
                         transform=axes[0].transAxes, 
                         bbox={ "boxstyle" : "round", 
                                "facecolor" : "wheat",
                                "alpha" : 0.2 
                              } )
            axes[1].text(0.95, 0.95, GBweight_text,
                         ha="right", va="top",
                         transform=axes[1].transAxes, 
                         bbox={ "boxstyle" : "round", 
                                "facecolor" : "wheat",
                                "alpha" : 0.2 
                              } )

            figure.suptitle(title)
            plt.tight_layout()
            pdf.savefig(figure)
            plt.clf()
            plt.close()
