using namespace RooFit;
void setStyle(TH1F &histos, const char* histname, TLegend & leg)
{
#include "style.h"
}

TCanvas* draw(TH1F* h140,  TH1F* h440)
{
  TH1F *h540 = (TH1F*)h440->Clone("hchi");
  h540->GetSumw2()->Set(0);
  h540->SetYTitle("");

  //h540->SetYTitle("");//new TH1F("hchi","",h440->GetNbinsX(), h440->GetXaxis()->GetXmin(), h440->GetXaxis()->GetXmax());//
//  TCanvas* c = new TCanvas(h440->GetTitle(),"",1366,768);
  TCanvas* c = new TCanvas(h440->GetTitle(),"",700,600);
  c->Divide(1,2,0.0,0.0);

  c->cd(1);
  TVirtualPad* c_1 = c->GetPad(1);
  c_1->SetPad(0,0.3,1,1);
  c_1->SetRightMargin(0.05);
  c_1->SetTopMargin(0.05);
  h140->SetMinimum(0);
  h140->SetMarkerStyle(1);
  h140->Draw("hist");
//  h140->GetYaxis()->SetTitleOffset(1.0);  
  h440->Draw("Same");
  TH1F *hpdf = (TH1F*)h140->Clone("hpdf");
  hpdf->Sumw2();

  double chi2mpp(0);
  int ndf(0);
  for(int i=1; i<=h440->GetNbinsX(); ++i) {
    double CHI = 0;//
    double err;
    if(h440->GetBinError(i)<=1) h440->SetBinError(i,1);
    if((err=sqrt(pow(h440->GetBinError(i),2)+pow(hpdf->GetBinError(i),2)))>0) CHI =(h440->GetBinContent(i)-hpdf->GetBinContent(i))/err;
//    cout << CHI << endl;
    chi2mpp += CHI*CHI;
    if(fabs(h440->GetBinContent(i))>1e-10||fabs(hpdf->GetBinContent(i))>1e-10) ndf++;
    //cout <<  i  << " " << CHI << " " << h440->GetBinError(i) << " " << h440->GetBinContent(i) << " " << hpdf->GetBinError(i) << " " << hpdf->GetBinContent(i) << endl;
    h540->SetBinContent(i,CHI);
  }
  cout << "===1D chi2 with for " << h440->GetTitle() << " with " << ndf << " bins =" << chi2mpp << endl;


  TVirtualPad* c_2 = c->GetPad(2);
  c->cd(2);
  c_2->SetPad(0,0.0,1,0.3);
  c_2->SetRightMargin(0.05);
  c_2->SetBottomMargin(0.5);
  h540->GetXaxis()->SetLabelFont(133);
  h540->GetYaxis()->SetLabelFont(133);
  h440->GetYaxis()->SetLabelSize(0.07*0.8);
  h540->GetXaxis()->SetLabelSize(18*1.8*0.8); // labels will be 14 pixels
  h540->GetYaxis()->SetLabelSize(18*1.8*0.8); // labels will be 14 pixels
  h540->GetYaxis()->SetNdivisions(503); // labels will be 14 pixels
  h540->GetXaxis()->SetTitleSize(0.24*0.8);
  h440->GetYaxis()->SetTitleSize(0.09*0.8);
  h440->GetYaxis()->SetTitleOffset(0.75*0.8);
  h540->SetMinimum(-5);
  h540->SetMaximum(5);
 // TCanvas *c2 = new TCanvas("c2","");
  h540->SetFillColor(21);
  h540->Draw();
  double xxmin=h540->GetXaxis()->GetXmin();
  double xxmax=h540->GetXaxis()->GetXmax();
  TLine *l1 = new TLine(xxmin,0,xxmax,0);
  l1->Draw();
  TLine *l2 = new TLine(xxmin,-3,xxmax,-3);
  l2->SetLineStyle(3);
  l2->Draw();
  TLine *l3 = new TLine(xxmin,3,xxmax,3);
  l3->SetLineStyle(3);
  l3->Draw();
  c->Update();
  return c;
}


void plotbineach(double x1, double x2, int ibin, char* name)
{
//#ifdef __CINT__
//  gROOT->ProcessLineSync(".x RooDalitzAmplitude.cxx+");
//#endif
  gROOT->ProcessLine(".x ~/lhcbStyle.C");
  gStyle->SetHistMinimumZero(1);
//  gStyle->SetPadLeftMargin(0.1);
    
  //gSystem->Load("RooIpatia2_cxx.so");

          
  RooRealVar *mkp = new RooRealVar("mkp","m(K^{-}p)",1.4,2.6);
  RooRealVar *mjpsip = new RooRealVar("mjpsip","",4.,5.2);
//  cosk:cosmu:chi
  RooRealVar *cosTheta_L = new RooRealVar("cosTheta_L","cosTheta_L",-1,1);
  RooRealVar *cosTheta_Jpsi = new RooRealVar("cosTheta_Jpsi","cosTheta_Jpsi",-1,1);
  RooRealVar *cosTheta_Lb = new RooRealVar("cosTheta_Lb","cosTheta_Lb",-1,1);
  RooRealVar Z_cosTheta_Lb("Z_cosTheta_Lb","Z_cosTheta_Lb",-1,1);
  RooRealVar Z_cosTheta_Z("Z_cosTheta_Z","Z_cosTheta_Z",-1,1);
  RooRealVar Z_cosTheta_Jpsi("Z_cosTheta_Jpsi","Z_cosTheta_Jpsi",-1,1);
  RooRealVar cosTheta_p("cosTheta_p","cosTheta_p",-1,1);
  RooRealVar *phiK= new RooRealVar("phiK","phiK",-TMath::Pi(), TMath::Pi());
  RooRealVar *phiMu= new RooRealVar("phiMu","phiMu",-TMath::Pi(), TMath::Pi());
  RooRealVar Z_phiZ("Z_phiZ","Z_phiZ",-TMath::Pi(), TMath::Pi());
  RooRealVar Z_phiJpsi("Z_phiJpsi","Z_phiJpsi",-TMath::Pi(), TMath::Pi());
  RooRealVar Z_phiMu("Z_phiMu","Z_phiMu",-TMath::Pi(), TMath::Pi());
  RooRealVar alpha_Mu("alpha_Mu","alpha_Mu",-TMath::Pi(), TMath::Pi());
  RooArgList *obs = new RooArgList(*mkp,*cosTheta_Lb,*cosTheta_L,*cosTheta_Jpsi,*phiK,*phiMu);  
  obs->add(Z_cosTheta_Lb);
  obs->add(Z_cosTheta_Z);
  obs->add(Z_cosTheta_Jpsi);
  obs->add(Z_phiZ);
  obs->add(Z_phiJpsi);
  obs->add(Z_phiMu);
  obs->add(cosTheta_p);
  obs->add(alpha_Mu);
  obs->add(*mjpsip);

  RooRealVar nsig_sw("nsig_sw","",0);
  obs->add(nsig_sw);
  RooArgList *obs1 = new RooArgList(*mjpsip,*cosTheta_Lb,*cosTheta_L,*cosTheta_Jpsi,*phiK,*phiMu);  
      
  TFile *fdata = new TFile("../Fit/sPlot.root");
  TTree *datree = (TTree*)fdata->Get("DTree");
  RooDataSet* datars1 = new RooDataSet("datars","",datree, *obs);
//  RooDataSet *datars1 = (RooDataSet*)fdata->Get("datars");
//  RooDataSet *datars = (RooDataSet*)datars1->reduce("mkp>1.431950001&&mkp<2.522583999999&&mjpsip>4.0351880460&&mjpsip<5.125823&&!(mjpsip>4.25&&mjpsip<4.58)");  
  RooDataSet *datars = (RooDataSet*)datars1->reduce("mkp>1.431950001&&mkp<2.522583999999&&mjpsip>4.0351880460&&mjpsip<5.125823");  
  RooDataSet * data_fit = new  RooDataSet(TString(datars->GetName())+TString("new"),datars->GetTitle(),datars,*datars->get(),0,"nsig_sw");

//  data_fit->Print("V");
  
  //TFile *toy = new TFile(TString::Format("/nishome/zhanglm/workdir/Lb/code/code-GPUv4/Fit/%s.root",name).Data());      
  TFile *toy = new TFile(TString::Format("/nishome/zhanglm/workdir/Lb/code/code-Reduced/Fit/toy/%s.root",name).Data());      
  //TFile *toy = new TFile("/home/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/code-GPUv2/Fit/wuda/toyfloatZFix18L-KM4Pc-IndividualLS-m1-m3-broadm3-m1-addNR-setLError.root");      
  //TFile *toy = new TFile("/home/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/code-GPUv2/Fit/toyfloatZFix18L-KM5Pc-IndividualLS-m3-m1-broadm3-m1-broadm3-new.root");      
  //TFile *toy = new TFile("/home/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/code-GPUv2/Fit/toyfloatZFix18L-KM6Pc-IndividualLS-m3-m1-broadm3-m1-broadm3-broadm1-new.root");      
  //TFile *toy = new TFile("/home/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/code-GPUv2/Fit/toyfloatZFix18L-IndividualLS-m3-m1-spnm3-m1.root");      
  //TFile *toy = new TFile(TString::Format("/home/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/code-GPUv2/Fit/wuda/toyfloatZFix18L-SameLS-m3-m1-spnm3-m1.root",name).Data());      
  //TFile *toy = new TFile(TString::Format("/home/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman_test/Fit/toyfitz2.root",name).Data());      
  //TFile *toy = new TFile(TString::Format("/home/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/code-GPUv2/Fit/toyfloatZFix18L-IndividualLS-m3-m1-spnm3-m1.root",name).Data());      
  TTree *toytree = (TTree*)toy->Get("tree");  
  TObjArray* listb = toytree->GetListOfBranches();
    
  std::vector<const char*> NameList;
  TString sname;
  std::cout << "list " << listb->GetSize() << std::endl;
  for(int i=0; i<listb->GetSize(); ++i) {
    TBranch *ibr = (TBranch*)listb->At(i);
    if(ibr==NULL) continue;
    const char* name = ibr->GetName();
    sname = TString(name);
    if((sname.Contains("wZ")|sname.Contains("wL")|sname.Contains("wPWZ")|sname.Contains("wX")) && !(sname.Contains("wL_Z"))) {
    //if(sname.Contains("wL_Z")|sname.Contains("wZ")|sname.Contains("wL")) {
    //if(sname.Contains("wPWS01")|sname.Contains("wPWP01")|sname.Contains("wPWP03")|sname.Contains("wPWD03")|sname.Contains("wPWD05")|sname.Contains("wPWF05")|sname.Contains("wPWF07")|sname.Contains("wPWG07")|sname.Contains("wPWG09")|sname.Contains("wPWH09")) {
      NameList.push_back(name);
      cout << sname << endl;
    }
//  NameList.push_back("wZ");
//  NameList.push_back("wL");
//  NameList.push_back("wPWS01");
//  NameList.push_back("wPWP01");
//  NameList.push_back("wPWP03");
//  NameList.push_back("wPWD03");
//  NameList.push_back("wPWD05");
//  NameList.push_back("wPWF05");
//  NameList.push_back("wPWF07");
//  NameList.push_back("wPWG07");
//  NameList.push_back("wPWG09");
//  NameList.push_back("wPWH09");
  }
  std::cout <<"Done 1" << std::endl;      
  TH1F *h101 = new TH1F("h101","",200,1.4,2.6);
  TH1F *h201 = new TH1F("h201","",200,1.4,2.6);
  TH1F *h301 = new TH1F("h301","",200,1.4,2.6);
  h101->Sumw2(); h201->Sumw2(); h301->Sumw2();
  data_fit->fillHistogram(h101,*mkp);
  toytree->Project("h201","mkp","w");
  h101->SetMinimum(0);
  h201->SetMarkerColor(2);
  h201->SetLineColor(2); 
//  h101->SetMarkerStyle(20);
  h201->SetMarkerSize(0.75);
  h101->SetMarkerSize(0.75);  
  double scale = h101->Integral()/h201->Integral();
  h201->Scale(scale);
  h301->Scale(h101->Integral()/h301->Integral());
//  std::cout << h201->Integral() << " " << h301->Integral() << std::endl;
  h201->SetMaximum(h101->GetMaximum()*1.2);
  h101->SetTitle("mkp");
//  h101->Draw();
//  h201->Draw("same,E");    
//  h201->SetMarkerSize(0.7);

  
//  h101->SetMarkerStyle(21);
  //h101->SetMarkerSize(1.1);
//  h201->Draw();  
//  h101->Draw("Same");
  h101->SetXTitle("#font[52]{m_{Kp}} [GeV/#it{c}^{2}]");
  h201->SetYTitle("Events/ (6 MeV)");
  if(ibin==0) {
  TCanvas *c1 = draw(h201,h101);  
  c1->cd(1);

  double mLegScale=2.0;
  TLegend *leg = new TLegend(0.6,0.28,1.02,0.88,NULL,"brNDC");
  leg->SetBorderSize(0);
  leg->SetLineColor(1);
  //   leg->SetLineStyle(lineStyle);
  leg->SetLineWidth(3);
  leg->SetFillColor(0);
  leg->SetFillStyle(0);  
  leg->AddEntry(h101,"data");
  leg->AddEntry(h201,"total fit");
  for(int i=0; i<NameList.size(); ++i) {
    TH1F *h301 = new TH1F("h301","",200,1.4,2.6);
    h301->Sumw2();
    toytree->Project("h301","mkp",NameList[i]);
    h301->Scale(scale);
    h301->SetTitle(NameList[i]);   
//    if(h301->Integral()>1e-5) setStyle(*h301,NameList[i],*leg);
    //    h301->DrawCopy("Same",NameList[i]);    
  }
  h201->Draw("hsame");
  h101->Draw("same");  
  lhcbLatex->SetTextSize(0.07);
  lhcbLatex->SetNDC(kTRUE);
  lhcbLatex->SetTextColor(kBlue);
  lhcbLatex->DrawLatex(0.2,0.85, "LHCb unofficial"); 
  leg->Draw();
  
  c1->SaveAs(TString::Format("eps/mkp-%s.pdf",name).Data());
  }
  char cut1[100],cut2[100], cut3[100];
  sprintf(cut1,"mkp>%10.3f&&mkp<%10.3f",x1, x2);
  sprintf(cut2,"w*(mkp>%10.3f&&mkp<%10.3f)",x1, x2);  
  sprintf(cut3,"*(mkp>%10.3f&&mkp<%10.3f)",x1, x2);      

  TH1F *h131 = new TH1F("h131","",200,4.2,4.7);
  TH1F *h231 = new TH1F("h231","",200,4.2,4.7);
  TH1F *h331 = new TH1F("h331","",200,4.2,4.7);
  h131->Sumw2(); h231->Sumw2(); h331->Sumw2();
  data_fit->fillHistogram(h131,*mjpsip,cut1);
  toytree->Project("h231","mjpsip",cut2);
  h131->SetMinimum(0);
  h231->SetMarkerColor(2);
  h231->SetLineColor(2); 
//  h131->SetMarkerStyle(24);
  h231->SetMarkerSize(0.75);
  h131->SetMarkerSize(0.75);  
  h231->Scale(scale);
//  h231->SetMarkerStyle(0);
//  std::cout << h231->Integral() << " " << h331->Integral() << std::endl;
  h231->SetMaximum(h131->GetMaximum()*1.2);
  h131->SetTitle("mjpsip1");
//  h131->Draw();
//  h231->Draw("same,E");    
//  h231->SetMarkerSize(0.7);

  
//  h131->SetMarkerStyle(21);
  //h131->SetMarkerSize(1.1);
//  h231->Draw();  
//  h131->Draw("Same");
  h131->SetXTitle("#font[52]{m_{J/#psip}} [GeV/#it{c}^{2}]");
  h231->SetYTitle("Events/ (2.5 MeV)");
  TCanvas *c2 = draw(h231,h131);  
  c2->cd(1);
   TLegend *leg3 = new TLegend(0.6,0.28,1.02,0.88,NULL,"brNDC");
  for(int i=0; i<NameList.size(); ++i) {
    TH1F *h331 = new TH1F("h331","",200,4.2,4.7);
    h331->Sumw2();
    toytree->Project("h331","mjpsip",TString(NameList[i])+TString(cut3));
    h331->Scale(scale);
    h331->SetTitle(NameList[i]);   
//  if(h331->Integral()>1e-5)   setStyle(*h331,NameList[i],*leg3);
//    h331->DrawCopy("Same",NameList[i]);    
  }
  h231->Draw("hsame");
  h131->Draw("same"); 
  lhcbLatex->SetTextSize(0.07);
  lhcbLatex->SetNDC(kTRUE);
  lhcbLatex->SetTextColor(kBlue);
  lhcbLatex->DrawLatex(0.5,0.85, TString::Format("m(Kp) in (%4.2f, %4.2f) GeV",x1,x2).Data());  
  lhcbLatex->DrawLatex(0.2,0.85, "LHCb unofficial"); 
  c2->SaveAs(TString::Format("eps/mjpsipzoom%i-%s.pdf",ibin,name).Data());  

  TH1F *h151 = new TH1F("h151","",200,4.,5.2);
  TH1F *h251 = new TH1F("h251","",200,4.,5.2);
  TH1F *h351 = new TH1F("h351","",200,4.,5.2);
  h151->Sumw2(); h251->Sumw2(); h351->Sumw2();
  data_fit->fillHistogram(h151,*mjpsip,cut1);
  toytree->Project("h251","mjpsip",cut2);
  h151->SetMinimum(0);
  h251->SetMarkerColor(2);
  h251->SetLineColor(2); 
//  h151->SetMarkerStyle(24);
  h251->SetMarkerSize(0.75);
  h151->SetMarkerSize(0.75);  
  h251->Scale(scale);
//  std::cout << h251->Integral() << " " << h351->Integral() << std::endl;
  h251->SetMaximum(h151->GetMaximum()*1.2);
  h151->SetTitle("mjpsip51");
//  h151->Draw();
//  h251->Draw("same,E");    
//  h251->SetMarkerSize(0.7);

  
//  h151->SetMarkerStyle(21);
  //h151->SetMarkerSize(1.1);
//  h251->Draw();  
//  h151->Draw("Same");
  h151->SetXTitle("#font[52]{m_{J/#psip}} [GeV/#it{c}^{2}]");
  h251->SetYTitle("Events/ (6 MeV)");
  TCanvas *c51 = draw(h251,h151);  
  c51->cd(1);
   TLegend *leg51 = new TLegend(0.6,0.28,1.02,0.88,NULL,"brNDC");
  for(int i=0; i<NameList.size(); ++i) {
    TH1F *h351 = new TH1F("h351","",200,4.,5.2);
    h351->Sumw2();
    toytree->Project("h351","mjpsip",TString(NameList[i])+TString(cut3));
    h351->Scale(scale);
    h351->SetTitle(NameList[i]);   
//  if(h351->Integral()>1e-5)   setStyle(*h351,NameList[i],*leg51);
//    h351->DrawCopy("Same",NameList[i]);    
  }
  h251->Draw("hsame");
  h151->Draw("same");   


  lhcbLatex->DrawLatex(0.5,0.85, TString::Format("m(Kp) in (%4.2f, %4.2f) GeV",x1,x2).Data());  
  lhcbLatex->DrawLatex(0.2,0.85, "LHCb unofficial"); 
  c51->SaveAs(TString::Format("eps/mjpsip%i-%s.pdf",ibin,name).Data());  

  
}


void plotbin(char* name)
{
   gROOT->ProcessLine(".x ~/lhcbStyle.C");
  lhcbStyle->SetHistMinimumZero(1);

  //gSystem->Load("RooIpatia2_cxx.so");
  plotbineach(1.4,2.6,0,name);
#if 1
    int i;
//  double x[]= {1.4,1.55,1.6,1.8,2.0,2.6};
  double x[]= {1.4,1.55,1.9,2.6};

  for(i=1; i<=3; ++i) {
    plotbineach(x[i-1],x[i],i,name);
  }  
  plotbineach(1.55,2.6,6,name);
//  plotbineach(1.9,2.26,8,name);  
//  plotbineach(2.26,2.6,9,name);
#endif
}
