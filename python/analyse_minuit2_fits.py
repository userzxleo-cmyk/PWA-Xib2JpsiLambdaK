import os, re, time, argparse, functools
from typing import List

import ROOT as r
from prettytable import PrettyTable
import pandas as pd


class Parameter:
    def __init__(self, variable):
        self.name = variable.GetName()
        self.value = variable.getVal()
        self.error = variable.getError()
        self.lower_limit = variable.getMin() if variable.hasMin() else None
        self.upper_limit = variable.getMax() if variable.hasMax() else None


class Constraint:
    def __init__(self, name, lower_mean, upper_mean, lower_sigma, upper_sigma):
        if lower_sigma <= 0:
            raise ValueError(
                f"Constraint <{name}> has a lower sigma less than 0: {lower_sigma:.4}"
            )
        if upper_sigma <= 0:
            raise ValueError(
                f"Constraint <{name}> has a lower sigma less than 0: {upper_sigma:.4}"
            )
        if lower_mean > upper_mean:
            raise ValueError(f"Constraint <{name}> has lower_mean > upper_mean!")

        self.name = name
        self.lower_mean = lower_mean
        self.upper_mean = upper_mean
        self.lower_sigma = lower_sigma
        self.upper_sigma = upper_sigma

    def get_deviation(self, value):
        if value < self.lower_mean:
            deviation = (value - self.lower_mean) / self.lower_sigma
        elif value > self.upper_mean:
            deviation = (value - self.upper_mean) / self.upper_sigma
        else:
            deviation = 0  # If upper_mean != lower_mean, there is a region in the middle with no Gaussian penalty
        return deviation


def parse_args():
    parser = argparse.ArgumentParser(
        description="Analyse the fit logs and RooFitResult."
    )
    parser.add_argument(
        "--fitresult", type=str, required=True, help="Path to fit result ROOT file."
    )
    parser.add_argument(
        "--stderr", type=str, required=True, help="Path to fit log file (stderr)."
    )
    parser.add_argument(
        "--constraint",
        type=str,
        required=True,
        help="Path to file containing Gaussian constraint.",
    )
    parser.add_argument(
        "--output",
        type=str,
        required=True,
        help="Path to output file.",
    )

    args = parser.parse_args()
    return args


def parse_iterations(lines: List[str]) -> pd.DataFrame:
    prefix = r"Info in <Minuit2>: VariableMetricBuilder\s+"
    info_string = (
        r"([0-9]+) \- FCN = \s+(\-?[0-9]+\.[0-9]+)\s+"
        + r"Edm = \s+([0-9]+\.[0-9]+) NCalls =\s+([0-9]+)"
    )
    iteration_pattern = re.compile(prefix + info_string)

    iteration = list()
    FCN_history = list()
    EDM_history = list()
    nEval = list()

    for i, line in enumerate(lines):
        match = iteration_pattern.match(line)
        if match:
            iteration.append(int(match.group(1)))
            FCN_history.append(float(match.group(2)))
            EDM_history.append(float(match.group(3)))
            nEval.append(float(match.group(4)))

    dataframe = pd.DataFrame(
        {"FCN": FCN_history, "EDM": EDM_history, "nEval": nEval}, index=iteration
    )

    return dataframe


def parse_gradient(lines: List[str]) -> pd.DataFrame:
    gradient_header = re.compile(r"\s+Parameter\s+Gradient\s+g2\s+step")
    last_gradient_line = 0
    for i in reversed(range(len(lines))):
        if gradient_header.match(lines[i]):
            last_gradient_line = i
            break

    names = list()
    gradient = list()
    second_derivative = list()
    step_size = list()

    float_pattern = r"(\-?[0-9]+\.[0-9]+|\-?[0-9]\.[0-9]+e\-?[0-9]+)"
    parameter_pattern = re.compile(
        r"\s+(\w+)\s+" + float_pattern + r"\s+" + float_pattern + r"\s+" + float_pattern
    )
    for line in lines[last_gradient_line + 1 :]:
        match = parameter_pattern.match(line)
        if match:
            names.append(match.group(1))
            gradient.append(float(match.group(2)))
            second_derivative.append(float(match.group(3)))
            step_size.append(float(match.group(4)))
        else:
            print("Stop at line", line)
            break  # Stop processing once we have gotten all the gradients

    gradient_at_minima = pd.DataFrame(
        {
            "gradient": gradient,
            "second_derivative": second_derivative,
            "step_size": step_size,
        },
        index=names,
    )

    return gradient_at_minima


def parse_stderr(filepath):
    with open(filepath, "r") as file:
        lines = file.readlines()

    history = parse_iterations(lines)
    gradient_at_minima = parse_gradient(lines)

    return history, gradient_at_minima


def parse_fitresult(filepath):
    file = r.TFile.Open(filepath, "read")
    fitresult = file.Get("nll")

    status = fitresult.status()
    covariance_quality = fitresult.covQual()
    parameters = [Parameter(variable) for variable in fitresult.floatParsFinal()]
    parameters = {parameter.name: parameter for parameter in parameters}

    return status, covariance_quality, parameters


def parse_gaussian_constraint(filepath):
    with open(filepath, "r") as file:
        lines = file.readlines()
    lines = [line.split() for line in lines]
    constraints = [
        Constraint(
            line[0], float(line[1]), float(line[2]), float(line[3]), float(line[4])
        )
        for line in lines
    ]
    constraints = {constraint.name: constraint for constraint in constraints}

    return constraints


def get_history_table(history: pd.DataFrame) -> PrettyTable:
    table = PrettyTable()
    table.field_names = ["Iteration"] + history.columns.to_list()
    for name, row in history.iterrows():
        table.add_row([name] + row.to_list())

    return table


def get_limits(parameter, step):
    value = parameter.value
    error = parameter.error
    has_lower_limit = parameter.lower_limit is not None
    has_upper_limit = parameter.upper_limit is not None

    lower_limit = parameter.lower_limit if has_lower_limit else "-"
    upper_limit = parameter.upper_limit if has_upper_limit else "-"

    distance_to_lower_limit = (value - lower_limit) if has_lower_limit else None
    distance_to_upper_limit = (upper_limit - value) if has_upper_limit else None

    if has_lower_limit and has_upper_limit:
        distance_to_limit = min(distance_to_lower_limit, distance_to_upper_limit)
    elif has_lower_limit and not has_upper_limit:
        distance_to_limit = distance_to_lower_limit
    elif not has_lower_limit and has_upper_limit:
        distance_to_limit = distance_to_upper_limit

    if has_upper_limit or has_lower_limit:
        nErrors_to_limit = distance_to_limit / error
        nSteps_to_limit = distance_to_limit / step
    else:
        nErrors_to_limit = "--"
        nSteps_to_limit = "--"

    return lower_limit, upper_limit, nErrors_to_limit, nSteps_to_limit


def format_row(parameter, FD):
    value = parameter.value
    error = parameter.error

    lower_limit = f"parameter.lower_limit" if parameter.lower_limit is not None else "-"
    upper_limit = f"parameter.upper_limit" if parameter.upper_limit is not None else "-"

    distance_to_lower_limit = (
        (value - lower_limit) if parameter.lower_limit is not None else None
    )
    distance_to_upper_limit = (
        (upper_limit - value) if parameter.upper_limit is not None else None
    )

    distance_to_limit = min()


def get_empty_table():
    parameter_table = PrettyTable()
    parameter_table.field_names = [
        "name",
        "value",
        "error",
        "error/value",
        "gradient",
        "grad/value",
        "min limit",
        "max limit",
        "dlim/error",
        "dlim/step",
        "constraint",
        "d2f/dx2",
        "step",
    ]

    return parameter_table


def get_parameter_table(parameters, gradient_at_minima, constraints, n_worst):
    # Find parameters with worst gradients
    gradients = gradient_at_minima.loc[:, "gradient"]
    values = pd.Series(
        {key: parameter.value for key, parameter in parameters.items()},
        index=[key for key in parameters.keys()],
    )
    relative_gradient = gradients / values
    sorted_indices = (
        relative_gradient.abs().mul(-1).argsort()
    )  # Multiply by -1 to sort by ascending order

    names = relative_gradient.index
    worst_gradients = {names[sorted_indices.iloc[i]]: None for i in range(n_worst)}

    parameter_table = get_empty_table()

    for name in names:
        parameter = parameters[name]
        value = parameter.value
        error = parameter.error

        series = gradient_at_minima.loc[name]
        gradient = series.loc["gradient"]
        second_derivative = series.loc["second_derivative"]
        step = series.loc["step_size"]

        if value != 0:
            relative_FD = gradient / value
            relative_error = error / value
        else:
            relative_FD = "NA"
            relative_error = "NA"

        (
            lower_limit,
            upper_limit,
            nErrors_to_limit,
            nSteps_to_limit,
        ) = get_limits(parameter, step)
        if name in constraints:
            deviation = constraints[name].get_deviation(value)
        else:
            deviation = "-"

        row = [
            name,
            value,
            error,
            relative_error,
            gradient,
            relative_FD,
            lower_limit,
            upper_limit,
            nErrors_to_limit,
            nSteps_to_limit,
            deviation,
            second_derivative,
            step,
        ]

        parameter_table.add_row(row)
        if name in worst_gradients:
            worst_gradients[name] = row

    parameter_table.float_format = ".4"
    parameter_table.align = "r"
    parameter_table.align["name"] = "c"

    worst_table = get_empty_table()
    for i in range(n_worst):
        index = sorted_indices.iloc[i]
        name = names[index]
        worst_table.add_row(worst_gradients[name])
    worst_table.float_format = ".4"
    worst_table.align = "r"
    worst_table.align["name"] = "c"

    return parameter_table, worst_table


if __name__ == "__main__":
    args = parse_args()
    n_worst = 30

    history, gradient_at_minima = parse_stderr(args.stderr)
    status, covariance_quality, parameters = parse_fitresult(args.fitresult)
    constraints = parse_gaussian_constraint(args.constraint)

    history_table = get_history_table(history)
    parameter_table, worst_table = get_parameter_table(
        parameters, gradient_at_minima, constraints, n_worst
    )

    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    with open(args.output, "w") as file:
        file.write(f"Fit Status         : {status}\n")
        file.write(f"Covariance Quality : {covariance_quality}\n")
        file.write("\n" * 5)
        file.write(history_table.get_string())
        file.write("\n" * 5)
        file.write(parameter_table.get_string())
        file.write("\n" * 5)
        file.write(f"Table with {n_worst} parameters with worst gradients:\n")
        file.write(worst_table.get_string())
        file.write("\n")
