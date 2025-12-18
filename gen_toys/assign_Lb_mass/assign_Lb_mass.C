void assign_Lb_mass( TString component, uint64_t seed, TString input_path, TString output_path ){
        if (component != "sig" && component != "bkg") {
                std::cout << "first config must be sig or bkg" << std::endl;
                return; 
        }
        const Double_t MassMin = 5480 ;
        const Double_t MassMax = 5760 ;
        TRandom r;
        r.SetSeed(seed);
        std::cout << seed << std::endl;
        TFile f(output_path.Data(), "RECREATE");
        TChain* chain = new TChain("DTree");
        chain->Add(input_path.Data());
        TTree* tree = (TTree*)chain->CloneTree(0);
        double Lb_LOKI_MASS_JpsiConstr;
        double Lb_mass_weight;
        TBranch* b_Lb_LOKI_MASS_JpsiConstr = tree->Branch("mass", &Lb_LOKI_MASS_JpsiConstr, "mass/D");
        TBranch* b_Lb_mass_weight = tree->Branch("Lb_mass_weight", &Lb_mass_weight, "Lb_mass_weight/D");

        //define Roofit functions for Lb mass weight
        RooRealVar *m = new RooRealVar("Lb_LOKI_MASS_JpsiConstr","M(#Lambda_{b}^{0})", MassMin,MassMax, "MeV/#it{c}^{2}") ;
        RooDataSet *empty = new RooDataSet("empty","",RooArgSet(*m));
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
        RooAbsPdf *comb_BkgMPdf=new RooExponential("comb_BkgMPdf","combine BkgMPdf",*m,*p0);
        RooRealVar *nSig = new RooRealVar("N_{Sig}","nsig",10);
        RooRealVar *nBkg_comb = new RooRealVar("N_{Bkg_comb}","nbkg combination",10);
        RooAbsPdf *MassPdf=new RooAddPdf("MassPdf","mass_pdf",RooArgList(*SigMPdf,*comb_BkgMPdf),RooArgList(*nSig,*nBkg_comb));
        RooArgSet* fitresult = MassPdf->getParameters(*empty);
        fitresult->readFromFile("assign_Lb_mass/get_parameter/fitresult.xml");

        TH1F* hist = new TH1F("hist","",5000, MassMin, MassMax);
        for (int i=1; i<5001; i++){
                m->setVal(hist->GetBinCenter(i));
                if(component=="sig"){
                        hist->SetBinContent(i, SigMPdf->getVal(RooArgSet(*m)));
                }
                else{
                        hist->SetBinContent(i, comb_BkgMPdf->getVal(RooArgSet(*m)));
                }
        }
        double max_weight = hist->GetMaximum(); 


        int nentries = chain->GetEntries();
        for (int i=0; i<nentries; i++){
                chain->GetEntry(i);
                Lb_LOKI_MASS_JpsiConstr = 0;
                while(true){
                        Lb_LOKI_MASS_JpsiConstr = r.Uniform(MassMin, MassMax);
                        //Hit and reject
                        double hit_reject =  r.Uniform(0, max_weight);
                        if ( hit_reject < hist->GetBinContent(hist->FindBin(Lb_LOKI_MASS_JpsiConstr)) ){
                                break;        
                        }
                }
                m->setVal(Lb_LOKI_MASS_JpsiConstr);
                if(component=="sig"){
                        Lb_mass_weight = SigMPdf->getVal(RooArgSet(*m));
                }
                else{
                        Lb_mass_weight = comb_BkgMPdf->getVal(RooArgSet(*m));
                }
                tree->Fill();
        }
        tree->Write();
        f.Close();
         
}
