// C++ std library
#include <fstream>

// ROOT and RooFit classes
#include "RooArgList.h"
#include "RooFormulaVar.h"
#include "RooRealConstant.h"
#include "RooRealVar.h"
#include "TList.h"

// Custom classes
#include "RooTDalitz/NDefine.h"
#include "RooTDalitz/Resonances.h"
#include "RooTDalitz/check.h"
#include "RooTDalitz/config.h"
#include "RooTDalitz/cxxopts.hpp"
#include "RooTDalitz/execute.h"
#include "RooTDalitz/parameter.h"
#include "RooTDalitz/parser.h"
#include "RooTDalitz/random.h"
#include "RooTDalitz/timer.h"

int main(int argc, char **argv) {
    // Check these during compile-time instead of running into assert during run-time
    Static_Check::Resonances::check_count<21,   // number of Lz,
                                          11,   // number of Pc,
                                          0>(); // number of Zcs
    Static_Check::Systematics::use_default<true>();
    Static_Check::Counters::use_default<true>();

    Time::Timer timer;
    timer.start();

    constexpr bool has_broad_Pc = false;
    auto parser_args = Parser::parse_args<has_broad_Pc>(
        argc,
        argv,
        "systematic_NoBroadPc_AdditionalLsNR1p",
        "Fits systematic model: replace BroadPc with non-resonant 1/2+ pK");
    Random::set_seed(parser_args["seed"].as<unsigned long>());

    auto Pc4312 = Config::get_spin_parity(parser_args["Pc4312"].as<std::vector<int>>());
    auto Pc4440 = Config::get_spin_parity(parser_args["Pc4440"].as<std::vector<int>>());
    auto Pc4457 = Config::get_spin_parity(parser_args["Pc4457"].as<std::vector<int>>());

    int ZS(1), ZP(-1);
    int ZS5(3), ZP5(-1);
    int ZS3(1), ZP3(1);
    int ZS2(1), ZP2(1);
    int ZS6(1), ZP6(1);

    // 4312
    int ZS4 = Pc4312.spin;
    int ZP4 = Pc4312.parity;
    // 4457
    ZS = Pc4457.spin;
    ZP = Pc4457.parity;
    // 4440
    ZS5 = Pc4440.spin;
    ZP5 = Pc4440.parity;

#include "RooTDalitz/ResL_fullPHSP_finnerBinning.h"
#include "RooTDalitz/ResX.h"
#include "RooTDalitz/ResZKM_alternateCoupling.h"

    listZ->Add(L_Z);  // 4457
    listZ->Add(L_Z4); // 4312
    listZ->Add(L_Z5); // 4440

    // Non-resonant terms in 1/2- Jpsi-p channel
    listZ->Add(L_ZM1SC1);
    listZ->Add(L_ZM1SC2);
    listZ->Add(L_ZM1SC3);
    listZ->Add(L_ZM1SC4);

    // Non-resonant terms in 3/2- Jpsi-p channel
    listZ->Add(L_ZM3SC1);
    listZ->Add(L_ZM3SC2);
    listZ->Add(L_ZM3SC3);
    listZ->Add(L_ZM3SC4);

    list->Add(L_S01);
    list->Add(L_S01_2);
    list->Add(L_1520);
    list->Add(L_1600);
    list->Add(L_1690);
    list->Add(L_1810);
    list->Add(L_1820);
    list->Add(L_1830);
    list->Add(L_1890);
    list->Add(L_2100);
    list->Add(L_2110);
    list->Add(L_2350);
    list->Add(L_2585);
    list->Add(L_1710);
    list->Add(L_2020);
    list->Add(L_2050);

    list->Add(L_1910);
    list->Add(L_2130);
    list->Add(L_2340);
    list->Add(L_2320);

    // Non-resonant 1/2+ pK
    list->Add(L_NR1p);

    // Ensures that the LS couplings are floating
    // Only float first 3 for spin 1/2
    for (int i = 0; i < 4; i++) {
        Parameter::FloatPar(L_NR1p, i);
    }

    RooArgList *ratioList = new RooArgList("ratioList");
    ratioList->add(rg1_Z);
    ratioList->add(rg3_Z);
    ratioList->add(rg4_Z);
    ratioList->add(rg1_Z4);
    ratioList->add(rg2_Z4);
    ratioList->add(rg3_Z4);
    ratioList->add(rg1_Z5);
    ratioList->add(rg3_Z5);
    ratioList->add(rg4_Z5);

    // Set the L(1600) couplings as constant
    a_p1p1_1600.setConstant(true);
    p_p1p1_1600.setConstant(true);
    auto arguments = Execution::get_arguments(list,
                                              listZ,
                                              listX,
                                              ratioList,
                                              500000,
                                              true,    // Fix Blatt-Weisskopf form factor
                                              nullptr, // Do not run MINOS
                                              parser_args);

    // Execute
    auto mode = Config::get_mode(parser_args["mode"].as<TString>());
    switch (mode) {
    case Config::Mode::Fit:
        Execution::fit(arguments);
        break;
    case Config::Mode::NLL:
        Execution::calculate_nll(arguments);
        break;
    case Config::Mode::Store_Weights:
        Execution::store_weights(arguments);
        break;
    default:
        throw std::invalid_argument("Invalid mode!");
    };

    timer.finish();
    timer.duration();
    return 0;
}
