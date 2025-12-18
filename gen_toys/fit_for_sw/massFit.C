void massFit(TString signal_nTuple, TString background_nTuple, TString output_path, TString plot_path){
  using namespace RooFit;

  const Double_t MassMin = 5480;
  const Double_t MassMax = 5760;

  //====================================================================
  // Input root files, Use TChain, easy to merge files, like MagUp and MagDown  
  //====================================================================
  TChain chain("DTree", "");
  chain.AddFile(signal_nTuple);
  chain.AddFile(background_nTuple);

  //====================================================================
  // RooDataSet  
  //====================================================================
  // Note, Var name should be the same as the branch name in TTree 
  RooRealVar m("mass", "M(#Lambda_{b}^{0})", MassMin, MassMax, "MeV/#it{c}^{2}");
  RooDataSet Data("Data", "", &chain, RooArgSet(m));

  //=================
  // Define regions 
  //=================
  RooRealVar SigM("M_Lb", "", 5.61799e+03, 5300, 6000,"MeV/#it{c}^{2}");
  RooRealVar SigMRes("sigma1", "", 0.56143e+01, 2, 100, "MeV/#it{c}^{2}");
  RooRealVar SigMRes2("sigma2", "", 0.5e+01, 1, 100, "MeV/#it{c}^{2}");
  
  RooRealVar n("n", "", 2.67492e+00);

  RooRealVar a1("a1", "", 1.45314e+00, -10, 10);
  RooRealVar a2("a2", "", -1.60845e+00, -10, 10);

  RooCBShape SigMPdf1("CB1", "", m, SigM, SigMRes, a1, n);
  RooCBShape SigMPdf2("CB2", "", m, SigM, SigMRes2, a2, n);
  
  RooRealVar CB_fr("CB_fr", "", 0.5, 0., 1.);
  RooAddPdf SigMPdf("SigMPdf", "SigMPdf", RooArgList(SigMPdf1,SigMPdf2), RooArgList(CB_fr));
  RooRealVar p0("p0","p0",-2.01551e-03,-0.5,0.5,"MeV/c^{2}");
//combination bkg
  RooExponential comb_BkgMPdf("comb_BkgMPdf", "combine BkgMPdf", m, p0);
  RooRealVar nSig("N_{Sig}", "nsig", Data.numEntries()*0.5, 0, Data.numEntries());
  RooRealVar nBkg_comb("N_{Bkg_comb}", "nbkg combination", Data.numEntries()*0.5, 0, Data.numEntries());

  RooAddPdf MassPdf("MassPdf", "mass_pdf", RooArgList(SigMPdf,comb_BkgMPdf), RooArgList(nSig,nBkg_comb));
  RooArgSet * fitresult = MassPdf.getParameters(Data);

  fitresult->readFromFile("assign_Lb_mass/get_parameter/fitresult.xml");
  
  MassPdf.fitTo(Data, Strategy(1));

  RooDataSet t_data(Data.GetName(), Data.GetTitle(), &Data, *Data.get());
  RooStats::SPlot sdata("sdata", "An SPlot", t_data, &MassPdf, RooArgList(nSig,nBkg_comb));

  TFile outFile(output_path, "RECREATE");
  TTree * outTree = chain.CloneTree();

  Float_t Sweight_sig, Sweight_bkg;
  TBranch* b_Sweight_sig = outTree->Branch("nsig_sw",&Sweight_sig,"nsig_sw/F");
  TBranch* b_Sweight_bkg = outTree->Branch("nbkg_sw",&Sweight_bkg,"sbkg_sw/F");

  t_data.Print();
  int evtnum = t_data.numEntries(); 
  cout<< "The total number of events: "<< evtnum << endl;
  RooRealVar* sweight_sig = static_cast<RooRealVar*>(t_data.get()->find("N_{Sig}_sw"));
  RooRealVar* sweight_bkg = static_cast<RooRealVar*>(t_data.get()->find("N_{Bkg_comb}_sw"));
  for(int i=0;i<evtnum;i++)
  {
    t_data.get(i);
    outTree->GetEntry(i);
    Sweight_sig = sweight_sig->getVal();
    Sweight_bkg = sweight_bkg->getVal();
    b_Sweight_sig->Fill();
    b_Sweight_bkg->Fill();
  }
  outTree->Write();
  outFile.Close();
  
  TCanvas canvas("canvas", "", 800, 600);
  RooPlot *mframe = m.frame(Bins(40));
  Data.plotOn(mframe, Name("myData"));
  MassPdf.plotOn(mframe, Name("mySig"),  Components(SigMPdf), LineColor(kRed) );
  MassPdf.plotOn(mframe, Name("myBkg_comb"), Components(comb_BkgMPdf), LineStyle(kDashed) );
  MassPdf.plotOn(mframe, Name("myTot") );
  Data.plotOn(mframe, Name("myData"));
  mframe->Draw();
  canvas.SaveAs(plot_path);
}
