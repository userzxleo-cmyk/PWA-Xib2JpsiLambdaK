#include "RooTDalitz/RooDalitzAmplitude.h"
#include "TList.h"
#include "RooRealVar.h"
#include "RooArgList.h"
#include "TSystem.h"
#include "RooDataSet.h"
#include "TFile.h"
#include "RooFitResult.h"
#include "TDatime.h"
#include "RooRealConstant.h"
#include "RooMinuit.h"
#include "RooAddition.h"
#include "TH2F.h"
#include "RooFormulaVar.h"
//#include "RooTDalitz/RooSUMVar.h"

using namespace RooFit;

inline double getcosReson(double mL, double mJpsip) {
//purpose is to calculate cosReson from mReson, and mX3872Pi

  double m0_Lb = 5.6195, m0_K = 0.493677, m0_p = 0.938272046, m0_Jpsi = 3.096916;

  double q = 0.5*sqrt( pow(mL*mL - m0_p*m0_p - m0_K*m0_K,2) - 4*pow(m0_p*m0_K,2))/mL;
  double p = 0.5*sqrt( pow(m0_Lb*m0_Lb - m0_Jpsi*m0_Jpsi - mL*mL,2) - 4*pow(m0_Jpsi*mL,2))/m0_Lb;
  

  double p2 = p*p;
  double q2 = q*q;


  double E_Jpsi =  ( sqrt((m0_Jpsi*m0_Jpsi+p2)*(mL*mL+p2))+p2 )/mL;
  double p_Jpsi = sqrt(E_Jpsi*E_Jpsi - m0_Jpsi*m0_Jpsi);
    
  double cosL = ( E_Jpsi*sqrt(m0_p*m0_p+q2) - (mJpsip*mJpsip - m0_Jpsi*m0_Jpsi - m0_p*m0_p)*0.5 )/(p_Jpsi*q);
  if( cosL<-1.0 )cosL=-1.0;
  if( cosL> 1.0 )cosL= 1.0;

  return cosL;


}

inline double DPmax(double x)
{
 double m_lb = 5.6195, m_k = 0.493677, m_p = 0.938272046, m_jpsi = 3.096916;

  double m3sq=m_jpsi*m_jpsi;
  double m1sq=m_k*m_k;
  double m2sq=m_p*m_p;
  double M2=m_lb*m_lb;
  double E2 = (x-m1sq+m2sq)/2/sqrt(x);
  double E3 = (M2-x-m3sq)/2/sqrt(x);
  return pow(E2+E3,2)-pow(sqrt(E2*E2-m2sq)-sqrt(E3*E3-m3sq),2);
}

inline double DPmin(double x)
{
 double m_lb = 5.6195, m_k = 0.493677, m_p = 0.938272046, m_jpsi = 3.096916;

  double m3sq=m_jpsi*m_jpsi;
  double m1sq=m_k*m_k;
  double m2sq=m_p*m_p;
  double M2=m_lb*m_lb;
  double E2 = (x-m1sq+m2sq)/2/sqrt(x);
  double E3 = (M2-x-m3sq)/2/sqrt(x);
  return pow(E2+E3,2)-pow(sqrt(E2*E2-m2sq)+sqrt(E3*E3-m3sq),2);
}


inline bool exists(const std::string& name) {
  return ( access( name.c_str(), F_OK ) != -1 );
//  struct stat buffer;   
//  return (stat (name.c_str(), &buffer) == 0); 
}

//set how many SL of Lambda to float
void SetLPar(RooArgList* argli, int maxind, bool fixfirst=false)
{
    int  spin = ((RooAbsReal&)(*argli)[argli->getSize()-2]).getVal();
//    std::cout << spin << std::endl;
  int maxi = maxind-1;
  if(spin==1&&maxi>3) maxi = 3;
//    std::cout << spin << std::endl;
//  if(spin==1&&ind>3)  return;
  int mini = 0;
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

void FloatPar(RooArgList* argli, int ind)
{
  if(ind>5) return;
    int  spin = ((RooAbsReal&)(*argli)[argli->getSize()-2]).getVal();
//    std::cout << spin << std::endl;
  if(spin==1&&ind>3)  return;
  RooRealVar *var = (RooRealVar*)(argli->at(2*ind));
  var->setError(0.1); var->setConstant(0);
  RooRealVar *var1 = (RooRealVar*)(argli->at(2*ind+1));
  var1->setError(0.1); var1->setConstant(0);
}

void ResetPar(RooArgList* argli)
{
  int  J = ((RooAbsReal&)(*argli)[argli->getSize()-2]).getVal();
  int ind(0);
  for(int S = abs(J-2); S<=J+2; S+=2) {
    for(int L = S - 1; L<= S + 1; L+=2) {       
      RooRealVar *var = (RooRealVar*)(argli->at(2*ind));
      var->setVal(var->getVal()*sqrt(2./((double)S+1.)));
      RooRealVar *var1 = (RooRealVar*)(argli->at(2*ind+1));
      var1->setVal(var1->getVal()*sqrt(2./((double)S+1.)));
      ind++;
   }
  }
}


void FloatZPar(RooArgList* argli)
{
  int  spin = ((RooAbsReal&)(*argli)[argli->getSize()-2]).getVal();
//    std::cout << spin << std::endl;
//  if(spin==1&&ind>3)  return;
  for(int ind=0; ind<=3; ++ind) {
    RooRealVar *var = (RooRealVar*)(argli->at(2*ind));
    var->setError(0.1); var->setConstant(0);
    RooRealVar *var1 = (RooRealVar*)(argli->at(2*ind+1));
    var1->setError(0.1); var1->setConstant(0);
  }
  if(abs(spin)<=1) {
    int ind=3;
    RooRealVar *var = (RooRealVar*)(argli->at(2*ind));
    var->setVal(0.); var->setConstant(1);
    RooRealVar *var1 = (RooRealVar*)(argli->at(2*ind+1));
    var1->setVal(0.); var1->setConstant(1);
  }
}

int main(int argc, char **argv)
{
 TDatime BeginTime;
 std::cout << "Time(begin)  " << BeginTime.GetYear() << "." << BeginTime.GetMonth() << "." << BeginTime.GetDay() << "    " << BeginTime.GetHour() << ":" << BeginTime.GetMinute() << ":" << BeginTime.GetSecond() << std::endl; 

 assert( argc == 5 );

  TString input_path = TString(argv[1]);
  TString output_path = TString(argv[2]);
  TString mcsw_path = TString(argv[3]);
  TString data_path = TString(argv[4]);
  std::cout << "Writing efficiencies from <" << input_path << "> to <" << output_path << ">." << std::endl;
  std::cout << "Using real data from: " << data_path << std::endl;
  std::cout << "Using MC from: " << mcsw_path << std::endl << std::endl;

//#ifdef __CINT__
//  gROOT->ProcessLineSync(".x RooDalitzAmplitude.cxx+");
//#endif
  //gSystem->Load("RooIpatia2_cxx.so");
  int ZS = 3;//atoi( argv[3] );
  int  ZP = 1;//atoi( argv[4] );
  std::cout << "Fit Z J P " << ZS << " " << ZP << std::endl;  

  int NoCPU(10);
  
  int ZS2 = 5;//atoi( argv[1] );
  int  ZP2 = -1;//atoi( argv[2] );  

  std::cout << "Fit Z2 J P " << ZS2 << " " << ZP2 << std::endl;

  double m_lb = 5.6195, m_k = 0.493677, m_p = 0.938272046, m_jpsi = 3.096916;

  RooRealVar mkp("mkp","m(K^{-}p)",m_k+m_p,m_lb-m_jpsi);
  RooRealVar mjpsip("mjpsip","",m_jpsi+m_p,m_lb-m_k);
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
  RooRealVar sw("nsig_sw","sw",0);
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

  obs1->add(sw);
  TFile *fdata = new TFile(data_path.Data());
  TTree *data_tree_sw = (TTree*) fdata->Get("DTree");
  RooDataSet* datars1 = new RooDataSet("datars","",data_tree_sw, *obs1);
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
  std::cout << "obs " << std::endl;
  obs->Print("V");
//  exit(0);
  TFile *fmc = new TFile(mcsw_path.Data());//mcswbin.root");
  TTree *tree = (TTree*)fmc->Get("h1");
  RooDataSet * mcsw = new RooDataSet("mcsw","",tree, *obs1);
  RooDataSet *INDMC = new RooDataSet("INDMC","", RooArgSet(*index));
  nev = mcsw->numEntries();

  for(int i=0; i<nev; ++i) {
     *index = i;
//     if(i%1000==0) std::cout << "i " << i << std::endl;
     INDMC->add(RooArgSet(*index));
  }
  mcsw->merge(INDMC); 
  mcsw->Print("V");  
  RooDataSet * mcnorm = new  RooDataSet(TString(mcsw->GetName())+TString("new"),mcsw->GetTitle(),mcsw,*mcsw->get(),0,"nsig_sw");
  mcnorm->Print("V");  
  std::cout.precision(20);
  std::cout << "# mc" << nev << " sum weighted " <<  std::endl;

  double mcr = nev/(double)datars->numEntries();
  int cpu1 = (double) NoCPU - (double)NoCPU/(1.+1./sqrt(mcr));
  int cpu2 = NoCPU-cpu1;
  std::cout << "CPU DIV " << cpu1 << " " << cpu2 << std::endl;

//  exit(0);  
  double w(0.);
  double w2(0.);
  for( int i = 0; i < data_fit->numEntries(); ++i ){
    data_fit->get(i);
    double nsw = data_fit->weight();
    w += nsw;
    w2 += pow(nsw,2);
  }
  double swf2 = w/w2;
  double swf  = sqrt(swf2);

  double Wmc = mcnorm->sumEntries();
  double Wda = data_fit->sumEntries();
  RooRealVar Weimc("Weimc","Wei of mc norm", Wmc);
  RooRealVar Weida("Weida","Wei of data", Wda);
  RooRealVar SWF2("SWF2","Scale ", swf2);
  
  std::cout << "w data " << w << " " << Wda << std::endl;
  std::cout << "alpha to factor the -2lnL" << swf2 << std::endl;  
  TList *listZ = new TList();
  //===Lambda(Z) 1/2-
  RooRealVar m0_Z("m0_Z","m0",4.45,4.2,4.5);
  RooRealVar width_Z("width_Z","width",0.04,0,0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z("a_Bp_Z","",0.1,-5,5);
  RooRealVar p_Bp_Z("p_Bp_Z","",0.1,-5,5);
  RooRealVar a_L0_Z("a_L0_Z","",0.01);
  RooRealVar p_L0_Z("p_L0_Z","",0.01);
  RooRealVar a_L1_Z("a_L1_Z","",0.01);
  RooRealVar p_L1_Z("p_L1_Z","",0.01);
  RooRealVar a_L2_Z("a_L2_Z","",0.01);
  RooRealVar p_L2_Z("p_L2_Z","",0.01);
 
  RooRealVar R_Z("R_Z","",-1.0);
  RooRealVar I_Z("I_Z","",0.0);
  R_Z.setConstant(0);
  I_Z.setConstant(0);
  RooFormulaVar aa_L0_Z("aa_L0_Z","","@0*@2-@1*@3",RooArgList(a_L0_Z,p_L0_Z,R_Z,I_Z));
  RooFormulaVar pp_L0_Z("pp_L0_Z","","@0*@3+@1*@2",RooArgList(a_L0_Z,p_L0_Z,R_Z,I_Z));
  RooFormulaVar aa_L1_Z("aa_L1_Z","","@0*@2-@1*@3",RooArgList(a_L1_Z,p_L1_Z,R_Z,I_Z));
  RooFormulaVar pp_L1_Z("pp_L1_Z","","@0*@3+@1*@2",RooArgList(a_L1_Z,p_L1_Z,R_Z,I_Z));  
  RooFormulaVar aa_L2_Z("aa_L2_Z","","@0*@2-@1*@3",RooArgList(a_L2_Z,p_L2_Z,R_Z,I_Z));
  RooFormulaVar pp_L2_Z("pp_L2_Z","","@0*@3+@1*@2",RooArgList(a_L2_Z,p_L2_Z,R_Z,I_Z));  
  RooArgList* L_Z = new RooArgList(a_Bp_Z,p_Bp_Z,
				      aa_L0_Z,pp_L0_Z,
				      aa_L1_Z,pp_L1_Z,
				      aa_L2_Z,pp_L2_Z,"L_Z");
  L_Z->add(m0_Z);
  L_Z->add(width_Z);
  L_Z->add(RooRealConstant::value(ZS));//2 x spin
  L_Z->add(RooRealConstant::value(ZP)); //parity
  
  listZ->Add(L_Z);
  
  //===Lambda(Z) 1/2-
  RooRealVar m0_Z2("m0_Z2","m0",4.376,4.2,4.4);
  RooRealVar width_Z2("width_Z2","width",0.2,0,0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z2("a_Bp_Z2","",0.1,-5,5);
  RooRealVar p_Bp_Z2("p_Bp_Z2","",0.1,-5,5);
  RooRealVar a_L0_Z2("a_L0_Z2","",0.1);
  RooRealVar p_L0_Z2("p_L0_Z2","",0.1);
  RooRealVar a_L1_Z2("a_L1_Z2","",0.1);
  RooRealVar p_L1_Z2("p_L1_Z2","",0.1);
  RooRealVar a_L2_Z2("a_L2_Z2","",0.1);
  RooRealVar p_L2_Z2("p_L2_Z2","",0.1);
  RooRealVar R_Z2("R_Z2","",-1.0);
  RooRealVar I_Z2("I_Z2","",0.0);
  R_Z2.setConstant(0);
  I_Z2.setConstant(0);
  RooFormulaVar aa_L0_Z2("aa_L0_Z2","","@0*@2-@1*@3",RooArgList(a_L0_Z2,p_L0_Z2,R_Z2,I_Z2));
  RooFormulaVar pp_L0_Z2("pp_L0_Z2","","@0*@3+@1*@2",RooArgList(a_L0_Z2,p_L0_Z2,R_Z2,I_Z2));
  RooFormulaVar aa_L1_Z2("aa_L1_Z2","","@0*@2-@1*@3",RooArgList(a_L1_Z2,p_L1_Z2,R_Z2,I_Z2));
  RooFormulaVar pp_L1_Z2("pp_L1_Z2","","@0*@3+@1*@2",RooArgList(a_L1_Z2,p_L1_Z2,R_Z2,I_Z2));  
  RooFormulaVar aa_L2_Z2("aa_L2_Z2","","@0*@2-@1*@3",RooArgList(a_L2_Z2,p_L2_Z2,R_Z2,I_Z2));
  RooFormulaVar pp_L2_Z2("pp_L2_Z2","","@0*@3+@1*@2",RooArgList(a_L2_Z2,p_L2_Z2,R_Z2,I_Z2));
  RooArgList* L_Z2 = new RooArgList(a_Bp_Z2,p_Bp_Z2,
				      aa_L0_Z2,pp_L0_Z2,
				      aa_L1_Z2,pp_L1_Z2,
				      aa_L2_Z2,pp_L2_Z2,"L_Z2");
  L_Z2->add(m0_Z2);
  L_Z2->add(width_Z2);
  L_Z2->add(RooRealConstant::value(ZS2));//2 x spin
  L_Z2->add(RooRealConstant::value(ZP2)); //parity
  
  listZ->Add(L_Z2);

  TList *list = new TList();
  //===Lambda(1520) 3/2-
  RooRealVar m0_1520("m0_1520","m0",1.515);
  RooRealVar width_1520("width_1520","width",0.115);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1520("a_p1p1_1520","",1.);
  RooRealVar p_p1p1_1520("p_p1p1_1520","",0.);
  RooRealVar a_p100_1520("a_p100_1520","",0);
  RooRealVar p_p100_1520("p_p100_1520","",0);
  RooRealVar a_m100_1520("a_m100_1520","",0);
  RooRealVar p_m100_1520("p_m100_1520","",0);
  RooRealVar a_m1m1_1520("a_m1m1_1520","",0);
  RooRealVar p_m1m1_1520("p_m1m1_1520","",0);
  RooRealVar a_p3p1_1520("a_p3p1_1520","",0);
  RooRealVar p_p3p1_1520("p_p3p1_1520","",0);
  RooRealVar a_m3m1_1520("a_m3m1_1520","",0);
  RooRealVar p_m3m1_1520("p_m3m1_1520","",0);

  RooArgList* L_1520 = new RooArgList(a_p1p1_1520,p_p1p1_1520,
				      a_p100_1520,p_p100_1520,
				      a_m100_1520,p_m100_1520,
				      a_m1m1_1520,p_m1m1_1520,"L_1520");
  L_1520->add(a_p3p1_1520);
  L_1520->add(p_p3p1_1520);
  L_1520->add(a_m3m1_1520);
  L_1520->add(p_m3m1_1520);
  L_1520->add(m0_1520);
  L_1520->add(width_1520);
  L_1520->add(RooRealConstant::value(3.));//2 x spin
  L_1520->add(RooRealConstant::value(-1)); //parity
  


    //===Lambda(1440) 1/2+
  RooRealVar m0_1440("m0_1440","m0",1.430);
  RooRealVar width_1440("width_1440","width",0.35);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1440("a_p1p1_1440","",1.0);
  RooRealVar p_p1p1_1440("p_p1p1_1440","",0.0);
  RooRealVar a_p100_1440("a_p100_1440","",0);
  RooRealVar p_p100_1440("p_p100_1440","",0);
  RooRealVar a_m100_1440("a_m100_1440","",0);
  RooRealVar p_m100_1440("p_m100_1440","",0);
  RooRealVar a_m1m1_1440("a_m1m1_1440","",0);
  RooRealVar p_m1m1_1440("p_m1m1_1440","",0);
  RooRealVar a_p3p1_1440("a_p3p1_1440","",0);
  RooRealVar p_p3p1_1440("p_p3p1_1440","",0);
  RooRealVar a_m3m1_1440("a_m3m1_1440","",0);
  RooRealVar p_m3m1_1440("p_m3m1_1440","",0);

  RooArgList* L_1440 = new RooArgList(a_p1p1_1440,p_p1p1_1440,
				      a_p100_1440,p_p100_1440,
				      a_m100_1440,p_m100_1440,
				      a_m1m1_1440,p_m1m1_1440,"L_1440");
  L_1440->add(a_p3p1_1440);
  L_1440->add(p_p3p1_1440);
  L_1440->add(a_m3m1_1440);
  L_1440->add(p_m3m1_1440);
  L_1440->add(m0_1440);
  L_1440->add(width_1440);
  L_1440->add(RooRealConstant::value(1.));//2 x spin
  L_1440->add(RooRealConstant::value(1)); //parity


  //===Lambda(1535) 1/2-
  RooRealVar m0_1535("m0_1535","m0",1.535);
  RooRealVar width_1535("width_1535","width",0.15);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1535("a_p1p1_1535","",1.);
  RooRealVar p_p1p1_1535("p_p1p1_1535","",0);
  RooRealVar a_p100_1535("a_p100_1535","",0);
  RooRealVar p_p100_1535("p_p100_1535","",0);
  RooRealVar a_m100_1535("a_m100_1535","",0);
  RooRealVar p_m100_1535("p_m100_1535","",0);
  RooRealVar a_m1m1_1535("a_m1m1_1535","",0);
  RooRealVar p_m1m1_1535("p_m1m1_1535","",0);
  RooRealVar a_p3p1_1535("a_p3p1_1535","",0);
  RooRealVar p_p3p1_1535("p_p3p1_1535","",0);
  RooRealVar a_m3m1_1535("a_m3m1_1535","",0);
  RooRealVar p_m3m1_1535("p_m3m1_1535","",0);

  RooArgList* L_1535 = new RooArgList(a_p1p1_1535,p_p1p1_1535,
				      a_p100_1535,p_p100_1535,
				      a_m100_1535,p_m100_1535,
				      a_m1m1_1535,p_m1m1_1535,"L_1535");
  L_1535->add(a_p3p1_1535);
  L_1535->add(p_p3p1_1535);
  L_1535->add(a_m3m1_1535);
  L_1535->add(p_m3m1_1535);
  L_1535->add(m0_1535);
  L_1535->add(width_1535);
  L_1535->add(RooRealConstant::value(1.));//2 x spin
  L_1535->add(RooRealConstant::value(-1)); //parity


  //===Lambda(1650) 1/2-
  RooRealVar m0_1650("m0_1650","m0",1.655);
  RooRealVar width_1650("width_1650","width",0.14);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1650("a_p1p1_1650","",1.0);
  RooRealVar p_p1p1_1650("p_p1p1_1650","",0);
  RooRealVar a_p100_1650("a_p100_1650","",0);
  RooRealVar p_p100_1650("p_p100_1650","",0);
  RooRealVar a_m100_1650("a_m100_1650","",0);
  RooRealVar p_m100_1650("p_m100_1650","",0);
  RooRealVar a_m1m1_1650("a_m1m1_1650","",0);
  RooRealVar p_m1m1_1650("p_m1m1_1650","",0);
  RooRealVar a_p3p1_1650("a_p3p1_1650","",0);
  RooRealVar p_p3p1_1650("p_p3p1_1650","",0);
  RooRealVar a_m3m1_1650("a_m3m1_1650","",0);
  RooRealVar p_m3m1_1650("p_m3m1_1650","",0);

  RooArgList* L_1650 = new RooArgList(a_p1p1_1650,p_p1p1_1650,
				      a_p100_1650,p_p100_1650,
				      a_m100_1650,p_m100_1650,
				      a_m1m1_1650,p_m1m1_1650,"L_1650");
  L_1650->add(a_p3p1_1650);
  L_1650->add(p_p3p1_1650);
  L_1650->add(a_m3m1_1650);
  L_1650->add(p_m3m1_1650);
  L_1650->add(m0_1650);
  L_1650->add(width_1650);
  L_1650->add(RooRealConstant::value(1.));//2 x spin
  L_1650->add(RooRealConstant::value(-1)); //parity
  

  //===Lambda(1675) 5/2-
  RooRealVar m0_1675("m0_1675","m0",1.675);
  RooRealVar width_1675("width_1675","width",0.15);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1675("a_p1p1_1675","",0.00);
  RooRealVar p_p1p1_1675("p_p1p1_1675","",0.00);
  RooRealVar a_p100_1675("a_p100_1675","",0);
  RooRealVar p_p100_1675("p_p100_1675","",0);
  RooRealVar a_m100_1675("a_m100_1675","",0);
  RooRealVar p_m100_1675("p_m100_1675","",0);
  RooRealVar a_m1m1_1675("a_m1m1_1675","",0);
  RooRealVar p_m1m1_1675("p_m1m1_1675","",0);
  RooRealVar a_p3p1_1675("a_p3p1_1675","",0);
  RooRealVar p_p3p1_1675("p_p3p1_1675","",0);
  RooRealVar a_m3m1_1675("a_m3m1_1675","",0);
  RooRealVar p_m3m1_1675("p_m3m1_1675","",0);

  RooArgList* L_1675 = new RooArgList(a_p1p1_1675,p_p1p1_1675,
				      a_p100_1675,p_p100_1675,
				      a_m100_1675,p_m100_1675,
				      a_m1m1_1675,p_m1m1_1675,"L_1675");
  L_1675->add(a_p3p1_1675);
  L_1675->add(p_p3p1_1675);
  L_1675->add(a_m3m1_1675);
  L_1675->add(p_m3m1_1675);
  L_1675->add(m0_1675);
  L_1675->add(width_1675);
  L_1675->add(RooRealConstant::value(5.));//2 x spin
  L_1675->add(RooRealConstant::value(-1)); //parity


  //===Lambda(1680) 5/2+
  RooRealVar m0_1680("m0_1680","m0",1.685);
  RooRealVar width_1680("width_1680","width",0.13);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1680("a_p1p1_1680","",-3.752/100);
  RooRealVar p_p1p1_1680("p_p1p1_1680","",-10.28/100);
  RooRealVar a_p100_1680("a_p100_1680","",0);
  RooRealVar p_p100_1680("p_p100_1680","",0);
  RooRealVar a_m100_1680("a_m100_1680","",0);
  RooRealVar p_m100_1680("p_m100_1680","",0);
  RooRealVar a_m1m1_1680("a_m1m1_1680","",0);
  RooRealVar p_m1m1_1680("p_m1m1_1680","",0);
  RooRealVar a_p3p1_1680("a_p3p1_1680","",0);
  RooRealVar p_p3p1_1680("p_p3p1_1680","",0);
  RooRealVar a_m3m1_1680("a_m3m1_1680","",0);
  RooRealVar p_m3m1_1680("p_m3m1_1680","",0);

  RooArgList* L_1680 = new RooArgList(a_p1p1_1680,p_p1p1_1680,
				      a_p100_1680,p_p100_1680,
				      a_m100_1680,p_m100_1680,
				      a_m1m1_1680,p_m1m1_1680,"L_1680");
  L_1680->add(a_p3p1_1680);
  L_1680->add(p_p3p1_1680);
  L_1680->add(a_m3m1_1680);
  L_1680->add(p_m3m1_1680);
  L_1680->add(m0_1680);
  L_1680->add(width_1680);
  L_1680->add(RooRealConstant::value(5.));//2 x spin
  L_1680->add(RooRealConstant::value(1)); //parity


  //===Lambda(1700) 3/2-
  RooRealVar m0_1700("m0_1700","m0",1.7);
  RooRealVar width_1700("width_1700","width",0.15);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1700("a_p1p1_1700","",0.2);
  RooRealVar p_p1p1_1700("p_p1p1_1700","",-10.28/100);
  RooRealVar a_p100_1700("a_p100_1700","",0);
  RooRealVar p_p100_1700("p_p100_1700","",0);
  RooRealVar a_m100_1700("a_m100_1700","",0);
  RooRealVar p_m100_1700("p_m100_1700","",0);
  RooRealVar a_m1m1_1700("a_m1m1_1700","",0);
  RooRealVar p_m1m1_1700("p_m1m1_1700","",0);
  RooRealVar a_p3p1_1700("a_p3p1_1700","",0);
  RooRealVar p_p3p1_1700("p_p3p1_1700","",0);
  RooRealVar a_m3m1_1700("a_m3m1_1700","",0);
  RooRealVar p_m3m1_1700("p_m3m1_1700","",0);

  RooArgList* L_1700 = new RooArgList(a_p1p1_1700,p_p1p1_1700,
				      a_p100_1700,p_p100_1700,
				      a_m100_1700,p_m100_1700,
				      a_m1m1_1700,p_m1m1_1700,"L_1700");
  L_1700->add(a_p3p1_1700);
  L_1700->add(p_p3p1_1700);
  L_1700->add(a_m3m1_1700);
  L_1700->add(p_m3m1_1700);
  L_1700->add(m0_1700);
  L_1700->add(width_1700);
  L_1700->add(RooRealConstant::value(3.));//2 x spin
  L_1700->add(RooRealConstant::value(-1)); //parity
  
//  list->Add(L_1700);


  //===Lambda(1710) 1/2+
  RooRealVar m0_1710("m0_1710","m0",1.71);
  RooRealVar width_1710("width_1710","width",0.1);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1710("a_p1p1_1710","",0.2);
  RooRealVar p_p1p1_1710("p_p1p1_1710","",-10.28/100);
  RooRealVar a_p100_1710("a_p100_1710","",0);
  RooRealVar p_p100_1710("p_p100_1710","",0);
  RooRealVar a_m100_1710("a_m100_1710","",0);
  RooRealVar p_m100_1710("p_m100_1710","",0);
  RooRealVar a_m1m1_1710("a_m1m1_1710","",0);
  RooRealVar p_m1m1_1710("p_m1m1_1710","",0);
  RooRealVar a_p3p1_1710("a_p3p1_1710","",0);
  RooRealVar p_p3p1_1710("p_p3p1_1710","",0);
  RooRealVar a_m3m1_1710("a_m3m1_1710","",0);
  RooRealVar p_m3m1_1710("p_m3m1_1710","",0);

  RooArgList* L_1710 = new RooArgList(a_p1p1_1710,p_p1p1_1710,
				      a_p100_1710,p_p100_1710,
				      a_m100_1710,p_m100_1710,
				      a_m1m1_1710,p_m1m1_1710,"L_1710");
  L_1710->add(a_p3p1_1710);
  L_1710->add(p_p3p1_1710);
  L_1710->add(a_m3m1_1710);
  L_1710->add(p_m3m1_1710);
  L_1710->add(m0_1710);
  L_1710->add(width_1710);
  L_1710->add(RooRealConstant::value(1.));//2 x spin
  L_1710->add(RooRealConstant::value(1)); //parity
  
//  list->Add(L_1710);

  //===Lambda(1720) 3/2+
  RooRealVar m0_1720("m0_1720","m0",1.72);
  RooRealVar width_1720("width_1720","width",0.25);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1720("a_p1p1_1720","",0.2);
  RooRealVar p_p1p1_1720("p_p1p1_1720","",-10.28/100);
  RooRealVar a_p100_1720("a_p100_1720","",0);
  RooRealVar p_p100_1720("p_p100_1720","",0);
  RooRealVar a_m100_1720("a_m100_1720","",0);
  RooRealVar p_m100_1720("p_m100_1720","",0);
  RooRealVar a_m1m1_1720("a_m1m1_1720","",0);
  RooRealVar p_m1m1_1720("p_m1m1_1720","",0);
  RooRealVar a_p3p1_1720("a_p3p1_1720","",0);
  RooRealVar p_p3p1_1720("p_p3p1_1720","",0);
  RooRealVar a_m3m1_1720("a_m3m1_1720","",0);
  RooRealVar p_m3m1_1720("p_m3m1_1720","",0);

  RooArgList* L_1720 = new RooArgList(a_p1p1_1720,p_p1p1_1720,
				      a_p100_1720,p_p100_1720,
				      a_m100_1720,p_m100_1720,
				      a_m1m1_1720,p_m1m1_1720,"L_1720");
  L_1720->add(a_p3p1_1720);
  L_1720->add(p_p3p1_1720);
  L_1720->add(a_m3m1_1720);
  L_1720->add(p_m3m1_1720);
  L_1720->add(m0_1720);
  L_1720->add(width_1720);
  L_1720->add(RooRealConstant::value(3.));//2 x spin
  L_1720->add(RooRealConstant::value(1)); //parity
  
//  list->Add(L_1720);


  //===Lambda(1875) 3/2-
  RooRealVar m0_1875("m0_1875","m0",1.875);
  RooRealVar width_1875("width_1875","width",0.25);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1875("a_p1p1_1875","",0.001);
  RooRealVar p_p1p1_1875("p_p1p1_1875","",0.);
  RooRealVar a_p100_1875("a_p100_1875","",0);
  RooRealVar p_p100_1875("p_p100_1875","",0);
  RooRealVar a_m100_1875("a_m100_1875","",0);
  RooRealVar p_m100_1875("p_m100_1875","",0);
  RooRealVar a_m1m1_1875("a_m1m1_1875","",0);
  RooRealVar p_m1m1_1875("p_m1m1_1875","",0);
  RooRealVar a_p3p1_1875("a_p3p1_1875","",0);
  RooRealVar p_p3p1_1875("p_p3p1_1875","",0);
  RooRealVar a_m3m1_1875("a_m3m1_1875","",0);
  RooRealVar p_m3m1_1875("p_m3m1_1875","",0);

  RooArgList* L_1875 = new RooArgList(a_p1p1_1875,p_p1p1_1875,
				      a_p100_1875,p_p100_1875,
				      a_m100_1875,p_m100_1875,
				      a_m1m1_1875,p_m1m1_1875,"L_1875");
  L_1875->add(a_p3p1_1875);
  L_1875->add(p_p3p1_1875);
  L_1875->add(a_m3m1_1875);
  L_1875->add(p_m3m1_1875);
  L_1875->add(m0_1875);
  L_1875->add(width_1875);
  L_1875->add(RooRealConstant::value(3.));//2 x spin
  L_1875->add(RooRealConstant::value(-1)); //parity
  
//  list->Add(L_1875);

  //===Lambda(1900) 3/2+
  RooRealVar m0_1900("m0_1900","m0",1.9);
  RooRealVar width_1900("width_1900","width",0.2);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1900("a_p1p1_1900","",0.001);
  RooRealVar p_p1p1_1900("p_p1p1_1900","",0.);
  RooRealVar a_p100_1900("a_p100_1900","",0);
  RooRealVar p_p100_1900("p_p100_1900","",0);
  RooRealVar a_m100_1900("a_m100_1900","",0);
  RooRealVar p_m100_1900("p_m100_1900","",0);
  RooRealVar a_m1m1_1900("a_m1m1_1900","",0);
  RooRealVar p_m1m1_1900("p_m1m1_1900","",0);
  RooRealVar a_p3p1_1900("a_p3p1_1900","",0);
  RooRealVar p_p3p1_1900("p_p3p1_1900","",0);
  RooRealVar a_m3m1_1900("a_m3m1_1900","",0);
  RooRealVar p_m3m1_1900("p_m3m1_1900","",0);

  RooArgList* L_1900 = new RooArgList(a_p1p1_1900,p_p1p1_1900,
				      a_p100_1900,p_p100_1900,
				      a_m100_1900,p_m100_1900,
				      a_m1m1_1900,p_m1m1_1900,"L_1900");
  L_1900->add(a_p3p1_1900);
  L_1900->add(p_p3p1_1900);
  L_1900->add(a_m3m1_1900);
  L_1900->add(p_m3m1_1900);
  L_1900->add(m0_1900);
  L_1900->add(width_1900);
  L_1900->add(RooRealConstant::value(3.));//2 x spin
  L_1900->add(RooRealConstant::value(1)); //parity
  
//  list->Add(L_1900);

  list->Add(L_1440);  
  list->Add(L_1520);  
  list->Add(L_1535);  
  list->Add(L_1650);  
  list->Add(L_1675);  
  list->Add(L_1680);  
  list->Add(L_1700);  
  list->Add(L_1710);
  list->Add(L_1720);
  list->Add(L_1875);  
  list->Add(L_1900);    
  
//  return ;

  datars->Delete();

//  RooDalitzAmplitude *sigCC = new RooDalitzAmplitude("sigCC","", *obs, list, listZ, "mcsw.root",*data_fit, false);
  RooDalitzAmplitude *sigCC = new RooDalitzAmplitude("sigCC","", *obs, list, listZ, "one.root",*data_fit);
  RooArgSet* setdlz = sigCC->getParameters(*data_fit);  
  char name[10];
  if(ZP>0) {
   sprintf(name,"p%i",ZS);
  } else {
   sprintf(name,"m%i",ZS);  
  }
  
  char nameZ2[10];
  if(ZP2>0) {
   sprintf(nameZ2,"p%i",ZS2);
  } else {
   sprintf(nameZ2,"m%i",ZS2);  
  }    
//  if( exists(TString::Format("func/fitzall-%s-resall.func",name).Data()) )
//    setdlz->readFromFile(TString::Format("func/fitzall-%s-resall.func",name).Data());    

  SetLPar(L_1440,3,true); //1/2  
  SetLPar(L_1520,1); //3/2
  SetLPar(L_1535,3); //1/2 
  SetLPar(L_1650,3); //1/2 
  SetLPar(L_1675,1); //5/2 
  SetLPar(L_1680,1); //5/2 
  SetLPar(L_1700,1); //3/2 
  SetLPar(L_1710,3); //1/2
  SetLPar(L_1720,1); //3/2
  SetLPar(L_1875,1); //3/2 
  SetLPar(L_1900,1); //3/2 
//  FloatZPar(L_Z);
  setdlz->readFromFile("m3p5.func");
  setdlz->readFromFile("fit2zall-m3-p5-res5.func");
a_L0_Z.setConstant(1);
a_L0_Z2.setConstant(1);
a_L1_Z.setConstant(1);
a_L1_Z2.setConstant(1);
a_L2_Z.setConstant(1);
a_L2_Z2.setConstant(1);
m0_Z.setConstant(1);
m0_Z2.setConstant(1);
p_L0_Z.setConstant(1);
p_L0_Z2.setConstant(1);
p_L1_Z.setConstant(1);
p_L1_Z2.setConstant(1);
p_L2_Z.setConstant(1);
p_L2_Z2.setConstant(1);
width_Z.setConstant(1);
width_Z2.setConstant(1);

  setdlz->Print("V");
//  int NPOINT = 4;
  int mc_massBins=50;
  int mc_ybins = 50;
  std::cout<<"MCeff INIT"<<std::endl;  
//  sig.set_mceff_npoint(NPOINT);
  sigCC->eff_init(mc_massBins,mc_ybins,mcsw_path.Data(),0);
  std::cout<<"MCeff initialized"<<std::endl;  

  TFile *fin = new TFile(input_path, "READ");
  TTree *inTree = fin->Get<TTree>("tree");

  Double_t xmkp, xcosTheta_L, xcosTheta_Jpsi, xcosTheta_Lb, xphi_Mu, xphi_K, xw, xwL, xwZ, wInt; 
  Double_t xeff;
  inTree->SetBranchAddress("mkp", &xmkp );
  inTree->SetBranchAddress("cosTheta_L", &xcosTheta_L );  
  inTree->SetBranchAddress("cosTheta_Jpsi", &xcosTheta_Jpsi );  
  inTree->SetBranchAddress("cosTheta_Lb", &xcosTheta_Lb );  
  inTree->SetBranchAddress("phiMu", &xphi_Mu );
  inTree->SetBranchAddress("phiK", &xphi_K );  

  TFile *fout = new TFile(output_path, "RECREATE");
  TTree * outTree = new TTree("tree", "");
  TBranch *nb = outTree->Branch("eff", &xeff, "eff/D");
  // TBranch *nb2 = fChain->Branch("nsig_sw", &xw, "nsig_sw/D");   

  Long64_t numevt = inTree->GetEntries();
 // numevt = 100;
  for(Long64_t i=0; i<numevt; ++i) {
    inTree->GetEntry(i);
    xeff = -1.0;
    xeff = sigCC->GetEff(xmkp,xcosTheta_Lb,xcosTheta_L,xcosTheta_Jpsi,xphi_Mu,xphi_K);
    if(i<=20) printf("%5.2f %5.2f %5.2f %5.2f %5.2f %5.2f %5.2e\n", xmkp,xcosTheta_Lb,xcosTheta_L,xcosTheta_Jpsi,xphi_Mu,xphi_K,xeff);

    outTree->Fill();
    // nb2->Fill();
  }
  outTree->Write("",TObject::kOverwrite);
  fin->Close();
  fout->Close();

}

