import argparse, os, json

import ROOT as r
import numpy as np

def parse_args():
    parser = argparse.ArgumentParser(description="Hit and reject nTuples.")
    parser.add_argument("--input", type=str, required=True, help="Path to input nTuple.")
    parser.add_argument("--friends", type=str, nargs="*", default=list(), help="Path to friend files.")
    parser.add_argument("--treename", type=str, default="tree", help="Name of input TTree.")
    parser.add_argument("--output", type=str, required=True, help="Path to output nTuple.")
    parser.add_argument("--seed", type=int, required=True, help="Seed of random number generator.")
    parser.add_argument("--hit_and_reject_weight", type=str, required=True, nargs="+", help="Branches to hit and reject on.")
    parser.add_argument("--max_weight_json", type=str, required=True, help="Path to JSON storing the maximum weight value.")
    args = parser.parse_args()

    return args

if __name__ == "__main__":
    args = parse_args()

    main_chain = r.TChain(args.treename, "")
    main_chain.AddFile(args.input)

    # Use a separate chain for RDataFrame so that we do not save the reference to friends
    dframe_chain = r.TChain(args.treename, "")
    dframe_chain.AddFile(args.input)
    friend_chains = [ r.TChain(args.treename, "") for path in args.friends ]
    for chain, path in zip(friend_chains, args.friends):
        chain.AddFile(path)
        dframe_chain.AddFriend(chain)

    # Either we take product of all weights or take the single weight branch
    hit_and_reject_weights = r.RDataFrame(dframe_chain).AsNumpy(args.hit_and_reject_weight)
    if len(args.hit_and_reject_weight) > 1:
        hit_and_reject_weights = np.prod(np.stack([ weights for weights in hit_and_reject_weights.values() ], axis=0), axis=0)
    else:
        hit_and_reject_weights = hit_and_reject_weights[args.hit_and_reject_weight[0]]

    with open(args.max_weight_json, "r") as file:
        maximum = json.load(file)["maximum"]

    # Normalise to maximum
    hit_and_reject_weights = hit_and_reject_weights/maximum
    generator = np.random.default_rng(seed=args.seed)
    should_keep = hit_and_reject_weights > generator.uniform(size=hit_and_reject_weights.size)

    print(f"Running hit-and-reject on {args.input}.")
    print(f"Maximum weight is {np.amax(hit_and_reject_weights)}")
    print(f"Will save output to {args.output}.")
    print(f"Hit and save on variables {args.hit_and_reject_weight}")
    print(f"Keeping {np.sum(should_keep)}/{should_keep.size} Events.")

    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    file = r.TFile(args.output, "RECREATE")
    tree = main_chain.CloneTree(0)

    for i, keep in enumerate(should_keep):
        if keep:
            main_chain.GetEntry(i)
            tree.Fill()
    tree.Write()
    file.Close()
