void gentoy_formhj(int seed, const char * output_path)
{
        int ntoys = 1000000;
        //int ntoys = 10000;
        TRandom r;
        r.SetSeed(seed);
        gRandom = new TRandom3(seed);
        //r.SetSeed(1);

        Double_t masses_Lb2JpsipK[3] = { 3.09691, 0.938272, 0.493677} ;
        Double_t masses_Jpsi2mumu[2] = { 0.105658, 0.105658 } ;

        //Define the generator for Lb2JpsipK decay. 
        TGenPhaseSpace Lb2JpsipK;
        TLorentzVector LbVector4_rest(0, 0, 0, 5.6196);
        Lb2JpsipK.SetDecay(LbVector4_rest, 3, masses_Lb2JpsipK);
        //Define the generator for Jpsi2mumu decay.
        TGenPhaseSpace Jpsi2mumu;
        TLorentzVector JpsiVector4_rest(0,0,0,3.09691);//Used to define 
        Jpsi2mumu.SetDecay(JpsiVector4_rest, 2, masses_Jpsi2mumu);

//        TVector3 LbVector3(0,0,1);//For boosting to lab frame
//        //generate a random booster. Shared by all events.
//        double Lbgamma = r.Uniform(0.7, 0.9);
//        double LbTheta = r.Uniform(0, TMath::Pi());
//        double LbPhi = r.Uniform(-TMath::Pi(), TMath::Pi());
//        LbVector3.SetMag(Lbgamma);
//        LbVector3.SetTheta(LbTheta);
//        LbVector3.SetPhi(LbPhi);
//        TLorentzVector LbVector4(0, 0, 0, 5.6196);
//        LbVector4.Boost(LbVector3);
        //Define a output tuple
//        for(int ifile = 0; ifile < nfiles; ifile ++){
                TFile f(output_path, "RECREATE");
                TTree* mytree = new TTree("tree", "");
                double Lb_PX, Lb_PY, Lb_PZ, Lb_PE;
                double K_PX, K_PY, K_PZ, K_PE;
                double P_PX, P_PY, P_PZ, P_PE;
                double Jpsi_PX, Jpsi_PY, Jpsi_PZ, Jpsi_PE;
                double mup_PX, mup_PY, mup_PZ, mup_PE;
                double mum_PX, mum_PY, mum_PZ, mum_PE;
                double weight_Lb2JpsipK, weight_Jpsi2mumu, weight;
                mytree->Branch("Lb_PX", &Lb_PX, "Lb_PX/D");
                mytree->Branch("Lb_PY", &Lb_PY, "Lb_PY/D");
                mytree->Branch("Lb_PZ", &Lb_PZ, "Lb_PZ/D");
                mytree->Branch("Lb_PE", &Lb_PE, "Lb_PE/D");
                mytree->Branch("K_PX", &K_PX, "K_PX/D");
                mytree->Branch("K_PY", &K_PY, "K_PY/D");
                mytree->Branch("K_PZ", &K_PZ, "K_PZ/D");
                mytree->Branch("K_PE", &K_PE, "K_PE/D");
                mytree->Branch("P_PX", &P_PX, "P_PX/D");
                mytree->Branch("P_PY", &P_PY, "P_PY/D");
                mytree->Branch("P_PZ", &P_PZ, "P_PZ/D");
                mytree->Branch("P_PE", &P_PE, "P_PE/D");
                mytree->Branch("Jpsi_PX", &Jpsi_PX, "Jpsi_PX/D");
                mytree->Branch("Jpsi_PY", &Jpsi_PY, "Jpsi_PY/D");
                mytree->Branch("Jpsi_PZ", &Jpsi_PZ, "Jpsi_PZ/D");
                mytree->Branch("Jpsi_PE", &Jpsi_PE, "Jpsi_PE/D");
                mytree->Branch("mup_PX", &mup_PX, "mup_PX/D");
                mytree->Branch("mup_PY", &mup_PY, "mup_PY/D");
                mytree->Branch("mup_PZ", &mup_PZ, "mup_PZ/D");
                mytree->Branch("mup_PE", &mup_PE, "mup_PE/D");
                mytree->Branch("mum_PX", &mum_PX, "mum_PX/D");
                mytree->Branch("mum_PY", &mum_PY, "mum_PY/D");
                mytree->Branch("mum_PZ", &mum_PZ, "mum_PZ/D");
                mytree->Branch("mum_PE", &mum_PE, "mum_PE/D");
                mytree->Branch("weight_Jpsi2mumu", &weight_Jpsi2mumu, "weight_Jpsi2mumu/D");
                mytree->Branch("weight_Lb2JpsipK", &weight_Lb2JpsipK, "weight_Lb2JpsipK/D");
                mytree->Branch("weight", &weight, "weight/D");
                
//                Double_t masses_Lb2JpsipK[3] = { 3.09691, 0.938272, 0.493677} ;
//                Double_t masses_Jpsi2mumu[2] = { 0.105658, 0.105658 } ;
//
//                //Define the generator for Lb2JpsipK decay. 
//                TGenPhaseSpace Lb2JpsipK;
//                TLorentzVector LbVector4_rest(0, 0, 0, 5.6196);
//                Lb2JpsipK.SetDecay(LbVector4_rest, 3, masses_Lb2JpsipK);
//                //Define the generator for Jpsi2mumu decay.
//                TGenPhaseSpace Jpsi2mumu;
//                TLorentzVector JpsiVector4_rest(0,0,0,3.09691);//Used to define 
//                Jpsi2mumu.SetDecay(JpsiVector4_rest, 2, masses_Jpsi2mumu);


                for(int ientry=0; ientry<ntoys; ientry++){
                        if(ientry%100000==0) cout << ientry << "/" << ntoys << endl;
                        //Lb2JpsipK
                        weight_Lb2JpsipK = Lb2JpsipK.Generate();
                        TLorentzVector *JpsiVector4 = Lb2JpsipK.GetDecay(0);
                        TLorentzVector *PVector4 = Lb2JpsipK.GetDecay(1);
                        TLorentzVector *KVector4 = Lb2JpsipK.GetDecay(2);
                        //Jpsi2mumu
                        weight_Jpsi2mumu = Jpsi2mumu.Generate();
                        TLorentzVector *mupVector4 = Jpsi2mumu.GetDecay(0);
                        TLorentzVector *mumVector4 = Jpsi2mumu.GetDecay(1);
                        weight = weight_Lb2JpsipK * weight_Jpsi2mumu;
                        //Boost mup mum to Lb rest frame
                        TVector3 JpsiBoostVector = JpsiVector4->BoostVector();
                        mupVector4->Boost(JpsiBoostVector);
                        mumVector4->Boost(JpsiBoostVector);
                        //Boost everything to lab frame
                        TVector3 LbVector3(0,0,1);//For boosting to lab frame
                        //generate a random booster. Shared by all events.
                        double Lbgamma = r.Uniform(0.7, 0.9);
                        double LbTheta = r.Uniform(0, TMath::Pi());
                        double LbPhi = r.Uniform(-TMath::Pi(), TMath::Pi());
                        LbVector3.SetMag(Lbgamma);
                        LbVector3.SetTheta(LbTheta);
                        LbVector3.SetPhi(LbPhi);
                        TLorentzVector LbVector4(0, 0, 0, 5.6196);
                        LbVector4.Boost(LbVector3);
                        JpsiVector4->Boost(LbVector3);
                        PVector4->Boost(LbVector3);
                        KVector4->Boost(LbVector3);
                        mupVector4->Boost(LbVector3);
                        mumVector4->Boost(LbVector3);
                        //record into tree
                        Lb_PX = LbVector4.X();
                        Lb_PY = LbVector4.Y();
                        Lb_PZ = LbVector4.Z();
                        Lb_PE = LbVector4.E();
                        Jpsi_PX = JpsiVector4->X();
                        Jpsi_PY = JpsiVector4->Y();
                        Jpsi_PZ = JpsiVector4->Z();
                        Jpsi_PE = JpsiVector4->E();
                        P_PX = PVector4->X();
                        P_PY = PVector4->Y();
                        P_PZ = PVector4->Z();
                        P_PE = PVector4->E();
                        K_PX = KVector4->X();
                        K_PY = KVector4->Y();
                        K_PZ = KVector4->Z();
                        K_PE = KVector4->E();
                        mum_PX = mumVector4->X();
                        mum_PY = mumVector4->Y();
                        mum_PZ = mumVector4->Z();
                        mum_PE = mumVector4->E();
                        mup_PX = mupVector4->X();
                        mup_PY = mupVector4->Y();
                        mup_PZ = mupVector4->Z();
                        mup_PE = mupVector4->E();
                        mytree->Fill();
                }

                mytree->Write();
                f.Close();
 //       }

}
