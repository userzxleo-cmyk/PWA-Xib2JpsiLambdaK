
#ifndef AngleCal_h
#define AngleCal_h

#include <TROOT.h>
#include <TLorentzVector.h>

class AngleCal {
 public:
  
  Double_t cosTheta_Lb, cosTheta_L, cosTheta_Jpsi;
  Double_t phiMu, phiK;

  Double_t Z_cosTheta_Lb, Z_cosTheta_Z, Z_cosTheta_Jpsi;
  Double_t Z_phiZ, Z_phiJpsi, Z_phiMu;

  Double_t cosTheta_p, alpha_Mu;
  /*
  AngleCal();
  ~AngleCal();

  void CalcAll(TLorentzVector p4_Mu,
		       TLorentzVector p4_Jpsi,
		       TLorentzVector p4_K,
		       TLorentzVector p4_p,
		       TLorentzVector p4_L,
		       TLorentzVector p4_Z,
		       TLorentzVector p4_Lb);
*/
void CalcAll(TLorentzVector p4_Mu,
			   TLorentzVector p4_Jpsi,
			   TLorentzVector p4_K,
			   TLorentzVector p4_p,
			   TLorentzVector p4_L,
			   TLorentzVector p4_Z,
		       TLorentzVector p4_Lb);


 private:

};

#endif
