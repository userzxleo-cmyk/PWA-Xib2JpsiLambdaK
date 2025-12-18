import os, argparse

import ROOT as r

def parse_args():
    parser = argparse.ArgumentParser(description="Filter ROOT file to be consistent with cuts in amplitude fit framework.")
    parser.add_argument("--input", type=str, required=True, help="Path to input nTuple.")
    parser.add_argument("--output", type=str, required=True, help="Path to output nTuple.")
    parser.add_argument("--treename", type=str, default="tree", help="Name of TTree.")
    args = parser.parse_args()

    return args

if __name__ == "__main__":
    args = parse_args()

    dataframe = r.RDataFrame(args.treename, args.input)
    dataframe = dataframe.Filter("mkp>1.431950001&&mkp<2.522583999999&&mjpsip>4.0351880460&&mjpsip<5.125823")
    dataframe.Snapshot(args.treename, args.output)
