import argparse, os, json

import ROOT as r

def parse_args():
    parser = argparse.ArgumentParser(description="Split amplitude weighted data into toys.")
    parser.add_argument("--json", type=str, required=True, help="JSON with list of files and indices.")
    parser.add_argument("--treename", type=str, default="tree", help="Name of input TTree.")
    parser.add_argument("--out_treename", type=str, default=None, help="Name of output TTree.")
    parser.add_argument("--output", type=str, required=True, help="Path to output nTuple.")
    args = parser.parse_args()

    return args

if __name__ == "__main__":
    args = parse_args()

    with open(args.json, "r") as file:
        options = json.load(file)
        file.close()

    chain = r.TChain(args.treename)
    for path in options["files"]:
        chain.AddFile(path)

    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    outfile = r.TFile.Open(args.output, "RECREATE")
    outtree = chain.CloneTree(0)
    if args.out_treename is not None:
        outtree.SetName(args.out_treename)
    for i in range(options["start"], options["end"]):
        chain.GetEntry(i)
        outtree.Fill()
    outtree.Write()
    outfile.Close()
