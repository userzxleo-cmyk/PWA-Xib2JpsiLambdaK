#include <stdexcept>
#include "RooTDalitz/config.h"

namespace Config{
    Mode get_mode(const TString& string_option){
        Mode mode;

        if (string_option == "fit") mode = Mode::Fit;
        else if (string_option == "nll") mode = Mode::NLL;
        else if (string_option == "weights") mode = Mode::Store_Weights;
        else{
            throw std::logic_error("Invalid mode: " + string_option);
            mode = Mode::Invalid;
        }

        return mode;
    }

    SpinParity get_spin_parity(const std::vector<int>& spin_parity_option){
        return SpinParity(spin_parity_option[0], spin_parity_option[1]);
    }
} // namespace Config
