from typing import List, Optional, Tuple
import glob, os

import ROOT as r
import numpy as np
import scipy.optimize

import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator, AutoMinorLocator

def nll(parameters: List[float], x: np.array, y: np.array, weights: Optional[np.array] = None):
    mu_x = parameters[0]
    mu_y = parameters[1]
    sigma_x = parameters[2]
    sigma_y = parameters[3]
    rho = parameters[4]

    Z_x = (x - mu_x) / sigma_x
    Z_y = (y - mu_y) / sigma_y

    # Only calculate negative of exponent since negative log likelihood will cancel the minus sign
    neg_exponent = (0.5 / (1 - rho * rho)) * (
        Z_x * Z_x - 2 * rho * Z_x * Z_y + Z_y * Z_y
    )
    # normalisation is 1./inv_normalisation
    inv_normalisation = 2 * np.pi * sigma_x * sigma_y * np.sqrt(1 - rho * rho)

    # Per-event negative log likelihood
    negative_log_likelihood = np.log(inv_normalisation) + neg_exponent
    if weights is not None:
        negative_log_likelihood = weights * negative_log_likelihood

    return np.sum(negative_log_likelihood)

def fit_2D(x: np.array, y: np.array, label: str, weights: Optional[np.array] = None):
    initial_values = [
        np.mean(x),
        np.mean(y),
        np.std(x),
        np.std(y),
        0.1,
    ]
    bounds = [(None, None), (None, None), (1e-5, None), (1e-5, None), (-0.9999, 0.9999)]

    print(f"Fitting {label}!")

    result = scipy.optimize.minimize(
        nll, x0=initial_values, args=(x, y, weights), bounds=bounds, method="L-BFGS-B"
    )

    if result.success:
        print("Fit success!")
        print(result.message)
    else:
        warnings.warn("Warning: Fit did not converge")
    print(result)
    print("")

    return result

def get_gaussian_contour(parameters: List[float]):
    mu_x = parameters[0]
    mu_y = parameters[1]
    sigma_x = parameters[2]
    sigma_y = parameters[3]
    rho = parameters[4]

    # Diagonalise the matrix so that we have an orthogonal basis
    inv_covariance = np.array(
        [
            [1 / np.square(sigma_x), -rho / sigma_x / sigma_y],
            [-rho / sigma_x / sigma_y, 1 / np.square(sigma_y)],
        ]
    )
    u, s, inv_u = np.linalg.svd(inv_covariance, hermitian=True)

    # Generate xy in the orthogonal basis
    theta = np.linspace(-np.pi, np.pi, 1000)
    x_prime = np.sqrt(2 * (1 - rho * rho) / s[0]) * np.cos(theta)
    y_prime = np.sqrt(2 * (1 - rho * rho) / s[1]) * np.sin(theta)

    vector_prime = np.stack((x_prime, y_prime), axis=-1)
    vector_prime = np.expand_dims(vector_prime, axis=-1)

    # Transform the orthogonal basis into our basis
    vector = np.matmul(u, vector_prime)
    vector = np.squeeze(vector)

    # At 1sigma, the Gaussian exponent is -0.5, at 2sigma it is -2.0
    gauss_1sigma_xy = vector * np.sqrt(0.5)
    gauss_2sigma_xy = vector * np.sqrt(2.0)

    # returns the contour as a list of (x, y) tuple
    gaussian_contour = [
        (gauss_1sigma_xy[:, 0] + mu_x, gauss_1sigma_xy[:, 1] + mu_y),
        (gauss_2sigma_xy[:, 0] + mu_x, gauss_2sigma_xy[:, 1] + mu_y),
    ]

    return gaussian_contour


def plot_2D(
    plot_path: str,
    x: np.array,
    y: np.array,
    gauss_contour: List[np.array],
    xlabel: str,
    ylabel: str,
    legend_position: str,
    colors: Optional[np.array] = None,
    highlight: Optional[Tuple[float, float, str]] = None,
    xlim: Optional[Tuple[float, float]] = None,
    ylim: Optional[Tuple[float, float]] = None,
    textbox: Optional[Tuple[str, float, float, str, str]] = None,
):
    figure = plt.figure(figsize=(12, 9))
    axes = plt.axes()

    if colors is not None:
        axes.scatter(x, y, c=colors)
    else:
        axes.scatter(x, y)

    if highlight is not None:
        axes.scatter(
            [highlight[0]],
            [highlight[1]],
            c=["tab:red"],
            s=[500],
            marker="*",
            label=highlight[2],
        )

    if xlim is not None:
        axes.set_xlim(xlim)
    if ylim is not None:
        axes.set_ylim(ylim)
    axes.set_xlabel(xlabel)
    axes.set_ylabel(ylabel)

    axes.plot(
        gauss_contour[0][0],
        gauss_contour[0][1],
        label="$1\\sigma$",
        alpha=0.5,
        color="tab:gray",
        linestyle="solid",
    )
    axes.plot(
        gauss_contour[1][0],
        gauss_contour[1][1],
        label="$2\\sigma$",
        alpha=0.5,
        color="tab:gray",
        linestyle="dashed",
    )

    axes.xaxis.set_major_locator(MaxNLocator(4))
    axes.xaxis.set_minor_locator(AutoMinorLocator(5))
    axes.yaxis.set_major_locator(MaxNLocator(4))
    axes.yaxis.set_minor_locator(AutoMinorLocator(5))
    axes.tick_params(axis="both", which="major", direction="inout", length=10, width=2)
    axes.tick_params(axis="both", which="minor", direction="inout", length=5, width=1)

    if textbox is not None:
        text_string = textbox[0]
        x_position = textbox[1]
        y_position = textbox[2]
        ha = textbox[3]
        va = textbox[4]

        axes.text(
            x_position,
            y_position,
            text_string,
            transform=axes.transAxes,
            fontsize=20,
            horizontalalignment=ha,
            verticalalignment=va,
            bbox={"boxstyle": "round", "alpha": 0.5, "facecolor": "wheat"},
        )

    axes.legend(loc=legend_position, fontsize=20)
    figure.tight_layout()
    figure.savefig(plot_path)
    figure.clear()
    plt.clf()
    plt.close()

def get_fit_results(glob_pattern):
    tags = list()
    fit_results = list()

    for filepath in glob.glob(glob_pattern):
        root_file = r.TFile.Open(filepath, "READ")
        if (not root_file) or (not root_file.IsOpen()):
            continue

        fitresult = root_file.Get("nll")
        # Skip RooFitResult if fit failed
        if fitresult and fitresult.status() == 0 and fitresult.minNll() > -1e6:
            label = os.path.splitext(os.path.basename(filepath))[0]

            tags.append(label)
            fit_results.append(fitresult)
        root_file.Close()

    return tags, fit_results
