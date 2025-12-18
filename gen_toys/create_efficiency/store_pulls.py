import os, argparse, yaml

import ROOT as r
import numpy as np
from scipy.spatial import KDTree
from typing import Dict, List, Optional

def parse_args():
    parser = argparse.ArgumentParser(description="Store chi2 pulls of toy vs full sim.")
    parser.add_argument("--mcsw", type=str, required=True, help="Path to mcsw.root.")
    parser.add_argument("--efficiency", type=str, required=True, nargs="+", help="Path to efficiency hit-and-reject nTuples.")
    parser.add_argument("--GBweight", type=str, required=True, nargs="+", help="Path to GBreweight hit-and-reject nTuples.")
    parser.add_argument("--yaml", type=str, required=True, help="Path to YAML with options for plotting.")
    parser.add_argument("--nbins", type=int, required=True, help="Number of bins for pull plots.")
    parser.add_argument("--output_path", type=str, required=True, help="Path to store plots.")

    args = parser.parse_args()

    return args

def get_kdtree(columns : Dict[str, np.array], 
               variables : List[str], 
               leafsize : int):
    kdtree_data = np.stack([columns[variable] for variable in variables], axis=-1)
    kdtree = KDTree(kdtree_data, leafsize)

    return kdtree

def split_sample_with_kdtree(kdtree : KDTree,
                             columns : Dict[str, np.array],
                             variables : List[str],
                             weight : Optional[np.array] = None):

    def recurse(node, node_columns, weight, variables):
        if hasattr(node, 'split'):
            left_tree = node.less
            right_tree = node.greater

            variable = variables[node.split_dim]

            left_split_indices = node_columns[variable] <= node.split
            right_split_indices = node_columns[variable] > node.split

            left_bin_columns = { key : value[left_split_indices] for key, value in node_columns.items() }
            right_bin_columns = { key : value[right_split_indices] for key, value in node_columns.items() }

            left_weight = weight[left_split_indices]
            right_weight = weight[right_split_indices]

            # Recursively split the left and right bins
            left_bin_weights = recurse(left_tree, left_bin_columns, left_weight, variables)
            right_bin_weights = recurse(right_tree, right_bin_columns, right_weight, variables)

            weights = left_bin_weights + right_bin_weights
        else:
            weights = [weight]

        return weights

    columns = { variable : columns[variable] for variable in variables }
    if weight is None:
        weight = np.ones_like(columns[variables[0]])

    splitted_weights = recurse(kdtree.tree, columns, weight, variables)

    return splitted_weights

def calculate_pulls(weightsA, weightsB):
    assert(len(weightsA) == len(weightsB))

    # Normalise to 1 for A and B
    normA = np.sum([ np.sum(weights) for weights in weightsA ])
    normB = np.sum([ np.sum(weights) for weights in weightsB ])
    weightsA = [ weights / normA for weights in weightsA ]
    weightsB = [ weights / normB for weights in weightsB ]

    per_bin_weightsA = np.array([ np.sum(weights) for weights in weightsA ])
    per_bin_weightsB = np.array([ np.sum(weights) for weights in weightsB ])

    # Per-bin variance is the sum of weight squared
    per_bin_varianceA = np.array([ np.sum(np.square(weights)) for weights in weightsA ])
    per_bin_varianceB = np.array([ np.sum(np.square(weights)) for weights in weightsB ])

    per_bin_error = np.sqrt(per_bin_varianceA + per_bin_varianceB)
    per_bin_pull = (per_bin_weightsA - per_bin_weightsB) / per_bin_error

    return per_bin_pull

def parse_yaml(yaml_file):
    with open(yaml_file, "r") as file:
        options = yaml.load(file, Loader=yaml.Loader)
        file.close()
    return options

if __name__ == "__main__":
    args = parse_args()
    options = parse_yaml(args.yaml)
    os.makedirs(os.path.dirname(args.output_path), exist_ok=True)

    mc_dataframe = r.RDataFrame("h1", args.mcsw)
    efficiency_chain = r.TChain("tree", "")
    GBweight_chain = r.TChain("tree", "")
    for path in args.efficiency:
        efficiency_chain.AddFile(path)
    for path in args.GBweight:
        GBweight_chain.AddFile(path)

    variables = [ variable for variable in options["variables"].keys() ]
    labels = options["variables"]
    assert(len(variables) == 6)

    mc_dataframe = r.RDataFrame("h1", args.mcsw)
    efficiency_dataframe = r.RDataFrame(efficiency_chain)
    GBweight_dataframe = r.RDataFrame(GBweight_chain)

    mc_columns = mc_dataframe.AsNumpy(variables + ["sw"])
    efficiency_columns = efficiency_dataframe.AsNumpy(variables)
    GBweight_columns = GBweight_dataframe.AsNumpy(variables)

    # Approximately number of events in GBweight / number of bins
    # We use GBweight to create the KDTree
    leafsize = GBweight_dataframe.Count().GetValue() // args.nbins

    pulls_to_store = dict()

    # Calculate pulls from 2D to 6D and store them in an npz for easier access while plotting
    for i in range(2, 7):
        pull_variables = variables[:i]

        kdtree = get_kdtree(GBweight_columns, pull_variables, leafsize)
        mc_weights = split_sample_with_kdtree(kdtree, mc_columns, pull_variables, mc_columns["sw"])
        efficiency_weights = split_sample_with_kdtree(kdtree, efficiency_columns, pull_variables)
        GBweight_weights = split_sample_with_kdtree(kdtree, GBweight_columns, pull_variables)

        efficiency_pulls = calculate_pulls(efficiency_weights, mc_weights)
        GBweight_pulls = calculate_pulls(GBweight_weights, mc_weights)

        pulls_to_store[f"efficiency_{i}D"] = efficiency_pulls
        pulls_to_store[f"GBweight_{i}D"] = GBweight_pulls

    os.makedirs(os.path.dirname(args.output_path), exist_ok=True)
    np.savez(args.output_path, **pulls_to_store)
