import ROOT as r
from prettytable import PrettyTable

import os, re, time, argparse


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
    parser.add_argument("--log", type=str, required=True, help="Path to fit log file.")
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


def convert_derivative_string_to_float(string):
    try:
        derivative = float(string)
    except ValueError:
        derivative = "**at limit**"

    return derivative


def parse_log(filepath):
    FCN_lines = list()
    EDM_lines = list()

    FCN_pattern = re.compile(".*FCN")
    EDM_pattern = re.compile(".*EDM=")
    FD_pattern = re.compile(".*DERIVATIVE")  # FD = first derivative

    # Location where first derivatives before convergence/failure is reported
    last_first_derivative_line = 0
    N = 0

    with open(filepath, "r") as file:
        lines = file.readlines()

    for line in lines:
        if FCN_pattern.match(line):
            FCN_lines.append(line)
        if EDM_pattern.match(line):
            EDM_lines.append(line)
        if FD_pattern.match(line):
            last_first_derivative_line = N
        N += 1

    FCN_history = list()
    EDM_history = list()
    FD_info = list()
    FCN_info_pattern = re.compile(
        " FCN=(\S+)\s+FROM\s+([A-Z]+)\s+STATUS=([A-Z\s]+)[0-9]+"
    )
    EDM_info_pattern = re.compile("\s+EDM=([0-9]+\.?[0-9]*)")
    FD_info_pattern = re.compile("\s+[0-9]+\s+\w+")

    for line in EDM_lines:
        match = EDM_info_pattern.match(line)
        if match:
            EDM_history.append(match.group(1))

    for line in FCN_lines:
        match = FCN_info_pattern.match(line)
        if match:
            FCN_history.append((match.group(1), match.group(2), match.group(3)))

    for line in lines[(last_first_derivative_line + 1) :]:
        match = FD_info_pattern.match(line)
        if match:
            FD_info.append(line)
        else:
            break
            # After the last line of FD info, we can break
    toc = time.perf_counter()

    FD_info = [info.split() for info in FD_info]
    FD_info = {
        info[1]: convert_derivative_string_to_float(info[-1]) for info in FD_info
    }

    return FCN_history, EDM_history, FD_info


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


def get_FCN_table(FCN_history):
    FCN_table = PrettyTable()
    FCN_table.field_names = ["Log Likelihood", "Algorithm", "Status"]
    for entry in FCN_history:
        FCN_table.add_row([entry[0], entry[1], entry[2].strip()])

    return FCN_table


def get_EDM_table(EDM_history):
    EDM_table = PrettyTable()
    EDM_table.field_names = ["Estimated Distance to Minimum"]
    for entry in EDM_history:
        EDM_table.add_row([entry])

    return EDM_table


def get_limits(parameter):
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
        relative_distance = distance_to_limit / error
    elif has_lower_limit and not has_upper_limit:
        distance_to_limit = distance_to_lower_limit
        relative_distance = distance_to_limit / error
    elif not has_lower_limit and has_upper_limit:
        distance_to_limit = distance_to_upper_limit
        relative_distance = distance_to_limit / error
    else:
        relative_distance = "-"

    return lower_limit, upper_limit, relative_distance


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


def get_parameter_table(parameters, FDs, constraints):
    names = list(parameters.keys())

    parameter_table = PrettyTable()
    parameter_table.field_names = [
        "name",
        "value",
        "error",
        "first derivative",
        "error/value",
        "div/value",
        "lower limit",
        "upper limit",
        "(limit - value)/error",
        "constraint deviation",
    ]
    for name in names:
        parameter = parameters[name]
        value = parameter.value
        error = parameter.error
        first_derivative = FDs[name]
        if value != 0:
            relative_FD = (
                first_derivative / value
                if type(first_derivative) == float and value != 0
                else "**at limit**"
            )
            relative_error = error / value
        else:
            relative_FD = "NA"
            relative_error = "NA"

        lower_limit, upper_limit, relative_distance_to_limit = get_limits(parameter)
        if name in constraints:
            deviation = constraints[name].get_deviation(value)
        else:
            deviation = "-"

        row = [
            name,
            value,
            error,
            first_derivative,
            relative_error,
            relative_FD,
            lower_limit,
            upper_limit,
            relative_distance_to_limit,
            deviation,
        ]

        parameter_table.add_row(row)
    parameter_table.float_format = ".4"
    return parameter_table


if __name__ == "__main__":
    args = parse_args()

    FCNs, EDMs, FDs = parse_log(args.log)
    status, covariance_quality, parameters = parse_fitresult(args.fitresult)
    constraints = parse_gaussian_constraint(args.constraint)

    FCN_table = get_FCN_table(FCNs)
    EDM_table = get_EDM_table(EDMs)
    parameter_table = get_parameter_table(parameters, FDs, constraints)

    os.makedirs(os.path.dirname(args.output), exist_ok=True)
    with open(args.output, "w") as file:
        file.write(f"Fit Status         : {status}\n")
        file.write(f"Covariance Quality : {covariance_quality}\n")
        file.write("\n" * 5)
        file.write(FCN_table.get_string())
        file.write("\n" * 5)
        file.write(EDM_table.get_string())
        file.write("\n" * 5)
        file.write(parameter_table.get_string())
