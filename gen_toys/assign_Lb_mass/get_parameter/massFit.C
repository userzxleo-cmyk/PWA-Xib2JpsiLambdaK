void massFit(){


  using namespace RooFit;
 
  RooRandom::randomGenerator()->SetSeed(1);

  // Define range
  const Double_t MassMin = 5480 ;
  const Double_t MassMax = 5760 ;
  TCut LbMassWindow = "Lb_LOKI_MASS_JpsiConstr  > 5480 && Lb_LOKI_MASS_JpsiConstr < 5760";

  //...mass resolution
  const Double_t MsigmaAve = 10. ;
  const Double_t MsigmaMin =  1. ;
  const Double_t MsigmaMax = 30. ;

  //====================================================================
  // Input root files, Use TChain, easy to merge files, like MagUp and MagDown  
  //====================================================================
  TChain *chain = new TChain("DTree");
  chain->Add("/nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/for_Pc4312JP/Fit/sPlot.root");

  //====================================================================
  // Apply cuts first 

  TCut totcut =  LbMassWindow;
        TTree* selTree = (TTree*)chain->CopyTree(totcut);
//TTree* selTree = (TTree*)chain->CopyTree("Lbcon_etac_MM_exchange<3096-5||Lbcon_etac_MM_exchange>3096+5");


  cout << "[INFO] Total candidates after selection: " << selTree->GetEntries() << endl;


  //====================================================================
  // RooDataSet  
  //====================================================================
  // Note, Var name should be the same as the branch name in TTree 
  RooRealVar *m = new RooRealVar("Lb_LOKI_MASS_JpsiConstr","M(#Lambda_{b}^{0})", MassMin,MassMax, "MeV/#it{c}^{2}") ;
  RooDataSet *Data = new RooDataSet("Data", "", selTree, RooArgSet(*m) ) ;
  Data->Print();


  //=================
  // Define regions 
  //=================


  RooRealVar *SigM= new RooRealVar("M_Lb","",5.61799e+03, 5300, 6000,"MeV/#it{c}^{2}");
  RooRealVar *SigMRes  = new RooRealVar("sigma1",    "", 0.56143e+01, 2, 100, "MeV/#it{c}^{2}");
  RooRealVar *SigMRes2 = new RooRealVar("sigma2",  "",  0.5e+01, 1, 100, "MeV/#it{c}^{2}");
  
  RooRealVar n("n","",2.67492e+00);

  RooRealVar a1("a1","",1.45314e+00, -10, 10);
  RooRealVar a2("a2","",-1.60845e+00, -10, 10);


  RooCBShape SigMPdf1("CB1","",*m,*SigM,*SigMRes ,a1, n);
  RooCBShape SigMPdf2("CB2","",*m,*SigM,*SigMRes2,a2, n);
  
  RooRealVar CB_fr("CB_fr","",0.5, 0., 1.);
  RooAddPdf *SigMPdf = new RooAddPdf("SigMPdf","SigMPdf", RooArgList(SigMPdf1,SigMPdf2),RooArgList(CB_fr));
  RooRealVar *p0=new RooRealVar("p0","p0",-2.01551e-03,-0.5,0.5,"MeV/c^{2}");
//combination bkg
  RooAbsPdf *comb_BkgMPdf=new RooExponential("comb_BkgMPdf","combine BkgMPdf",*m,*p0);
  RooRealVar *nSig = new RooRealVar("N_{Sig}","nsig",Data->numEntries()*0.5,0,Data->numEntries());
  RooRealVar *nBkg_comb = new RooRealVar("N_{Bkg_comb}","nbkg combination",Data->numEntries()*0.5,0,Data->numEntries());
  
  RooAbsPdf *MassPdf=new RooAddPdf("MassPdf","mass_pdf",RooArgList(*SigMPdf,*comb_BkgMPdf),RooArgList(*nSig,*nBkg_comb));
  RooArgSet* fitresult = MassPdf->getParameters(*Data);

  fitresult->readFromFile("fitresult.xml");
  
  RooFitResult* fitres = MassPdf->fitTo(*Data,Minos(1),NumCPU(50));



  fitresult->writeToFile("fitresult.xml");

  //generate toys
//  RooDataSet* signal = (RooDataSet*)SigMPdf->generate(RooArgSet(*m, NumEvents(10)))
//create a new RooDataSet with sweight
#if 0
   RooDataSet *t_data = new RooDataSet(Data->GetName(),Data->GetTitle(),Data,*Data->get());
   RooStats::SPlot *sdata = new RooStats::SPlot("sdata","An SPlot",*t_data,MassPdf,RooArgList(*nSig,*nBkg_comb));
   TString outputdata;
   ifstream outputdatalist("outputdata.xml");
   outputdatalist>>outputdata;
   outputdatalist.close();
   cout << outputdata << endl;
   TFile f(outputdata,"recreate");
   Float_t Sweight_sig, Sweight_bkg;
   TBranch* b_Sweight_sig = selTree->Branch("Sweight_sig",&Sweight_sig,"Sweight_sig/F");
   TBranch* b_Sweight_bkg = selTree->Branch("Sweight_bkg",&Sweight_bkg,"Sweight_bkg/F");


   t_data->Print();
   evtnum = t_data->numEntries(); 
   cout<< "The total number of events: "<< evtnum << endl;
   RooRealVar* sweight_sig = (RooRealVar*)t_data->get()->find("N_{Sig}_sw");
   RooRealVar* sweight_bkg = (RooRealVar*)t_data->get()->find("N_{Bkg_comb}_sw");
   for(i=0;i<evtnum;i++)
     {
       t_data->get(i);
       selTree->GetEntry(i);


       Sweight_sig = sweight_sig->getVal();
       Sweight_bkg = sweight_bkg->getVal();
       b_Sweight_sig->Fill();
       b_Sweight_bkg->Fill();

     }
   selTree->Write();
   f.Close();
#endif
  
  TCanvas* MyCan = new TCanvas("MyCan","",680,600);
  RooPlot *mframe = m->frame(Bins(40));
  Data->plotOn(mframe, Name("myData"));
  MassPdf->plotOn(mframe, Name("mySig"),  Components(*SigMPdf), LineColor(kRed) );
//  MassPdf->plotOn(mframe, Name("myBkg_comb"), Components(*comb_BkgMPdf), LineColor(kDark), LineStyle(kDashed) );
  MassPdf->plotOn(mframe, Name("myBkg_comb"), Components(*comb_BkgMPdf), LineStyle(kDashed) );
  MassPdf->plotOn(mframe, Name("myTot") );
  Data->plotOn(mframe, Name("myData"));
  mframe->Draw();
  TString fileName = "plots/Lb_data";
  TString epsName = fileName + ".eps";
  TString pngName = fileName + ".png";
  TString pdfName = fileName + ".pdf";
  TString CName = fileName + ".C";
  MyCan->Print( epsName );
  MyCan->Print( pngName );
  MyCan->Print( pdfName );
  MyCan->Print( CName );


  TCanvas* MyCan_log = new TCanvas("MyCan_log","",680,600);
  gPad->SetLogy(); 
  mframe->Draw();
  TString fileName_log = "plots/Lb_data_log";
  TString epsName_log = fileName_log + ".eps";
  TString pngName_log = fileName_log + ".png";
  TString pdfName_log = fileName_log + ".pdf";
  TString CName_log = fileName_log + ".C";
  MyCan_log->Print( epsName_log );
  MyCan_log->Print( pngName_log );
  MyCan_log->Print( pdfName_log );
  MyCan_log->Print( CName_log );

}
