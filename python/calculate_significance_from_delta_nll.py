import argparse, math
import scipy.stats

def parse_args():
    parser = argparse.ArgumentParser(
        description="Calculate significance from delta nll."
    )
    parser.add_argument(
        "delta", type=float, help="The delta log-likelihood."
    )
    parser.add_argument(
        "dof", type=int, help="The difference in degree of freedom."
    )

    args = parser.parse_args()
    return args

if __name__ == "__main__":
    args = parse_args()

    p_value = scipy.stats.chi2.sf(2 * args.delta, args.dof)
    significance = scipy.stats.norm.isf(p_value)
    print(significance)
