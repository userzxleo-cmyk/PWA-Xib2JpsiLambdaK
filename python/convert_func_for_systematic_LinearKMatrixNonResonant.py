import argparse, itertools


def parse_args():
    parser = argparse.ArgumentParser(
        description="Convert func from N to N+1 point spline."
    )
    parser.add_argument("infile", type=str, help="Input func file with N spline nodes.")
    parser.add_argument(
        "outfile", type=str, help="Output func file with N+1 spline nodes."
    )

    args = parser.parse_args()
    return args

def add_linear_b_production_term(lines):
    new_lines = list()

    for spin, channelID in itertools.product([1, 3], range(1, 5)):
        line = f"b1prod_ZM{spin}SC{channelID} = 0.0 +/- 0.01 L(-10.0 - 10.0)\n"
        new_lines.append(line)

    return lines + new_lines

if __name__ == "__main__":
    args = parse_args()

    with open(args.infile, "r") as file:
        lines = file.readlines()
    lines = add_linear_b_production_term(lines) # linear term is only added to production

    with open(args.outfile, "w") as file:
        for line in lines:
            file.write(line)
