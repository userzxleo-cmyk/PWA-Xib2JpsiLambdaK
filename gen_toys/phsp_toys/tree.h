//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jan 18 16:55:24 2021 by ROOT version 6.06/02
// from TTree tree/
// found on file: tuples/phsp.root
//////////////////////////////////////////////////////////

#ifndef tree_h
#define tree_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   int Lb_ID;
   double mkp, mjpsip, mjpsik;
   double H1_PX_Fit, H1_PY_Fit, H1_PZ_Fit, H1_PE_Fit;
   double H2_PX_Fit, H2_PY_Fit, H2_PZ_Fit, H2_PE_Fit;
   double muplus_PX_Fit, muplus_PY_Fit, muplus_PZ_Fit, muplus_PE_Fit;
   double muminus_PX_Fit, muminus_PY_Fit, muminus_PZ_Fit, muminus_PE_Fit;
   double costheta, costheta1, costheta2, phi, phi1, phi2, costheta_Z, costheta1_Z, costheta2_Z, phi_Z, phi1_Z, phi2_Z, costheta_p, alpha_p, alpha_mu;
   double sw;
   Double_t        Lb_PX;
   Double_t        Lb_PY;
   Double_t        Lb_PZ;
   Double_t        Lb_PE;
   Double_t        K_PX;
   Double_t        K_PY;
   Double_t        K_PZ;
   Double_t        K_PE;
   Double_t        P_PX;
   Double_t        P_PY;
   Double_t        P_PZ;
   Double_t        P_PE;
   Double_t        Jpsi_PX;
   Double_t        Jpsi_PY;
   Double_t        Jpsi_PZ;
   Double_t        Jpsi_PE;
   Double_t        mup_PX;
   Double_t        mup_PY;
   Double_t        mup_PZ;
   Double_t        mup_PE;
   Double_t        mum_PX;
   Double_t        mum_PY;
   Double_t        mum_PZ;
   Double_t        mum_PE;
   Double_t        weight_Jpsi2mumu;
   Double_t        weight_Lb2JpsipK;
   Double_t        weight;

   // List of branches
   TBranch        *b_Lb_PX;   //!
   TBranch        *b_Lb_PY;   //!
   TBranch        *b_Lb_PZ;   //!
   TBranch        *b_Lb_PE;   //!
   TBranch        *b_K_PX;   //!
   TBranch        *b_K_PY;   //!
   TBranch        *b_K_PZ;   //!
   TBranch        *b_K_PE;   //!
   TBranch        *b_P_PX;   //!
   TBranch        *b_P_PY;   //!
   TBranch        *b_P_PZ;   //!
   TBranch        *b_P_PE;   //!
   TBranch        *b_Jpsi_PX;   //!
   TBranch        *b_Jpsi_PY;   //!
   TBranch        *b_Jpsi_PZ;   //!
   TBranch        *b_Jpsi_PE;   //!
   TBranch        *b_mup_PX;   //!
   TBranch        *b_mup_PY;   //!
   TBranch        *b_mup_PZ;   //!
   TBranch        *b_mup_PE;   //!
   TBranch        *b_mum_PX;   //!
   TBranch        *b_mum_PY;   //!
   TBranch        *b_mum_PZ;   //!
   TBranch        *b_mum_PE;   //!
   TBranch        *b_weight_Jpsi2mumu;   //!
   TBranch        *b_weight_Lb2JpsipK;   //!
   TBranch        *b_weight;   //!

   tree(TTree *tree=0);
   virtual ~tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(const char * outfile_path);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
   void helicityJpsiLam();
   void helicityZK();
   void helicityTwoFrame();
};

#endif

#ifdef tree_cxx
tree::tree(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
//      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("tuples/phsp.root");
//      if (!f || !f->IsOpen()) {
//         f = new TFile("tuples/phsp.root");
//      }
//      f->GetObject("tree",tree);

   }
   Init(tree);
}

tree::~tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t tree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void tree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("Lb_PX", &Lb_PX, &b_Lb_PX);
   fChain->SetBranchAddress("Lb_PY", &Lb_PY, &b_Lb_PY);
   fChain->SetBranchAddress("Lb_PZ", &Lb_PZ, &b_Lb_PZ);
   fChain->SetBranchAddress("Lb_PE", &Lb_PE, &b_Lb_PE);
   fChain->SetBranchAddress("K_PX", &K_PX, &b_K_PX);
   fChain->SetBranchAddress("K_PY", &K_PY, &b_K_PY);
   fChain->SetBranchAddress("K_PZ", &K_PZ, &b_K_PZ);
   fChain->SetBranchAddress("K_PE", &K_PE, &b_K_PE);
   fChain->SetBranchAddress("P_PX", &P_PX, &b_P_PX);
   fChain->SetBranchAddress("P_PY", &P_PY, &b_P_PY);
   fChain->SetBranchAddress("P_PZ", &P_PZ, &b_P_PZ);
   fChain->SetBranchAddress("P_PE", &P_PE, &b_P_PE);
   fChain->SetBranchAddress("Jpsi_PX", &Jpsi_PX, &b_Jpsi_PX);
   fChain->SetBranchAddress("Jpsi_PY", &Jpsi_PY, &b_Jpsi_PY);
   fChain->SetBranchAddress("Jpsi_PZ", &Jpsi_PZ, &b_Jpsi_PZ);
   fChain->SetBranchAddress("Jpsi_PE", &Jpsi_PE, &b_Jpsi_PE);
   fChain->SetBranchAddress("mup_PX", &mup_PX, &b_mup_PX);
   fChain->SetBranchAddress("mup_PY", &mup_PY, &b_mup_PY);
   fChain->SetBranchAddress("mup_PZ", &mup_PZ, &b_mup_PZ);
   fChain->SetBranchAddress("mup_PE", &mup_PE, &b_mup_PE);
   fChain->SetBranchAddress("mum_PX", &mum_PX, &b_mum_PX);
   fChain->SetBranchAddress("mum_PY", &mum_PY, &b_mum_PY);
   fChain->SetBranchAddress("mum_PZ", &mum_PZ, &b_mum_PZ);
   fChain->SetBranchAddress("mum_PE", &mum_PE, &b_mum_PE);
   fChain->SetBranchAddress("weight_Jpsi2mumu", &weight_Jpsi2mumu, &b_weight_Jpsi2mumu);
   fChain->SetBranchAddress("weight_Lb2JpsipK", &weight_Lb2JpsipK, &b_weight_Lb2JpsipK);
   fChain->SetBranchAddress("weight", &weight, &b_weight);
   Notify();
}

Bool_t tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef tree_cxx
