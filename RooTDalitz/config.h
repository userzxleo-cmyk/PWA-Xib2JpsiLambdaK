#pragma once

#include <vector>
#include "TString.h"

// Just a struct containing the configurations for the executables
// For simple parsing between function calls
namespace Config{
    enum Mode {
        Fit,
        NLL,
        Store_Weights,
        Invalid
    };

    struct SpinParity{
        int spin;
        int parity;

        SpinParity(int s, int p) : spin(s), parity(p) {};
    };

    Mode get_mode(const TString& string_option);
    SpinParity get_spin_parity(const std::vector<int>& spin_parity_option);
}
