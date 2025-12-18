import argparse, os, pickle, yaml, array

import ROOT as r
import numpy as np
from hep_ml import reweight

def parse_args():
    parser = argparse.ArgumentParser(description="Apply a trained GB reweighter.")
    parser.add_argument("--model_path", type=str, required=True, help="Path to store trained model.")
    parser.add_argument("--input_path", type=str, required=True, help="Path to input nTuple.")
    parser.add_argument("--treename", type=str, default="tree", help="Name of TTree in input and output nTuple.")
    parser.add_argument("--output_path", type=str, required=True, help="Path to store weights.")
    parser.add_argument("--yaml", type=str, required=True, help="Path to GB reweighter variables yaml.")
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

    with open(args.model_path, "rb") as model_file:
        model = pickle.load(model_file)

    dataframe = r.RDataFrame(args.treename, args.input_path)
    columns = dataframe.AsNumpy(options["toy_variables"])
    features = np.stack([columns[variable] for variable in options["toy_variables"]], axis=-1)

    weights = model.predict_weights(features)
    weights = weights / np.amax(weights)

    print(f"Strong GBweights for {args.input_path} to {args.output_path}.")
    print(f"Average value of GBweights is {np.mean(weights)}.")
    print(f"The median value of GBweights is {np.median(weights)}.")

    w = array.array('d', [0])
    file = r.TFile(args.output_path, "RECREATE")
    tree = r.TTree(args.treename, "")
    tree.Branch("GBweight", w, "GBweight/D")

    for weight in weights:
        w[0] = weight
        tree.Fill()
    tree.Write()
    file.Close()
