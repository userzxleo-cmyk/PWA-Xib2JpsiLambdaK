import argparse, itertools, re


def parse_args():
    parser = argparse.ArgumentParser(
        description="Convert func to have fixed parameters for the Pcc variables."
    )
    parser.add_argument("infile", type=str, help="Input func file.")
    parser.add_argument("reference", type=str, help="Reference func file, which parameter to set floating.")
    parser.add_argument(
        "outfile", type=str, help="Output func file with fixed Pcc."
    )

    args = parser.parse_args()
    return args


def parse_line_from_groups(input_match, reference_match):
    name_and_value = input_match.group(1)
    uncertainty = input_match.group(5)
    value_range = input_match.group(9)

    # Get constant flag from reference, if unavailable, use constant flag of input
    is_constant = reference_match.group(8) if reference_match else input_match.group(8)

    line = name_and_value
    if uncertainty:
        line += uncertainty
    if is_constant:
        line += is_constant
    line += value_range

    return line


def float_Pcc_parameters(input_lines, reference_lines):
    float_pattern = r"-?\d+\.\d+(e[+-]?\d+)?"
    common_pattern = f"((\w+)\s+=\s+({float_pattern})\s+)" # Common pattern: "parameter = value "
    uncertainty_pattern = f"(\+/-\s+({float_pattern})\s+)?" # Some lines do not have uncertainty, hence the trailing ? (match 0 or 1 occurrence)
    constant_pattern = r"(C\s+)?"  # Some parameters are constants, hence the trailing ? (match 0 or 1 occurrence)
    # range_pattern = f"\(({float_pattern}|-INF) \- ({float_pattern}|+INF)\)" # Range are either float or -INF/+INF, hence the | match float or -INF/+INF
    range_pattern = f"(L\(({float_pattern}|-INF)\s+-\s+({float_pattern}|\+INF)\))" # Range are either float or -INF/+INF, hence the | match float or -INF/+INF
    parameter_pattern = re.compile(f"{common_pattern}{uncertainty_pattern}{constant_pattern}{range_pattern}")

    input_matches = dict()
    for line in input_lines:
        parameter_match = parameter_pattern.match(line)
        if parameter_match:
            input_matches[parameter_match.group(2)] = parameter_match

    reference_matches = dict()
    for line in reference_lines:
        parameter_match = parameter_pattern.match(line)
        if parameter_match:
            reference_matches[parameter_match.group(2)] = parameter_match

    output_lines = []
    for key in input_matches.keys():
        input_match = input_matches[key]
        reference_match = reference_matches.get(key, None)
        line = parse_line_from_groups(input_match, reference_match)
        output_lines.append(line)

    return output_lines


if __name__ == "__main__":
    args = parse_args()

    with open(args.infile, "r") as file:
        input_lines = file.readlines()
    with open(args.reference, "r") as file:
        reference_lines = file.readlines()
    output_lines = float_Pcc_parameters(input_lines, reference_lines)

    output_lines = "\n".join(output_lines) + "\n"  # Ensure the output ends with a newline

    with open(args.outfile, "w") as file:
        file.write(output_lines)
