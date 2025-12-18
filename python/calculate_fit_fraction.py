import os, argparse

from prettytable import PrettyTable
import numpy as np
import ROOT as r

LABELS_MAP = {
    "amp2_weight": "Total",
    "allLz": "All $\\Lambda^{*}$",
    "allPc": "All $P_{c}$",
    "LZ_JP_1m": "$\\frac{1}{2}^{-} \\Lambda^{*}$",
    "LZ_JP_3m": "$\\frac{3}{2}^{-} \\Lambda^{*}$",
    "LZ_JP_1p": "$\\frac{1}{2}^{+} \\Lambda^{*}$",
    "LZ_JP_5p": "$\\frac{5}{2}^{+} \\Lambda^{*}$",
    "LZ_JP_5m": "$\\frac{5}{2}^{-} \\Lambda^{*}$",
    "LZ_JP_3p": "$\\frac{3}{2}^{+} \\Lambda^{*}$",
    "LZ_JP_7m": "$\\frac{7}{2}^{-} \\Lambda^{*}$",
    "LZ_JP_9p": "$\\frac{9}{2}^{+} \\Lambda^{*}$",
    "LZ_JP_7p": "$\\frac{7}{2}^{+} \\Lambda^{*}$",
    "Pc_JP_3m": "$\\frac{3}{2}^{-}\\ J/\\psi p$",
    "Pc_JP_1m": "$\\frac{1}{2}^{-}\\ J/\\psi p$",
    "Pc_JP_3p": "$\\frac{3}{2}^{+}\\ J/\\psi p$",
    "Pc_JP_1p": "$\\frac{1}{2}^{+}\\ J/\\psi p$",
    "LZ_high_J": "$\\Lambda^{*}$ with $J>\\frac{3}{2}$",
    "Ls_1520": "$\\Lambda(1520)$",
    "Ls_1600": "$\\Lambda(1600)$",
    "Ls_1690": "$\\Lambda(1690)$",
    "Ls_1710": "$\\Lambda(1710)$",
    "Ls_1810": "$\\Lambda(1810)$",
    "Ls_1820": "$\\Lambda(1820)$",
    "Ls_1830": "$\\Lambda(1830)$",
    "Ls_1890": "$\\Lambda(1890)$",
    "Ls_1910": "$\\Lambda(1910)$",
    "Ls_2020": "$\\Lambda(2020)$",
    "Ls_2050": "$\\Lambda(2050)$",
    "Ls_2070": "$\\Lambda(2070)$",
    "Ls_2080": "$\\Lambda(2080)$",
    "Ls_2100": "$\\Lambda(2100)$",
    "Ls_2110": "$\\Lambda(2110)$",
    "Ls_2130": "$\\Lambda(2130)$",
    "Ls_2320": "$\\Lambda(2320)$",
    "Ls_2340": "$\\Lambda(2340)$",
    "Ls_2350": "$\\Lambda(2350)$",
    "Ls_2585": "$\\Lambda(2585)$",
    "Pc_4312": "$P_{c}(4312)$",
    "Pc_4385": "$P_{c}(4385)$",
    "Pc_4440": "$P_{c}(4440)$",
    "Pc_4457": "$P_{c}(4457)$",
    "Pc_4500": "$P_{c}(4500)$",
    "allLz_Pc_JP_3m": "$\\frac{3}{2}^{-} P_{c}$ + $\\Lambda^{*}$",
    "allLz_Pc_JP_1m": "$\\frac{1}{2}^{-} P_{c}$ + $\\Lambda^{*}$",
    "allLz_Pc_JP_3p": "$\\frac{3}{2}^{+} P_{c}$ + $\\Lambda^{*}$",
    "allLz_Pc_JP_1p": "$\\frac{1}{2}^{+} P_{c}$ + $\\Lambda^{*}$",
    "Zcs_Xplus": "$Z_{zs}(4000)$",
}


def parse_args():
    parser = argparse.ArgumentParser(
        description="Calculate fit fractions based on amplitude weights."
    )
    parser.add_argument(
        "--mc_weights", type=str, required=True, help="Path to MC weight file."
    )
    parser.add_argument(
        "--outfile", type=str, required=True, help="File to dump fit fraction table."
    )
    parser.add_argument(
        "--Zcs", action="store_true", help="Whether to include Zcs in fit fractions."
    )
    args = parser.parse_args()
    return args


def filter_components(columns, components):
    selected_components = []
    for component in components:
        if component in columns:
            selected_components.append(component)
        else:
            print(f"Info: Component {component} not found in columns. Skipping...")
    return selected_components


def filter_interference_waves(columns, components):
    selected_components = []
    for component in components:
        if component[1] in columns:
            selected_components.append(component)
        else:
            print(f"Info: Component {component[1]} not found in columns. Skipping...")
    return selected_components


if __name__ == "__main__":
    args = parse_args()

    weights_variables = [
        "LZ_JP_1m",
        "LZ_JP_1p",
        "LZ_JP_3m",
        "LZ_JP_3p",
        "LZ_JP_5p",
        "LZ_JP_5m",
        "LZ_JP_7m",
        "LZ_JP_7p",
        "LZ_JP_9p",
        "allLz",
        "Pc_JP_1m",
        "Pc_JP_3m",
        "Pc_JP_1p",
        "Pc_JP_3p",
        "allPc",
        "allLz_Pc_JP_1m",
        "allLz_Pc_JP_3m",
        "allLz_Pc_JP_1p",
        "allLz_Pc_JP_3p",
        "amp2_weight",
        "Ls_1520",
        "Ls_1600",
        "Ls_1690",
        "Ls_1710",
        "Ls_1810",
        "Ls_1820",
        "Ls_1830",
        "Ls_1890",
        "Ls_1910",
        "Ls_2020",
        "Ls_2050",
        "Ls_2070",
        "Ls_2080",
        "Ls_2100",
        "Ls_2110",
        "Ls_2130",
        "Ls_2320",
        "Ls_2340",
        "Ls_2350",
        "Ls_2585",
        "Pc_4312",
        "Pc_4385",
        "Pc_4440",
        "Pc_4457",
        "Pc_4500",
    ]

    individual_waves = [
        "LZ_JP_1m",
        "LZ_JP_1p",
        "LZ_JP_3m",
        "LZ_JP_3p",
        "LZ_JP_5p",
        "LZ_JP_5m",
        "LZ_JP_7m",
        "LZ_JP_7p",
        "LZ_JP_9p",
        "Pc_JP_1m",
        "Pc_JP_3m",
        "Pc_JP_1p",
        "Pc_JP_3p",
    ]

    Lz_waves = [
        "LZ_JP_1m",
        "LZ_JP_1p",
        "LZ_JP_3m",
        "LZ_JP_3p",
        "LZ_JP_5p",
        "LZ_JP_5m",
        "LZ_JP_7m",
        "LZ_JP_7p",
        "LZ_JP_9p",
    ]

    Pc_waves = [
        "Pc_JP_1m",
        "Pc_JP_3m",
        "Pc_JP_1p",
        "Pc_JP_3p",
    ]

    interference_waves = [
        ("allLz_Pc_JP_1m", "Pc_JP_1m"),
        ("allLz_Pc_JP_3m", "Pc_JP_3m"),
        ("allLz_Pc_JP_1p", "Pc_JP_1p"),
        ("allLz_Pc_JP_3p", "Pc_JP_3p"),
    ]

    resonances = [
        "Ls_1520",
        "Ls_1600",
        "Ls_1690",
        "Ls_1710",
        "Ls_1810",
        "Ls_1820",
        "Ls_1830",
        "Ls_1890",
        "Ls_1910",
        "Ls_2020",
        "Ls_2050",
        "Ls_2070",
        "Ls_2080",
        "Ls_2100",
        "Ls_2110",
        "Ls_2130",
        "Ls_2320",
        "Ls_2340",
        "Ls_2350",
        "Ls_2585",
        "Pc_4312",
        "Pc_4385",
        "Pc_4440",
        "Pc_4457",
        "Pc_4500",
    ]
    if args.Zcs:
        weights_variables.append("Zcs_Xplus")
        resonances.append("Zcs_Xplus")

    dataframe = r.RDataFrame("tree", args.mc_weights)
    columns = [str(name) for name in dataframe.GetColumnNames()]
    # Filter out the components that are not present in the dataframe
    weights_variables = filter_components(columns, weights_variables)
    individual_waves = filter_components(columns, individual_waves)
    Lz_waves = filter_components(columns, Lz_waves)
    Pc_waves = filter_components(columns, Pc_waves)
    resonances = filter_components(columns, resonances)
    interference_waves = filter_interference_waves(columns, interference_waves)

    mc_weights = dataframe.AsNumpy(weights_variables)
    fit_fractions = {key: np.sum(weights) for key, weights in mc_weights.items()}
    total = fit_fractions["amp2_weight"]
    fit_fractions = {
        key: float(100 * value / total) for key, value in fit_fractions.items()
    }

    sum_Lz = float(np.sum([fit_fractions[key] for key in Lz_waves]))
    sum_Pc = float(np.sum([fit_fractions[key] for key in Pc_waves]))
    sum_waves = float(np.sum([fit_fractions[key] for key in individual_waves]))
    sum_resonances = float(np.sum([fit_fractions[key] for key in resonances]))

    table = PrettyTable()
    table.field_names = ["Component", "Fit Fraction"]

    for key in Lz_waves:
        table.add_row([LABELS_MAP[key], fit_fractions[key]])
    table.add_row(["Sum FF $\\Lambda^{*}$", sum_Lz])
    table.add_row([LABELS_MAP["allLz"], fit_fractions["allLz"]])

    for key in Pc_waves:
        table.add_row([LABELS_MAP[key], fit_fractions[key]])
    table.add_row(["Sum FF $P_{c}$", sum_Pc])
    table.add_row([LABELS_MAP["allPc"], fit_fractions["allPc"]])

    table.add_row(["Sum FF", sum_waves])
    table.add_row([LABELS_MAP["amp2_weight"], fit_fractions["amp2_weight"]])

    for key, individual_key in interference_waves:
        table.add_row([LABELS_MAP[key], fit_fractions[key]])
        table.add_row(
            [
                "Sum FF All $\\Lambda^{*} + $" + LABELS_MAP[individual_key],
                sum_Lz + fit_fractions[individual_key],
            ]
        )

    for key in resonances:
        table.add_row([LABELS_MAP[key], fit_fractions[key]])
    table.add_row(["Sum Resonances", sum_resonances])

    table.float_format = ".2"
    os.makedirs(os.path.dirname(args.outfile), exist_ok=True)
    with open(args.outfile, "w") as file:
        file.write(table.get_formatted_string("latex"))
