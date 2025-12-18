#pragma once

#include <string>
#include <vector>
#include <stdexcept>

#include "TString.h"

#include "RooTDalitz/cxxopts.hpp"
#include "RooTDalitz/config.h"

namespace Parser{
    void checkPcOpts(const std::vector<int>& option, const std::string PcName){
        if ( option.size() != 2 ){
            std::string message ="Received " + std::to_string(option.size()) + " integers for " + PcName + " spin-parity! Only input 2";
            throw std::length_error(message);
        }
        if ( option[0] % 2 != 1 ){
            std::string message = "Spin " + std::to_string(option[0]) + " for " + PcName + " is not physical!";
            throw std::logic_error(message);
        }
        if ( !(option[0] == 1 || option[0] == 3) ){
            std::string message = "Spin " + std::to_string(option[0]) + " for " + PcName + " is not allowed! Only [1, 3] is allowed.";
            throw std::logic_error(message);
        }
        if ( ! (option[1] == 1 || option[1] == -1) ) {
            std::string message = "Invalid parity " + std::to_string(option[1]) + " for " + PcName + "! Only [1, -1] is allowed.";
            throw std::logic_error(message);
        }
    }

    void checkFitOpts(const std::vector<int>& option, const std::string PcName){
        if ( option.size() != 2 ){
            std::string message ="Received " + std::to_string(option.size()) + " integers for " + PcName + " spin-parity! Only input 2";
            throw std::length_error(message);
        }
        if ( option[0] % 2 != 1 ){
            std::string message = "Spin " + std::to_string(option[0]) + " for " + PcName + " is not physical!";
            throw std::logic_error(message);
        }
        if ( !(option[0] == 1 || option[0] == 3) ){
            std::string message = "Spin " + std::to_string(option[0]) + " for " + PcName + " is not allowed! Only [1, 3] is allowed.";
            throw std::logic_error(message);
        }
        if ( ! (option[1] == 1 || option[1] == -1) ) {
            std::string message = "Invalid parity " + std::to_string(option[1]) + " for " + PcName + "! Only [1, -1] is allowed.";
            throw std::logic_error(message);
        }
    }

    void checkModeOpts(const TString mode){
        if (mode != "fit" && mode != "nll" && mode != "weights"){
            throw std::logic_error("Invalid mode! Only [fit, nll, weights] are allowed.");
        }
    }

    template<bool hasBroadPc>
    auto parse_args(int argc, char **argv, const std::string executable_name, const std::string description){
        cxxopts::Options options(executable_name, description);

        // Start with Pc spin-parity arguments
        options
            .set_width(70)
            .set_tab_expansion()
            .add_options()
            ("Pc4457", "Spin-Parity of Pc(4457)", cxxopts::value<std::vector<int>>())
            ("Pc4440", "Spin-Parity of Pc(4440)", cxxopts::value<std::vector<int>>())
            ("Pc4312", "Spin-Parity of Pc(4312)", cxxopts::value<std::vector<int>>());

        if constexpr (hasBroadPc){
            options.add_options()
                ("Pc4500", "Spin-Parity of broad Pc", cxxopts::value<std::vector<int>>());
        };

        options.add_options()
            ("d,data_tuple", "Path to sPlot.root (data) nTuple", cxxopts::value<TString>()->default_value("sPlot.root"))
            ("m,mc_tuple", "Path to mcsw.root (MC) nTuple", cxxopts::value<TString>()->default_value("mcsw.root"))
            ("c,constraint", "Path to constraint definition file", cxxopts::value<TString>()->default_value("err/PRC_Zcs4000Cons.err"))
            ("i,initial", "Path to initial parameter func", cxxopts::value<TString>()->default_value(""))
            ("f,final", "Path to dump final (post-fit) parameter func", cxxopts::value<TString>()->default_value(""))
            ("r,result", "Path to RooFitResult", cxxopts::value<TString>()->default_value(""))
            ("h,history", "Path to fit history (and gradients)", cxxopts::value<TString>()->default_value(""))
            ("w,data_weights", "Path to store per-event amplitude weights for data sample", cxxopts::value<TString>()->default_value(""))
            ("e,mc_weights", "Path to store per-event amplitude weights for mc (normalisation) sample", cxxopts::value<TString>()->default_value(""))
            ("s,scale_gJpsip_4457", "Scale of Pc(4457) Jpsip coupling", cxxopts::value<double>()->default_value("1.32"))
            ("u,widthUpperLimit", "Upper limit of width parameter", cxxopts::value<double>()->default_value("0.5"))
            ("l,widthLowerLimit", "Lower limit of width parameter", cxxopts::value<double>()->default_value("100."))
            ("mode", "Mode to run in, allowed values: [fit, nll, weights]", cxxopts::value<TString>()->default_value("fit"))
            ("device", "Which GPU to run on", cxxopts::value<int>()->default_value("0"))
            ("randomLs", "Whether to randomise Ls initial parameters", cxxopts::value<bool>()->default_value("false"))
            ("randomPc", "Whether to randomise Pc initial parameters", cxxopts::value<bool>()->default_value("false"))
            ("largeRandom", "Whether to increase random Gaussian width", cxxopts::value<bool>()->default_value("false"))
            ("seed", "Seed for random number generator", cxxopts::value<unsigned long>()->default_value("1234"))
            ("away_from_limit", "Whether to set values away from limit: newValue = limit +- 1.1 * error", cxxopts::value<bool>()->default_value("false"))
            ("refit", "Re-run the fit if failed the first time", cxxopts::value<bool>()->default_value("false"))
            ("hesse", "Run HESSE after end of fit.", cxxopts::value<bool>()->default_value("false"))
            ("help", "Print help")
            ("tab-expansion", "Tab\texpansion");

        auto args = options.parse(argc, argv);

        if (args.count("help"))
        {
            std::cout << options.help() << std::endl;
            exit(0);
        }

        // Check data and mc weights options are not empty in no-fit mode
        if (args["mode"].as<TString>() == "weights"){
            if (args["data_weights"].as<TString>().Length() == 0 || args["mc_weights"].as<TString>().Length() == 0){
                throw std::logic_error("In no-fit mode, data and mc weights must be provided!");
            }
        }

        // Check Pc spin-parity options
        auto Pc4312 = args["Pc4312"].as<std::vector<int>>();
        auto Pc4440 = args["Pc4440"].as<std::vector<int>>();
        auto Pc4457 = args["Pc4457"].as<std::vector<int>>();

        checkPcOpts(Pc4312, "Pc(4312)");
        checkPcOpts(Pc4440, "Pc(4440)");
        checkPcOpts(Pc4457, "Pc(4457)");

        std::string Pc4312P = Pc4312[1] == 1 ? "+" : "-";
        std::string Pc4440P = Pc4440[1] == 1 ? "+" : "-";
        std::string Pc4457P = Pc4457[1] == 1 ? "+" : "-";

        std::cout << "Pc(4312) Spin-parity : " + std::to_string(Pc4312[0]) + "/2" + Pc4312P << std::endl;
        std::cout << "Pc(4440) Spin-parity : " + std::to_string(Pc4440[0]) + "/2" + Pc4440P << std::endl;
        std::cout << "Pc(4457) Spin-parity : " + std::to_string(Pc4457[0]) + "/2" + Pc4457P << std::endl;

        if constexpr(hasBroadPc){
            auto Pc4500 = args["Pc4500"].as<std::vector<int>>();
            checkPcOpts(Pc4500, "Broad Pc <Pc4500>");
            std::string Pc4500P = Pc4500[1] == 1 ? "+" : "-";
            std::cout << "Pc(4500) Spin-parity : " + std::to_string(Pc4500[0]) + "/2" + Pc4500P << std::endl;
        }
        std::cout << std::endl;

        // GPU options
        std::cout << "Will use GPU with ID: " << args["device"].as<int>() << std::endl;
        std::cout << std::endl;

        // Scale and limits
        std::cout << "Pc(4457) Jpsip scale : " + std::to_string(args["scale_gJpsip_4457"].as<double>()) << std::endl;
        std::cout << "Width upper limit    : " + std::to_string(args["widthUpperLimit"].as<double>()) << std::endl;
        std::cout << "Width lower limit    : " + std::to_string(args["widthLowerLimit"].as<double>()) << std::endl;
        std::cout << std::endl;

        // Input collision and MC data
        std::cout << "Reading data nTuple from : " + args["data_tuple"].as<TString>() << std::endl;
        std::cout << "Reading normalisation nTuple from : " + args["mc_tuple"].as<TString>() << std::endl;
        std::cout << std::endl;

        // Print store weights mode options
        if (args["mode"].as<TString>() == "weights"){
            std::cout << "Running in weights mode. Will only store resonance weights." << std::endl;
            std::cout << std::endl;

            std::cout << "Using parameters from RooFitResult : " + args["result"].as<TString>() << std::endl;
            std::cout << std::endl;
        }
        // Print fitter mode options
        else { 
            std::cout << "Running in fit or nll mode." << std::endl;
            std::cout << "Reading input func from : " + args["initial"].as<TString>() << std::endl;
            std::cout << "Reading constraint : " + args["constraint"].as<TString>() << std::endl;
            std::cout << std::endl;

            std::cout << "Will write final parameters to func file : " + args["final"].as<TString>() << std::endl;
            std::cout << "Will write RooFitResult to file : " + args["result"].as<TString>() << std::endl;
            std::cout << "Will write fit history (with gradients) to file : " + args["history"].as<TString>() << std::endl;
            std::cout << std::endl;

            auto printBool = [](bool boolean) { std::string statement = boolean ? "True" : "False"; return statement; };

            std::cout << "RandomLs    : " << printBool(args["randomLs"].as<bool>()) << std::endl;
            std::cout << "RandomPc    : " << printBool(args["randomPc"].as<bool>()) << std::endl;
            std::cout << "largeRandom : " << printBool(args["largeRandom"].as<bool>()) << std::endl;
            std::cout << "Refit       : " << printBool(args["refit"].as<bool>()) << std::endl;
            std::cout << "HESSE       : " << printBool(args["hesse"].as<bool>()) << std::endl;

            if (args["randomLs"].as<bool>() || args["randomPc"].as<bool>()){
            std::cout << "Random seed to randomise initial parameters: " << args["seed"].as<unsigned long>() << std::endl;
            std::cout << std::endl;
            }
            if (args["away_from_limit"].as<bool>()){
            std::cout << "Will seed parameters to be away from limits " << std::endl;
            std::cout << std::endl;
            }
        }

        // Print weights options
        auto data_weights_path = args["data_weights"].as<TString>();
        auto mc_weights_path = args["mc_weights"].as<TString>();
        if (data_weights_path != "" && mc_weights_path != ""){
            std::cout << "Will write data per-event amplitude^2 to : " + data_weights_path << std::endl;
            std::cout << "Will write normalisation per-event amplitude^2 to : " + mc_weights_path << std::endl;
            std::cout << std::endl;
        }
        else{
            std::cout << "Data weights path or mc weights path not parsed. Not storing per-event weights..." << std::endl;
            std::cout << std::endl;
        }

        return args;
    }
}
