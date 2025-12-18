import argparse, json
from typing import List

import ROOT as r
import numpy as np

from common.kdtree import Node, KDTree

def split_sample_with_kdtree(
    kdtree: KDTree, variables: List[np.array], weights: np.array
):
    def recurse(node: Node, variables: List[np.array], weights: np.array):
        if node.splitable:
            left_tree = node.less
            right_tree = node.greater

            variable = variables[node.split_dim]
            left_indices = variable <= node.split
            right_indices = variable > node.split

            left_variables = [ variable[left_indices] for variable in variables ]
            right_variables = [ variable[right_indices] for variable in variables ]

            left_weights = weights[left_indices]
            right_weights = weights[right_indices]

            # Recursively split the left and right bins
            left_binned_weights = recurse(
                left_tree, left_variables, left_weights
            )
            right_binned_weights = recurse(
                right_tree, right_variables, right_weights
            )

            binned_weights = left_binned_weights + right_binned_weights
        else:
            binned_weights = [weights]

        return binned_weights

    per_bin_weights = recurse(kdtree.tree, variables, weights)
    return per_bin_weights

def parse_args():
    parser = argparse.ArgumentParser(description="Script to evaluate 6D chi2.")
    parser.add_argument(
        "--mc_weights", type=str, required=True, help="Path to MC weight file."
    )
    parser.add_argument(
        "--data_root",
        type=str,
        default="sPlot.root",
        help="Path to data variables ROOT.",
    )
    parser.add_argument(
        "--mc_root", type=str, default="mcsw.root", help="Path to MC variables ROOT."
    )
    parser.add_argument("--outfile", type=str, required=True, help="Output JSON file to store results.")
    parser.add_argument("--nparams", type=int, required=True, help="Number free parameters in the fit.")
    parser.add_argument(
        "--min_sig",
        type=float,
        default=5.,
        help="Minimum significance in each bin."
    )

    args = parser.parse_args()
    return args

def calculate_chi2(data_weights: np.array, phsp_weights: np.array):
    difference = np.sum(phsp_weights) - np.sum(data_weights)
    variance = np.sum(np.square(phsp_weights)) + np.sum(np.square(data_weights))

    return np.square(difference) / variance

if __name__ == "__main__":
    args = parse_args()

    # variables = ["mkp", "mjpsip"]
    variables = ["mkp", "mjpsip", "cosTheta_L", "cosTheta_Jpsi", "cosTheta_Lb", "phiMu"]

    dataframe = r.RDataFrame("DTree", args.data_root)
    dataframe = dataframe.Filter(
        "mkp>1.431950001&&mkp<2.522583999999&&mjpsip>4.0351880460&&mjpsip<5.125823"
    )
    data_columns = dataframe.AsNumpy(variables + ["nsig_sw"])
    # data_columns = dataframe.AsNumpy(variables + ["nsig_sw", "Lb_LOKI_MASS_JpsiConstr"])
    # data_selection = (
    #     np.abs(data_columns["Lb_LOKI_MASS_JpsiConstr"] - 5619.6) < 20
    # )

    dataframe = r.RDataFrame("h1", args.mc_root)
    dataframe = dataframe.Filter(
        " mkp >= (0.493677 + 0.938272046) && mkp <= (5.61951 - 3.096916) && mjpsip >= (3.096916 + 0.938272046) && (mjpsip + 0.493677) <= 5.61951"
    )
    phsp_columns = dataframe.AsNumpy(variables)

    data_variables = [ data_columns[name] for name in variables ]
    phsp_variables = [ phsp_columns[name] for name in variables ]

    dataframe = r.RDataFrame("tree", args.mc_weights)
    phsp_weights = dataframe.AsNumpy(["amp2_weight"])["amp2_weight"]
    data_weights = data_columns["nsig_sw"]

    phsp_scale = np.sum(data_weights) / np.sum(phsp_weights)
    phsp_weights = phsp_scale * phsp_weights

    # selected_data_variables = [ variable[data_selection] for variable in data_variables ]
    # selected_data_weights = np.ones_like(selected_data_variables[0])
    # kdtree = KDTree(selected_data_variables, selected_data_weights, args.min_sig)

    kdtree = KDTree(data_variables, data_weights, args.min_sig)
    data_binned_weights = split_sample_with_kdtree(kdtree, data_variables, data_weights)
    phsp_binned_weights = split_sample_with_kdtree(kdtree, phsp_variables, phsp_weights)

    per_bin_chi2 = [ calculate_chi2(w_data, w_phsp) for w_data, w_phsp in zip(data_binned_weights, phsp_binned_weights) ]
    chi2 = np.sum(per_bin_chi2)
    ndof = len(per_bin_chi2) - args.nparams
    chi2ndof = chi2/ndof

    output_dict = {
        "chi2" : chi2,
        "ndof" : ndof,
        "chi2ndof" : chi2ndof
    }

    with open(args.outfile, "w") as file:
        file.write(json.dumps(output_dict, indent=2))
