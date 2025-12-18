#pragma once

// ROOT classes
#include "TList.h"
#include "TString.h"
#include "RooArgList.h"
#include "RooArgSet.h"

// Custom classes
#include "RooTDalitz/config.h"

namespace Execution{
    struct Arguments{
        // Executable specific arguments
        TList * const listL;
        TList * const listZ;
        TList * const listX;
        RooArgList * const ratioList;
        const size_t maxCalls;
        const bool fixed_BlattWeisskopf;
        RooArgSet * const minosList;
        // Parser arguments
        const TString dataTuplePath;
        const TString mcTuplePath;
        const TString initial_func_path;
        const TString final_func_path;
        const TString fitresultPath;
        const TString historyPath;
        const TString dataWeightsPath;
        const TString mcWeightsPath;
        const bool randomLs;
        const bool randomPc;
        const bool largeRandom;
        const double widthUpperLimit;
        const double widthLowerLimit;
        const double scale_gJpsip_4457;
        const bool away_from_limit;
        const int deviceID;
        const TString errlist;
        const bool refit;
        const bool do_hesse;

        // Default copy-constructor
        Arguments(const Arguments&) = default;
    };

    template <typename PARSER_ARGS>
    Arguments get_arguments(
            TList * _listL,
            TList * _listZ,
            TList * _listX,
            RooArgList *_ratioList,
            size_t _maxCalls,
            bool _fixed_BlattWeisskopf,
            RooArgSet* _minosList,
            PARSER_ARGS& parser_args){

            Arguments arguments = {
                _listL,
                _listZ,
                _listX,
                _ratioList,
                _maxCalls,
                _fixed_BlattWeisskopf,
                _minosList,
                // Parser arguments
                parser_args["data_tuple"].template as<TString>(),
                parser_args["mc_tuple"].template as<TString>(),
                parser_args["initial"].template as<TString>(),
                parser_args["final"].template as<TString>(),
                parser_args["result"].template as<TString>(),
                parser_args["history"].template as<TString>(),
                parser_args["data_weights"].template as<TString>(),
                parser_args["mc_weights"].template as<TString>(),
                parser_args["randomLs"].template as<bool>(),
                parser_args["randomPc"].template as<bool>(),
                parser_args["largeRandom"].template as<bool>(),
                parser_args["widthUpperLimit"].template as<double>(),
                parser_args["widthLowerLimit"].template as<double>(),
                parser_args["scale_gJpsip_4457"].template as<double>(),
                parser_args["away_from_limit"].template as<bool>(),
                parser_args["device"].template as<int>(),
                parser_args["constraint"].template as<TString>(),
                parser_args["refit"].template as<bool>(),
                parser_args["hesse"].template as<bool>()
            };
        return arguments;
    }

    void execute(Arguments& arguments, Config::Mode mode);
    // These are just renaming
    inline void fit(Arguments arguments) { execute(arguments, Config::Mode::Fit); };
    inline void calculate_nll(Arguments arguments) { execute(arguments, Config::Mode::NLL); };
    inline void store_weights(Arguments arguments) { execute(arguments, Config::Mode::Store_Weights); };
} // namespace Execution
