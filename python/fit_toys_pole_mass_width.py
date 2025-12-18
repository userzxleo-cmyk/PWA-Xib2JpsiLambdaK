import os, sys, argparse, glob, yaml
import json, scipy, matplotlib, warnings
from typing import List, Optional, Dict
import numpy as np
import matplotlib.pyplot as plt
import ROOT as r
from enum import Enum

from common.fit_and_plot_2D import (
    get_fit_results,
    fit_2D,
    get_gaussian_contour,
    plot_2D,
)

from matplotlib.ticker import MaxNLocator, AutoMinorLocator

# import json
# from array import array

# from ROOT import TGraph, TFile, TCanvas, TF2, TH2F
# import ROOT

# import lhcbStyle

# from typing import List


class Hypothesis(Enum):
    Normal = 1
    Inverted = 2

    def get_hypothesis(hypothesis):
        if hypothesis == "normal":
            return Hypothesis.Normal
        elif hypothesis == "inverted":
            return Hypothesis.Inverted
        else:
            raise ValueError("Invalid hypothesis!")


class SpinParity(Enum):
    OneHalfMinus = 1
    ThreeHalfMinus = 2


class Channels(Enum):
    Jpsip = 1
    SigmacD = 2
    SigmacDst = 3
    SigmacstD = 4
    SigmacstDst = 5

    def __str__(self):
        return {
            Channels.Jpsip: "Jpsip",
            Channels.SigmacD: "SigmacD",
            Channels.SigmacDst: "SigmacDst",
            Channels.SigmacstD: "SigmacstD",
            Channels.SigmacstDst: "SigmacstDst",
        }[self]

    def latex(self):
        return {
            Channels.Jpsip: "J/\\psi p",
            Channels.SigmacD: "\\Sigma_{c}D",
            Channels.SigmacDst: "\\Sigma_{c}D^{*}",
            Channels.SigmacstD: "\\Sigma_{c}^{*}D",
            Channels.SigmacstDst: "\\Sigma_{c}^{*}D^{*}",
        }[self]


def parse_args():
    parser = argparse.ArgumentParser(description="Run Pc pole calculation.")
    parser.add_argument(
        "--hypothesis",
        type=str,
        required=True,
        choices=["normal", "inverted"],
        help="Spin-parity hypothesis.",
    )
    parser.add_argument(
        "--glob",
        type=str,
        required=True,
        help="Glob pattern to find ROOT file containing fit results.",
    )
    parser.add_argument(
        "--pole_dir", type=str, required=True, help="Path pole JSON directory."
    )
    parser.add_argument(
        "--scattering_dir",
        type=str,
        required=True,
        help="Path scattering JSON directory.",
    )
    parser.add_argument(
        "--output_dir", type=str, required=True, help="Path to output directory."
    )
    parser.add_argument(
        "--exclude",
        type=str,
        help="Path to txt file containing list of fits to exclude.",
    )
    parser.add_argument(
        "--limits", type=str, required=True, help="Path to yaml containing limits."
    )
    parser.add_argument(
        "--filter_limits",
        action="store_true",
        help="Filter out fits which have results out of limits.",
    )
    parser.add_argument(
        "--default_poles",
        type=str,
        required=True,
        help="Path to pole JSON of default fit.",
    )
    parser.add_argument(
        "--default_scattering",
        type=str,
        required=True,
        help="Path to scattering JSON of default fit.",
    )

    args = parser.parse_args()
    return args


def read_json_file(path):
    with open(path, "r") as file:
        results = json.load(file)
    return results


def get_Pc_pole_parameters(pole_dir, tags):
    pole_parameters = dict()

    for tag in tags:
        path = os.path.join(pole_dir, f"{tag}.json")
        if os.path.exists(path):
            results = read_json_file(path)
            pole_parameters[tag] = results
            # (results["poles"], results["residues_moduli"])

    return pole_parameters


def get_Pc_scattering_parameters(scattering_dir, tags):
    scattering_parameters = dict()

    for tag in tags:
        path = os.path.join(scattering_dir, f"{tag}.json")
        if os.path.exists(path):
            results = read_json_file(path)
            scattering_parameters[tag] = results["scatteringparameters"]
            # (results["poles"], results["residues_moduli"])

    return scattering_parameters


def write_2D_fit_result_to_json(
        default_x,
        default_y,
        values,
        errors,
        xtag,
        ytag,
        path
):
    dump_dict = {
        "default": {
            f"{xtag}": default_x,
            f"{ytag}": default_y,
        },
        f"mu_{xtag}": {
            "value": values[0],
            "error": errors[0],
        },
        f"mu_{ytag}": {
            "value": values[1],
            "error": errors[1],
        },
        f"sigma_{xtag}": {
            "value": values[2],
            "error": errors[2],
        },
        f"sigma_{ytag}": {
            "value": values[3],
            "error": errors[3],
        },
        "rho": {
            "value": values[4],
            "error": errors[4],
        },
    }
    with open(path, "w") as file:
        json.dump(dump_dict, file, indent=2)

    

def write_mass_width_to_json(default_mass, default_width, values, errors, path):
    dump_dict = {
        "default": {
            "mass": default_mass,
            "width": default_width,
        },
        "mu_mass": {
            "value": values[0],
            "error": errors[0],
        },
        "mu_width": {
            "value": values[1],
            "error": errors[1],
        },
        "sigma_mass": {
            "value": values[2],
            "error": errors[2],
        },
        "sigma_width": {
            "value": values[3],
            "error": errors[3],
        },
        "rho": {
            "value": values[4],
            "error": errors[4],
        },
    }
    with open(path, "w") as file:
        json.dump(dump_dict, file, indent=2)


def fit_and_plot_pole_positions(
    output_dir,
    this_pole_parameters,
    default_mass,
    default_width,
    label,
    plot_label,
    mass_width_limits,
):
    os.makedirs(output_dir, exist_ok=True)
    pole_masses = np.array([parameters["m_MeV"] for parameters in this_pole_parameters])
    pole_widths = np.array([parameters["Γ_MeV"] for parameters in this_pole_parameters])

    mass_limits = mass_width_limits["m_MeV"] if mass_width_limits else None
    width_limits = mass_width_limits["Γ_MeV"] if mass_width_limits else None

    mass_limits = (mass_limits["min"], mass_limits["max"]) if mass_limits else None
    width_limits = (width_limits["min"], width_limits["max"]) if width_limits else None

    result = fit_2D(pole_masses, pole_widths, label)
    dense_hess_inv = result.hess_inv.todense()

    values = result.x
    errors = [dense_hess_inv[i, i] for i in range(len(values))]
    plot_path = os.path.join(output_dir, f"{label}_mass_width.pdf")
    gauss_contour = get_gaussian_contour(values)

    xlabel = f"{plot_label} Mass [MeV]"
    ylabel = f"{plot_label} Width [MeV]"

    # text_string = "\n".join(
    #     [
    #         f"$\\mu_{{m}} = {parameters[0]:.2f}$,\t $\\sigma_{{m}} = {parameters[2]:.2f}$, \t $m_{{default}} = {default_mass:.2f}$",
    #         f"$\\mu_{{\\Gamma}} = {parameters[1]:.2f}$,\t\t $\\sigma_{{\\Gamma}} = {parameters[3]:.2f}$, \t $\\Gamma_{{default}} = {default_width:.2f}$",
    #         f"$\\rho = {parameters[4]:.3f}$",
    #     ]
    # )
    # textbox = (text_string, 0.02, 0.98, "left", "top")

    write_2D_fit_result_to_json(
        default_mass,
        default_width,
        values,
        errors,
        "mass",
        "width",
        os.path.join(output_dir, f"{label}_mass_width.json"),
    )

    plot_2D(
        plot_path,
        pole_masses,
        pole_widths,
        gauss_contour,
        xlabel,
        ylabel,
        legend_position="upper right",
        highlight=[default_mass, default_width, "Default Model"],
        xlim=mass_limits,
        ylim=width_limits,
        textbox=None,
    )


def fit_and_plot_mass_residues(
    output_dir: str,
    pole_masses: np.array,
    pole_residues: np.array,
    default_mass: float,
    default_residue: float,
    pole_key: str,
    residue_key: str,
    pole_label: str,
    ylabel: str,
    limits: Optional[Dict[str, float]] = None,
):
    os.makedirs(output_dir, exist_ok=True)

    mass_limits = limits["m_MeV"] if limits else None
    residue_limits = limits[residue_key] if limits else None

    mass_limits = (mass_limits["min"], mass_limits["max"]) if mass_limits else None
    residue_limits = (
        (residue_limits["min"], residue_limits["max"]) if residue_limits else None
    )

    result = fit_2D(pole_masses, pole_residues, f"{pole_label}_{residue_key}")

    parameters = result.x
    plot_path = os.path.join(output_dir, f"{pole_label}_mass_residue_{residue_key}.pdf")
    gauss_contour = get_gaussian_contour(parameters)

    text_string = "\n".join(
        [
            f"$\\mu_{{m}} = {parameters[0]:.2f}$,\t $\\sigma_{{m}} = {parameters[2]:.2f}$, \t $m_{{default}} = {default_mass:.2f}$",
            f"$\\mu_{{{ylabel}}} = {parameters[1]:.2f}$,\t\t $\\sigma_{{{ylabel}}} = {parameters[3]:.2f}$, \t ${ylabel}_{{default}} = {default_residue:.2f}$",
            f"$\\rho = {parameters[4]:.3f}$",
        ]
    )
    textbox = (text_string, 0.02, 0.98, "left", "top")

    xlabel = f"{pole_label} Mass [MeV]"
    ylabel = f"${ylabel}$"
    plot_2D(
        plot_path,
        pole_masses,
        pole_residues,
        gauss_contour,
        xlabel,
        ylabel,
        legend_position="upper right",
        highlight=[default_mass, default_residue, "Default Model"],
        xlim=mass_limits,
        ylim=residue_limits,
        textbox=textbox,
    )


def fit_and_plot_residues(
    output_dir: str,
    residues: List[np.array],
    default_residues: List[float],
    channels: List[Channels],
    pole_tag: str,
    pole_label: str,
    limits: Optional[Dict[str, float]] = None,
):
    os.makedirs(output_dir, exist_ok=True)

    residue_limits = (
        {
            str(channel): limits[str(channel)]
            for channel in channels
            if str(channel) in limits
        }
        if limits
        else None
    )

    print(residue_limits)

    # Jpsip_limits = limits["Jpsip"] if limits else None
    # SigD_limits = limits[SigD_key] if limits else None

    # Jpsip_limits = (Jpsip_limits["min"], Jpsip_limits["max"]) if Jpsip_limits else None
    # SigD_limits = (SigD_limits["min"], SigD_limits["max"]) if SigD_limits else None

    Jpsip_channel = channels[0]
    Jpsip_residue = residues[0]
    default_Jpsip_residue = default_residues[0]
    Jpsip_limits = (
        residue_limits[str(Jpsip_channel)]
        if residue_limits and str(Jpsip_channel) in residue_limits
        else None
    )
    Jpsip_limits = (Jpsip_limits["min"], Jpsip_limits["max"]) if Jpsip_limits else None

    for i in range(len(residues) - 1):
        channel = channels[i + 1]
        residue = residues[i + 1]
        default_residue = default_residues[i + 1]
        residue_tag = str(channel)
        limits = (
            residue_limits[str(channel)]
            if residue_limits and str(channel) in residue_limits
            else None
        )
        limits = (limits["min"], limits["max"]) if limits else None

        result = fit_2D(Jpsip_residue, residue, f"{pole_tag}_{residue_tag}_vs_Jpsip")
        dense_hess_inv = result.hess_inv.todense()

        values = result.x
        errors = [dense_hess_inv[i, i] for i in range(len(values))]
        plot_path = os.path.join(
            output_dir, f"{pole_tag}_residue_{residue_tag}_vs_Jpsip.pdf"
        )
        gauss_contour = get_gaussian_contour(values)

        xlabel = f"${Jpsip_channel.latex()}$ Residue [GeV]"
        ylabel = f"${channel.latex()}$ Residue [GeV]"

        plot_2D(
            plot_path,
            Jpsip_residue,
            residue,
            gauss_contour,
            xlabel,
            ylabel,
            legend_position="upper right",
            highlight=[default_Jpsip_residue, default_residue, "Default Model"],
            xlim=Jpsip_limits,
            ylim=limits,
            textbox=None,
        )
        
        write_2D_fit_result_to_json(
                default_Jpsip_residue,
                default_residue,
                values,
                errors,
                str(Jpsip_channel),
                str(channel),
                os.path.join(output_dir, f"{pole_tag}_residue_{residue_tag}_vs_Jpsip.json"),
            )

def fit_and_plot_scattering(
    output_dir: str,
    scatterings: List[Dict[str, float]],
    default_scattering: Dict[str, float],
    pole_name: str,
    pole_label: str,
    limits: Optional[Dict[str, float]] = None,
):
    os.makedirs(output_dir, exist_ok=True)

    # Get the limits for a and r
    a_limits = limits["a"] if limits and "a" in limits else None
    r_limits = limits["r"] if limits and "r" in limits else None
    a_limits = (a_limits["min"], a_limits["max"]) if a_limits else None
    r_limits = (r_limits["min"], r_limits["max"]) if r_limits else None

    # Get the values for a, r, and X
    a_values = np.array([parameters[f"summary_{pole_name}"]["a_fm"] for parameters in scatterings])
    r_values = np.array([parameters[f"summary_{pole_name}"]["r_fm"]["re"] for parameters in scatterings])
    X_values = np.array([parameters[f"summary_{pole_name}"]["X"] for parameters in scatterings])

    # Get default fit values for a, r, and X
    a_default = default_scattering[f"summary_{pole_name}"]["a_fm"]
    r_default = default_scattering[f"summary_{pole_name}"]["r_fm"]["re"]
    X_default = default_scattering[f"summary_{pole_name}"]["X"]

    # Fit the scattering parameters
    result = fit_2D(a_values, r_values, f"Pc_{pole_name}_a_vs_r")
    dense_hess_inv = result.hess_inv.todense()

    values = result.x
    errors = [dense_hess_inv[i, i] for i in range(len(values))]
    plot_path = os.path.join(
        output_dir, f"Pc_{pole_name}_scattering_ar.pdf"
    )
    gauss_contour = get_gaussian_contour(values)

    xlabel = "Scattering Length, $a$ [fm]"
    ylabel = "Effective Range, $r$ [fm]"

    plot_2D(
        plot_path,
        a_values,
        r_values,
        gauss_contour,
        xlabel,
        ylabel,
        legend_position="upper right",
        highlight=[a_default, r_default, "Default Model"],
        xlim=a_limits,
        ylim=r_limits,
        textbox=None,
    )

    write_2D_fit_result_to_json(
            a_default,
            r_default,
            values,
            errors,
            "a",
            "r",
            os.path.join(output_dir, f"Pc_{pole_name}_scattering_ar.json"),
        )

    if X_default is not None:
        compositeness_path = os.path.join(
            output_dir, f"Pc_{pole_name}_scattering_X.pdf"
        )
        xlabel = "Compositeness, $X$"
        ylabel = "Number of Toys"
        plot_1D(
            compositeness_path,
            X_values,
            X_default,
            xlabel,
            ylabel,
            100,
            0.0,
            1.0,
        )
    # scattering_lengths = np.array([parameters["a_fm"] for parameters in scatterings])
    # effective_range = np.array(
    #     [
    #         np.sqrt(
    #             np.square(parameters["r_fm"]["re"])
    #             + np.square(parameters["r_fm"]["im"])
    #         )
    #         for parameters in scatterings
    #     ]
    # )

    # default_a = default_scattering["a_fm"]
    # default_r = np.sqrt(
    #     np.square(default_scattering["r_fm"]["re"])
    #     + np.square(default_scattering["r_fm"]["im"])
    # )

    # result = fit_2D(scattering_lengths, effective_range, f"{pole_label}_scattering")

    # parameters = result.x
    # plot_path = os.path.join(output_dir, f"{pole_label}_scattering.pdf")
    # gauss_contour = get_gaussian_contour(parameters)

    # text_string = "\n".join(
    #     [
    #         f"$\\mu_{{a}} = {parameters[0]:.2f}$,\t $\\sigma_{{a}} = {parameters[2]:.2f}$, \t $a_{{default}} = {default_a:.2f}$",
    #         f"$\\mu_{{|r|}} = {parameters[1]:.2f}$,\t\t $\\sigma_{{|r|}} = {parameters[3]:.2f}$, \t $|r|_{{default}} = {default_r:.2f}$",
    #         f"$\\rho = {parameters[4]:.3f}$",
    #     ]
    # )
    # textbox = (text_string, 0.02, 0.98, "left", "top")

    # xlabel = "Scattering Length [fm]"
    # ylabel = "Modulus Effective Range [fm]"
    # plot_2D(
    #     plot_path,
    #     scattering_lengths,
    #     effective_range,
    #     gauss_contour,
    #     xlabel,
    #     ylabel,
    #     legend_position="upper right",
    #     highlight=[default_a, default_r, "Default Model"],
    #     xlim=a_limits,
    #     ylim=r_limits,
    #     textbox=textbox,
    # )


def exclude_fit_results(exclude_txtfile, tags, fit_results):
    with open(exclude_txtfile, "r") as file:
        tags_to_exclude = file.readlines()
        file.close()

    tags_to_exclude = [tag.strip("\n") for tag in tags_to_exclude]

    filtered_tags = list()
    filtered_results = list()

    for tag, result in zip(tags, fit_results):
        if tag in tags_to_exclude:
            print(f"Excluding fit: {tag}!")
        else:
            filtered_tags.append(tag)
            filtered_results.append(result)

    return filtered_tags, filtered_results


def read_limits_yaml(yaml_path):
    with open(yaml_path, "r") as file:
        limits = yaml.safe_load(file)

    return limits


def filter_results_out_of_limits(
    fit_results, pole_parameters, scattering_parameters, limits, pole_settings
):
    filtered_results = list()
    filtered_poles = list()
    filtered_scattering = list()

    for result, parameters, scattering in zip(
        fit_results, pole_parameters, scattering_parameters
    ):
        in_limit = True

        for pole_name, _ in pole_settings:
            json_key = f"pole{pole_name}"
            this_pole_parameters = parameters["poles"][json_key]
            this_pole_limits = limits[json_key]

            if (this_pole_parameters["m_MeV"] < this_pole_limits["m_MeV"]["min"]) or (
                this_pole_parameters["m_MeV"] > this_pole_limits["m_MeV"]["max"]
            ):
                in_limit = False
                break

            if (this_pole_parameters["Γ_MeV"] < this_pole_limits["Γ_MeV"]["min"]) or (
                this_pole_parameters["Γ_MeV"] > this_pole_limits["Γ_MeV"]["max"]
            ):
                in_limit = False
                break

        if in_limit:
            filtered_results.append(result)
            filtered_poles.append(parameters)
            filtered_scattering.append(scattering)

    return filtered_results, filtered_poles, filtered_scattering


def filter_toys(fit_results, pole_parameters, scattering_parameters, tags):
    # Get both fit results and pole parameters in the same order
    fit_results = [
        result for result, tag in zip(fit_results, tags) if tag in pole_parameters
    ]
    reordered_pole_parameters = [
        pole_parameters[tag] for tag in tags if tag in pole_parameters
    ]
    reordered_scattering_parameters = [
        scattering_parameters[tag] for tag in tags if tag in pole_parameters
    ]

    to_exclude = [
        any(
            [
                single_pole_parameters["Γ_MeV"] < 0.5
                for single_pole_parameters in parameters["poles"].values()
            ]
        )
        for parameters in reordered_pole_parameters
    ]

    fit_results = [
        result for exclude, result in zip(to_exclude, fit_results) if not exclude
    ]
    reordered_pole_parameters = [
        parameters
        for exclude, parameters in zip(to_exclude, reordered_pole_parameters)
        if not exclude
    ]
    reordered_scattering_parameters = [
        parameters
        for exclude, parameters in zip(to_exclude, reordered_scattering_parameters)
        if not exclude
    ]

    return fit_results, reordered_pole_parameters, reordered_scattering_parameters

def plot_1D(
    plot_path: str,
    values: np.array,
    default: float,
    xlabel: str,
    ylabel: str,
    nbins: int,
    xmin: float,
    xmax: float,
):
    plt.rcParams["font.size"] = 20
    figure = plt.figure(figsize=(12, 9))
    axes = plt.axes()

    edges = np.linspace(xmin, xmax, nbins + 1)
    y, _ = np.histogram(values, bins=edges)
    axes.stairs(
        values=y,
        edges=edges,
        color="tab:blue",
        label="Toy Results",
    )
    axes.axvline(
        x=default,
        color="tab:red",
        label="Default Model",
    )
    axes.set_xlabel(xlabel)
    axes.set_ylabel(ylabel)
    axes.xaxis.set_major_locator(MaxNLocator(5))
    axes.xaxis.set_minor_locator(AutoMinorLocator(5))
    axes.yaxis.set_major_locator(MaxNLocator(5))
    axes.yaxis.set_minor_locator(AutoMinorLocator(5))
    axes.tick_params(axis="both", which="major", direction="inout", length=10, width=2)
    axes.tick_params(axis="both", which="minor", direction="inout", length=5, width=1)
    axes.legend()
    # axes.legend(loc=legend_position, fontsize=20)
    figure.tight_layout()
    figure.savefig(plot_path)
    figure.clear()
    plt.clf()
    plt.close()


def plot_residue_ratios(
    output_dir: str,
    residues: List[np.array],
    default_residues: List[float],
    channels: List[Channels],
    pole_tag: str,
    pole_label: str,
):
    os.makedirs(output_dir, exist_ok=True)

    Jpsip_channel = channels[0]
    Jpsip_residue = residues[0]
    default_Jpsip_residue = default_residues[0]

    for i in range(len(residues) - 1):
        channel = channels[i + 1]
        residue = residues[i + 1]
        default_residue = default_residues[i + 1]
        residue_tag = str(channel)

        ratios = residue / Jpsip_residue
        default_ratio = default_residue / default_Jpsip_residue

        xlabel = f"${channel.latex()}/{Jpsip_channel.latex()}$ Residue Ratio"
        ylabel = "Number of Toys"
        plot_path = os.path.join(
            output_dir, f"{pole_tag}_residue_ratio_{residue_tag}_vs_Jpsip.pdf"
        )

        nbins = 30
        xrange = np.amax(ratios) - np.amin(ratios)
        xmin = min(0, np.amin(ratios) - 0.1 * xrange)
        xmax = np.amax(ratios) + 0.1 * xrange

        plot_1D(
            plot_path,
            ratios,
            default_ratio,
            xlabel,
            ylabel,
            nbins,
            xmin,
            xmax,
        )

        # write_2D_fit_result_to_json(
        #         default_Jpsip_residue,
        #         default_residue,
        #         values,
        #         errors,
        #         str(Jpsip_channel),
        #         str(channel),
        #         os.path.join(output_dir, f"{pole_tag}_residue_{residue_tag}_vs_Jpsip.json"),
        #     )

def main():
    plt.rcParams["font.size"] = 32

    args = parse_args()
    # Read fit results and various settings
    tags, fit_results = get_fit_results(args.glob)
    if args.exclude:
        tags, fit_results = exclude_fit_results(args.exclude, tags, fit_results)
    print(args.hypothesis)
    hypothesis = Hypothesis.get_hypothesis(args.hypothesis)
    pole_parameters = get_Pc_pole_parameters(args.pole_dir, tags)
    scattering_parameters = get_Pc_scattering_parameters(args.scattering_dir, tags)
    limits = read_limits_yaml(args.limits) if args.limits else None
    default_poles = read_json_file(args.default_poles)
    default_scattering = read_json_file(args.default_scattering)["scatteringparameters"]

    if hypothesis == Hypothesis.Normal:
        pole_settings = [
            (4312, SpinParity.OneHalfMinus),
            (4440, SpinParity.OneHalfMinus),
            (4457, SpinParity.ThreeHalfMinus),
        ]
    elif hypothesis == Hypothesis.Inverted:
        pole_settings = [
            (4312, SpinParity.OneHalfMinus),
            (4440, SpinParity.ThreeHalfMinus),
            (4457, SpinParity.OneHalfMinus),
        ]
    else:
        raise ValueError("Invalid hypothesis!")

    fit_results, pole_parameters, scattering_parameters = filter_toys(
        fit_results, pole_parameters, scattering_parameters, tags
    )

    if limits and args.filter_limits:
        (
            fit_results,
            pole_parameters,
            scattering_parameters,
        ) = filter_results_out_of_limits(
            fit_results, pole_parameters, scattering_parameters, limits, pole_settings
        )

    for pole_name, spin_parity in pole_settings:
        pole_key = f"pole{pole_name}"
        pole_positions = [
            parameters["poles"][pole_key] for parameters in pole_parameters
        ]
        pole_limits = limits[pole_key] if limits else None

        default_mass = default_poles["poles"][pole_key]["m_MeV"]
        default_width = default_poles["poles"][pole_key]["Γ_MeV"]

        tag = f"Pc_{pole_name}"
        label = f"$P_{{c\\bar{{c}}}}({pole_name})$"
        fit_and_plot_pole_positions(
            args.output_dir,
            pole_positions,
            default_mass,
            default_width,
            tag,
            label,
            pole_limits,
        )

        residue_key = f"gv_{pole_name}_GeV"
        residues = [
            np.array(
                [
                    parameters["residues_moduli"][residue_key][i]
                    for parameters in pole_parameters
                ]
            )
            for i in range(4)
        ]
        default_residues = [
            default_poles["residues_moduli"][residue_key][i] for i in range(4)
        ]

        if spin_parity == SpinParity.OneHalfMinus:
            channels = [
                Channels.Jpsip,
                Channels.SigmacDst,
                Channels.SigmacstDst,
                Channels.SigmacD,
            ]
        elif spin_parity == SpinParity.ThreeHalfMinus:
            channels = [
                Channels.Jpsip,
                Channels.SigmacDst,
                Channels.SigmacstDst,
                Channels.SigmacstD,
            ]
        else:
            raise ValueError("Invalid spin-parity!")

        for i, residue in enumerate(residues):
            fit_and_plot_residues(
                args.output_dir,
                residues,
                default_residues,
                channels,
                tag,
                label,
                pole_limits,
            )
            plot_residue_ratios(
                args.output_dir,
                residues,
                default_residues,
                channels,
                tag,
                label,
            )

        fit_and_plot_scattering(
            args.output_dir,
            scattering_parameters,
            default_scattering,
            pole_name,
            label,
            pole_limits,
        )

            # fit_and_plot_mass_residues(
            #     args.output_dir,
            #     this_pole_masses,
            #     residue,
            #     default_mass,
            #     default_residues[i],
            #     pole_key,
            #     residue_key,
            #     label,
            #     f"g()",
            #     this_pole_limits,
            # )

#         this_pole_scattering = [
#             parameters[scattering_key] for parameters in scattering_parameters
#         ]
#         this_pole_default_scattering = default_scattering[scattering_key]

#         fit_and_plot_scattering(
#             args.output_dir,
#             this_pole_scattering,
#             this_pole_default_scattering,
#             label,
#             this_pole_limits,
#         )


if __name__ == "__main__":
    main()
