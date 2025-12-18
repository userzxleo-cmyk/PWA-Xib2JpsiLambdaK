import os, sys, argparse
import array

import ROOT as r

def parse_args():
    parser = argparse.ArgumentParser(description="Smear mJpsip.")
    parser.add_argument("--input", type=str, required=True, help="Path to input nTuple.")
    parser.add_argument("--output", type=str, required=True, help="Path to output nTuple.")
    parser.add_argument("--treename", type=str, default="DTree", help="Name of TTree.")
    parser.add_argument("--seed", type=int, required=True, help="Seed to smear m(Jpsip).")
    args = parser.parse_args()

    return args

def get_resolution(mass):
    mres0 = -0.134064 ;
    mres1 =  0.0599014;
    mres2 = -0.00655841;
    return mres0 + mres1*mass + mres2*mass*mass

if __name__ == "__main__":
    args = parse_args()

    chain = r.TChain(args.treename)
    chain.AddFile(args.input)

    generator = r.TRandom3(seed=args.seed)
    mjpsip = array.array('d',[0])

    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    output_file = r.TFile(args.output, "RECREATE")
    tree = chain.CloneTree(0)
    branch = chain.SetBranchAddress("mjpsip", mjpsip)

    for i in range(chain.GetEntries()):
        chain.GetEntry(i)
        resolution = get_resolution(mjpsip[0])
        mjpsip[0] += generator.Gaus(0., resolution)
        tree.Fill()

    tree.Write()
    output_file.Close()
