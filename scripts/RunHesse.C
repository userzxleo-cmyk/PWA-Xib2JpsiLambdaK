#include "RooTDalitz/RooDalitzAmplitude.h"
#include "RooFormulaVar.h"
#include "TList.h"
#include "RooRealVar.h"
#include "RooArgList.h"
#include "TSystem.h"
#include "RooDataSet.h"
#include "TFile.h"
#include "RooFitResult.h"
#include "TDatime.h"
#include "RooRealConstant.h"
#include "TRandom2.h"
#include "RooMinimizer.h"
#include "RooAddition.h"
#include "RooTDalitz/RooFGaussian.h"
#include "RooTDalitz/Resonances.h"
#include "RooMsgService.h"
#include "RooTDalitz/cxxopts.hpp"
#include <stdexcept>

#define smear
//#define flip
using namespace RooFit;
TRandom2 *g_rnd;

#define FourPc

//Z 4457
//Z4 4312
//Z5 4440

inline double Rwidth(double m0, double g0)
{
const double mres0 = -0.134064 ;
const double mres1 =  0.0599014;
const double mres2 = -0.00655841;
  double mpp=m0;
  double mres = mres0+mpp*(mres1+mpp*mres2);
  return sqrt(fabs(g0*g0-9.*mres*mres));
}


#include "RooTDalitz/Setup.h"

void checkPcOpts(const std::vector<int>& option, const std::string PcName){
  if ( option.size() != 2 ){
    std::string message ="Received " + std::to_string(option.size()) + " integers for " + PcName + " spin-parity! Only input 2";
    throw std::length_error(message);
  }
  if ( option[0] % 2 != 1 ){
    std::string message = "Spin " + std::to_string(option[0]) + " for " + PcName + " is not physical!";
    throw std::logic_error(message);
  }
  if ( ! (option[1] == 1 || option[1] == -1) ) {
    std::string message = "Invalid parity " + std::to_string(option[1]) + " for " + PcName + "! Only [1, -1] is allowed.";
    throw std::logic_error(message);
  }
}

auto parse_args(int argc, char **argv){
  cxxopts::Options options("FitRCModel", "Fits the Pc lineshape model, used during review committee");

  options
      .set_width(70)
      .set_tab_expansion()
      .add_options()
    ("Pc4457", "Spin-Parity of Pc(4457)", cxxopts::value<std::vector<int>>())
    ("Pc4440", "Spin-Parity of Pc(4440)", cxxopts::value<std::vector<int>>())
    ("Pc4312", "Spin-Parity of Pc(4312)", cxxopts::value<std::vector<int>>())
    ("Pc4500", "Spin-Parity of broad Pc", cxxopts::value<std::vector<int>>())
    ("d,data_tuple", "Path to sPlot.root (data) nTuple", cxxopts::value<TString>()->default_value("sPlot.root"))
    ("m,mc_tuple", "Path to mcsw.root (MC) nTuple", cxxopts::value<TString>()->default_value("mcsw.root"))
    ("c,constraint", "Path to constraint definition file", cxxopts::value<TString>()->default_value("err/PRC_Zcs4000Cons.err"))
    ("r,result", "Path to RooFitResult of parameters", cxxopts::value<TString>())
    ("o,output", "Path to store output RooFitResult", cxxopts::value<TString>())
    ("s,scale_gJpsip_4457", "Scale of Pc(4457) Jpsip coupling", cxxopts::value<float>()->default_value("1.32"))
    ("u,widthUpperLimit", "Upper limit of width parameter", cxxopts::value<float>()->default_value("0.5"))
    ("l,widthLowerLimit", "Lower limit of width parameter", cxxopts::value<float>()->default_value("100."))
    ("help", "Print help")
    ("tab-expansion", "Tab\texpansion");

    auto args = options.parse(argc, argv);

    if (args.count("help"))
    {
      std::cout << options.help() << std::endl;
      exit(0);
    }

    auto Pc4312 = args["Pc4312"].as<std::vector<int>>();
    auto Pc4440 = args["Pc4440"].as<std::vector<int>>();
    auto Pc4457 = args["Pc4457"].as<std::vector<int>>();
    auto Pc4500 = args["Pc4500"].as<std::vector<int>>();

    checkPcOpts(Pc4312, "Pc(4312)");
    checkPcOpts(Pc4440, "Pc(4440)");
    checkPcOpts(Pc4457, "Pc(4457)");
    checkPcOpts(Pc4500, "Broad Pc <Pc4500>");

    std::string Pc4312P = Pc4312[1] == 1 ? "+" : "-";
    std::string Pc4440P = Pc4440[1] == 1 ? "+" : "-";
    std::string Pc4457P = Pc4457[1] == 1 ? "+" : "-";
    std::string Pc4500P = Pc4500[1] == 1 ? "+" : "-";

    std::cout << "Pc(4312) Spin-parity : " + std::to_string(Pc4312[0]) + "/2" + Pc4312P << std::endl;
    std::cout << "Pc(4440) Spin-parity : " + std::to_string(Pc4440[0]) + "/2" + Pc4440P << std::endl;
    std::cout << "Pc(4457) Spin-parity : " + std::to_string(Pc4457[0]) + "/2" + Pc4457P << std::endl;
    std::cout << "Pc(4500) Spin-parity : " + std::to_string(Pc4500[0]) + "/2" + Pc4500P << std::endl;
    std::cout << std::endl;

    std::cout << "Pc(4457) Jpsip scale : " + std::to_string(args["scale_gJpsip_4457"].as<float>()) << std::endl;
    std::cout << "Width upper limit : " + std::to_string(args["widthUpperLimit"].as<float>()) << std::endl;
    std::cout << "Width lower limit : " + std::to_string(args["widthLowerLimit"].as<float>()) << std::endl;
    std::cout << std::endl;

    std::cout << "Reading input parameters from : " + args["result"].as<TString>() << std::endl;
    std::cout << "Reading constraint : " + args["constraint"].as<TString>() << std::endl;
    std::cout << "Reading normalisation nTuple from : " + args["mc_tuple"].as<TString>() << std::endl;
    std::cout << "Reading data nTuple from : " + args["data_tuple"].as<TString>() << std::endl;
    std::cout << std::endl;

    std::cout << "Will write output of HESSE to : " + args["output"].as<TString>() << std::endl;
    std::cout << std::endl;

    return args;
}

int main(int argc, char **argv)
{
  TDatime BeginTime;
  std::cout << "Time(begin)  " << BeginTime.GetYear() << "." << BeginTime.GetMonth() << "." << BeginTime.GetDay() ;
  std::cout << "    " << BeginTime.GetHour() << ":" << BeginTime.GetMinute() << ":" << BeginTime.GetSecond() << std::endl;
  bool method=false;

  auto args = parse_args(argc, argv);

  std::vector<int> Pc4312 = args["Pc4312"].as<std::vector<int>>();
  std::vector<int> Pc4440 = args["Pc4440"].as<std::vector<int>>();
  std::vector<int> Pc4457 = args["Pc4457"].as<std::vector<int>>();
  std::vector<int> Pc4500 = args["Pc4500"].as<std::vector<int>>();

  float CalFWHM_nbin = 1000;
  float CalFWHM_range = 5.*args["scale_gJpsip_4457"].as<float>();

  int ZS(1), ZP(-1);
  int ZS5(3), ZP5(-1);
  int ZS3(1), ZP3(1);
  int ZS2(1), ZP2(1);
  int ZS6(1), ZP6(1);

  //4312
  int ZS4 = Pc4312[0];
  int ZP4 = Pc4312[1];
  // //4457
  ZS = Pc4457[0];
  ZP = Pc4457[1];
  // //4440
  ZS5 = Pc4440[0];
  ZP5 = Pc4440[1];
  // //4500
  ZS2 = Pc4500[0];
  ZP2 = Pc4500[1];

  RooRealVar *FFB_var = new RooRealVar("FFB_var","FFB_var",3.0f, 1.f, 5.f);
  RooRealVar *FFR_var = new RooRealVar("FFR_var","FFR_var",3.0f, 1.f, 5.f);

  FFB_var->setConstant(1);
  FFR_var->setConstant(1);
  FFB_var->setError(0.1);
  FFR_var->setError(0.1);

  RooRealVar *widthUpperLimit_var = new RooRealVar("widthUpperLimit_var","widthUpperLimit_var", args["widthUpperLimit"].as<float>());
  RooRealVar *widthLowerLimit_var = new RooRealVar("widthLowerLimit_var","widthLowerLimit_var", args["widthLowerLimit"].as<float>());
  RooRealVar *CalFWHM_nbin_var = new RooRealVar("CalFWHM_nbin_var","CalFWHM_nbin_var", CalFWHM_nbin);
  RooRealVar *CalFWHM_range_var = new RooRealVar("CalFWHM_range_var","CalFWHM_range_var", CalFWHM_range);
  widthUpperLimit_var->setConstant(1);
  widthLowerLimit_var->setConstant(1);
  CalFWHM_nbin_var->setConstant(1);
  CalFWHM_range_var->setConstant(1);



  RooRealVar mkp("mkp","m(K^{-}p)",1.4,2.6);
  RooRealVar mjpsip("mjpsip","",4.,5.2);
  RooRealVar mjpsik("mjpsik","",3.,5.2);
//  cosk:cosmu:chi
  RooRealVar cosTheta_L("cosTheta_L","cosTheta_L",-1,1);
  RooRealVar cosTheta_Jpsi("cosTheta_Jpsi","cosTheta_Jpsi",-1,1);
  RooRealVar cosTheta_Lb("cosTheta_Lb","cosTheta_Lb",-1,1);
  RooRealVar Z_cosTheta_Lb("Z_cosTheta_Lb","Z_cosTheta_Lb",-1,1);
  RooRealVar Z_cosTheta_Z("Z_cosTheta_Z","Z_cosTheta_Z",-1,1);
  RooRealVar Z_cosTheta_Jpsi("Z_cosTheta_Jpsi","Z_cosTheta_Jpsi",-1,1);
  RooRealVar cosTheta_p("cosTheta_p","cosTheta_p",-1,1);
  RooRealVar phiK("phiK","phiK",-TMath::Pi(), TMath::Pi());
  RooRealVar phiMu("phiMu","phiMu",-TMath::Pi(), TMath::Pi());
  RooRealVar Z_phiZ("Z_phiZ","Z_phiZ",-TMath::Pi(), TMath::Pi());
  RooRealVar Z_phiJpsi("Z_phiJpsi","Z_phiJpsi",-TMath::Pi(), TMath::Pi());
  RooRealVar Z_phiMu("Z_phiMu","Z_phiMu",-TMath::Pi(), TMath::Pi());
  RooRealVar alpha_Mu("alpha_Mu","alpha_Mu",-TMath::Pi(), TMath::Pi());
  RooRealVar X_phiX("X_phiX","X_phiX",-TMath::Pi(), TMath::Pi());
  RooRealVar X_phiJpsi("X_phiJpsi","X_phiJpsi",-TMath::Pi(), TMath::Pi());
  RooRealVar X_phiMu("X_phiMu","X_phiMu",-TMath::Pi(), TMath::Pi());
  RooRealVar X_alpha_Mu("X_alpha_Mu","X_alpha_Mu",-TMath::Pi(), TMath::Pi());
  RooRealVar X_cosTheta_Lb("X_cosTheta_Lb","X_cosTheta_Lb",-1,1);
  RooRealVar X_cosTheta_X("X_cosTheta_X","X_cosTheta_X",-1,1);
  RooRealVar X_cosTheta_Jpsi("X_cosTheta_Jpsi","X_cosTheta_Jpsi",-1,1);
  RooRealVar X_cosTheta_p("X_cosTheta_p","X_cosTheta_p",-1,1);
  RooRealVar nsig_sw("nsig_sw","",0);
  RooArgList *obs = new RooArgList(mkp,cosTheta_Lb,cosTheta_L,cosTheta_Jpsi,phiK,phiMu);
  obs->add(Z_cosTheta_Lb);
  obs->add(Z_cosTheta_Z);
  obs->add(Z_cosTheta_Jpsi);
  obs->add(Z_phiZ);
  obs->add(Z_phiJpsi);
  obs->add(Z_phiMu);
  obs->add(cosTheta_p);
  obs->add(alpha_Mu);
  obs->add(mjpsip);
  obs->add(mjpsik);
  obs->add(X_cosTheta_Lb);
  obs->add(X_phiX);
  obs->add(X_cosTheta_X);
  obs->add(X_phiJpsi);
  obs->add(X_cosTheta_Jpsi);
  obs->add(X_phiMu);
  obs->add(X_cosTheta_p);
  obs->add(X_alpha_Mu);

  RooArgList *obs2 = new RooArgList(mkp,cosTheta_Lb,cosTheta_L,cosTheta_Jpsi,phiK,phiMu);
  obs2->add(Z_cosTheta_Lb);
  obs2->add(Z_cosTheta_Z);
  obs2->add(Z_cosTheta_Jpsi);
  obs2->add(Z_phiZ);
  obs2->add(Z_phiJpsi);
  obs2->add(Z_phiMu);
  obs2->add(cosTheta_p);
  obs2->add(alpha_Mu);
  obs2->add(mjpsip);
  obs2->add(nsig_sw);
  obs2->add(mjpsik);
  obs2->add(X_cosTheta_Lb);
  obs2->add(X_phiX);
  obs2->add(X_cosTheta_X);
  obs2->add(X_phiJpsi);
  obs2->add(X_cosTheta_Jpsi);
  obs2->add(X_phiMu);
  obs2->add(X_cosTheta_p);
  obs2->add(X_alpha_Mu);

  RooRealVar sw("sw","sw",0);
  RooArgList *obs1 = new RooArgList(mkp,cosTheta_Lb,cosTheta_L,cosTheta_Jpsi,phiK,phiMu);
  obs1->add(Z_cosTheta_Lb);
  obs1->add(Z_cosTheta_Z);
  obs1->add(Z_cosTheta_Jpsi);
  obs1->add(Z_phiZ);
  obs1->add(Z_phiJpsi);
  obs1->add(Z_phiMu);
  obs1->add(cosTheta_p);
  obs1->add(alpha_Mu);
  obs1->add(mjpsip);
  obs1->add(mjpsik);
  obs1->add(X_cosTheta_Lb);
  obs1->add(X_phiX);
  obs1->add(X_cosTheta_X);
  obs1->add(X_phiJpsi);
  obs1->add(X_cosTheta_Jpsi);
  obs1->add(X_phiMu);
  obs1->add(X_cosTheta_p);
  obs1->add(X_alpha_Mu);

  obs1->add(sw);
  TFile *fdata = new TFile(args["data_tuple"].as<TString>());
  TTree *datree = (TTree*)fdata->Get("DTree");
  RooDataSet* datars1 = new RooDataSet("datars","",datree, *obs2);
//  RooDataSet *datars1 = (RooDataSet*)fdata->Get("datars");
  RooDataSet *datars = (RooDataSet*)datars1->reduce("mkp>1.431950001&&mkp<2.522583999999&&mjpsip>4.0351880460&&mjpsip<5.125823");
  RooRealVar *index = new RooRealVar("index","index",0);
  RooDataSet *IND = new RooDataSet("IND","", RooArgSet(*index));


  double nev = datars->numEntries();
  std::cout << "sum " << nev << std::endl;
  for(int i=0; i<nev; ++i) {
     *index = i;
//     if(i%1000==0) std::cout << "i " << i << std::endl;
     IND->add(RooArgSet(*index));

  }
  datars->merge(IND);
  datars->Print("V");
  RooDataSet * data_fit = new  RooDataSet(TString(datars->GetName())+TString("new"),datars->GetTitle(),datars,*datars->get(),0,"nsig_sw");
  data_fit->Print("V");

  obs->add(*index);

  RooDataSet *datars2 = (RooDataSet*)datars->reduce("(mjpsip>4.2&&mjpsip<4.6)");
  RooRealVar *index2 = new RooRealVar("index","index",0);
  RooDataSet *IND2 = new RooDataSet("IND","", RooArgSet(*index2));


  nev = datars2->numEntries();
  for(int i=0; i<nev; ++i) {
     *index2 = i;
//     if(i%1000==0) std::cout << "i " << i << std::endl;
     IND2->add(RooArgSet(*index2));

  }
  datars2->merge(IND2);
  RooDataSet * data_fit26 = new  RooDataSet(TString(datars2->GetName())+TString("new26"),datars2->GetTitle(),datars2,*datars2->get(),0,"nsig_sw");

#include "RooTDalitz/ResL_fullPHSP_finnerBinning.h"
#include "RooTDalitz/ResZKM_alternateCoupling.h"
#include "RooTDalitz/ResX.h"
  listZ->Add(L_Z);
  listZ->Add(L_Z4);
  listZ->Add(L_Z5);//4457
//  listZ->Add(L_Z3);
  listZ->Add(L_Z2);
  listZ->Add(L_ZM1SC1);
  listZ->Add(L_ZM1SC2);
  listZ->Add(L_ZM1SC3);
  listZ->Add(L_ZM1SC4);

  listZ->Add(L_ZM3SC1);
  listZ->Add(L_ZM3SC2);
  listZ->Add(L_ZM3SC3);
  listZ->Add(L_ZM3SC4);

  list->Add(L_S01);
  list->Add(L_S01_2);
  list->Add(L_1520);
  list->Add(L_1600);
//  list->Add(L_1670);
  list->Add(L_1690);
//  list->Add(L_1800);
  list->Add(L_1810);
  list->Add(L_1820);
  list->Add(L_1830);
  list->Add(L_1890);
  list->Add(L_2100);
  list->Add(L_2110);
  list->Add(L_2350);
  list->Add(L_2585);
  list->Add(L_1710);
//  list->Add(L_2000);
  list->Add(L_2020);
  list->Add(L_2050);

  list->Add(L_1910);
  list->Add(L_2130);
  list->Add(L_2340);
  list->Add(L_2320);

  datars1->Delete();
  datars->Delete();
  datars2->Delete();
  fdata->Close();

  RooArgList * ratioList = new RooArgList("ratioList");
  ratioList->add(rg1_Z);
  ratioList->add(rg3_Z);
  ratioList->add(rg4_Z);
  ratioList->add(rg1_Z4);
  ratioList->add(rg2_Z4);
  ratioList->add(rg3_Z4);
  ratioList->add(rg1_Z5);
  ratioList->add(rg3_Z5);
  ratioList->add(rg4_Z5);

  RooDalitzAmplitude *sig = new RooDalitzAmplitude("sig",
                                                   "",
                                                   *obs,
                                                   list,
                                                   listZ,
                                                   listX,
                                                   ratioList,
                                                   *FFR_var,
                                                   *FFB_var,
                                                   "one.root",
                                                   *data_fit,
                                                   *widthUpperLimit_var,
                                                   *widthLowerLimit_var,
                                                   *CalFWHM_nbin_var,
                                                   *CalFWHM_range_var);
  RooArgSet* setdlz = sig->getParameters(*data_fit);

  char name[10];
  if(ZP>0) {
   sprintf(name,"p%i",ZS);
  } else {
   sprintf(name,"m%i",ZS);
  }

  char nameZ4[10];
  if(ZP4>0) {
   sprintf(nameZ4,"p%i",ZS4);
  } else {
   sprintf(nameZ4,"m%i",ZS4);
  }
  char nameZ5[10];
  if(ZP5>0) {
   sprintf(nameZ5,"p%i",ZS5);
  } else {
   sprintf(nameZ5,"m%i",ZS5);
  }

  char nameZ2[10];
  if(ZP2>0) {
   sprintf(nameZ2,"p%i",ZS2);
  } else {
   sprintf(nameZ2,"m%i",ZS2);
  }

  TFile roofitresult_file(args["result"].as<TString>(), "READ");
  RooFitResult * pars_values = roofitresult_file.Get<RooFitResult>("nll");
  roofitresult_file.Close();

  for ( auto iter = pars_values->constPars().begin(); iter < pars_values->constPars().end(); iter++ ) {
    const RooRealVar * var = dynamic_cast<const RooRealVar *>(*iter);
    TString varName(var->GetName());
    RooRealVar * varToSet = dynamic_cast<RooRealVar *>( setdlz->find(varName) );

    if ( varToSet != nullptr ){
      var->hasMin() ? varToSet->setMin( var->getMin() ) : varToSet->removeMin();
      var->hasMax() ? varToSet->setMax( var->getMax() ) : varToSet->removeMax();
      varToSet->setVal( var->getVal() );
      if ( var->hasError() ) varToSet->setError( var->getError() );
      varToSet->setConstant(1);
    }
    else{
      std::cout << "Error while setting variables. Cannot find: " << varName << std::endl;
    }
  }

  for ( auto iter = pars_values->floatParsFinal().begin(); iter < pars_values->floatParsFinal().end(); iter++ ) {
    const RooRealVar * var = dynamic_cast<const RooRealVar *>(*iter);
    TString varName(var->GetName());
    RooRealVar * varToSet = dynamic_cast<RooRealVar *>( setdlz->find(varName) );

    if ( varToSet != nullptr ){
      var->hasMin() ? varToSet->setMin( var->getMin() ) : varToSet->removeMin();
      var->hasMax() ? varToSet->setMax( var->getMax() ) : varToSet->removeMax();
      varToSet->setVal( var->getVal() );
      varToSet->setError( var->getError() );
      varToSet->setConstant(0);
    }
    else{
      std::cout << "Error while setting variables. Cannot find: " << varName << std::endl;
    }
  }

  setdlz->Print("V");
  sig->Delete();

  runHesse(obs,
           list,
           listZ,
           listX,
           ratioList,
           data_fit,
           args["mc_tuple"].as<TString>(),
           args["output"].as<TString>(),
           FFR_var,
           FFB_var,
           widthUpperLimit_var,
           widthLowerLimit_var,
           CalFWHM_nbin_var,
           CalFWHM_range_var,
           args["constraint"].as<TString>(),
           300000
          );

  TDatime FinishTime;
  std::cout << "Time(finish)  " << FinishTime.GetYear() << "." << FinishTime.GetMonth() << "." << FinishTime.GetDay();
  std::cout << "    " << FinishTime.GetHour() << ":" << FinishTime.GetMinute() << ":" << FinishTime.GetSecond() << std::endl;
  return 0;
}
