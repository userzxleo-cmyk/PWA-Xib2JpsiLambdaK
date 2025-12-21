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

void SetLPar(RooArgList* argli, int maxind, bool fixfirst=false)
{
    int  spin = ((RooAbsReal&)(*argli)[argli->getSize()-2]).getVal();
   double  parity = -1.*((RooAbsReal&)(*argli)[argli->getSize()-1]).getVal();
//    std::cout << spin << std::endl;
  int maxi = maxind-1;
  if(spin==1&&maxi>3) maxi = 3;
//    std::cout << spin << std::endl;
//  if(spin==1&&ind>3)  return;
  int mini = 0;
  for(int ind=0; ind<6; ++ind) {
    RooRealVar *var = (RooRealVar*)(argli->at(2*ind));
    var->setVal(parity*var->getVal()); 
    RooRealVar *var1 = (RooRealVar*)(argli->at(2*ind+1));
    var1->setVal(parity*var1->getVal()); 
  }
  if(fixfirst) mini = 1;
  for(int ind=mini; ind<=maxi; ++ind) {
    RooRealVar *var = (RooRealVar*)(argli->at(2*ind));
    var->setError(0.1); var->setConstant(0);
    RooRealVar *var1 = (RooRealVar*)(argli->at(2*ind+1));
    var1->setError(0.1); var1->setConstant(0);
  }
  for(int ind=maxi+1; ind<6; ++ind) {
    RooRealVar *var = (RooRealVar*)(argli->at(2*ind));
    var->setVal(0.); var->setConstant(1);
    RooRealVar *var1 = (RooRealVar*)(argli->at(2*ind+1));
    var1->setVal(0.); var1->setConstant(1);
  }
}


int main(int argc, char **argv) {
    // Check these during compile-time instead of running into assert during run-time
    Static_Check::use_default<true>(); // Use same settings as default fit

    Time::Timer timer;
    timer.start();

    constexpr bool has_broad_Pc = true;
    auto parser_args = Parser::parse_args<has_broad_Pc>(
        argc, argv, "RCModel", "Fits default model used during review committee stage");
    Random::set_seed(parser_args["seed"].as<unsigned long>());

    auto Pc4312 = Config::get_spin_parity(parser_args["Pc4312"].as<std::vector<int>>());
    auto Pc4440 = Config::get_spin_parity(parser_args["Pc4440"].as<std::vector<int>>());
    auto Pc4457 = Config::get_spin_parity(parser_args["Pc4457"].as<std::vector<int>>());
    auto Pc4500 = Config::get_spin_parity(parser_args["Pc4500"].as<std::vector<int>>());

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
    // Broad Pc
    ZS2 = Pc4500.spin;
    ZP2 = Pc4500.parity;

// #include "RooTDalitz/ResL_fullPHSP_finnerBinning.h"
    int J1690 = 1;
    int P1690 = 1;
    int J1950 = 1;
    int P1950 = -1;
    int J2030 = 5;
    int P2030 = 1;
#include "RooTDalitz/JPResL.h"
#include "RooTDalitz/ResX.h"
#include "RooTDalitz/ResZKM_alternateCoupling.h"

    listZ->Add(L_Z);  // 4457
    listZ->Add(L_Z4); // 4312
    listZ->Add(L_Z5); // 4440
    listZ->Add(L_Z2); // Broad Pc

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

    list->Add(X_1690);
    list->Add(X_1820);
    list->Add(X_1950);
    list->Add(X_2030);
    list->Add(X_NR);
    // list->Add(L_S01);
    // list->Add(L_S01_2);
    // list->Add(L_1520);
    // list->Add(L_1600);
    // list->Add(L_1690);
    // list->Add(L_1810);
    // list->Add(L_1820);
    // list->Add(L_1830);
    // list->Add(L_1890);
    // list->Add(L_2100);
    // list->Add(L_2110);
    // list->Add(L_2350);
    // list->Add(L_2585);
    // list->Add(L_1710);
    // list->Add(L_2020);
    // list->Add(L_2050);

    // list->Add(L_1910);
    // list->Add(L_2130);
    // list->Add(L_2340);
    // list->Add(L_2320);
    SetLPar(X_1690,4);
    SetLPar(X_1820,3,true);
    SetLPar(X_1950,3);
    SetLPar(X_2030,3);
    SetLPar(X_NR,4);

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

    a_p1p1_1820.setVal(1); a_p1p1_1820.setConstant(1);
    p_p1p1_1820.setVal(0); p_p1p1_1820.setConstant(1);
    width_1690.setConstant(0);
    m0_1690.setConstant(0);
    width_1690.setRange(0.0001,0.2);
    m0_1690.setRange(1.68,1.70);
    width_1820.setConstant(0);
    m0_1820.setConstant(0);
    width_1820.setRange(0.0001,0.2);
    m0_1820.setRange(1.81,1.83);
    width_1950.setConstant(0);
    m0_1950.setConstant(0);
    width_1950.setRange(0.0001,0.2);
    m0_1950.setRange(1.94,1.96);
    width_2030.setConstant(0);
    m0_2030.setConstant(0);
    width_2030.setRange(0.0001,0.2);
    m0_2030.setRange(2.01,2.04);
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
