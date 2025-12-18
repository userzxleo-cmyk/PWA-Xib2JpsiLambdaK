import numpy as np
import ROOT as r
import argparse, os, copy
# from scipy.spatial import KDTree
from common.kdtree import Node, KDTree

import matplotlib
import matplotlib.pyplot as plt
from matplotlib.collections import PolyCollection

from typing import List, Tuple


class Bin2D:
    def __init__(self, xlow, ylow, xhigh, yhigh):
        self.xlow = xlow
        self.ylow = ylow
        self.xhigh = xhigh
        self.yhigh = yhigh

    def __repr__(self):
        return f"x : [{self.xlow}, {self.xhigh}] y : [{self.ylow}, {self.yhigh}]"


def refine_binnings(
    binnings: List[Bin2D],
    data_bin_x: List[np.array],
    data_bin_y: List[np.array],
    phsp_bin_x: List[np.array],
    phsp_bin_y: List[np.array],
    kinematic_limits: Tuple[float, float, float, float],
):
    def at_kinematic_limit(binning: Bin2D, kinematic_limits: Bin2D):
        at_limit = any(
            [
                binning.xlow <= kinematic_limits.xlow,
                binning.ylow <= kinematic_limits.ylow,
                binning.xhigh >= kinematic_limits.xhigh,
                binning.yhigh >= kinematic_limits.yhigh,
            ]
        )
        return at_limit

    def refine_bin(
        binning: Bin2D,
        data_bin_x: np.array,
        data_bin_y: np.array,
        phsp_bin_x: np.array,
        phsp_bin_y: np.array,
    ):
        dataset_min_x = max(min(np.amin(data_bin_x), np.amin(data_bin_x)), kinematic_limits.xlow)
        dataset_max_x = min(max(np.amax(data_bin_x), np.amax(data_bin_x)), kinematic_limits.xhigh)
        dataset_min_y = max(min(np.amin(data_bin_y), np.amin(data_bin_y)), kinematic_limits.ylow)
        dataset_max_y = min(max(np.amax(data_bin_y), np.amax(data_bin_y)), kinematic_limits.yhigh)
        dataset_delta_x = dataset_max_x - dataset_min_x
        dataset_delta_y = dataset_max_y - dataset_min_y

        distance_min_x = dataset_min_x - binning.xlow
        distance_max_x = binning.xhigh - dataset_max_x
        distance_min_y = dataset_min_y - binning.ylow
        distance_max_y = binning.yhigh - dataset_max_y

        new_binning = copy.deepcopy(binning)

        if distance_min_x > 0.5 * dataset_delta_x:
            new_binning.xlow = dataset_min_x
        if distance_max_x > 0.5 * dataset_delta_x:
            new_binning.xhigh = dataset_max_x
        if distance_min_y > 0.5 * dataset_delta_y:
            new_binning.ylow = dataset_min_y
        if distance_max_y > 0.5 * dataset_delta_y:
            new_binning.yhigh = dataset_max_y

        return new_binning

    binnings = [
        refine_bin(binning, data_bin_x[i], data_bin_y[i], phsp_bin_x[i], phsp_bin_y[i])
        if at_kinematic_limit(binning, kinematic_limits)
        else binning
        for i, binning in enumerate(binnings)
    ]
    return binnings


def split_sample_with_kdtree(
    kdtree: KDTree, variableX: np.array, variableY: np.array, weights: np.array
):
    def recurse(node, weight, variables):
        if node.splitable:
        # if hasattr(node, "split"): # scipy KDTree
            left_tree = node.less
            right_tree = node.greater

            variable = variables[node.split_dim]
            left_split_indices = variable <= node.split
            right_split_indices = variable > node.split

            left_variables = variables[:, left_split_indices]
            right_variables = variables[:, right_split_indices]

            left_weight = weight[left_split_indices]
            right_weight = weight[right_split_indices]

            # Recursively split the left and right bins
            left_bin_variables, left_bin_weights = recurse(
                left_tree, left_weight, left_variables
            )
            right_bin_variables, right_bin_weights = recurse(
                right_tree, right_weight, right_variables
            )

            variables = left_bin_variables + right_bin_variables
            weights = left_bin_weights + right_bin_weights
        else:
            variables = [variables]
            weights = [weight]

        return variables, weights

    variables = np.stack([variableX, variableY], axis=0)
    per_bin_variables, per_bin_weights = recurse(kdtree.tree, weights, variables)

    per_bin_x = [bin_variables[0] for bin_variables in per_bin_variables]
    per_bin_y = [bin_variables[1] for bin_variables in per_bin_variables]

    return per_bin_x, per_bin_y, per_bin_weights


def get_kdtree_binnings(kdtree: KDTree, source_bin: Bin2D):
    def recurse(node, node_binning):
        if node.splitable:
        # if hasattr(node, "split"): # SciPy KDTree
            left_tree = node.less
            right_tree = node.greater
            left_binning = copy.deepcopy(node_binning)
            right_binning = copy.deepcopy(node_binning)
            if node.split_dim == 0:
                left_binning.xhigh = node.split
                right_binning.xlow = node.split
            else:
                left_binning.yhigh = node.split
                right_binning.ylow = node.split
            left_binnings = recurse(left_tree, left_binning)
            right_binnings = recurse(right_tree, right_binning)
            binnings = left_binnings + right_binnings
        else:
            binnings = [node_binning]
        return binnings

    binnings = recurse(kdtree.tree, source_bin)

    return binnings


def get_kdtree(variableA: np.array, variableB: np.array, weights: np.array, min_sig: float):
    kdtree = KDTree([ variableA, variableB ], weights, args.min_sig)
    return kdtree

# def get_kdtree(variableA: np.array, variableB: np.array, leafsize: int): # SciPy KDTree
#     kdtree_data = np.stack([variableA, variableB], axis=-1)
#     kdtree = KDTree(kdtree_data, leafsize)
#     return kdtree


def get_poly_vertices(binnings: List[Bin2D]):
    vertices_x = [
        np.array([binning.xlow, binning.xlow, binning.xhigh, binning.xhigh])
        for binning in binnings
    ]
    vertices_y = [
        np.array([binning.ylow, binning.yhigh, binning.yhigh, binning.ylow])
        for binning in binnings
    ]

    vertices_x = np.stack(vertices_x, axis=0)
    vertices_y = np.stack(vertices_y, axis=0)

    vertices = np.stack((vertices_x, vertices_y), axis=2)
    return vertices


def get_per_bin_difference(data_bin_weights, phsp_bin_weights):
    deltas = [
        np.sum(phsp_weights) - np.sum(data_weights)
        for phsp_weights, data_weights in zip(data_bin_weights, phsp_bin_weights)
    ]
    variances = [
        np.sum(np.square(phsp_weights)) + np.sum(np.square(data_weights))
        for phsp_weights, data_weights in zip(data_bin_weights, phsp_bin_weights)
    ]

    deltas = np.array(deltas)
    variances = np.array(variances)
    error = np.sqrt(variances)
    pulls = deltas / error

    return pulls

def plot_chi2(
    kdtree,
    x_data,
    y_data,
    data_weights,
    data_selection,
    x_phsp,
    y_phsp,
    phsp_weights,
    kinematic_limits,
    xlabel,
    ylabel,
    nparams,
    output_path,
):
    binnings = get_kdtree_binnings(kdtree, kinematic_limits)
    data_bin_x, data_bin_y, data_bin_weights = split_sample_with_kdtree(
        kdtree, x_data, y_data, data_weights
    )
    phsp_bin_x, phsp_bin_y, phsp_bin_weights = split_sample_with_kdtree(
        kdtree, x_phsp, y_phsp, phsp_weights
    )

    binnings = refine_binnings(
        binnings, data_bin_x, data_bin_y, phsp_bin_x, phsp_bin_y, kinematic_limits
    )

    scatter_x = x_data[data_selection]
    scatter_y = y_data[data_selection]

    # Only plot 10% of data points
    # to_scatter = np.random.uniform(size=np.sum(data_selection)) > 0.9

    # scatter_x = scatter_x[to_scatter]
    # scatter_y = scatter_y[to_scatter]

    vertices = get_poly_vertices(binnings)
    pulls = get_per_bin_difference(data_bin_weights, phsp_bin_weights)
    chi2 = np.sum(np.square(pulls))
    ndof = pulls.size - nparams
    chi2ndof = chi2/ndof

    plt.rcParams["font.size"] = 28
    figure, axes = plt.subplots(figsize=(15, 12), dpi=100)

    cmap = matplotlib.colormaps["seismic"]  # , 'plasma', 'inferno', 'magma', 'cividis'
    norm = matplotlib.colors.Normalize(vmin=-5, vmax=5)
    # colormap = matplotlib.cm.ScalarMappable(norm=norm, cmap=cmap)

    poly = PolyCollection(vertices, closed=True, linewidth=0, cmap=cmap, norm=norm)
    poly.set_array(pulls)
    axes.add_collection(poly)
    xmin, xmax = axes.get_xlim()
    ymin, ymax = axes.get_ylim()
    xlen = xmax - xmin
    ylen = ymax - ymin
    axes.set_xlim( xmin=xmin - 0.1 * xlen, xmax=xmax + 0.1 * xlen )
    axes.set_ylim( ymin=ymin - 0.1 * ylen, ymax=ymax + 0.1 * ylen )

    figure.colorbar(poly, ax=axes)
    axes.autoscale()
    axes.set_ylabel(ylabel)
    axes.set_xlabel(xlabel)

    figure.suptitle(f"$\\chi^{2}$/ndof = {int(chi2)}/{ndof} = {chi2ndof:.3f}")
    figure.tight_layout()
    figure.savefig(output_path)

    plt.clf()
    plt.close()

    # xy = get_poly_xy(binnings)
    # ndof = 389 # This is fixed

    # histogram = r.TH2Poly("hist_data", "#chi", xlow, xhigh, ylow, yhigh)
    # model_histogram =  r.TH2Poly("hist_model", "#chi", xlow, xhigh, ylow, yhigh)

    # data_bin_

    # for binning in binnings:
    #     histogram.AddBin(binning.xlow, binning.ylow, binning.xhigh, binning.yhigh)
    #     model_histogram.AddBin(binning.xlow, binning.ylow, binning.xhigh, binning.yhigh)

    # # Change the boundaries of the bins to reflect data
    # for binning in bins:
    #     x_band_data = np.logical_and(y_data > binning.ylow, y_data < binning.yhigh)
    #     x_band_phsp = np.logical_and(y_phase_space > binning.ylow, y_phase_space < binning.yhigh)
    #     y_band_data = np.logical_and(x_data > binning.xlow, x_data < binning.xhigh)
    #     y_band_phsp = np.logical_and(x_phase_space > binning.xlow, x_phase_space < binning.xhigh)

    #     x_band_points = np.concatenate((x_data[x_band_data], x_phase_space[x_band_phsp]))
    #     y_band_points = np.concatenate((y_data[y_band_data], y_phase_space[y_band_phsp]))

    #     if not np.any(x_band_points > binning.xhigh):
    #         indices_data = find_indices(x_data, y_data, binning)
    #         indices_phsp = find_indices(x_phase_space, y_phase_space, binning)
    #         xmax = max(np.amax(x_data[indices_data]), np.amax(x_phase_space[indices_phsp]))
    #         binning.xhigh = min(xhigh, xmax + 0.01 * (xmax - binning.xlow))
    #     elif not np.any(x_band_points < binning.xlow):
    #         indices_data = find_indices(x_data, y_data, binning)
    #         indices_phsp = find_indices(x_phase_space, y_phase_space, binning)
    #         xmin = min(np.amin(x_data[indices_data]), np.amin(x_phase_space[indices_phsp]))
    #         binning.xlow = max(xlow, xmin - 0.01 * (binning.xhigh - xmin))

    #     if not np.any(y_band_points < binning.ylow):
    #         indices_data = find_indices(x_data, y_data, binning)
    #         indices_phsp = find_indices(x_phase_space, y_phase_space, binning)
    #         ymin = min(np.amin(y_data[indices_data]), np.amin(y_phase_space[indices_phsp]))
    #         binning.ylow = max(ylow, ymin - 0.01 * (binning.yhigh - ymin))
    #     elif not np.any(y_band_points > binning.yhigh):
    #         indices_data = find_indices(x_data, y_data, binning)
    #         indices_phsp = find_indices(x_phase_space, y_phase_space, binning)
    #         ymax = max(np.amax(y_data[indices_data]), np.amax(y_phase_space[indices_phsp]))
    #         binning.yhigh = min(yhigh, ymax + 0.01 * (ymax - binning.ylow))

    # histogram.Sumw2()
    # model_histogram.Sumw2()

    # histogram.FillN(weights.shape[0], x_data.data, y_data.data, weights.data)
    # model_histogram.FillN(weights_phase_space.shape[0], x_phase_space.data, y_phase_space.data, weights_phase_space.data)
    # scale = np.sum(weights_data) / np.sum(weights_phase_space)

    # chi2 = 0
    # for i in range(1, histogram.GetNumberOfBins()+1):
    #     model_this_bin = scale * model_histogram.GetBinContent(i)
    #     model_this_bin_error = scale * model_histogram.GetBinError(i)
    #     delta = histogram.GetBinContent(i) - model_this_bin
    #     error = np.sqrt( np.square( model_this_bin_error ) + np.square( histogram.GetBinError( i ) ) )
    #     this_bin_chi2 = np.sign(delta) * np.square( delta / error )
    #     chi2 += np.abs(this_bin_chi2)
    #     histogram.SetBinContent( i, max( min( this_bin_chi2, 5 ), -5 ) )

    # histogram.SetMaximum(5)
    # histogram.SetMinimum(-5)
    # histogram.SetStats(False)
    # histogram.GetXaxis().SetTitle(xlabel)
    # histogram.GetYaxis().SetTitle(ylabel)
    # histogram.SetMarkerSize(0)
    # histogram.SetLineWidth(1)
    # histogram.SetLineColorAlpha(r.kBlack, 0.3)
    # canvas = r.TCanvas("canvas", "canvas", 1200, 1200)
    # histogram.Draw("colz")

    # indices = np.random.uniform(0, 1, size=weights.shape[0]) < 0.04
    # x_scatter = x_data[indices].astype(np.float64)
    # y_scatter = y_data[indices].astype(np.float64)

    # scatter_plot = r.TGraph(int(np.sum(indices)), x_scatter.data, y_scatter.data)
    # scatter_plot.Draw("P;same")
    # histogram.Draw("colz;L;same")

    # canvas.Update()
    # canvas.SaveAs(output_path)
    # canvas.Clear()
    # canvas.Close()

    # print(histogram.Integral(), chi2, histogram.GetNumberOfBins(), histogram.GetNumberOfBins() - 318)

    # del histogram
    # del model_histogram
    # del scatter_plot
    # del canvas


def parse_args():
    parser = argparse.ArgumentParser(description="Script to plot 2D chi2 of fit.")
    parser.add_argument(
        "--mc_weights", type=str, required=True, help="Path to MC weight file."
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
    parser.add_argument("--outdir", type=str, required=True, help="Output directory.")
    parser.add_argument("--nparams", type=int, required=True, help="Number free parameters in the fit.")
    parser.add_argument(
        "--min_sig",
        type=float,
        default=7.,
        help="Minimum significance in each bin."
    )

    args = parser.parse_args()
    return args


if __name__ == "__main__":
    PROTON_MASS = 0.938272
    K_MASS = 0.493677
    JPSI_MASS = 3.096916
    LB_MASS = 5.6196

    settings = [
        (
            "mkp",
            "mjpsip",
            "$m(Kp)$ [GeV]",
            "$m(J/\\psi p)$ [GeV]",
            K_MASS + PROTON_MASS,
            LB_MASS - JPSI_MASS,
            JPSI_MASS + PROTON_MASS,
            LB_MASS - K_MASS,
        ),
        (
            "mkp",
            "cosTheta_L",
            "$m(Kp)$ [GeV]",
            "$cos(\\theta_{\\Lambda})$",
            K_MASS + PROTON_MASS,
            LB_MASS - JPSI_MASS,
            -1,
            1,
        ),
        (
            "mjpsip",
            "Z_cosTheta_Z",
            "$m(Kp)$ [GeV]",
            "$cos(\\theta_{P_{c}})$",
            JPSI_MASS + PROTON_MASS,
            LB_MASS - K_MASS,
            -1,
            1,
        ),
    ]

    args = parse_args()

    variables = list()
    for setting in settings:
        variables.append(setting[0])
        variables.append(setting[1])

    variables = list(set(variables))  # Only keep unique variables

    dataframe = r.RDataFrame("DTree", args.data_root)
    dataframe = dataframe.Filter(
        "mkp>1.431950001&&mkp<2.522583999999&&mjpsip>4.0351880460&&mjpsip<5.125823"
    )
    data_columns = dataframe.AsNumpy(variables + ["nsig_sw", "Lb_LOKI_MASS_JpsiConstr"])
    # Apply a tight +/- 20 MeV cut when choosing data to choose cleaner dataset
    #   when making KDTree and plotting scatter plot
    data_selection = (
        np.abs(data_columns["Lb_LOKI_MASS_JpsiConstr"] - 1000 * LB_MASS) < 20
    )
    # data_columns = { key: column[data_selection] for key, column in data_columns.items() }
    data_weights = data_columns["nsig_sw"]

    dataframe = r.RDataFrame("h1", args.mc_root)
    dataframe = dataframe.Filter(
        " mkp >= (0.493677 + 0.938272046) && mkp <= (5.61951 - 3.096916) && mjpsip >= (3.096916 + 0.938272046) && (mjpsip + 0.493677) <= 5.61951"
    )
    phsp_columns = dataframe.AsNumpy(variables)

    dataframe = r.RDataFrame("tree", args.mc_weights)
    phsp_weights = dataframe.AsNumpy(["amp2_weight"])["amp2_weight"]

    phsp_scale = np.sum(data_weights) / np.sum(phsp_weights)
    phsp_weights = phsp_scale * phsp_weights


    os.makedirs(args.outdir, exist_ok=True)

    for setting in settings:
        nameA = setting[0]
        nameB = setting[1]
        labelA = setting[2]
        labelB = setting[3]

        kinematic_limits = Bin2D(setting[4], setting[6], setting[5], setting[7])

        varA = data_columns[nameA]
        varB = data_columns[nameB]
        # varA = data_columns[nameA][data_selection]
        # varB = data_columns[nameB][data_selection]
        kdtree = get_kdtree(varA, varB, data_weights, args.min_sig)

        plot_chi2(
            kdtree,
            data_columns[nameA],
            data_columns[nameB],
            data_weights,
            data_selection,
            phsp_columns[nameA],
            phsp_columns[nameB],
            phsp_weights,
            kinematic_limits,
            labelA,
            labelB,
            args.nparams,
            os.path.join(args.outdir, f"{nameA}_vs_{nameB}_2Dchi2.pdf"),
        )
