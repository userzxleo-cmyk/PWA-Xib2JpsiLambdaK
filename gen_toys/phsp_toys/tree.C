#define tree_cxx
#include "TMVA/Tools.h"
#include "TMVA/Reader.h"
#include "tree.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TRandom.h"
#include "TLorentzVector.h"

void tree::Loop(const char * outfile_path)
{
//   In a ROOT session, you can do:
//      Root > .L tree.C
//      Root > tree t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   TFile f2(outfile_path, "recreate");
   TTree *h1 = new TTree("tree","Lb->J/psi p K");
   h1->Branch("mkp", &mkp, "mkp/D");
   h1->Branch("mjpsip", &mjpsip, "mjpsip/D");
   h1->Branch("mjpsik", &mjpsik, "mjpsik/D");
   h1->Branch("Lb_ID", &Lb_ID, "Lb_ID/I");
   h1->Branch("H1_PX_Fit", &H1_PX_Fit, "H1_PX_Fit/D");
   h1->Branch("H2_PX_Fit", &H2_PX_Fit, "H2_PX_Fit/D");
   h1->Branch("H1_PY_Fit", &H1_PY_Fit, "H1_PY_Fit/D");
   h1->Branch("H2_PY_Fit", &H2_PY_Fit, "H2_PY_Fit/D");
   h1->Branch("H1_PZ_Fit", &H1_PZ_Fit, "H1_PZ_Fit/D");
   h1->Branch("H2_PZ_Fit", &H2_PZ_Fit, "H2_PZ_Fit/D");
   h1->Branch("H1_PE_Fit", &H1_PE_Fit, "H1_PE_Fit/D");
   h1->Branch("H2_PE_Fit", &H2_PE_Fit, "H2_PE_Fit/D");
   h1->Branch("muplus_PX_Fit", &muplus_PX_Fit, "muplus_PX_Fit/D");
   h1->Branch("muminus_PX_Fit", &muminus_PX_Fit, "muminus_PX_Fit/D");
   h1->Branch("muplus_PY_Fit", &muplus_PY_Fit, "muplus_PY_Fit/D");
   h1->Branch("muminus_PY_Fit", &muminus_PY_Fit, "muminus_PY_Fit/D");
   h1->Branch("muplus_PZ_Fit", &muplus_PZ_Fit, "muplus_PZ_Fit/D");
   h1->Branch("muminus_PZ_Fit", &muminus_PZ_Fit, "muminus_PZ_Fit/D");
   h1->Branch("muplus_PE_Fit", &muplus_PE_Fit, "muplus_PE_Fit/D");
   h1->Branch("muminus_PE_Fit", &muminus_PE_Fit, "muminus_PE_Fit/D");
   h1->Branch("cosTheta_Lb", &costheta, "cosTheta_Lb/D");
   h1->Branch("cosTheta_L", &costheta1, "cosTheta_L/D");
   h1->Branch("cosTheta_Jpsi", &costheta2, "cosTheta_Jpsi/D");
   h1->Branch("phiK", &phi1, "phiK/D");
   h1->Branch("phiMu", &phi2, "phiMu/D");
   h1->Branch("Z_cosTheta_Lb", &costheta_Z, "Z_cosTheta_Lb/D");
   h1->Branch("Z_cosTheta_Z", &costheta1_Z, "Z_cosTheta_Z/D");
   h1->Branch("Z_cosTheta_Jpsi", &costheta2_Z, "Z_cosTheta_Jpsi/D");
   h1->Branch("Z_phiZ", &phi_Z, "Z_phiZ/D");
   h1->Branch("Z_phiJpsi", &phi1_Z, "Z_phiJpsi/D");
   h1->Branch("Z_phiMu", &phi2_Z, "Z_phiMu/D");
   h1->Branch("cosTheta_p", &costheta_p, "cosTheta_p/D");
   h1->Branch("alpha_p", &alpha_p, "alpha_p/D");
   h1->Branch("alpha_Mu", &alpha_mu, "alpha_Mu/D");
   h1->Branch("sw", &sw, "sw/D");

   Long64_t nentries = fChain->GetEntries();
   cout << "nentries = " << nentries << endl;
   Long64_t nbytes = 0, nb = 0;
   for ( size_t jentry = 0; jentry < nentries; jentry++ ) {
      if(jentry%100000==0) cout << jentry << "/" << nentries << endl;
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      muplus_PX_Fit = mup_PX;
      muplus_PY_Fit = mup_PY;
      muplus_PZ_Fit = mup_PZ;
      muplus_PE_Fit = mup_PE;
      muminus_PX_Fit= mum_PX;
      muminus_PY_Fit= mum_PY;
      muminus_PZ_Fit= mum_PZ;
      muminus_PE_Fit= mum_PE;
      H1_PX_Fit = P_PX;
      H1_PY_Fit = P_PY;
      H1_PZ_Fit = P_PZ;
      H1_PE_Fit = P_PE;
      H2_PX_Fit = K_PX;
      H2_PY_Fit = K_PY;
      H2_PZ_Fit = K_PZ;
      H2_PE_Fit = K_PE;
      sw = weight;
      helicityJpsiLam();
      helicityZK();
      helicityTwoFrame();
      mkp = sqrt((H1_PE_Fit+H2_PE_Fit)*(H1_PE_Fit+H2_PE_Fit)-(H1_PX_Fit+H2_PX_Fit)*(H1_PX_Fit+H2_PX_Fit)-(H1_PY_Fit+H2_PY_Fit)*(H1_PY_Fit+H2_PY_Fit)-(H1_PZ_Fit+H2_PZ_Fit)*(H1_PZ_Fit+H2_PZ_Fit));
      // if(mkp<1.6) continue;
      mjpsip = sqrt((H1_PE_Fit+muplus_PE_Fit+muminus_PE_Fit)*(H1_PE_Fit+muplus_PE_Fit+muminus_PE_Fit)-(H1_PX_Fit+muplus_PX_Fit+muminus_PX_Fit)*(H1_PX_Fit+muplus_PX_Fit+muminus_PX_Fit)-(H1_PY_Fit+muplus_PY_Fit+muminus_PY_Fit)*(H1_PY_Fit+muplus_PY_Fit+muminus_PY_Fit)-(H1_PZ_Fit+muplus_PZ_Fit+muminus_PZ_Fit)*(H1_PZ_Fit+muplus_PZ_Fit+muminus_PZ_Fit));
      mjpsik = sqrt((H2_PE_Fit+muplus_PE_Fit+muminus_PE_Fit)*(H2_PE_Fit+muplus_PE_Fit+muminus_PE_Fit)-(H2_PX_Fit+muplus_PX_Fit+muminus_PX_Fit)*(H2_PX_Fit+muplus_PX_Fit+muminus_PX_Fit)-(H2_PY_Fit+muplus_PY_Fit+muminus_PY_Fit)*(H2_PY_Fit+muplus_PY_Fit+muminus_PY_Fit)-(H2_PZ_Fit+muplus_PZ_Fit+muminus_PZ_Fit)*(H2_PZ_Fit+muplus_PZ_Fit+muminus_PZ_Fit));

      Lb_ID = 5122;
      int ID = (Lb_ID)/abs(Lb_ID); 
      phi1 *= ID;
      phi2 *= ID;
      phi_Z *= ID;
      phi1_Z *= ID;
      phi2_Z *= ID;
      alpha_mu *= ID;
      alpha_p *= ID;
      h1->Fill();
   }
   h1->Write();
   f2.Close();
}


////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////
void tree::helicityJpsiLam()
{
  Double_t muplusPE = muplus_PE_Fit;
  Double_t muplusPX = muplus_PX_Fit;
  Double_t muplusPY = muplus_PY_Fit;
  Double_t muplusPZ = muplus_PZ_Fit;
  Double_t muminusPE = muminus_PE_Fit;
  Double_t muminusPX = muminus_PX_Fit;
  Double_t muminusPY = muminus_PY_Fit;
  Double_t muminusPZ = muminus_PZ_Fit;
  Double_t protonPE = H1_PE_Fit;
  Double_t protonPX = H1_PX_Fit;
  Double_t protonPY = H1_PY_Fit;
  Double_t protonPZ = H1_PZ_Fit;
  Double_t kaonPE = H2_PE_Fit;
  Double_t kaonPX = H2_PX_Fit;
  Double_t kaonPY = H2_PY_Fit;
  Double_t kaonPZ = H2_PZ_Fit;
  int ID = Lb_ID;
  TLorentzVector *Pmu1 = new TLorentzVector( muplusPX, muplusPY, muplusPZ, muplusPE);
  TLorentzVector *Pmu2 = new TLorentzVector( muminusPX, muminusPY, muminusPZ, muminusPE);
  //  if(ID<0) {
  //    Pmu1 = new TLorentzVector( muminusPX, muminusPY, muminusPZ, muminusPE);
  //    Pmu2 = new TLorentzVector( muplusPX, muplusPY, muplusPZ, muplusPE);
  //  }
  TLorentzVector *Pproton = new TLorentzVector( protonPX, protonPY, protonPZ, protonPE);
  TLorentzVector *Pkaon = new TLorentzVector( kaonPX, kaonPY, kaonPZ, kaonPE);
  TLorentzVector PJpsi = *Pmu1 + *Pmu2;
  TLorentzVector PLam = *Pproton+ *Pkaon;
  TLorentzVector PB = PJpsi+PLam;
  
//   Dalitz_DTF_Kminus_pplus_M2 = PLam.Mag2();
//   Dalitz_DTF_pplus_J_psi_1S_M2 = (PJpsi + *Pproton).Mag2();
//   Dalitz_DTF_Kminus_J_psi_1S_M2 = (PJpsi + *Pkaon).Mag2();
  
  TVector3 PBunit = PB.Vect().Unit();
  TVector3 Pbeam(0.0,0.0,1.0);
  //  TVector3  nvec = PBunit.Cross(TVector3(0,0,1));
  
  TVector3 boosttoparent = -PB.BoostVector();
  
  //  std::cout << "before " <<std::endl;
  //  PLam.Print();
  Pmu1->Boost(boosttoparent);
  Pmu2->Boost(boosttoparent);
  Pproton->Boost(boosttoparent);
  Pkaon->Boost(boosttoparent);
  PJpsi.Boost(boosttoparent);
  PLam.Boost(boosttoparent);
  
  TVector3 LUnit = PLam.Vect().Unit();
  TVector3 psiUnit = PJpsi.Vect().Unit();

  costheta = PBunit.Dot(LUnit);
  phi = 0;
  //Lam rest frame
  boosttoparent = -PLam.BoostVector();
  Pproton->Boost(boosttoparent);

  TVector3 Unit = Pproton->Vect().Unit();
  costheta1 = -(psiUnit).Dot(Unit);
  
  TVector3 aboost = -(PB.Vect() - ((PB.Vect()).Dot(LUnit))*LUnit).Unit();
  double cosphi = aboost.Dot(Unit);
  double sinphi = (((-psiUnit).Cross(aboost)).Unit()).Dot(Unit);
  phi1 = atan2(sinphi,cosphi);
  
  //Jpsi rest frame
  //different from the paper, use y = y, x => -x, z => -z for J/psi frame vs L* frame 
  aboost = -aboost;
  boosttoparent = -PJpsi.BoostVector();
  Pmu2->Boost(boosttoparent);
  Unit = Pmu2->Vect().Unit();
  costheta2 = (psiUnit).Dot(Unit);
  
  cosphi = aboost.Dot(Unit);
  sinphi = (((psiUnit).Cross(aboost)).Unit()).Dot(Unit);
  phi2 = atan2(sinphi,cosphi);
    
}

void tree::helicityZK()
{
  Double_t muplusPE = muplus_PE_Fit;
  Double_t muplusPX = muplus_PX_Fit;
  Double_t muplusPY = muplus_PY_Fit;
  Double_t muplusPZ = muplus_PZ_Fit;
  Double_t muminusPE = muminus_PE_Fit;
  Double_t muminusPX = muminus_PX_Fit;
  Double_t muminusPY = muminus_PY_Fit;
  Double_t muminusPZ = muminus_PZ_Fit;
  Double_t protonPE = H1_PE_Fit;
  Double_t protonPX = H1_PX_Fit;
  Double_t protonPY = H1_PY_Fit;
  Double_t protonPZ = H1_PZ_Fit;
  Double_t kaonPE = H2_PE_Fit;
  Double_t kaonPX = H2_PX_Fit;
  Double_t kaonPY = H2_PY_Fit;
  Double_t kaonPZ = H2_PZ_Fit;
  int ID = Lb_ID;
  TLorentzVector *Pmu1 = new TLorentzVector( muplusPX, muplusPY, muplusPZ, muplusPE);
  TLorentzVector *Pmu2 = new TLorentzVector( muminusPX, muminusPY, muminusPZ, muminusPE);
/*  if(ID<0) {
      Pmu1 = new TLorentzVector( muminusPX, muminusPY, muminusPZ, muminusPE);
      Pmu2 = new TLorentzVector( muplusPX, muplusPY, muplusPZ, muplusPE);
  }*/
  TLorentzVector *Pproton = new TLorentzVector( protonPX, protonPY, protonPZ, protonPE);
  TLorentzVector *Pkaon = new TLorentzVector( kaonPX, kaonPY, kaonPZ, kaonPE);
  TLorentzVector PJpsi = *Pmu1 + *Pmu2;
  TLorentzVector PLam = *Pproton+ *Pkaon;
  TLorentzVector PB = PJpsi+PLam;
  TLorentzVector PZ = PJpsi + *Pproton;
  TVector3 PBunit = PB.Vect().Unit();
  TVector3 Pbeam(0.0,0.0,1.0);
  //  TVector3  nvec = PBunit.Cross(TVector3(0,0,1));
  
  TVector3 boosttoparent = -PB.BoostVector();
  
  //  std::cout << "before " <<std::endl;
  //  PLam.Print();
  Pmu1->Boost(boosttoparent);
  Pmu2->Boost(boosttoparent);
  Pproton->Boost(boosttoparent);
  Pkaon->Boost(boosttoparent);
  PJpsi.Boost(boosttoparent);
  PLam.Boost(boosttoparent);
  PZ.Boost(boosttoparent);
  
  TVector3 LUnit = PLam.Vect().Unit();
  TVector3 psiUnit = PJpsi.Vect().Unit();
  TVector3 ZUnit = PZ.Vect().Unit();
  
  costheta_Z = PBunit.Dot(ZUnit);
  TVector3 aboost = (PLam.Vect()-(PLam.Vect().Dot(PBunit))*PBunit).Unit();
  double cosphi = aboost.Dot(ZUnit);
  double sinphi = ((PBunit.Cross(aboost)).Unit()).Dot(ZUnit);
  phi_Z = atan2(sinphi,cosphi);
  
  //Z rest frame
  boosttoparent = -PZ.BoostVector();
  PJpsi.Boost(boosttoparent);
  Pmu1->Boost(boosttoparent);
  Pmu2->Boost(boosttoparent);
  Pproton->Boost(boosttoparent);
  Pkaon->Boost(boosttoparent);
  
  TVector3 Unit = PJpsi.Vect().Unit();
  costheta1_Z = -(Pkaon->Vect().Unit()).Dot(Unit);
  
  aboost = -(PB.Vect() - ((PB.Vect()).Dot(ZUnit))*ZUnit).Unit();
  cosphi = aboost.Dot(Unit);
  sinphi = (((-Pkaon->Vect().Unit()).Cross(aboost)).Unit()).Dot(Unit);
  phi1_Z = atan2(sinphi,cosphi);
  
  //Jpsi rest frame
  psiUnit = PJpsi.Vect().Unit();
  boosttoparent = -PJpsi.BoostVector();
  Pmu2->Boost(boosttoparent);
  Unit = Pmu2->Vect().Unit();
  costheta2_Z = (psiUnit).Dot(Unit);
  aboost = -(-Pkaon->Vect() + ((Pkaon->Vect()).Dot(psiUnit))*(psiUnit)).Unit();
  cosphi = aboost.Dot(Unit);
  sinphi = (((psiUnit).Cross(aboost)).Unit()).Dot(Unit);
  phi2_Z = atan2(sinphi,cosphi);
    
  
}


void tree::helicityTwoFrame()
{
  Double_t muplusPE = muplus_PE_Fit;
  Double_t muplusPX = muplus_PX_Fit;
  Double_t muplusPY = muplus_PY_Fit;
  Double_t muplusPZ = muplus_PZ_Fit;
  Double_t muminusPE = muminus_PE_Fit;
  Double_t muminusPX = muminus_PX_Fit;
  Double_t muminusPY = muminus_PY_Fit;
  Double_t muminusPZ = muminus_PZ_Fit;
  Double_t protonPE = H1_PE_Fit;
  Double_t protonPX = H1_PX_Fit;
  Double_t protonPY = H1_PY_Fit;
  Double_t protonPZ = H1_PZ_Fit;
  Double_t kaonPE = H2_PE_Fit;
  Double_t kaonPX = H2_PX_Fit;
  Double_t kaonPY = H2_PY_Fit;
  Double_t kaonPZ = H2_PZ_Fit;
  int ID = Lb_ID;
  TLorentzVector *Pmu1 = new TLorentzVector( muplusPX, muplusPY, muplusPZ, muplusPE);
  TLorentzVector *Pmu2 = new TLorentzVector( muminusPX, muminusPY, muminusPZ, muminusPE);
/*  if(ID<0) {
     Pmu1 = new TLorentzVector( muminusPX, muminusPY, muminusPZ, muminusPE);
     Pmu2 = new TLorentzVector( muplusPX, muplusPY, muplusPZ, muplusPE);
  }*/
  TLorentzVector *Pproton = new TLorentzVector( protonPX, protonPY, protonPZ, protonPE);
  TLorentzVector *Pkaon = new TLorentzVector( kaonPX, kaonPY, kaonPZ, kaonPE);
  TLorentzVector PJpsi = *Pmu1 + *Pmu2;
  TLorentzVector PLam = *Pproton+ *Pkaon;
  TLorentzVector PB = PJpsi+PLam;
  TLorentzVector PZ = PJpsi + *Pproton;
  TVector3 PBunit = PB.Vect().Unit();
  TVector3 Pbeam(0.0,0.0,1.0);
  //  TVector3  nvec = PBunit.Cross(TVector3(0,0,1));
  
  TVector3 boosttoparent = -PB.BoostVector();
  
  //  std::cout << "before " <<std::endl;
  //  PLam.Print();
  Pmu1->Boost(boosttoparent);
  Pmu2->Boost(boosttoparent);
  Pproton->Boost(boosttoparent);
  Pkaon->Boost(boosttoparent);
  PJpsi.Boost(boosttoparent);
  PLam.Boost(boosttoparent);
  PZ.Boost(boosttoparent);
  
   
  TLorentzVector PKaon_p = *Pkaon;
  TLorentzVector PJpsi_p = PJpsi;
  
  boosttoparent = -Pproton->BoostVector();
  PKaon_p.Boost(boosttoparent);
  PJpsi_p.Boost(boosttoparent);
  /*
   */ 
  costheta_p = (PKaon_p.Vect().Unit()).Dot(PJpsi_p.Vect().Unit());

   //alpha_p

  TLorentzVector PKaon_Z = *Pkaon;
  TLorentzVector PJpsi_Z = PJpsi;
  TLorentzVector PKaon_L = *Pkaon;
  TLorentzVector PJpsi_L = PJpsi;
  boosttoparent = -PZ.BoostVector();
  PKaon_Z.Boost(boosttoparent);
  PJpsi_Z.Boost(boosttoparent);  
  boosttoparent = -PLam.BoostVector();
  PKaon_L.Boost(boosttoparent);
  PJpsi_L.Boost(boosttoparent);  
  
  TVector3 x0_Z = -(-PKaon_Z.Vect()+(PKaon_Z.Vect().Dot(PJpsi_Z.Vect().Unit()))*(PJpsi_Z.Vect().Unit())).Unit();
  TVector3 x0_L = -(-PJpsi_L.Vect()+(PJpsi_L.Vect().Dot(PKaon_L.Vect().Unit()))*(PKaon_L.Vect().Unit())).Unit();  
  TVector3 z0_Z = -PJpsi_p.Vect().Unit();
  double sinphip = ((z0_Z.Cross(x0_Z)).Unit()).Dot(x0_L);
  double cosphip = x0_Z.Dot(x0_L);
  sinphip = 0;
  alpha_p =  atan2(sinphip, cosphip);
    
/*  
  TLorentzVector PKaon_Z = *Pkaon;
  TLorentzVector PJpsi_Z = PJpsi;
  TLorentzVector PKaon_L = *Pkaon;
  TLorentzVector PJpsi_L = PJpsi;
  boosttoparent = -PZ.BoostVector();
  PKaon_Z.Boost(boosttoparent);
  PJpsi_Z.Boost(boosttoparent);  
  boosttoparent = -PLam.BoostVector();
  PKaon_L.Boost(boosttoparent);
  PJpsi_L.Boost(boosttoparent);  
  
  TVector3 x0_Z = (-PKaon_Z.Vect()+(PKaon_Z.Vect().Dot(PJpsi_Z.Vect().Unit()))*(PJpsi_Z.Vect().Unit())).Unit();
  TVector3 x0_L = (-PJpsi_L.Vect()+(PJpsi_L.Vect().Dot(PKaon_L.Vect().Unit()))*(PKaon_L.Vect().Unit())).Unit();  
  TVector3 z0_Z = -PJpsi_p.Vect().Unit();
  std::cout << (z0_Z.Cross(x0_Z)).Unit().Dot(x0_L) << std::endl;
  */
  
  
  //Jpsi rest frame
  boosttoparent = -PJpsi.BoostVector();
  Pmu2->Boost(boosttoparent);
  Pproton->Boost(boosttoparent);
  PLam.Boost(boosttoparent);
  TVector3 z3 = Pmu2->Vect().Unit();
  TVector3 x3_Z = -(-Pproton->Vect()+(Pproton->Vect().Dot(Pmu2->Vect().Unit()))*Pmu2->Vect().Unit()).Unit();    
  TVector3 x3_L = -(-PLam.Vect()+(PLam.Vect().Dot(Pmu2->Vect().Unit()))*Pmu2->Vect().Unit()).Unit();      
  
  double sinphi = ((z3.Cross(x3_Z)).Unit()).Dot(x3_L);
  double cosphi = x3_Z.Dot(x3_L);
  
  alpha_mu = atan2(sinphi,cosphi);
  
}

