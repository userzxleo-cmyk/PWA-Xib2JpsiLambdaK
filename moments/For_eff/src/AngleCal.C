#define AngleCal_cxx
#include "RooTDalitz/AngleCal.h"


  /*
    =============================================================================
    Notation is (type of vector)_(particle)_(rest frame)
    type of vector := {unit (u), three vector (p3), four vector (p4) }

    =============================================================================
  */


/*
AngleCal::AngleCal()
{
}

AngleCal::~AngleCal()
{
}
*/



void AngleCal::CalcAll(TLorentzVector p4_Mu,
			   TLorentzVector p4_Jpsi,
			   TLorentzVector p4_K,
			   TLorentzVector p4_p,
			   TLorentzVector p4_L,
			   TLorentzVector p4_Z,
			   TLorentzVector p4_Lb){



  /*
    =============================================================================
    LAB frame quantities
    =============================================================================
  */

  //Lb unit direction
  TVector3 p3_Lb_LAB = p4_Lb.Vect();
  TVector3 u_Lb_LAB = p3_Lb_LAB.Unit();

  TVector3 p3_L_LAB = p4_L.Vect(); 
  TVector3 u_L_LAB = p3_L_LAB.Unit();

  TVector3 p3_Z_LAB = p4_Z.Vect();
  TVector3 u_Z_LAB = p3_Z_LAB.Unit();


  /*
    ============================================================================
    Lb rest frame quantities
    ============================================================================
  */
   
  ///Boost everything to Lb
  TVector3 boostToLb = - p4_Lb.BoostVector();

  // Lambda in Lb frame
  TLorentzVector p4_L_Lb = p4_L; p4_L_Lb.Boost( boostToLb );
  //Jpsi in Lb frame
  TLorentzVector p4_Jpsi_Lb = p4_Jpsi; p4_Jpsi_Lb.Boost( boostToLb );
  //Mu in Lb frame
  TLorentzVector p4_Mu_Lb = p4_Mu; p4_Mu_Lb.Boost( boostToLb );  
  //K in Lb frame
  TLorentzVector p4_K_Lb = p4_K; p4_K_Lb.Boost( boostToLb );
  //p in Lb frame
  TLorentzVector p4_p_Lb = p4_p; p4_p_Lb.Boost( boostToLb );
  //Z in Lb frame
  TLorentzVector p4_Z_Lb = p4_Z; p4_Z_Lb.Boost( boostToLb);

  TVector3 p3_L_Lb = p4_L_Lb.Vect(); 
  TVector3 u_L_Lb = p3_L_Lb.Unit();
  TVector3 p3_Z_Lb = p4_Z_Lb.Vect();
  TVector3 u_Z_Lb = p3_Z_Lb.Unit();


  //X axis of Lb frame
  TVector3 p3_x_Lb = u_L_Lb - ( u_L_Lb.Dot( u_Lb_LAB ))*u_Lb_LAB;
  TVector3 u_x_Lb = p3_x_Lb.Unit();


  //X axis of Lambda frame
  TVector3 p3_x_L = -u_Lb_LAB + (u_Lb_LAB.Dot(u_L_Lb))*u_L_Lb;
  TVector3 u_x_L = p3_x_L.Unit();

  //X axis of Z frame
  TVector3 p3_x_Z = -u_Lb_LAB + (u_Lb_LAB.Dot( u_Z_Lb))*u_Z_Lb;
  TVector3 u_x_Z = p3_x_Z.Unit();


 
  /*
    ============================================================================
   Z frame
    ============================================================================
   */
  //Boost to Z
  TVector3 boostToZ = -p4_Z_Lb.BoostVector();

  //Jpsi in Z frame
  TLorentzVector p4_Jpsi_Z  = p4_Jpsi_Lb; p4_Jpsi_Z.Boost( boostToZ);
  //K in Z frame
  TLorentzVector p4_K_Z  = p4_K_Lb; p4_K_Z.Boost( boostToZ);
  //Mu in Z frame
  TLorentzVector p4_Mu_Z  = p4_Mu_Lb; p4_Mu_Z.Boost( boostToZ);
  //p in Z frame
  TLorentzVector p4_p_Z  = p4_p_Lb; p4_p_Z.Boost( boostToZ);


  TVector3 p3_K_Z = p4_K_Z.Vect(); 
  TVector3 u_K_Z = p3_K_Z.Unit(); 
  TVector3 u_Z_Z = -u_K_Z;
  TVector3 p3_Jpsi_Z = p4_Jpsi_Z.Vect(); TVector3 u_Jpsi_Z = p3_Jpsi_Z.Unit();


  //x axis in JpsiZ frame
  TVector3 p3_x_JpsiZ = u_K_Z - (u_K_Z.Dot( u_Jpsi_Z))*u_Jpsi_Z;
  TVector3 u_x_JpsiZ = p3_x_JpsiZ.Unit();


  /*
    ============================================================================
    Lambda rest frame quantities
    ============================================================================
  */

  TVector3 boostToL = -p4_L_Lb.BoostVector();
  TLorentzVector p4_Jpsi_L = p4_Jpsi_Lb; p4_Jpsi_L.Boost( boostToL );
  TLorentzVector p4_K_L = p4_K_Lb; p4_K_L.Boost( boostToL );
  TLorentzVector p4_p_L = p4_p_Lb; p4_p_L.Boost( boostToL );

  TVector3 u_Jpsi_L = p4_Jpsi_L.Vect().Unit();
  TVector3 u_L_L = - u_Jpsi_L;
  TVector3 u_K_L = p4_K_L.Vect().Unit();


  /*
    ============================================================================
    Jpsi rest frame quantities
    ============================================================================
  */

  ///Boost everything to Jpsi
  TVector3 boostToJpsi = - p4_Jpsi_Lb.BoostVector();

  TLorentzVector p4_L_Jpsi = p4_L_Lb; p4_L_Jpsi.Boost( boostToJpsi );
  TLorentzVector p4_Mu_Jpsi = p4_Mu_Lb; p4_Mu_Jpsi.Boost( boostToJpsi );
  TLorentzVector p4_p_Jpsi = p4_p_Lb; p4_p_Jpsi.Boost( boostToJpsi );

  TVector3 u_L_Jpsi = p4_L_Jpsi.Vect().Unit();
  TVector3 u_Jpsi_Jpsi = - u_L_Jpsi;
  TVector3 u_Mu_Jpsi = p4_Mu_Jpsi.Vect().Unit();
  TVector3 u_p_Jpsi = p4_p_Jpsi.Vect().Unit();

  TVector3 u_z_Mu = u_Mu_Jpsi;
  TVector3 p3_x_Mu = u_L_Jpsi -  (u_L_Jpsi.Dot( u_Mu_Jpsi))*u_Mu_Jpsi;
  TVector3 u_x_Mu = p3_x_Mu.Unit();


  TVector3 p3_x_MuZ = u_p_Jpsi  - ( u_p_Jpsi.Dot( u_Mu_Jpsi))*u_Mu_Jpsi;
  TVector3 u_x_MuZ = p3_x_MuZ.Unit();




  /*
    ============================================================================
    JpsiZ rest frame quantities
    ============================================================================
  */
  
  ///Boost everything to Jpsi
  TVector3 boostToJpsiZ = - p4_Jpsi_Z.BoostVector();

  TLorentzVector p4_Mu_JpsiZ = p4_Mu_Z; p4_Mu_JpsiZ.Boost( boostToJpsiZ );
  TLorentzVector p4_p_JpsiZ = p4_p_Z; p4_p_JpsiZ.Boost( boostToJpsiZ );

  TVector3 u_Mu_JpsiZ = p4_Mu_JpsiZ.Vect().Unit();
  TVector3 u_p_JpsiZ = p4_p_JpsiZ.Vect().Unit();
  TVector3 u_Jpsi_JpsiZ = -u_p_JpsiZ;

  /*
    //CANT CALCULATE THIS HERE NEED TO DO IT IN SAME FRAME AS OTHER JPSI,
    //BOOSTING PAATH AFFECTS THE FRAME
  TVector3 p3_x_MuZ = u_p_JpsiZ  - ( u_p_JpsiZ.Dot( u_Mu_JpsiZ))*u_Mu_JpsiZ;
  TVector3 u_x_MuZ = p3_x_MuZ.Unit();

  */

  /*
    ============================================================================
    p/pZ rest frame quantities
    ============================================================================
  */

  ///Boost everything to p
  //from Lambda
  TVector3 boostTop = - p4_p.BoostVector();
  //BOTH NEED TO FOLLOW THE SAME BOOST PATH... JUST TAKE THEM FROM THE LAB FRAME
  TLorentzVector p4_K_p = p4_K; p4_K_p.Boost( boostTop );
  TLorentzVector p4_Jpsi_p = p4_Jpsi; p4_Jpsi_p.Boost( boostTop );

  TVector3 p3_K_p = p4_K_p.Vect();
  TVector3 u_K_p = p3_K_p.Unit();
  TVector3 p3_Jpsi_p = p4_Jpsi_p.Vect();
  TVector3 u_Jpsi_p = p3_Jpsi_p.Unit();


  /*
    ============================================================================
    Calculations
    ============================================================================
   */

  cosTheta_Lb = u_Lb_LAB.Dot(u_L_Lb);
  cosTheta_L = -u_Jpsi_L.Dot(u_K_L);
  cosTheta_Jpsi = -u_L_Jpsi.Dot( u_Mu_Jpsi );

  phiK = atan2( -(u_Jpsi_L.Cross(u_x_L)).Dot(u_K_L) , u_x_L.Dot( u_K_L)  );
  phiMu = atan2( -( u_L_Jpsi.Cross(u_x_L) ).Dot(u_Mu_Jpsi), u_x_L.Dot(u_Mu_Jpsi) );

  Z_cosTheta_Lb = u_Lb_LAB.Dot( u_Z_Lb);
  Z_cosTheta_Z = -u_K_Z.Dot( u_Jpsi_Z);
  Z_cosTheta_Jpsi = -u_p_JpsiZ.Dot( u_Mu_JpsiZ);

  Z_phiZ = atan2( (u_Lb_LAB.Cross(u_x_Lb)).Dot(u_Z_Lb)  , u_x_Lb.Dot(u_Z_Lb) );
  Z_phiJpsi = atan2( -(u_K_Z.Cross(u_x_Z)).Dot(u_Jpsi_Z), u_x_Z.Dot(u_Jpsi_Z));
  Z_phiMu = atan2( -(u_p_JpsiZ.Cross( u_x_JpsiZ)).Dot(u_Mu_JpsiZ) , u_x_JpsiZ.Dot( u_Mu_JpsiZ));

 
  cosTheta_p = u_K_p.Dot(u_Jpsi_p);
  alpha_Mu = atan2( (u_z_Mu.Cross(u_x_MuZ)).Dot( u_x_Mu) , u_x_MuZ.Dot(u_x_Mu));


}



