#pragma once

// Core ROOT classes
#include "TString.h"

// RooFit classes
#include "RooArgList.h"
#include "RooArgSet.h"
#include "RooDataSet.h"

// Custom classes
#include "RooTDalitz/RooDalitzAmplitude.h"

namespace Fitter {
    double doFit(RooDalitzAmplitude *amplitude,
                 RooArgList *obs,
                 RooDataSet *data_fit,
                 TString fit_result_path,
                 TString history_path,
                 TString constraint_file,
                 size_t maxCalls = 300000,
                 bool refit = false,
                 bool do_hesse = false,
                 RooArgSet *minosList = nullptr);

    double calculate_nll(RooDalitzAmplitude *amplitude,
                         RooArgList *obs,
                         RooDataSet *data_fit,
                         TString fit_result_path,
                         TString history_path,
                         TString constraint_file,
                         size_t maxCalls = 300000,
                         bool refit = false,
                         bool do_hesse = false,
                         RooArgSet *minosList = nullptr);
} // namespace Fitter
