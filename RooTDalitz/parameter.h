#pragma once

#include "RooRealVar.h"
#include "RooArgList.h"

namespace Parameter{
    void random_smear(RooRealVar* variable, double delta);
    void configure_error(RooRealVar* variable, bool setError = false);
    void FloatZPar(
        RooArgList *arglist,
        bool reset = true,
        bool dornd_LS = false,
        bool lowestL = false,
        bool dornd_mass = false,
        bool largeRandom = false
    );
    void FloatRGPar(RooArgList *arglist, bool dornd = false, bool dornd_crazy = false);
    void FloatPar(RooArgList *argli, int index);
} // namespace Parameter
