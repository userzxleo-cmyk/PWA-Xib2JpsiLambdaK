import os, re, time, argparse, functools, datetime
from typing import List

import ROOT as r
from prettytable import PrettyTable
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

from matplotlib.backends.backend_pdf import PdfPages


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
        "--fitresult",
        type=str,
        required=True,
        help="Path to fit result ROOT file.",
    )
    parser.add_argument(
        "--history",
        type=str,
        required=True,
        help="Path to fit history.",
    )
    parser.add_argument(
        "--constraint",
        type=str,
        required=True,
        help="Path to file containing Gaussian constraint.",
    )
    parser.add_argument(
        "--pdf",
        type=str,
        required=True,
        help="Path to store plots.",
    )
    parser.add_argument(
        "--table",
        type=str,
        required=True,
        help="Path to store table in text form.",
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


def parse_history(filepath):
    file = r.TFile.Open(filepath)

    fit_history_tree = file.Get("fit_history")
    fit_history_dframe = r.RDataFrame(fit_history_tree)
    fit_history = fit_history_dframe.AsNumpy(["FCN", "EDM", "nFCN"])
    fit_history = pd.DataFrame(fit_history)

    value_tree = file.Get("value")
    value_dframe = r.RDataFrame(value_tree)

    parameter_history = value_dframe.AsNumpy(
        [
            "m0_Z",
            "m0_Z5",
            "m0_Z4",
            "g2_Z",
            "g2_Z5",
            "g4_Z4",
            "rg1_Z",
            "rg1_Z5",
            "rg1_Z4",
            "fsc_ZM1SC1",
            "fsc_ZM1SC2",
            "fsc_ZM1SC3",
            "fsc_ZM1SC4",
            "fsc_ZM3SC1",
            "fsc_ZM3SC2",
            "fsc_ZM3SC3",
            "fsc_ZM3SC4",
        ]
    )
    parameter_history = pd.DataFrame(parameter_history)

    names = [str(name) for name in value_dframe.GetColumnNames()]

    FD_tree = file.Get("first_derivative")
    FD_dframe = r.RDataFrame(FD_tree)
    FD_history = FD_dframe.AsNumpy(names)
    FD_at_minima = [FD_history[name][-1] for name in names]

    SD_tree = file.Get("second_derivative")
    SD_dframe = r.RDataFrame(SD_tree)
    SD_history = SD_dframe.AsNumpy(names)
    SD_at_minima = [SD_history[name][-1] for name in names]

    SS_tree = file.Get("step_size")
    SS_dframe = r.RDataFrame(SS_tree)
    SS_history = SS_dframe.AsNumpy(names)
    SS_at_minima = [SS_history[name][-1] for name in names]

    derivatives_at_minima = pd.DataFrame(
        {
            "gradient": FD_at_minima,
            "second_derivative": SD_at_minima,
            "step_size": SS_at_minima,
        },
        index=names,
    )

    return fit_history, parameter_history, derivatives_at_minima


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
    else:
        nErrors_to_limit = "--"
        distance_to_limit = "--"

    return lower_limit, upper_limit, nErrors_to_limit, distance_to_limit


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
        "dlim/range",
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
            distance_to_limit,
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
            distance_to_limit,
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


def plot_history(filepath, fit_history, parameter_history):
    nIteration = len(fit_history.index)
    iteration = np.linspace(1, nIteration, nIteration, dtype=np.uint32)
    plt.ioff()
    plt.rcParams["font.size"] = 16
    with PdfPages(filepath) as pdf:
        plt.figure()
        figure, axes = plt.subplots(2, 2, figsize=(18, 12))
        delta_nll = (
            fit_history.loc[:, "FCN"] - fit_history.loc[nIteration - 1, "FCN"] + 1e-2
        )
        axes[0, 0].plot(iteration, delta_nll, "bo-")
        axes[0, 0].plot(
            iteration[-1],
            delta_nll.loc[nIteration - 1],
            "ro",
            label="min -log$(\\mathcal{L})$",
        )
        axes[0, 0].set_yscale("log")
        axes[0, 0].set_ylabel("$\\Delta$ -log$(\\mathcal{L})$")
        axes[0, 0].set_xlabel("Iteration")
        axes[0, 0].legend()
        axes[0, 1].plot(iteration, fit_history.loc[:, "EDM"], "bo-")
        axes[0, 1].axhline(3e-3, color="red", label="Convergence Criteria")
        axes[0, 1].set_yscale("log")
        axes[0, 1].set_ylabel("log(Estimated Distance to Minimum)")
        axes[0, 1].set_xlabel("Iteration")
        axes[0, 1].legend()
        axes[1, 0].plot(iteration, fit_history.loc[:, "nFCN"], "bo-")
        axes[1, 0].set_ylabel("Number of Function Calls")
        axes[1, 0].set_xlabel("Iteration")
        figure.suptitle("Fit History")
        figure.tight_layout()
        pdf.savefig(figure)
        plt.clf()
        plt.close()

        figure, axes = plt.subplots(2, 2, figsize=(18, 12))
        axes[0, 0].plot(iteration, parameter_history.loc[:, "m0_Z4"], "bo-")
        axes[0, 0].set_ylabel("$P_{c}(4312)$ Pole Mass")
        axes[0, 0].set_xlabel("Iteration")
        axes[0, 1].plot(iteration, parameter_history.loc[:, "g4_Z4"], "bo-")
        axes[0, 1].set_ylabel("$P_{c}(4312)$ $\\Sigma_{c}D$ coupling")
        axes[0, 1].set_xlabel("Iteration")
        axes[1, 0].plot(iteration, parameter_history.loc[:, "rg1_Z4"], "bo-")
        axes[1, 0].set_ylabel("$P_{c}(4312)$ $J/\\psi p$ coupling ratio")
        axes[1, 0].set_xlabel("Iteration")
        figure.suptitle("$P_{c}(4312)$")
        figure.tight_layout()
        pdf.savefig(figure)
        plt.clf()
        plt.close()

        figure, axes = plt.subplots(2, 2, figsize=(18, 12))
        axes[0, 0].plot(iteration, parameter_history.loc[:, "m0_Z5"], "bo-")
        axes[0, 0].set_ylabel("$P_{c}(4440)$ Pole Mass")
        axes[0, 0].set_xlabel("Iteration")
        axes[0, 1].plot(iteration, parameter_history.loc[:, "g2_Z5"], "bo-")
        axes[0, 1].set_ylabel("$P_{c}(4440)$ $\\Sigma_{c}D^{*}$ coupling")
        axes[0, 1].set_xlabel("Iteration")
        axes[1, 0].plot(iteration, parameter_history.loc[:, "rg1_Z5"], "bo-")
        axes[1, 0].set_ylabel("$P_{c}(4440)$ $J/\\psi p$ coupling ratio")
        axes[1, 0].set_xlabel("Iteration")
        figure.suptitle("$P_{c}(4440)$")
        figure.tight_layout()
        pdf.savefig(figure)
        plt.clf()
        plt.close()

        figure, axes = plt.subplots(2, 2, figsize=(18, 12))
        axes[0, 0].plot(iteration, parameter_history.loc[:, "m0_Z"], "bo-")
        axes[0, 0].set_ylabel("$P_{c}(4457)$ Pole Mass")
        axes[0, 0].set_xlabel("Iteration")
        axes[0, 1].plot(iteration, parameter_history.loc[:, "g2_Z"], "bo-")
        axes[0, 1].set_ylabel("$P_{c}(4457)$ $\\Sigma_{c}D^{*}$ coupling")
        axes[0, 1].set_xlabel("Iteration")
        axes[1, 0].plot(iteration, parameter_history.loc[:, "rg1_Z"], "bo-")
        axes[1, 0].set_ylabel("$P_{c}(4457)$ $J/\\psi p$ coupling ratio")
        axes[1, 0].set_xlabel("Iteration")
        figure.suptitle("$P_{c}(4457)$")
        figure.tight_layout()
        pdf.savefig(figure)
        plt.clf()
        plt.close()

        figure, axes = plt.subplots(2, 2, figsize=(18, 12))
        axes[0, 0].plot(iteration, parameter_history.loc[:, "fsc_ZM1SC1"], "bo-")
        axes[0, 0].set_ylabel("$J/\\psi p$ Coupling")
        axes[0, 0].set_xlabel("Iteration")
        axes[0, 1].plot(iteration, parameter_history.loc[:, "fsc_ZM1SC2"], "bo-")
        axes[0, 1].set_ylabel("$\\Sigma_{c}D^{*}$ Coupling")
        axes[0, 1].set_xlabel("Iteration")
        axes[1, 0].plot(iteration, parameter_history.loc[:, "fsc_ZM1SC3"], "bo-")
        axes[1, 0].set_ylabel("$\\Sigma_{c}^{*}D^{*}$ Coupling")
        axes[1, 0].set_xlabel("Iteration")
        axes[1, 1].plot(iteration, parameter_history.loc[:, "fsc_ZM1SC4"], "bo-")
        axes[1, 1].set_ylabel("$\\Sigma_{c}D$ Coupling")
        axes[1, 1].set_xlabel("Iteration")
        figure.suptitle("$\\frac{1}{2}^{-}$ $J/\\psi p$ Non-resonant")
        figure.tight_layout()
        pdf.savefig(figure)
        plt.clf()
        plt.close()

        figure, axes = plt.subplots(2, 2, figsize=(18, 12))
        axes[0, 0].plot(iteration, parameter_history.loc[:, "fsc_ZM3SC1"], "bo-")
        axes[0, 0].set_ylabel("$J/\\psi p$ Coupling")
        axes[0, 0].set_xlabel("Iteration")
        axes[0, 1].plot(iteration, parameter_history.loc[:, "fsc_ZM3SC2"], "bo-")
        axes[0, 1].set_ylabel("$\\Sigma_{c}D^{*}$ Coupling")
        axes[0, 1].set_xlabel("Iteration")
        axes[1, 0].plot(iteration, parameter_history.loc[:, "fsc_ZM3SC3"], "bo-")
        axes[1, 0].set_ylabel("$\\Sigma_{c}^{*}D^{*}$ Coupling")
        axes[1, 0].set_xlabel("Iteration")
        axes[1, 1].plot(iteration, parameter_history.loc[:, "fsc_ZM3SC4"], "bo-")
        axes[1, 1].set_ylabel("$\\Sigma_{c}^{*}D$ Coupling")
        axes[1, 1].set_xlabel("Iteration")
        figure.suptitle("$\\frac{3}{2}^{-}$ $J/\\psi p$ Non-resonant")
        figure.tight_layout()
        pdf.savefig(figure)
        plt.clf()
        plt.close()

        infodict = pdf.infodict()
        infodict["Title"] = "Fit History"
        infodict["Author"] = os.getlogin()
        infodict["CreationDate"] = datetime.datetime.today()
        infodict["ModDate"] = datetime.datetime.today()


def write_table(
    filepath,
    status,
    covariance_quality,
    history_table,
    parameter_table,
    n_worst,
    worst_table,
):
    with open(filepath, "w") as file:
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


if __name__ == "__main__":
    args = parse_args()
    n_worst = 30

    fit_history, parameter_history, derivatives_at_minima = parse_history(args.history)
    status, covariance_quality, parameters = parse_fitresult(args.fitresult)
    constraints = parse_gaussian_constraint(args.constraint)

    history_table = get_history_table(fit_history)
    parameter_table, worst_table = get_parameter_table(
        parameters, derivatives_at_minima, constraints, n_worst
    )

    os.makedirs(os.path.dirname(args.pdf), exist_ok=True)
    os.makedirs(os.path.dirname(args.table), exist_ok=True)
    plot_history(args.pdf, fit_history, parameter_history)
    write_table(
        args.table,
        status,
        covariance_quality,
        history_table,
        parameter_table,
        n_worst,
        worst_table,
    )
