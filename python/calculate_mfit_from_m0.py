import argparse

import numpy as np
import scipy.optimize


def parse_args():
    parser = argparse.ArgumentParser(
        description="Calculates mfit of new parameterisation given m0."
    )
    parser.add_argument("--m0", type=float, required=True, help="Old m0.")

    parser.add_argument(
        "--g1", type=float, required="True", help="Coupling to Jpsip channel."
    )
    parser.add_argument(
        "--rg",
        type=float,
        required="True",
        help="Coupling ratio of threshold channel / Jpsi p.",
    )
    parser.add_argument(
        "--channel",
        type=str,
        required=True,
        choices=["SigmacD", "SigmacDst"],
        help="Threshold channel",
    )
    args = parser.parse_args()

    return args


def get_rho(s: float, m1: np.complex64, m2: np.complex64):
    sum_12 = m1 + m2
    delta_12 = m1 - m2
    sum_sq = sum_12 * sum_12
    delta_sq = delta_12 * delta_12
    numerator_sq = (s - sum_sq) * (s - delta_sq)
    rho = np.sqrt(numerator_sq) / s

    return rho


def get_mKmatrix_square(
    mfit: float, g: float, Sigmac_mass: float, Sigmac_width: float, D_mass: float
):
    s = mfit * mfit
    rho = get_rho(
        s,
        np.complex64(Sigmac_mass + Sigmac_width * 1j),
        np.complex64(D_mass),
    )
    return s + np.real(np.square(g) * rho * 1j)


def func(
    mfit: float,
    g: float,
    Sigmac_mass: float,
    Sigmac_width: float,
    D_mass: float,
    m0: float,
):
    return get_mKmatrix_square(mfit, g, Sigmac_mass, Sigmac_width, D_mass) - m0 * m0


if __name__ == "__main__":
    Sigmacst_width = -0.00800
    Sigmac_width = -0.00105
    Dst_mass = 2.008555
    D_mass = 1.86719
    Sigmac_mass = 2.453435
    Sigmacst_mass = 2.517955

    meson_masses = {"SigmacD": D_mass, "SigmacDst": Dst_mass}

    args = parse_args()

    g = args.g1 * args.rg

    result = scipy.optimize.fsolve(
        func,
        args.m0 + 0.03,
        args=(g, Sigmac_mass, Sigmac_width, meson_masses[args.channel], args.m0),
    )
    print(result)
