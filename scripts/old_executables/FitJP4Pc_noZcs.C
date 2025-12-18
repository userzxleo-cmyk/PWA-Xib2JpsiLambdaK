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
#include "TRandom2.h"
#include "RooMinimizer.h"
#include "RooAddition.h"
#include "RooTDalitz/RooFGaussian.h"
#include "RooTDalitz/Resonances.h"
#include "RooMsgService.h"
#define smear
#define read
using namespace RooFit;
TRandom2 *g_rnd;

//Z 4440
//Z2 4380
//Z3 4457 
//Z4 4312
//Z5 broad  
//Z6 new at 4.50 broad
#if 0
void SetWave(RooArgSet &setdlz, RooDalitzAmplitude *pdf, int index, bool fixmax=true)
{
  TString lwvname[]={"1P", "1M", "3P", "3M" };
  
  char varname[100];
  double re, im;
  //double min = 1.43;
  //double max = 2.53;
  for(int i=0; i<NSP; ++i) {
    sprintf(varname,"%sR_%02d",TString(lwvname[iwave]).Data(),i);
    RooRealVar *var = (RooRealVar*)setdlz.find(varname);
    var->setVal(re);
    sprintf(varname,"%sI_%02d",TString(lwvname[iwave]).Data(),i);
    var = (RooRealVar*)setdlz.find(varname);
    var->setVal(im);
  }  
  if(fixmax) {
    sprintf(varname,"%sR_%02d",TString(lwvname[iwave]).Data(),imax);
    RooRealVar *var = (RooRealVar*)setdlz.find(varname);
    var->setConstant(1);
    sprintf(varname,"%sI_%02d",TString(lwvname[iwave]).Data(),imax);
    var = (RooRealVar*)setdlz.find(varname);
    var->setConstant(1);
  }
}
#endif

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


int main(int argc, char **argv)
{
 TDatime BeginTime;
 std::cout << "Time(begin)  " << BeginTime.GetYear() << "." << BeginTime.GetMonth() << "." << BeginTime.GetDay() << "    " << BeginTime.GetHour() << ":" << BeginTime.GetMinute() << ":" << BeginTime.GetSecond() << std::endl; 
//  int seed = atoi( argv[11] );
  bool method=false;

  int ZS0(1), ZP0(1);
  int ZS5(1), ZP5(1);
  //4440
  int ZS = atoi( argv[1] );
  int  ZP = atoi( argv[2] );
  std::cout << "Fit 4440 J P " << ZS << " " << ZP << std::endl;  

  //model indipendent
  int ZS6 = 666;//atoi( argv[9] );
  int  ZP6 = 666;//atoi( argv[10] );  
  std::cout << "no Fit broad J P " << ZS6 << " " << ZP6 << std::endl;      

  int ZS2 = atoi( argv[7] );
  int  ZP2 = atoi( argv[8] );  
  
  std::cout << "Fit broad1 J P " << ZS2 << " " << ZP2 << std::endl;    

  int ZS4 = atoi( argv[5] );
  int  ZP4 = atoi( argv[6] );  
  
  std::cout << "Fit 4312 J P " << ZS4 << " " << ZP4 << std::endl;    

  int ZS3 = 666;//atoi( argv[7] );
  int  ZP3 = 666;//atoi( argv[8] );
  std::cout << "Fit 4500 J P " << ZS3 << " " << ZP3 << std::endl;
#if 1
  ZS5 = atoi( argv[3] );
  ZP5 = atoi( argv[4] );  
  
  std::cout << "Fit 4457 J P " << ZS5 << " " << ZP5 << std::endl;    
#endif
          
          
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
  TFile *fdata = new TFile("sPlot.root");
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


#include "RooTDalitz/ResX.h"
//  listX->Add(L_Xminus);
#include "RooTDalitz/ResZ.h"
  listZ->Add(L_Z);
  listZ->Add(L_Z2);
//  listZ->Add(L_Z3);
  listZ->Add(L_Z4);
  listZ->Add(L_Z5);  
//  listZ->Add(L_Z6);
//  listZ->Add(L_Z1P);    
//  LISTz->Add(L_Z1M);    
//  TList *list = new TList();
//#include "RooTDalitz/ResLSameCoup.h"
#include "RooTDalitz/ResL.h"

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
  

  datars1->Delete();
  datars->Delete();
  datars2->Delete();  
  fdata->Close();

  RooDalitzAmplitude *sig = new RooDalitzAmplitude("sig","", *obs, list, listZ, listX,  "one.root",*data_fit);  
  RooArgSet* setdlz = sig->getParameters(*data_fit);  
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
  char nameZ3[10];
  if(ZP3>0) {
   sprintf(nameZ3,"p%i",ZS3);
  } else {
   sprintf(nameZ3,"m%i",ZS3);
  }
  char nameZ4[10];
  if(ZP4>0) {
   sprintf(nameZ4,"p%i",ZS4);
  } else {
   sprintf(nameZ4,"m%i",ZS4);  
  }    

  char nameZ6[10];
  if(ZP6>0) {
   sprintf(nameZ6,"p%i",ZS6);
  } else {
   sprintf(nameZ6,"m%i",ZS6);  
  }    

  char nameZ5[10];
  if(ZP5>0) {
   sprintf(nameZ5,"p%i",ZS5);
  } else {
   sprintf(nameZ5,"m%i",ZS5);  
  }    
    
//  setdlz->readFromFile("func/fitL-IndividialLS-BWfactor-converted.func");
  TString errlist = "err/PRC.err";

//  setdlz->readFromFile("func/floatZFloat18L-KM5PcSPLS01-IndividualLS_floatL-4440m1-4457m3-4312m1-4500m1-broadp3-setLError-AllFlPc.func");
//  setdlz->readFromFile("func/floatZFloat18L-KM5PcSPLS01-IndividualLS_floatL-4440m1-4457m3-4312m3-4500m1-broadp3-setLError-AllFlPc.func");
  //setdlz->readFromFile("func/floatZFloat18L-KM5PcSPLS01-IndividualLS_floatL-4440m1-4457m3-4312m1-4500m3-broadp3-setLError-AllFlPc.func");
  setdlz->readFromFile(TString::Format("func/floatZFloat18L-KM3PcSPLS01-noZcs-IndividualLS_floatL-4440%s-4457%s-4312%s-setLError-AllFlPc.func",name, nameZ5, nameZ4).Data());
  std::cout << "read from file: " << "func/floatZFloat18L-KM5PcSPLS01-IndividualLS_floatL-4440p3-4457p1-4312m1-4500m1-broadm3-setLError-AllFlPc.func" << std::endl;
/*  FloatZPar(L_Z,sig,3,-1);
  FloatZPar(L_Z2,sig,1,-1);
  FloatZPar(L_Z3,sig,3,-1);
  FloatZPar(L_Z4,sig,1,-1);
  FloatZPar(L_Z5,sig,5,-1);  
  FloatZPar(L_Z6,sig,3,1);  
  */
  
  FloatZPar(L_Z,false);
  FloatZPar(L_Z2,false);
  FloatZPar(L_Z3,false);
  FloatZPar(L_Z4,false);
  FloatZPar(L_Z5,false);
  FloatZPar(L_Z6,false);
//  m0_Z.setVal(4.440); width_Z.setVal(0.03);
  setdlz->Print("V");

#if 0
  m0_Z.setConstant(1);
  m0_Z4.setConstant(1);
  m0_Z5.setConstant(1);
  width_Z.setConstant(1);
  width_Z4.setConstant(1);
  width_Z5.setConstant(1);  
  
  double NLL0 = doFit(obs, list, listZ,  data_fit, 0, false, errlist);
#endif
  m0_Z.setConstant(0);
  m0_Z.setRange(4.4403-9.75/1000, 4.4403+8.602/1000);
  m0_Z4.setConstant(0);
  m0_Z4.setRange(4.3119-1.84/1000, 4.3119+13.67/1000);
  m0_Z5.setConstant(0);
  m0_Z5.setRange(4.4573-3.6/1000, 4.4573+8.3/1000);
  m0_Z2.setConstant(0);
  m0_Z2.setRange(4.1, 4.7);
  width_Z.setConstant(0);
  width_Z.setRange(0.003, 20.6/1000+20./1000);
  width_Z4.setConstant(0);
  width_Z4.setRange(0.003, 9.8/1000+9.7/1000);
  width_Z5.setConstant(0); 
  width_Z5.setRange(0.003, 6.4/1000+12.1/1000);
  width_Z2.setConstant(0); 
  width_Z2.setRange(0.003, 0.8); 
  #if 1
  double NLL2 = doFit(obs, list, listZ, listX,  data_fit, 0, true, errlist);
  setdlz->writeToFile(TString::Format("func/floatZFloat18L-KM4PcSPLS01-noZcs-IndividualLS_floatL-4440%s-4457%s-4312%s-broad1%s-setLError-AllFlPc.func",name, nameZ5, nameZ4, nameZ2).Data());
  setdlz->Print("V");
  std::cout << "NLLHalfwFloat " << NLL2 << std::endl;    
   double ll26 = LL(obs, list, listZ, listX,  data_fit26, "mcsw_26.root");
   printf("LLCheck-all-26: %10.3f,  %10.3f\n",NLL2,ll26);

  std::cout << "Finished first migrade" << std::endl; 
  #endif
  if(1) {
//  setdlz->writeToFile(TString::Format("func/floatZFloat18L-KM6PcSPLS01-IndividualLS_floatL-%s-4457%s-broad%s-4312%s-broad%s-setLError-fixPc.func",name, nameZ5, nameZ2,nameZ4, nameZ3).Data());
  RooDalitzAmplitude *signew = new RooDalitzAmplitude("signew","", *obs, list, listZ, listX, "mcsw.root",*data_fit);
//  setdlznew = setdlz;
  std::cout << "Generate Toy ...." << std::endl;
//  setdlznew->Print("V");
//
//  signew->genToy("Toy");        

  signew->genToy("mcsw.root",TString::Format("floatZFloat18L-KM4PcSPLS01-noZcs-IndividualLS_floatL-4440%s-4457%s-4312%s-broad1%s-setLError-AllFlPc",name, nameZ5, nameZ4, nameZ2).Data());
  signew->Delete(); 
  RooDalitzAmplitude *sigflat = new RooDalitzAmplitude("sigflat","", *obs, list, listZ, listX, "MCFlatAcc.root", *data_fit);
  //RooArgSet* setdlzflat = sigflat->getParameters(*data_fit);


  //===Calculate Fit Fraction===
  int nres = list->GetSize();//+listZ->GetSize();
  double Dsum[30];
  sigflat->getInt(Dsum);
  char resname[30];
  double sum(0);
  std::cout << "======Fit Fraction======" << std::endl;
  for(int i=0; i<nres; ++i) {
     printf("%10s %6.2f\n", (list->At(i))->GetName(),Dsum[i]*100.0 );
     sum += Dsum[i];
  }
  for(int i=0; i<listZ->GetSize(); ++i) {
    printf("%10s %6.2f\n", (listZ->At(i))->GetName(),Dsum[i+nres]*100.0 );
    sum += Dsum[i+nres];    
  }
  for(int i=0; i<listX->GetSize(); ++i) {
    printf("%10s %6.2f\n", (listX->At(i))->GetName(),Dsum[i+nres]*100.0 );
    sum += Dsum[i+nres];    
  }
  printf("Total  %6.2f\n", sum*100.0 );  

//#ifndef fit  
  sigflat->Delete();     
  }
//  doFit(obs, list, listZ, data_fit);

//#endif
 std::cout << "AmplitudeFit() successfully completed!" << std::endl;
 TDatime FinishTime;
 std::cout << "Time(finish)  " << FinishTime.GetYear() << "." << FinishTime.GetMonth() << "." << FinishTime.GetDay() << "    " << FinishTime.GetHour() << ":" << FinishTime.GetMinute() << ":" << FinishTime.GetSecond() << std::endl;
 return 0;            
  
}

