import numpy as np
import ROOT as r
import matplotlib.pyplot as plt
import matplotlib, os, argparse
import scipy.interpolate

LEGENDS_MAP = {
    "amp2_weight": "Total",
    "allLz": "All $\\Lambda^{*}$",
    "allPc": "All $P_{c}$",
    "LZ_JP_1m": "$1^{-} \\Lambda^{*}$",
    "LZ_JP_3m": "$3^{-} \\Lambda^{*}$",
    "LZ_JP_1p": "$1^{+} \\Lambda^{*}$",
    "LZ_JP_5p": "$5^{+} \\Lambda^{*}$",
    "LZ_JP_5m": "$5^{-} \\Lambda^{*}$",
    "LZ_JP_3p": "$3^{+} \\Lambda^{*}$",
    "LZ_JP_7m": "$7^{-} \\Lambda^{*}$",
    "LZ_JP_9p": "$9^{+} \\Lambda^{*}$",
    "LZ_JP_7p": "$7^{+} \\Lambda^{*}$",
    "Pc_JP_3m": "$3^{-} P_{c}$",
    "Pc_JP_1m": "$1^{-} P_{c}$",
    "Pc_JP_3p": "$3^{+} P_{c}$",
    "Pc_JP_1p": "$1^{+} P_{c}$",
    "LZ_high_J": "$\\Lambda^{*}$ with $J>\\frac{3}{2}$",
    "allLz_Pc_JP_3m": "$3^{-} P_{c}$ + $\\Lambda^{*}$",
    "allLz_Pc_JP_1m": "$1^{-} P_{c}$ + $\\Lambda^{*}$",
    "allLz_Pc_JP_3p": "$3^{+} P_{c}$ + $\\Lambda^{*}$",
    "allLz_Pc_JP_1p": "$1^{+} P_{c}$ + $\\Lambda^{*}$",
    "allZcs": "$Z_{zs}(4000)$",
}

NBINS_MC_INTERPOLATE = 100
NBINS_MC_PLOT = 1000


def plot(
    save_path,
    data,
    data_weights,
    mc,
    mc_weights,
    nbins,
    nbins_interpolate,
    xmin,
    xmax,
    title,
    xlabel,
    colors,
    model_scale,
):
    data_bin_weights, bin_edges = np.histogram(
        data, bins=nbins, range=(xmin, xmax), weights=data_weights
    )
    data_bin_variance, _ = np.histogram(
        data, bins=nbins, range=(xmin, xmax), weights=np.square(data_weights)
    )
    data_bin_errors = np.sqrt(data_bin_variance)
    bin_centres = (bin_edges[:-1] + bin_edges[1:]) / 2
    bin_width = (xmax - xmin) / nbins

    model_bin_weights, _ = np.histogram(
        mc, bins=nbins, range=(xmin, xmax), weights=mc_weights["amp2_weight"]
    )
    model_bin_variance, _ = np.histogram(
        mc, bins=nbins, range=(xmin, xmax), weights=np.square(mc_weights["amp2_weight"])
    )
    model_bin_weights = model_scale * model_bin_weights
    model_bin_variance = model_scale * model_scale * model_bin_variance

    difference = model_bin_weights - data_bin_weights
    errors = np.sqrt(data_bin_variance + model_bin_variance)
    pulls = difference / errors
    pulls = np.where(errors != 0.0, pulls, np.zeros_like(difference))

    # mc_bin_edges = np.linspace( xmin, xmax, nbins_interpolate + 1 )
    mc_width = (np.amax(mc) - np.amin(mc)) / nbins_interpolate
    mc_bin_edges = np.linspace(
        np.amin(mc) - mc_width, np.amax(mc) + mc_width, nbins_interpolate + 3
    )
    mc_bin_centres = (mc_bin_edges[:-1] + mc_bin_edges[1:]) / 2
    mc_bin_weights = {
        key: np.histogram(mc, bins=mc_bin_edges, weights=weights)[0]
        for key, weights in mc_weights.items()
    }
    mc_scale = (
        np.sum(data_weights) * bin_width / np.sum(mc_weights["amp2_weight"]) / mc_width
    )
    mc_bin_weights = {
        key: weights * mc_scale for key, weights in mc_bin_weights.items()
    }

    mc_splines = {
        key: scipy.interpolate.CubicSpline(mc_bin_centres, weights, extrapolate=False)
        for key, weights in mc_bin_weights.items()
    }
    mc_plot_x = np.linspace(xmin, xmax, NBINS_MC_PLOT)
    mc_plot_y = {key: spline(mc_plot_x) for key, spline in mc_splines.items()}
    # Since we set extrapolate=False in Cubicspline, we have to convert NaN for values out of spline bounds to 0.
    mc_plot_y = {
        key: np.where(np.isnan(y), np.zeros_like(y), y) for key, y in mc_plot_y.items()
    }

    plt.rcParams["font.size"] = 16
    figure, axes = plt.subplots(
        nrows=2, height_ratios=[0.8, 0.2], figsize=(12, 10), dpi=100, sharex=True
    )

    axes[0].set_xlim(xmin, xmax)
    axes[1].set_xlim(xmin, xmax)
    axes[1].set_ylim(-5, 5)

    for key, y, color in zip(mc_plot_y.keys(), mc_plot_y.values(), colors):
        legend = LEGENDS_MAP[key]
        width = 2.0 if key == "amp2_weight" else 1.0
        alpha = 1.0 if key == "amp2_weight" else 0.6
        axes[0].plot(mc_plot_x, y, color=color, label=legend, linewidth=width)
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
    axes[0].set_xlabel(xlabel)
    axes[0].set_ylabel(f"Events per {(xmax - xmin)/nbins:.3f} GeV")
    axes[0].legend()
    axes[1].bar(
        x=bin_centres,
        height=np.abs(pulls),
        width=bin_width,
        bottom=np.minimum(pulls, 0.0),
        color="tab:gray",
        alpha=0.5,
    )
    axes[1].set_ylabel("Pulls")
    axes[1].set_yticks(ticks=[-5, -3, 0, 3, 5], labels=["-5", "-3", "0", "3", "5"])
    axes[1].axhline(
        y=3, linestyle=(0, (10, 5)), color="tab:gray", alpha=0.3, linewidth=0.8
    )
    axes[1].axhline(
        y=-3, linestyle=(0, (10, 5)), color="tab:gray", alpha=0.3, linewidth=0.8
    )
    figure.suptitle(title)
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

    WEIGHT_GROUP = {
        "mjpsip": ["amp2_weight", "allLz"]
        + [name for name in Pc_columns if name.startswith("Pc")],
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
        + get_colors(len(WEIGHT_GROUP["mjpsip"]) - 1, args.cmap_name, False),
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
            setting[2],
            setting[3],
            setting[4],
            setting[1],
            setting[1],
            colors,
            model_scale,
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
            "mjpsip_zoomed_4312_Pc_enchanced",
            "$m(J/\\psi p)$ [GeV/c$^{2}$]",
            50,
            4.25,
            4.35,
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
            "mjpsip_zoomed_4450_Pc_enchanced",
            "$m(J/\\psi p)$ [GeV/c$^{2}$]",
            50,
            4.4,
            4.5,
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
            "mjpsip_zoomed_Pc_enchanced",
            "$m(J/\\psi p)$ [GeV/c$^{2}$]",
            100,
            4.2,
            4.7,
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
        )
    )

    # Draw normally, no interference
    for setting in more_settings:
        (data_indices, mc_indices, mass, title, xlabel, nbins, xmin, xmax) = setting

        save_path = os.path.join(args.outdir, f"{title}.pdf")
        colors = COLOR_GROUP[mass]
        weights = {key: mc_weights[key] for key in WEIGHT_GROUP[mass]}
        weights = {key: mc_weights[mc_indices] for key, mc_weights in weights.items()}
        plot(
            save_path,
            data_columns[mass][data_indices],
            data_weights[data_indices],
            mc_columns[mass][mc_indices],
            weights,
            nbins,
            nbins,
            xmin,
            xmax,
            title,
            xlabel,
            colors,
            model_scale,
        )
