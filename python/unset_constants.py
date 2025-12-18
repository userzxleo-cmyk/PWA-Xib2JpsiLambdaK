import argparse, re

def parse_args():
    parser = argparse.ArgumentParser(description="Convert RooFitResult to func file.")
    parser.add_argument("--input", type=str, required=True, help="Input func.")
    parser.add_argument("--reference", type=str, required=True, help="Reference func to unset constant parameters.")
    parser.add_argument("--output", type=str, required=True, help="Output func.")

    args = parser.parse_args()
    return args


class Parameter:
    def __init__(self, string):
        self.parse_string(string)

    def parse_string(self, string):
        float_regex = r"[-+]?[0-9]+\.?[0-9]*e?[-+]?[0-9]*"
        infinity_regex = r"[-+]?INF"
        parameter_regex = f"(\w+)\s+=\s+({float_regex})\s+(\+/-\s+{float_regex}\s+)?(C\s+)?L\(({infinity_regex}|{float_regex})\s+-\s+({infinity_regex}|{float_regex})\)"
        error_regex = f"\+/-\s+({float_regex})"
        error_pattern = re.compile(error_regex)
        parameter_pattern = re.compile(parameter_regex)
        infinity_patttern = re.compile(infinity_regex)
        match = parameter_pattern.match(string)
        if match:
            self.match = True
            self.name = match.group(1)
            self.value = float(match.group(2))
            self.error = float(error_pattern.match(match.group(3)).group(1)) if match.group(3) else None
            self.constant = True if match.group(4) else False
            lower_infinity = infinity_patttern.match(match.group(5))
            upper_infinity = infinity_patttern.match(match.group(6))
            self.lower_limit = match.group(5) if lower_infinity else float(match.group(5))
            self.upper_limit = match.group(6) if upper_infinity else float(match.group(6))
        else:
            self.match = False


def write_func_file(outfile, input_parameters, reference_parameters):
    infinity_regex = r"[-+]?INF"
    infinity_patttern = re.compile(infinity_regex)
    with open(outfile, "w" ) as file:
        for key, parameter in input_parameters.items():
            error_string = f"+/- {parameter.error:.6} " if parameter.error else ""
            lower_limit_string = f"{parameter.lower_limit}" if isinstance(parameter.lower_limit, str) else f"{parameter.lower_limit:.7}"
            upper_limit_string = f"{parameter.upper_limit}" if isinstance(parameter.upper_limit, str) else f"{parameter.upper_limit:.7}"
            limit_string = f"L({lower_limit_string} - {upper_limit_string})"
            is_constant = reference_parameters[key].constant
            constant_string = "C " if is_constant else ""
            parameter_string = f"{parameter.name} = {parameter.value:.7} {error_string}{constant_string}{limit_string}\n"
            file.write(parameter_string)
        file.close()


def parse_parameters(filepath):
    parameters = dict()
    with open(filepath, "r") as file:
        lines = file.readlines()
        for line in lines:
            parameter = Parameter(line)
            if parameter.match:
                parameters[parameter.name] = parameter
    return parameters


def main():
    args = parse_args()
    input_parameters = parse_parameters(args.input)
    reference_parameters = parse_parameters(args.reference)
    write_func_file(args.output, input_parameters, reference_parameters)
    # write_func_file(args.outfile, fit_result, args.initial, args.refit)

if __name__ == "__main__":
    main()
