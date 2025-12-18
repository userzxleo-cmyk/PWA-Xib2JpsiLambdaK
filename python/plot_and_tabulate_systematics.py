import os, json, glob

import matplotlib
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
from prettytable import PrettyTable


def get_setting():
    SETTINGS = [
        ("AdditionalLs", "Add $\\Lambda(2070)$ and $\\Lambda(2080)$"),
        ("AddZcs4000", "Add $Z_{cs}(4000)$"),
        ("FloatBlattWeisskopf", "Float Blatt-Weisskopf $d$"),
        ("JpsipResolutionMinus", "$-$ $m(J/\\psi p)$ Resolution"),
        ("JpsipResolutionPlus", "$+$ $m(J/\\psi p)$ Resolution"),
        ("LBPlusOne", "$L_{lowest} + 1$"),
        ("LinearKMatrixNonResonant", "Linear term in $K$-matrix NR"),
        ("SigmacDIsospin", "Isospin symmetry in $\\Sigma_{c}^{(*)}D^{(*)}$"),
        ("Spline", "Add Knot in $m(Kp)$ Spline"),
        ("NoBroadPc", "Fits without broad $P_{c}$"),
        ("KMatrixLs", "$\\mathcal{K}-\\text{matrix}$ parameterisation of $\\Lambda^{*}$."),
        ("BroadPcJP1m", "Broad $P_{c}\\ J^{P}=\\frac{1}{2}^{-}$"),
        ("BroadPcJP1p", "Broad $P_{c}\\ J^{P}=\\frac{1}{2}^{+}$"),
        ("BroadPcJP3m", "Broad $P_{c}\\ J^{P}=\\frac{3}{2}^{-}$")
    ]
    return SETTINGS


def get_colors(n):
    colormap = matplotlib.colormaps["viridis"]
    factor = 1 / (n - 1)
    colors = [colormap(factor * i) for i in range(n)]

    return colors


class Result:
    def __init__(self, Pc4312, Pc4440, Pc4457):
        self.Pc4312 = Pc4312
        self.Pc4440 = Pc4440
        self.Pc4457 = Pc4457


class Resonance:
    def __init__(self, mass, width):
        self.mass = mass
        self.width = width


def parse_result(path):
    files = glob.glob(path)
    print(files, path)
    assert(len(files) == 1)
    with open(files[0], "r") as file:
        result = json.load(file)

    Pc4312 = Resonance(
        mass=result["poles"]["pole4312"]["m_MeV"],
        width=result["poles"]["pole4312"]["Γ_MeV"],
    )
    Pc4440 = Resonance(
        mass=result["poles"]["pole4440"]["m_MeV"],
        width=result["poles"]["pole4440"]["Γ_MeV"],
    )
    Pc4457 = Resonance(
        mass=result["poles"]["pole4457"]["m_MeV"],
        width=result["poles"]["pole4457"]["Γ_MeV"],
    )

    result = Result(Pc4312, Pc4440, Pc4457)
    return result


def print_table(output_path, default, systematic_results):
    table = PrettyTable()
    table.field_names = [
        "Systematic",
        "$P_{c}(4312)$ Mass",
        "$P_{c}(4312)$ Width",
        "$P_{c}(4440)$ Mass",
        "$P_{c}(4440)$ Width",
        "$P_{c}(4457)$ Mass",
        "$P_{c}(4457)$ Width",
    ]

    for key, result in systematic_results.items():
        table.add_row(
            [
                key,
                result.Pc4312.mass - default.Pc4312.mass,
                result.Pc4312.width - default.Pc4312.width,
                result.Pc4440.mass - default.Pc4440.mass,
                result.Pc4440.width - default.Pc4440.width,
                result.Pc4457.mass - default.Pc4457.mass,
                result.Pc4457.width - default.Pc4457.width,
            ]
        )

    table.float_format = ".3"
    with open(output_path, "w") as file:
        file.write(table.get_formatted_string("latex"))


def plot_Pc_mass_widths(output_path, default, systematic_results):
    SETTINGS = get_setting()
    COLORS = get_colors(len(SETTINGS))
    Pc_labels = {
        "Pc4312": "$P_{c}(4312)$",
        "Pc4440": "$P_{c}(4440)$",
        "Pc4457": "$P_{c}(4457)$",
    }
    parameter_labels = {"mass": "Mass", "width": "Width"}

    with PdfPages(output_path) as pdf:
        for pc in ["Pc4312", "Pc4440", "Pc4457"]:
            figure, axes = plt.subplots(figsize=(8, 6), dpi=100)

            resonance = getattr(default, pc)
            axes.scatter(
                resonance.mass,
                resonance.width,
                label="default",
                color="darkred",
                marker="*",
            )

            for i, (key, label) in enumerate(SETTINGS):
                resonance = getattr(systematic_results[key], pc)
                axes.scatter(
                    resonance.mass,
                    resonance.width,
                    label=label,
                    color=COLORS[i],
                    marker="o",
                )

            axes.legend()
            axes.set_xlabel("Mass [MeV]")
            axes.set_ylabel("Width [MeV]")
            figure.tight_layout()
            pdf.savefig(figure)

            plt.clf()
            plt.close()

        infodict = pdf.infodict()
        infodict["Title"] = "Systematics"
        infodict["Author"] = "Da Yu Tou"


if __name__ == "__main__":
    input_directory = "/disk401/lhcb/dtou/Lb2JpsipK/Pc_lineshape/lb2jpsipkcoupledchannels.jl/workspace/systematics/pole_json"
    output_directory = "/disk401/lhcb/dtou/Lb2JpsipK/lb2jpsipk_run12_aman/Fit_fullPHSP/systematics/pole_results"
    os.makedirs(output_directory, exist_ok=True)

    SETTINGS = get_setting()
    keys = [setting[0] for setting in SETTINGS]

    for jp in ["4440m1-4457m3", "4440m3-4457m1"]:
        default = parse_result(os.path.join(input_directory, f"default-{jp}.json"))

        systematic_results = {
            key: parse_result(os.path.join(input_directory, f"{key}-{jp}*.json"))
            for key in keys
        }
        print_table(
            os.path.join(output_directory, f"systematics_table-{jp}.txt"),
            default,
            systematic_results,
        )

        # plt.rcParams["font.size"] = 14
        # plt.rcParams["text.usetex"] = True
        # plot_Pc_mass_widths(
        #     os.path.join(output_directory, f"systematics_Pc_mass_width-{jp}.pdf"),
        #     default,
        #     systematic_results,
        # )
