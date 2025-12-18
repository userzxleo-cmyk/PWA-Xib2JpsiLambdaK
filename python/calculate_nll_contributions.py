import ROOT as r
import numpy as np
import argparse

def parse_args():
    parser = argparse.ArgumentParser(description="Calculates and prints the data and MC contribution to NLL.")
    parser.add_argument("--data", type=str, required=True, help="Path to data weight nTuple.")
    parser.add_argument("--mc", type=str, required=True, help="Path to MC weight nTuple.")

    args = parser.parse_args()
    return args

def main():
    args = parse_args()
    data_dataframe = r.RDataFrame("tree", args.data)
    mc_dataframe = r.RDataFrame("tree", args.mc)

    data_columns = data_dataframe.AsNumpy( ["amp2", "weight"] )
    mc_columns = mc_dataframe.AsNumpy( ["amp2_weight", "weight"] )

    sumW_data = np.sum(data_columns["weight"])
    sumW2_data = np.sum(np.square(data_columns["weight"]))

    sumW_mc = np.sum(mc_columns["weight"])

    alpha = sumW_data / sumW2_data

    data_nll = alpha * np.sum( data_columns["weight"] * np.where( data_columns["amp2"] > 0, np.log(data_columns["amp2"] ), np.zeros_like(data_columns["amp2"]) ) )
    mc_nll = alpha * sumW_data * np.log( np.sum( mc_columns["amp2_weight"] ) / sumW_mc )

    print(f"Data contribution to nll is -{data_nll:.10}.")
    print(f"MC contribution to nll is   {mc_nll:.10}.")
    print(f"Total contribution to nll is  {mc_nll - data_nll:.10}.")

if __name__ == "__main__":
    main()

