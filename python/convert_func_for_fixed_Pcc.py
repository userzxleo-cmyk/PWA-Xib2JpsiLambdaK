import argparse, itertools, re


def parse_args():
    parser = argparse.ArgumentParser(
        description="Convert func to have fixed parameters for the Pcc variables."
    )
    parser.add_argument("infile", type=str, help="Input func file.")
    parser.add_argument(
        "outfile", type=str, help="Output func file with fixed Pcc."
    )
    parser.add_argument(
        "--fixed-broad-Pc", action="store_true", help="Fix the parameters for the Broad Pc."
    )

    args = parser.parse_args()
    return args


def parse_line_from_groups(match, set_constant):
    name_and_value = match.group(1)
    uncertainty = match.group(5)
    is_constant = match.group(8)
    value_range = match.group(9)
    
    line = name_and_value
    if uncertainty:
        line += uncertainty
    if is_constant or set_constant:
        line += "C " # Add constant flag
    line += value_range

    return line


def fix_Pcc_parameters(lines, fixed_broad_Pc):
    float_pattern = r"-?\d+\.\d+(e[+-]?\d+)?"
    common_pattern = f"((\w+)\s+=\s+({float_pattern})\s+)" # Common pattern: "parameter = value "
    uncertainty_pattern = f"(\+/-\s+({float_pattern})\s+)?" # Some lines do not have uncertainty, hence the trailing ? (match 0 or 1 occurrence)
    constant_pattern = r"(C\s+)?"  # Some parameters are constants, hence the trailing ? (match 0 or 1 occurrence)
    # range_pattern = f"\(({float_pattern}|-INF) \- ({float_pattern}|+INF)\)" # Range are either float or -INF/+INF, hence the | match float or -INF/+INF
    range_pattern = f"(L\(({float_pattern}|-INF)\s+-\s+({float_pattern}|\+INF)\))" # Range are either float or -INF/+INF, hence the | match float or -INF/+INF
    parameter_pattern = re.compile(f"{common_pattern}{uncertainty_pattern}{constant_pattern}{range_pattern}")

    matches = []
    for line in lines:
        parameter_match = parameter_pattern.match(line)
        if parameter_match:
            matches.append(parameter_match)

    output_lines = []
    set_constant_pattern = re.compile(r"\w+(ZM[0-9]SC[0-9]|Z[0-9]?)$") if fixed_broad_Pc else re.compile(r"\w+(ZM[0-9]SC[0-9]|Z(?!2)[0-9]?)$")
    for match in matches:
        set_constant = set_constant_pattern.match(match.group(2))
        line = parse_line_from_groups(match, set_constant)
        output_lines.append(line)

    return output_lines


if __name__ == "__main__":
    args = parse_args()

    with open(args.infile, "r") as file:
        lines = file.readlines()
    output_lines = fix_Pcc_parameters(lines, args.fixed_broad_Pc)

    output_lines = "\n".join(output_lines) + "\n"  # Ensure the output ends with a newline

    with open(args.outfile, "w") as file:
        file.write(output_lines)
