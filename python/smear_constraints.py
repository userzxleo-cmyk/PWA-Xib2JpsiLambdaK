import argparse, random, os

import numpy as np
import ROOT as r


def parse_args():
    parser = argparse.ArgumentParser(
        description="Plot mass distribution based on amplitude weights."
    )
    parser.add_argument(
        "--fitresult", type=str, required=True, help="Path RooFitResult to smear from."
    )
    parser.add_argument(
        "--constraint",
        type=str,
        required=True,
        help="Path to constraint file used in data fit.",
    )
    parser.add_argument("--outdir", type=str, required=True, help="Output directory.")
    parser.add_argument(
        "--nFiles",
        type=int,
        default=200,
        help="Number of smeared constraints to produce.",
    )
    parser.add_argument(
        "--seed", type=int, required=True, help="Seed of random number generator."
    )
    args = parser.parse_args()
    return args


class Constraint:
    def __init__(self, line):
        setting = line.strip("\n").split()
        self.name = setting[0]
        self.left_mean = float(setting[1])
        self.right_mean = float(setting[2])
        self.left_width = float(setting[3])
        self.right_width = float(setting[4])
        self.lower_limit = float(setting[5])
        self.upper_limit = float(setting[6])

        self.left_width_string = setting[3]
        self.right_width_string = setting[4]
        self.lower_limit_string = setting[5]
        self.upper_limit_string = setting[6]

    def set_fitted_value(self, fitted_value):
        self.fitted_value = fitted_value
        self.setup_smearing_function()

    def generate_smeared_line(self, left_mean, right_mean):
        line = (
            f"{self.name} {left_mean:.7f} {right_mean:.7f}"
            + f" {self.left_width_string} {self.right_width_string}"
            + f" {self.lower_limit_string} {self.upper_limit_string}"
        )
        return line

    def gaussian_smear(self):
        assert self.left_mean == self.right_mean
        assert self.left_width == self.right_width
        smeared_value = random.gauss(self.left_mean, self.left_width)

        return ( smeared_value, smeared_value )

    def asymmetric_gaussian_smear(self):
        assert self.left_mean == self.right_mean

        normal = random.gauss()
        rng.normal(0.0, 1.0, size=10 * n_generated)
        # normal = normal[1::10]

        smeared_value = normal * self.left_width if normal < 0 else normal * self.right_width
        smeared_value = self.left_mean + smeared_value

        return ( smeared_value, smeared_value )

    def flat_top_gaussian_smear(self):
        assert (self.fitted_value < self.left_mean) or (
            self.fitted_value > self.right_mean
        )

        left_smear = self.fitted_value < self.left_mean
        mean = self.left_mean if left_smear else self.right_mean
        width = self.left_width if left_smear else self.right_width

        smeared_value = random.gauss(mean, width)
        # smeared_values = smeared_values[1::10]

        return_tuple = (
            (smeared_value, self.right_mean)
            if left_smear
            else (self.left_mean, smeared_value)
        )

        return return_tuple

    def return_fitted_value(self):
        return self.fitted_value

    def setup_smearing_function(self):
        assert hasattr(self, "fitted_value")

        same_mean = self.left_mean == self.right_mean
        same_width = self.left_width == self.right_width

        if same_mean and same_width:
            self.smearing_function = self.gaussian_smear
        elif same_mean and not same_width:
            self.smearing_function = self.asymmetric_gaussian_smear
        elif self.fitted_value < self.left_mean or self.fitted_value > self.right_mean:
            self.smearing_function = self.flat_top_gaussian_smear
        else:
            self.smearing_function = self.return_fitted_value

    def smeared_line(self):
        assert hasattr(self, "smearing_function")
        smeared_value = self.smearing_function()
        smeared_line = self.generate_smeared_line(smeared_value[0], smeared_value[1])

        return smeared_line


def load_constraints(path):
    with open(path, "r") as file:
        lines = file.readlines()

    constraints = [Constraint(line) for line in lines]
    constraints = {c.name: c for c in constraints}

    return constraints


def load_fitted_values(path, constraints):
    file = r.TFile.Open(path, "read")
    results = file.Get("nll")
    parameters = results.floatParsFinal()
    file.Close()

    constraints_in_fit = list()

    for p in parameters:
        name = p.GetName()
        if name in constraints:
            constraints[name].set_fitted_value(p.getVal())
            constraints_in_fit.append(name)

    constraints = {name: constraints[name] for name in constraints_in_fit}

    return constraints


def generate_smeared_lines(outdir, nFiles, seed, constraints):
    os.makedirs(outdir, exist_ok=True)
    random.seed(seed)

    # rng = np.random.default_rng(seed=seed)
    # constraint_lines = [c.smeared_line(nFiles, rng) for c in constraints.values()]

    for i in range(nFiles):
        with open(os.path.join(outdir, f"toy_constraint_{i}.err"), "w") as file:
            for constraint in constraints.values():
                file.write(f"{constraint.smeared_line()}\n")
            file.close()


if __name__ == "__main__":
    args = parse_args()

    constraints = load_constraints(args.constraint)
    constraints = load_fitted_values(args.fitresult, constraints)
    generate_smeared_lines(args.outdir, args.nFiles, args.seed, constraints)
