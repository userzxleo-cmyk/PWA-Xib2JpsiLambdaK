import argparse, os, json

import ROOT as r
import numpy as np

def parse_args():
    parser = argparse.ArgumentParser(description="Get the input files and entry numbers to split into toys.")
    parser.add_argument("--inputs", type=str, required=True, nargs="+", help="Input nTuples.")
    parser.add_argument("--treename", type=str, default="tree", help="Name of input TTree.")
    parser.add_argument("--average_nentries_toy", type=int, required=True, help="Average entries for each toy.")
    parser.add_argument("--seed", type=int, default=12345, help="Seed of random number generator.")
    parser.add_argument("--checkpoint", type=str, required=True, help="Checkpoint file to store number of output nTuples we can split into.")
    parser.add_argument("--output_dir", type=str, required=True, help="Directory to store input files and indices for each output split.")
    args = parser.parse_args()

    return args

def get_entries(treename, filepaths):
    n_entries = list()
    for path in filepaths:
        file = r.TFile.Open(path, "READ")
        if not file.IsOpen():
            raise ValueError(f"{path} cannot be opened")
        tree = file.Get(treename)
        n_entries.append(tree.GetEntries())
        file.Close()

    return np.array(n_entries)

def get_in_out_indices(average_nentries_toy, total_entries_input, seed):
    generator = np.random.default_rng(seed=seed)

    accumulated_output_entries = list()
    total_entries_output = 0
    while( total_entries_output < total_entries_input ):
        accumulated_output_entries.append( total_entries_output )
        total_entries_output += generator.poisson(lam=average_nentries_toy)

    if (len(accumulated_output_entries)) == 0:
        raise ValueError("Insufficient input entries to create even 1 output")

    accumulated_output_entries = np.array(accumulated_output_entries)
    start_indices = accumulated_output_entries[0:-1]
    end_indices = accumulated_output_entries[1:]

    return start_indices, end_indices

if __name__ == "__main__":
    args = parse_args()

    n_entries_input = get_entries(args.treename, args.inputs)
    accumulated_input = np.add.accumulate(n_entries_input)
    total_entries_input = accumulated_input[-1]

    start_indices, end_indices = get_in_out_indices(args.average_nentries_toy, total_entries_input, args.seed)

    n_output = len(start_indices)
    os.makedirs(os.path.dirname(args.checkpoint), exist_ok=True)
    with open(args.checkpoint, "w") as file:
        dump_dict = { "n_splits" : n_output }
        file.write(json.dumps(dump_dict, indent=2))
        file.close()

    print(accumulated_input)

    os.makedirs(args.output_dir, exist_ok=True)
    for i, (start, end) in enumerate(zip(start_indices, end_indices)):
        start_file_index = np.searchsorted(accumulated_input, start, side="right")
        end_file_index = np.searchsorted(accumulated_input, end, side="left")
        start_offset = accumulated_input[start_file_index - 1] if start_file_index > 0 else 0
        chain_start_index = start - start_offset
        chain_end_index = chain_start_index + ( end - start )

        files = [ args.inputs[i] for i in range(start_file_index, end_file_index + 1) ]
        dump_dict = { "files" : files, "start" : int(chain_start_index), "end" : int(chain_end_index) }

        output_path = os.path.join(args.output_dir, f"indices_{i}.json")
        with open(output_path, "w") as file:
            file.write(json.dumps(dump_dict, indent=2))
            file.close()
