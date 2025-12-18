import os, argparse, re
from typing import List

import ROOT as r
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors


def parse_args():
    parser = argparse.ArgumentParser(
        description="Plot number of function calls to reach minima."
    )
    parser.add_argument(
        "--result_dir",
        type=str,
        required=True,
        help="Path to directory containing RooFitResult.",
    )
    parser.add_argument(
        "--regex", type=str, required=True, help="Regular expression to match to files."
    )
    parser.add_argument(
        "--history_dir",
        type=str,
        required=True,
        help="Path to directory containing fit histories.",
    )
    parser.add_argument(
        "--savepath", type=str, required=True, help="Path to store plots."
    )
    args = parser.parse_args()
    return args


def get_status_and_nll(fitresults: List[str]):
    statuses = list()
    nlls = list()
    for path in fitresults:
        file = r.TFile.Open(path)
        fitresult = file.Get("nll")
        statuses.append(fitresult.status() == 0)
        nlls.append(fitresult.minNll())

    return np.array(statuses), np.array(nlls)


def get_nFCN(histories: List[str]):
    nFCNs = list()
    nFCNs_last_half_nll = list()

    for path in histories:
        dataframe = r.RDataFrame("fit_history", path)
        columns = dataframe.AsNumpy(["FCN", "nFCN"])
        nFCNs.append(columns["nFCN"][-1])
        threshold = columns["FCN"][-1] + 0.5
        index = np.searchsorted(-columns["FCN"], -threshold)
        nFCN_for_last_half_nll = columns["nFCN"][-1] - columns["nFCN"][index]
        nFCNs_last_half_nll.append(nFCN_for_last_half_nll)

    return np.array(nFCNs), np.array(nFCNs_last_half_nll)


def plot_nFCN_vs_nll(nlls, nFCN, nFCN_last_half_nll, statuses, savepath):
    indices = nlls > -178600
    nlls = nlls[indices]
    nFCN = nFCN[indices]
    nFCN_last_half_nll = nFCN_last_half_nll[indices]
    statuses = statuses[indices]

    two_delta_nlls = 2 * (np.amin(nlls) - nlls)

    plt.rcParams["font.size"] = 16

    figure, axes = plt.subplots(2, 1, sharex=True, figsize=(9, 12))
    figure.subplots_adjust(hspace=0)

    axes[0].scatter(
        two_delta_nlls[statuses],
        nFCN[statuses],
        color="blue",
        label="Converged",
    )
    axes[0].scatter(
        two_delta_nlls[np.logical_not(statuses)],
        nFCN[np.logical_not(statuses)],
        color="red",
        label="Failed",
    )
    axes[1].scatter(
        two_delta_nlls[statuses],
        nFCN_last_half_nll[statuses],
        color="blue",
        label="Converged",
    )
    axes[1].scatter(
        two_delta_nlls[np.logical_not(statuses)],
        nFCN_last_half_nll[np.logical_not(statuses)],
        color="red",
        label="Failed",
    )
    axes[1].set_xlabel("$2\\Delta$log$\\mathcal{L}_{min}$")
    axes[0].set_ylabel("Function calls to minima.")
    axes[1].set_ylabel("Function calls for last 0.5 of -log$\\mathcal{L}$.")
    axes[0].legend()
    figure.tight_layout()
    figure.savefig(savepath)
    plt.clf()
    plt.close()


def main():
    args = parse_args()
    pattern = re.compile(args.regex)

    files = os.listdir(args.result_dir)
    tags = list()

    for path in files:
        tag = os.path.splitext(path)[0]
        if pattern.match(path):
            tags.append(tag)

    fitresults = [os.path.join(args.result_dir, f"{tag}.root") for tag in tags]
    histories = [os.path.join(args.history_dir, f"{tag}.root") for tag in tags]
    statuses, nlls = get_status_and_nll(fitresults)
    nFCN, nFCN_last_half_nll = get_nFCN(histories)
    plot_nFCN_vs_nll(nlls, nFCN, nFCN_last_half_nll, statuses, args.savepath)


if __name__ == "__main__":
    main()
