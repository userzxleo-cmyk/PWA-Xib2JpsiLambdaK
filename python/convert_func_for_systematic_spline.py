import argparse, os, re, math, itertools


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


def get_largest_index(lines):
    pattern = re.compile("m_([0-3][0-9])")
    indices = list()
    for line in lines:
        match = pattern.match(line)
        if match:
            indices.append(int(match.group(1)))

    return max(indices)


def get_parameter_lines(lines):
    mass_pattern = re.compile("m_([0-3][0-9]|666)")
    spline_pattern = re.compile("S[IR]2*")

    spline_parameters = list()
    other_parameters = list()

    for line in lines:
        # We remove mass parameters since this is set by .dat file
        if mass_pattern.match(line):
            continue
        elif spline_pattern.match(line):
            spline_parameters.append(line)
        else:
            other_parameters.append(line)

    return spline_parameters, other_parameters


def interpolate(x, x0, x1, y0, y1, err_y0, err_y1):
    y = (y0 * (x1 - x) + y1 * (x - x0)) / (x1 - x0)
    err_y0_propagated = err_y0 * (x1 - x) / (x1 - x0)
    err_y1_propagated = err_y1 * (x - x0) / (x1 - x0)
    err_y = math.sqrt(err_y0_propagated**2 + err_y1_propagated**2)

    return y, err_y


def add_one_point(lines, prefix):
    node1600_string = "00"
    node1625_string = "666"
    node1650_string = "01"
    node1675_string = "02"
    node1700_string = "03"

    # Give uncertainty of 0.1
    nodes_of_interest = list()
    nodes_to_shift = list()
    unchanged_nodes = list()

    nodes_of_interest_OR = "|".join(
        [
            node1600_string,
            node1625_string,
            node1650_string,
            node1675_string,
            node1700_string,
        ]
    )
    pattern_1600_1700 = re.compile(f"{prefix}({nodes_of_interest_OR}) ")
    pattern_unchanged = re.compile(f"{prefix}([4-9][0-9]|00) ")
    pattern_to_shift = re.compile(f"{prefix}(0[3-9]|[1-3][0-9]) ")

    for line in lines:
        if pattern_1600_1700.match(line):
            nodes_of_interest.append(line)
        if pattern_unchanged.match(line):
            unchanged_nodes.append(line)
        if pattern_to_shift.match(line):
            nodes_to_shift.append(line)

    shifted_nodes = list()
    for line in nodes_to_shift:
        new_index = int(pattern_to_shift.match(line).group(1)) + 1
        if new_index >= 10:
            new_line = pattern_to_shift.sub(f"{prefix}{new_index}", line)
        else:
            new_line = pattern_to_shift.sub(f"{prefix}0{new_index}", line)
        shifted_nodes.append(new_line)

    # Interpolation from [1600, 1625, 1650, 1675, 1700]
    # to [1600, 1620, 1640, 1660, 1680, 1700]
    nodes_of_interest = [line.split() for line in nodes_of_interest]
    interpolation_values = {line[0]: float(line[2]) for line in nodes_of_interest}
    interpolation_errors = {line[0]: float(line[4]) for line in nodes_of_interest}

    y_1620, err_y_1620 = interpolate(
        1620,
        1600,
        1625,
        interpolation_values[f"{prefix}00"],
        interpolation_values[f"{prefix}666"],
        interpolation_errors[f"{prefix}00"],
        interpolation_errors[f"{prefix}666"],
    )
    y_1640, err_y_1640 = interpolate(
        1640,
        1625,
        1650,
        interpolation_values[f"{prefix}666"],
        interpolation_values[f"{prefix}01"],
        interpolation_errors[f"{prefix}666"],
        interpolation_errors[f"{prefix}01"],
    )
    y_1660, err_y_1660 = interpolate(
        1660,
        1650,
        1675,
        interpolation_values[f"{prefix}01"],
        interpolation_values[f"{prefix}02"],
        interpolation_errors[f"{prefix}01"],
        interpolation_errors[f"{prefix}02"],
    )
    y_1680, err_y_1680 = interpolate(
        1680,
        1675,
        1700,
        interpolation_values[f"{prefix}02"],
        interpolation_values[f"{prefix}03"],
        interpolation_errors[f"{prefix}02"],
        interpolation_errors[f"{prefix}03"],
    )

    nodes_1600_1700 = [
        f"{prefix}01 = {y_1620:.5f} +/- {err_y_1620:.5f} L(-INF - +INF)\n",
        f"{prefix}666 = {y_1640:.5f} +/- {err_y_1640:.5f} L(-INF - +INF)\n",
        f"{prefix}02 = {y_1660:.5f} +/- {err_y_1660:.5f} L(-INF - +INF)\n",
        f"{prefix}03 = {y_1680:.5f} +/- {err_y_1680:.5f} L(-INF - +INF)\n",
    ]

    returned_nodes = nodes_1600_1700 + unchanged_nodes + shifted_nodes

    return returned_nodes


def modify_splines(lines):
    prefixes = ["SR_", "SR2_", "SI_", "SI2_"]

    modified_lines = list()

    for prefix in prefixes:
        pattern = re.compile(prefix)
        lines_to_modify = [line for line in lines if pattern.match(line)]
        nodes = add_one_point(lines_to_modify, prefix)
        modified_lines.extend(nodes)

    return modified_lines


if __name__ == "__main__":
    args = parse_args()

    with open(args.infile, "r") as file:
        lines = file.readlines()

    largest_index = get_largest_index(lines)
    spline_parameters, other_parameters = get_parameter_lines(lines)
    spline_parameters = modify_splines(spline_parameters)

    with open(args.outfile, "w") as file:
        for line in itertools.chain(other_parameters, spline_parameters):
            file.write(line)
