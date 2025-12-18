import numpy as np
import ROOT as r
import matplotlib.pyplot as plt
import matplotlib, os, argparse, re
from matplotlib.lines import Line2D
from matplotlib.ticker import MaxNLocator, AutoMinorLocator
import scipy.interpolate

HATCHES = {
    "Pc_4312": "-",
    "Pc_4440": "/",
    "Pc_4457": "\\",
    "Pc_4500": "+",
    "Pc_4385": "x",
}

LEGENDS_MAP = {
    "amp2_weight": "Total",
    "allLz": "$\\Lambda^{*}$ + NR $Kp$",
    "allPc": "$P_{c}$ + NR $J/\\psi p$",
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
    # "Ls_1520": "$\\Lambda(1520)$",
    # "Ls_1600": "$\\Lambda(1600)$",
    # "Ls_1690": "$\\Lambda(1690)$",
    # "Ls_1710": "$\\Lambda(1710)$",
    # "Ls_1810": "$\\Lambda(1810)$",
    # "Ls_1820": "$\\Lambda(1820)$",
    # "Ls_1830": "$\\Lambda(1830)$",
    # "Ls_1890": "$\\Lambda(1890)$",
    # "Ls_1910": "$\\Lambda(1910)$",
    # "Ls_2020": "$\\Lambda(2020)$",
    # "Ls_2050": "$\\Lambda(2050)$",
    # "Ls_2100": "$\\Lambda(2100)$",
    # "Ls_2110": "$\\Lambda(2110)$",
    # "Ls_2130": "$\\Lambda(2130)$",
    # "Ls_2320": "$\\Lambda(2320)$",
    # "Ls_2340": "$\\Lambda(2340)$",
    # "Ls_2350": "$\\Lambda(2350)$",
    # "Ls_2585": "$\\Lambda(2585)$",
    "Pc_4312": "$P_{c\\bar{c}}(4312)^{+}$",
    "Pc_4440": "$P_{c\\bar{c}}(4440)^{+}$",
    "Pc_4457": "$P_{c\\bar{c}}(4457)^{+}$",
    "Pc_4500": "$P_{c\\bar{c}}(4500)^{+}$",
    "Pc_4385": "$P_{c\\bar{c}}(4385)^{+}$",
    "allLz_Pc_JP_3m": "$\\frac{3}{2}^{-} J/\\psi p$ + $\\Lambda^{*}$",
    "allLz_Pc_JP_1m": "$\\frac{1}{2}^{-} J/\\psi p$ + $\\Lambda^{*}$",
    "allLz_Pc_JP_3p": "$\\frac{3}{2}^{+} J/\\psi p$ + $\\Lambda^{*}$",
    "allLz_Pc_JP_1p": "$\\frac{1}{2}^{+} J/\\psi p$ + $\\Lambda^{*}$",
    "allZcs": "$Z_{cs}(4000)^{+}$",
}

NBINS_MC_PLOT = 1000


def plot(
    save_path,
    data,
    data_weights,
    mc,
    mc_weights,
    nbins,
    xmin,
    xmax,
    xlabel,
    colors,
    model_scale,
    extra_components=None,
    text=None,
):
    # Calculate data histograms
    data_bin_weights, bin_edges = np.histogram(
        data, bins=nbins, range=(xmin, xmax), weights=data_weights
    )
    data_bin_variance, _ = np.histogram(
        data, bins=nbins, range=(xmin, xmax), weights=np.square(data_weights)
    )
    data_bin_errors = np.sqrt(data_bin_variance)
    bin_centres = (bin_edges[:-1] + bin_edges[1:]) / 2
    bin_width = (xmax - xmin) / nbins

    # Calculate model/MC histograms
    model_bin_weights, _ = np.histogram(
        mc, bins=nbins, range=(xmin, xmax), weights=mc_weights["amp2_weight"]
    )
    model_bin_variance, _ = np.histogram(
        mc, bins=nbins, range=(xmin, xmax), weights=np.square(mc_weights["amp2_weight"])
    )
    model_bin_weights = model_scale * model_bin_weights
    model_bin_variance = model_scale * model_scale * model_bin_variance
    model_bin_error = np.sqrt(model_bin_variance)

    # Calculate pulls
    difference = model_bin_weights - data_bin_weights
    errors = np.sqrt(data_bin_variance + model_bin_variance)
    pulls = difference / errors
    pulls = np.where(errors != 0.0, pulls, np.zeros_like(difference))

    plt.rcParams["font.size"] = 24
    # plt.rcParams["text.usetex"] = True
    # plt.rcParams["font.family"] = "Times New Roman"

    figure, axes = plt.subplots(
        nrows=2,
        height_ratios=[0.8, 0.2],
        figsize=(12, 10),
        dpi=100,
        sharex=True,
        gridspec_kw={"hspace": 0},
    )

    axes[0].set_xlim(xmin, xmax)
    axes[0].set_ylim(
        0, 1.2 * max(np.amax(data_bin_weights), np.amax(model_bin_weights))
    )
    axes[1].set_xlim(xmin, xmax)
    axes[1].set_ylim(-5, 5)

    for key, weights, color in zip(mc_weights.keys(), mc_weights.values(), colors):
        if key == "amp2_weight":
            continue

        legend = LEGENDS_MAP[key]
        axes[0].hist(
            mc,
            weights=2 * model_scale * weights,
            bins=2 * nbins,
            range=(xmin, xmax),
            histtype="step",
            color=color,
            linewidth=1.0,
            label=legend,
        )

    if extra_components is not None:
        cmap = matplotlib.colormaps["Pastel1"]
        for i, (key, weights) in enumerate(extra_components.items()):
            color = cmap(i)
            hatch = HATCHES[key]
            legend = LEGENDS_MAP[key]

            axes[0].hist(
                mc,
                weights=2 * model_scale * weights,
                bins=2 * nbins,
                range=(xmin, xmax),
                histtype="stepfilled",
                color=color,
                alpha=0.75,
                hatch=hatch,
                label=legend,
            )

    axes[0].errorbar(
        bin_centres,
        model_bin_weights,
        xerr=bin_width / 2,
        yerr=model_bin_error,
        fmt="o",
        linewidth=1.0,
        color="red",
        label=LEGENDS_MAP["amp2_weight"],
        ls="none",
        markersize=2,
    )
    axes[0].stairs(
        model_bin_weights,
        bin_edges,
        linewidth=1.0,
        color="red"
    )
    axes[0].errorbar(
        bin_centres,
        data_bin_weights,
        xerr=bin_width / 2,
        yerr=data_bin_errors,
        fmt="o",
        linewidth=1.0,
        color="black",
        label="Data",
        ls="none",
        markersize=2,
    )
    axes[0].ticklabel_format(axis="y", scilimits=[-3, 3], useMathText=True, style="sci")
    axes[0].set_ylabel(f"Events per {(xmax - xmin)/nbins:.3f} GeV")
    axes[0].yaxis.set_major_locator(MaxNLocator(4))
    axes[0].yaxis.set_minor_locator(AutoMinorLocator(5))
    axes[0].tick_params(axis="y", which="major", direction="inout", length=10, width=2)
    axes[0].tick_params(axis="y", which="minor", direction="inout", length=5, width=1)

    # Make step histogram use line as legend
    handles, labels = axes[0].get_legend_handles_labels()

    if extra_components is None:
        legends_to_redraw = [
            LEGENDS_MAP[key] for key in mc_weights.keys() if key != "amp2_weight"
        ]
        new_handles = [
            Line2D([], [], c=h.get_edgecolor()) if (l in legends_to_redraw) else h
            for h, l in zip(handles, labels)
        ]
        axes[0].legend(handles=new_handles, labels=labels)
    else:
        legends_to_draw = [LEGENDS_MAP[key] for key in extra_components.keys()]
        new_handles = [h for h, l in zip(handles, labels) if l in legends_to_draw]
        new_labels = [l for h, l in zip(handles, labels) if l in legends_to_draw]
        axes[0].legend(handles=new_handles, labels=new_labels)

    if text is not None:
        ymax = 1.2 * max(np.amax(data_bin_weights), np.amax(model_bin_weights))
        xmin = xmin
        xrange = xmax - xmin
        axes[0].text(
            xmin + 0.05 * xrange,
            0.9 * ymax,
            text,
        )

    axes[1].bar(
        x=bin_centres,
        height=np.abs(pulls),
        width=bin_width,
        bottom=np.minimum(pulls, 0.0),
        color="tab:gray",
        alpha=0.5,
    )
    axes[1].set_ylabel("Pulls")
    axes[1].set_yticks(ticks=[-3, 0, 3], labels=["-3", "0", "3"])
    axes[1].axhline(
        y=3, linestyle=(0, (10, 5)), color="tab:gray", alpha=0.3, linewidth=0.8
    )
    axes[1].axhline(
        y=-3, linestyle=(0, (10, 5)), color="tab:gray", alpha=0.3, linewidth=0.8
    )
    axes[1].set_xlabel(xlabel)
    axes[1].xaxis.set_major_locator(MaxNLocator(4))
    axes[1].xaxis.set_minor_locator(AutoMinorLocator(5))
    axes[1].tick_params(axis="both", which="major", direction="inout", length=10, width=2)
    axes[1].tick_params(axis="both", which="minor", direction="inout", length=5, width=1)

    plt.tight_layout()
    plt.savefig(save_path)
    plt.clf()
    plt.close()


def get_colors(n, colormap_name, cyclic):
    cmap = matplotlib.colormaps[
        colormap_name
    ]  #'viridis', 'plasma', 'inferno', 'magma', 'cividis'
    values = np.linspace(0.0, 1.0 - 1.0 / n, n) if cyclic else np.linspace(0.0, 1.0, n)
    colors = [cmap(value) for value in values]
    return colors


def parse_args():
    parser = argparse.ArgumentParser(
        description="Plot mass distribution based on amplitude weights."
    )
    parser.add_argument(
        "--data_weights", type=str, required=True, help="Path to data weight file."
    )
    parser.add_argument(
        "--mc_weights", type=str, required=True, help="Path to MC weight file."
    )
    parser.add_argument("--outdir", type=str, required=True, help="Output directory.")
    parser.add_argument(
        "--cmap_name", type=str, default="viridis", help="Name of pyplot colormap."
    )
    parser.add_argument(
        "--data_root",
        type=str,
        default="sPlot.root",
        help="Path to data variables ROOT.",
    )
    parser.add_argument(
        "--mc_root", type=str, default="mcsw.root", help="Path to MC variables ROOT."
    )
    parser.add_argument(
        "--Zcs", action="store_true", help="Whether to include Zcs in fit projections."
    )
    args = parser.parse_args()
    return args


if __name__ == "__main__":
    args = parse_args()

    settings = [
        ("mjpsip", "$m(J/\\psi p)$ [GeV/c$^{2}$]", 100, 4.0, 5.2),
        ("mkp", "$m(pK)$ [GeV/c$^{2}$]", 100, 1.4, 2.6),
        ("cosTheta_Jpsi", "cos $\\theta_{\\psi}$", 100, -1.0, 1.0),
        ("cosTheta_Lb", "cos $\\theta_{\\Lambda_{b}}$", 100, -1.0, 1.0),
        ("cosTheta_L", "cos $\\theta_{\\Lambda^{*}}$", 100, -1.0, 1.0),
    ]

    if args.Zcs:
        settings.append(("mjpsik", "$m(J/\\psi K)$ [GeV/c$^{2}$]", 100, 3.5, 4.8))

    variables = [setting[0] for setting in settings]
    dataframe = r.RDataFrame("DTree", args.data_root)
    dataframe = dataframe.Filter(
        "mkp>1.431950001&&mkp<2.522583999999&&mjpsip>4.0351880460&&mjpsip<5.125823"
    )
    data_columns = dataframe.AsNumpy(variables)
    dataframe = r.RDataFrame("tree", args.data_weights)
    data_weights = dataframe.AsNumpy(["weight"])["weight"]

    dataframe = r.RDataFrame("h1", args.mc_root)
    dataframe = dataframe.Filter(
        " mkp >= (0.493677 + 0.938272046) && mkp <= (5.61951 - 3.096916) && mjpsip >= (3.096916 + 0.938272046) && (mjpsip + 0.493677) <= 5.61951"
    )
    mc_columns = dataframe.AsNumpy(variables)

    dataframe = r.RDataFrame("tree", args.mc_weights)
    column_names = [str(name) for name in dataframe.GetColumnNames()]
    Pc_columns = [
        name
        for name in column_names
        if name.startswith("Pc") or name.startswith("allLz_Pc")
    ]

    columns = [
        "amp2_weight",
        "allLz",
        "allPc",
        "allZcs",
        "LZ_JP_1m",
        "LZ_JP_3m",
        "LZ_JP_1p",
        "LZ_JP_5p",
        "LZ_JP_5m",
        "LZ_JP_3p",
        "LZ_JP_7m",
        "LZ_JP_9p",
        "LZ_JP_7p",
        "LZ_high_J",
    ]
    columns.extend(Pc_columns)
    mc_weights = dataframe.AsNumpy(columns)
    model_scale = np.sum(data_weights) / np.sum(mc_weights["amp2_weight"])
    narrow_Pc_pattern = re.compile(r"Pc_(?!4500)[0-9]+")
    narrow_Pc = [ column for column in Pc_columns if narrow_Pc_pattern.match(column) ]

    WEIGHT_GROUP = {
        "mjpsip": ["amp2_weight", "allLz"]
        + [name for name in Pc_columns if name.startswith("Pc_JP")],
        "mkp": [
            "amp2_weight",
            "allPc",
            "LZ_JP_1m",
            "LZ_JP_3m",
            "LZ_JP_1p",
            "LZ_JP_3p",
            "LZ_high_J",
        ],
        "cosTheta_Jpsi": ["amp2_weight", "allLz", "allPc"],
        "cosTheta_Lb": ["amp2_weight", "allLz", "allPc"],
        "cosTheta_L": ["amp2_weight", "allLz", "allPc"],
    }

    if args.Zcs:
        settings.append(("mjpsik", "$m(J/\\psi K)$ [GeV/c$^{2}$]", 100, 3.5, 4.7))

        WEIGHT_GROUP["mjpsip"].insert(2, "allZcs")
        WEIGHT_GROUP["mkp"].insert(2, "allZcs")
        WEIGHT_GROUP["cosTheta_Jpsi"].append("allZcs")
        WEIGHT_GROUP["cosTheta_Lb"].append("allZcs")
        WEIGHT_GROUP["cosTheta_L"].append("allZcs")
        WEIGHT_GROUP["mjpsik"] = ["amp2_weight", "allLz", "allPc", "allZcs"]

    COLOR_GROUP = {
        "mjpsip": ["red"]
        + get_colors(max(len(WEIGHT_GROUP["mjpsip"]) - 1, 5), args.cmap_name, False),
        "mkp": ["red"]
        + get_colors(len(WEIGHT_GROUP["mkp"]) - 1, args.cmap_name, False),
        "cosTheta_Jpsi": ["red"]
        + get_colors(len(WEIGHT_GROUP["cosTheta_Jpsi"]) - 1, args.cmap_name, False),
        "cosTheta_Lb": ["red"]
        + get_colors(len(WEIGHT_GROUP["cosTheta_Lb"]) - 1, args.cmap_name, False),
        "cosTheta_L": ["red"]
        + get_colors(len(WEIGHT_GROUP["cosTheta_L"]) - 1, args.cmap_name, False),
    }

    if args.Zcs:
        COLOR_GROUP["mjpsik"] = ["red"] + get_colors(
            len(WEIGHT_GROUP["mjpsik"]) - 1, args.cmap_name, False
        )

    os.makedirs(args.outdir, exist_ok=True)

    # Draw normally, no interference
    for setting in settings:
        save_path = os.path.join(args.outdir, f"{setting[0]}.pdf")
        weights = {key: mc_weights[key] for key in WEIGHT_GROUP[setting[0]]}
        colors = COLOR_GROUP[setting[0]]
        plot(
            save_path,
            data_columns[setting[0]],
            data_weights,
            mc_columns[setting[0]],
            weights,
            setting[2],
            setting[3],
            setting[4],
            setting[1],
            colors,
            model_scale,
            text=None,
        )

    # Creates a list of extra settings that we deal with differently
    more_settings = list()

    # Zoomed into 4312, cut mkp
    data_indices = np.logical_and(
        np.logical_and(data_columns["mkp"] > 1.9, data_columns["mkp"] < 2.7),
        np.logical_and(data_columns["mjpsip"] >= 4.25, data_columns["mjpsip"] <= 4.35),
    )
    mc_indices = np.logical_and(
        np.logical_and(mc_columns["mkp"] > 1.9, mc_columns["mkp"] < 2.7),
        np.logical_and(mc_columns["mjpsip"] >= 4.25, mc_columns["mjpsip"] <= 4.35),
    )
    more_settings.append(
        (
            data_indices,
            mc_indices,
            "mjpsip",
            "mjpsip_zoomed_4312_Pc_enhanced",
            "$m(J/\\psi p)$ [GeV/c$^{2}$]",
            50,
            4.25,
            4.35,
            ["Pc_4312"],
            "$m(Kp)\\in[1.9,2.7]~\\text{GeV}$",
        )
    )

    # Zoomed into 4450, cut mkp
    data_indices = np.logical_and(
        np.logical_and(data_columns["mkp"] > 1.9, data_columns["mkp"] < 2.7),
        np.logical_and(data_columns["mjpsip"] >= 4.4, data_columns["mjpsip"] <= 4.5),
    )
    mc_indices = np.logical_and(
        np.logical_and(mc_columns["mkp"] > 1.9, mc_columns["mkp"] < 2.7),
        np.logical_and(mc_columns["mjpsip"] >= 4.4, mc_columns["mjpsip"] <= 4.5),
    )
    more_settings.append(
        (
            data_indices,
            mc_indices,
            "mjpsip",
            "mjpsip_zoomed_4450_Pc_enhanced",
            "$m(J/\\psi p)$ [GeV/c$^{2}$]",
            50,
            4.4,
            4.5,
            ["Pc_4440", "Pc_4457"],
            "$m(Kp)\\in[1.9,2.7]~\\text{GeV}$",
        )
    )

    # Zoomed into Pc region, cut mkp
    data_indices = np.logical_and(
        np.logical_and(data_columns["mkp"] > 1.9, data_columns["mkp"] < 2.7),
        np.logical_and(data_columns["mjpsip"] >= 4.2, data_columns["mjpsip"] <= 4.7),
    )
    mc_indices = np.logical_and(
        np.logical_and(mc_columns["mkp"] > 1.9, mc_columns["mkp"] < 2.7),
        np.logical_and(mc_columns["mjpsip"] >= 4.2, mc_columns["mjpsip"] <= 4.7),
    )
    more_settings.append(
        (
            data_indices,
            mc_indices,
            "mjpsip",
            "mjpsip_zoomed_Pc_enhanced",
            "$m(J/\\psi p)$ [GeV/c$^{2}$]",
            100,
            4.2,
            4.7,
            narrow_Pc,
            "$m(Kp)\\in[1.9,2.7]~\\text{GeV}$",
        )
    )

    # Zoomed into Pc region, plot Pc_4385
    # data_indices = np.logical_and(
    #     np.logical_and(data_columns["mkp"] > 1.9, data_columns["mkp"] < 2.7),
    #     np.logical_and(data_columns["mjpsip"] >= 4.2, data_columns["mjpsip"] <= 4.7),
    # )
    # mc_indices = np.logical_and(
    #     np.logical_and(mc_columns["mkp"] > 1.9, mc_columns["mkp"] < 2.7),
    #     np.logical_and(mc_columns["mjpsip"] >= 4.2, mc_columns["mjpsip"] <= 4.7),
    # )
    # more_settings.append(
    #     (
    #         data_indices,
    #         mc_indices,
    #         "mjpsip",
    #         "mjpsip_highlight_Pc_4385",
    #         "$m(J/\\psi p)$ [GeV/c$^{2}$]",
    #         100,
    #         4.2,
    #         4.7,
    #         ["Pc_4385"],
    #     )
    # )

    data_indices = np.logical_and(data_columns["mkp"] > 1.9, data_columns["mkp"] < 2.7)
    mc_indices = np.logical_and(mc_columns["mkp"] > 1.9, mc_columns["mkp"] < 2.7)
    more_settings.append(
        (
            data_indices,
            mc_indices,
            "mjpsip",
            "mjpsip_Pc_enhanced",
            "$m(J/\\psi p)$ [GeV/c$^{2}$]",
            100,
            4.0,
            5.2,
            narrow_Pc,
            "$m(Kp)\\in[1.9,2.7]~\\text{GeV}$",
        )
    )

    # Draw mkp low region
    data_indices = data_columns["mkp"] < 1.6
    mc_indices = mc_columns["mkp"] < 1.6
    more_settings.append(
        (
            data_indices,
            mc_indices,
            "mkp",
            "mkp_low_region",
            "$m(pK)$ [GeV/c$^{2}$]",
            100,
            1.4,
            1.6,
            None,
            None,
        )
    )

    # Draw mkp middle region
    data_indices = np.logical_and(data_columns["mkp"] > 1.6, data_columns["mkp"] < 2.0)
    mc_indices = np.logical_and(mc_columns["mkp"] > 1.6, mc_columns["mkp"] < 2.0)
    more_settings.append(
        (
            data_indices,
            mc_indices,
            "mkp",
            "mkp_middle_region",
            "$m(pK)$ [GeV/c$^{2}$]",
            100,
            1.6,
            2.0,
            None,
            None,
        )
    )

    # Draw mkp high region
    data_indices = data_columns["mkp"] > 2.0
    mc_indices = mc_columns["mkp"] > 2.0
    more_settings.append(
        (
            data_indices,
            mc_indices,
            "mkp",
            "mkp_high_region",
            "$m(pK)$ [GeV/c$^{2}$]",
            100,
            2.0,
            2.6,
            None,
            None,
        )
    )

    # Draw mkp zoomed region
    data_indices = np.logical_and(data_columns["mkp"] > 1.8, data_columns["mkp"] < 1.85)
    mc_indices = np.logical_and(mc_columns["mkp"] > 1.8, mc_columns["mkp"] < 1.85)
    more_settings.append(
        (
            data_indices,
            mc_indices,
            "mkp",
            "mkp_zoomed_middle_region",
            "$m(pK)$ [GeV/c$^{2}$]",
            50,
            1.8,
            1.85,
            None,
            None,
        )
    )

    # Draw normally, no interference
    for setting in more_settings:
        (
            data_indices,
            mc_indices,
            mass,
            title,
            xlabel,
            nbins,
            xmin,
            xmax,
            extra_keys,
            text,
        ) = setting

        save_path = os.path.join(args.outdir, f"{title}.pdf")
        colors = COLOR_GROUP[mass]
        weights = {key: mc_weights[key] for key in WEIGHT_GROUP[mass]}
        weights = {
            key: component_weights[mc_indices]
            for key, component_weights in weights.items()
        }

        if extra_keys is not None:
            extra_components = {key: mc_weights[key] for key in extra_keys}
            extra_components = {
                key: component_weights[mc_indices]
                for key, component_weights in extra_components.items()
            }
        else:
            extra_components = None

        plot(
            save_path,
            data_columns[mass][data_indices],
            data_weights[data_indices],
            mc_columns[mass][mc_indices],
            weights,
            nbins,
            xmin,
            xmax,
            xlabel,
            colors,
            model_scale,
            extra_components,
            text,
        )
