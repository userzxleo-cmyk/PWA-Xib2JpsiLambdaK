import argparse, os, json

import ROOT as r
import numpy as np

def parse_args():
    parser = argparse.ArgumentParser(description="Get maximum weight to run hit and reject on.")
    parser.add_argument("--input", type=str, required=True, help="Path to input nTuple.")
    parser.add_argument("--friends", type=str, nargs="*", default=list(), help="Path to friend files.")
    parser.add_argument("--treename", type=str, default="tree", help="Name of input TTree.")
    parser.add_argument("--output", type=str, required=True, help="Path to store maximum as JSON.")
    parser.add_argument("--hit_and_reject_weight", type=str, required=True, nargs="+", help="Branches to hit and reject on.")
    args = parser.parse_args()

    return args

if __name__ == "__main__":
    args = parse_args()

    main_chain = r.TChain(args.treename, "")
    main_chain.AddFile(args.input)

    friend_chains = list()
    for friend in args.friends:
        chain = r.TChain(args.treename, "")
        chain.AddFile(friend)
        main_chain.AddFriend(chain)
        friend_chains.append(chain)

    # Either we take product of all weights or take the single weight branch
    hit_and_reject_weights = r.RDataFrame(main_chain).AsNumpy(args.hit_and_reject_weight)
    if len(args.hit_and_reject_weight) > 1:
        hit_and_reject_weights = np.prod(np.stack([ weights for weights in hit_and_reject_weights.values() ], axis=0), axis=0)
    else:
        hit_and_reject_weights = np.array(hit_and_reject_weights[args.hit_and_reject_weight[0]])

    maximum = np.amax(hit_and_reject_weights)

    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    dump_dict = { "maximum" : maximum }
    with open(args.output, "w") as file:
        file.write(json.dumps(dump_dict, indent=2))
        file.close()
