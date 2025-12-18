import pickle, os, argparse, yaml

import ROOT as r
import numpy as np
from hep_ml import reweight

def parse_args():
    parser = argparse.ArgumentParser(description="Training a gbreweighter.")
    parser.add_argument("--MC", type=str, required=True, help="Path to MC nTuple for target distribution and weight")
    parser.add_argument("--toys", type=str, required=True, nargs="+", help="Path(s) to toy nTuple(s) for original distribution and weight.")
    parser.add_argument("--yaml", type=str, required=True, help="Path to GB reweighter variables yaml.")
    parser.add_argument("--output_path", type=str, required=True, help="Path to store trained model.")
    args = parser.parse_args()

    return args

def parse_yaml(yaml_file):
    with open(yaml_file, "r") as file:
        options = yaml.load(file, Loader=yaml.Loader)
        file.close()
    return options

if __name__ == "__main__":
    args = parse_args()
    os.makedirs(os.path.dirname(args.output_path), exist_ok=True)

    options = parse_yaml(args.yaml)

    toys_chain = r.TChain("tree")
    for path in args.toys:
        toys_chain.AddFile(path)
    toys_dataframe = r.RDataFrame(toys_chain)

    mc_dataframe = r.RDataFrame("h1", args.MC)

    print(f"training... with {toys_dataframe.Count().GetValue():d} original and {mc_dataframe.Count().GetValue():d} target")

    toys_columns = toys_dataframe.AsNumpy(options["toy_variables"] + ["sw"])
    mc_columns = mc_dataframe.AsNumpy(options["MC_variables"] + ["sw"])

    target_features = np.stack([mc_columns[variable] for variable in options["MC_variables"]], axis=-1)
    original_features = np.stack([toys_columns[variable] for variable in options["toy_variables"]], axis=-1)

    target_weights = mc_columns['sw']
    original_weights = toys_columns['sw']

    reweighter = reweight.GBReweighter(n_estimators = options["n_estimators"],
                                       learning_rate = options["learning_rate"],
                                       max_depth = options["max_depth"],
                                       min_samples_leaf = options["min_samples_leaf"],
                                       gb_args={'subsample' : options["subsample"],
                                                'max_features' : options["max_features"]})

    reweighter.fit(original_features, target_features, original_weight=original_weights, target_weight=target_weights)

    training_result = open(args.output_path, "wb")
    pickle.dump(reweighter, training_result, pickle.HIGHEST_PROTOCOL)
