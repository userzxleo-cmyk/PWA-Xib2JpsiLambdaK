  double gmScale=1.3;

  Int_t lineStyle=1;
  Int_t L_lineStyle=1;

  Int_t k1440=kYellow+1;
  Int_t  m1440=34;
  Double_t ms1440=gmScale*0.1;

  Int_t k1535=kBlue-7;
  Int_t  m1535=24;
  Double_t ms1535=gmScale*0.1;

  Int_t k1650=kMagenta-7;
  Int_t  m1650=27;
  Double_t ms1650=gmScale*0.1;

  Int_t k666 =kMagenta-9;
  Int_t m666 = 29;
  Double_t ms666 = gmScale*0.1;


  Int_t k1680=7;
  Int_t  m1680=32;
  Double_t ms1680=gmScale*0.1;

  Int_t k1520=kGreen-5;  
  Int_t  m1520=5;
  Double_t ms1520=gmScale*0.1;

  Int_t k1720=9;
  Int_t  m1720=3;
  Double_t ms1720=gmScale*0.1;

  Int_t k1675=49;
  Int_t  m1675=25;
  Double_t ms1675=gmScale*0.1;

  Int_t k1700=46;
  Int_t  m1700=30;
  Double_t ms1700=gmScale*0.1;

  Int_t k1710=38;
  Int_t  m1710=23;
  Double_t ms1710=gmScale*0.1;

  Int_t k1875=35;
  Int_t  m1875=22;
  Double_t ms1875=gmScale*0.1;

  Int_t k2100=kCyan-2; //30;
  Int_t  m2100=28;
  Double_t ms2100=gmScale*0.1;

  Int_t k2110=28;
  Int_t  m2110=26;
  Double_t ms2110=gmScale*0.1;

  Int_t kNR=kGray;
  Int_t  mNR=29;
  Double_t msNR=gmScale*0.1;

  Int_t kLASS=kYellow-2;
  Int_t  mLASS=33;
  Double_t msLASS=gmScale*0.1;


  Int_t mdata=21; //was 33 diamond
  double data_size = gmScale*0.8; // was 1.5; for diamond   
  double total_fit_size = gmScale*0.8;   

  Int_t f4450 =3004; 
  Int_t f4380 =3005; 
  Int_t f4312 =3006;
  
  Int_t k4380 = 6;
  Int_t m4380 = 21;
  Double_t ms4380 = 1.0;

  Int_t k4450 = 4;
  Int_t m4450 = 22;
  Double_t ms4450 = 1.0;

  Int_t f4200 =3005; 

  Int_t k4200 = kGreen+1;
  Int_t m4200 = 20;
  Double_t ms4200 = 1.0;


  Int_t kD1620=46;
  Int_t  mD1620=29;
  Double_t msD1620=gmScale*0.7;

  Int_t kD1232=39;
  Int_t  mD1232=33;
  Double_t msD1232=gmScale*0.5;

  Int_t kD1600=kViolet-3;
  Int_t  mD1600=0;
  Double_t msD1600=gmScale*0.5;

  Int_t k892=kCyan-3;
  Int_t  m892=8;
  Double_t ms892=gmScale*0.5;

  TString T_histoname = TString(histname);
  if(T_histoname.Contains("S01")) {
     histos.SetLineColor(k1440);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k1440);
     histos.SetMarkerStyle(m1440);
     histos.SetMarkerSize(ms1440);
     leg.AddEntry(&histos,"1/2^{-} P_{c}");
     histos.DrawCopy("hsame",histname);
  }  else if(T_histoname.Contains("P01")) {
     histos.SetLineColor(k1535);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k1535);
     histos.SetMarkerStyle(m1535);
     histos.SetMarkerSize(ms1535);
     leg.AddEntry(&histos,"1/2^{+} P_{c}"); 
     histos.DrawCopy("hsame",histname);       
  }  else if(T_histoname.Contains("S03")) {
     histos.SetLineColor(k666);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k666);
     histos.SetMarkerStyle(m666);
     histos.SetMarkerSize(ms666);
     leg.AddEntry(&histos,"3/2^{-} P_{c}");
     histos.DrawCopy("hsame",histname);  
  }  else if(T_histoname.Contains("P03")) {
     histos.SetLineColor(k1650);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k1650);
     histos.SetMarkerStyle(m1650);
     histos.SetMarkerSize(ms1650);
     leg.AddEntry(&histos,"3/2^{+} P_{c}");          
     histos.DrawCopy("hsame",histname);     
  }  else if(T_histoname.Contains("D03")) {
     histos.SetLineColor(k1675);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k1675);
     histos.SetMarkerStyle(m1675);
     histos.SetMarkerSize(ms1675);
     leg.AddEntry(&histos,"D03"); 
     histos.DrawCopy("hsame",histname);         
  }  else if(T_histoname.Contains("D05")) {
     histos.SetLineColor(k1680);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k1680);
     histos.SetMarkerStyle(m1680);
     histos.SetMarkerSize(ms1680);
     leg.AddEntry(&histos,"D05");          
     histos.DrawCopy("hsame",histname);     
  }  else if(T_histoname.Contains("P05")) {
     histos.SetLineColor(k1520);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k1520);
     histos.SetMarkerStyle(m1520);
     histos.SetMarkerSize(ms1520);
     leg.AddEntry(&histos,"P05");          
     histos.DrawCopy("hsame",histname);     
  }    else if(T_histoname.Contains("F07")) {
     histos.SetLineColor(k1700);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k1700);
     histos.SetMarkerStyle(m1700);
     histos.SetMarkerSize(ms1700);
     leg.AddEntry(&histos,"F07");          
     histos.DrawCopy("hsame",histname);     
  }else if(T_histoname.Contains("G07")) {
     histos.SetLineColor(k1710);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k1710);
     histos.SetMarkerStyle(m1710);
     histos.SetMarkerSize(ms1710);
     leg.AddEntry(&histos,"G07");          
     histos.DrawCopy("hsame",histname);     
  }  else if(T_histoname.Contains("G09")) {
     histos.SetLineColor(k1720);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k1720);
     histos.SetMarkerStyle(m1720);
     histos.SetMarkerSize(ms1720);
     leg.AddEntry(&histos,"G09");          
     histos.DrawCopy("hsame",histname);     
  } else if(T_histoname.Contains("H09")) {
     histos.SetLineColor(k1875);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k1875);
     histos.SetMarkerStyle(m1875);
     histos.SetMarkerSize(ms1875);
     leg.AddEntry(&histos,"H09"); 
     histos.DrawCopy("hsame",histname);         
  }     else if(T_histoname.EqualTo("wL")) {
     histos.SetLineColor(kNR);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(kNR);
     histos.SetMarkerStyle(mNR);
     histos.SetMarkerSize(msNR);
     leg.AddEntry(&histos,"All #Lambda^{*}");          
     histos.DrawCopy("hist,same",histname);    
  }     else if(T_histoname.EqualTo("wX")) {
     histos.SetLineColor(kD1232);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(kD1232);
     histos.SetMarkerStyle(mD1232);
     histos.SetMarkerSize(msD1232);
     leg.AddEntry(&histos,"All Z_{cs}");          
     histos.DrawCopy("hist,same",histname);    
  }   else   if(T_histoname.Contains("Z3")) {
     histos.SetLineColor(k4380);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k4380);
     histos.SetMarkerStyle(0);
     histos.SetMarkerSize(ms4380);
     histos.SetFillStyle(f4380);
     histos.SetFillColor(k4380);
     leg.AddEntry(&histos,"P_{c}");               
     histos.DrawCopy("hsame",histname);     
  }  else if(T_histoname.Contains("Z5")) {
     histos.SetLineColor(k4450);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k4450);
     histos.SetMarkerStyle(0);
     histos.SetMarkerSize(ms4450);
     histos.SetFillStyle(f4450);
     histos.SetFillColor(k4450);    
     leg.AddEntry(&histos,"P_{c}(4457)");           
     histos.DrawCopy("hsame",histname);     
  }    else if(T_histoname.Contains("wZ")) {
     histos.SetLineColor(k4200);
     histos.SetLineStyle(lineStyle);
  //   histos.SetFillStyle(f4200);
   //  histos.SetFillColor(k4200);    
     leg.AddEntry(&histos,"All Pc","L");           
     histos.DrawCopy("hist,same",histname);     
  }   else if(T_histoname.Contains("Z2")) {
     histos.SetLineColor(k892);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k892);
     histos.SetMarkerStyle(m892);
     histos.SetMarkerSize(msD1600);
     leg.AddEntry(&histos,"P_{c}(4380)"); 
     histos.DrawCopy("same",histname);         
  }   else if(T_histoname.Contains("Z6")) {
     histos.SetLineColor(kD1600);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(kD1600);
     histos.SetMarkerStyle(0);
     histos.SetMarkerSize(ms4450);
     histos.SetFillStyle(f4312);
     histos.SetFillColor(k4380);     
     leg.AddEntry(&histos,"P_{c}"); 
     histos.DrawCopy("hsame",histname);         
  }  else if(T_histoname.Contains("Z7")) {
     histos.SetLineColor(kD1232);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(kD1232);
     histos.SetMarkerStyle(mD1232);
     histos.SetMarkerSize(msD1232);
     leg.AddEntry(&histos,"P_{c}"); 
     histos.DrawCopy("hsame",histname);         
  }      else if(T_histoname.Contains("Z4")) {
     histos.SetLineColor(k4200+1);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(k4200);
     histos.SetMarkerStyle(m4200);
     histos.SetMarkerSize(0.4);
     leg.AddEntry(&histos,"P_{c}(4312)");          
     histos.DrawCopy("same",histname);     
  }     else if(T_histoname.Contains("Z1")) {
     histos.SetLineColor(kLASS);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(kLASS);
     histos.SetMarkerStyle(mLASS);
     histos.SetMarkerSize(msLASS);
     leg.AddEntry(&histos,"P_{c}"); 
     histos.DrawCopy("hsame",histname);         
  } else if(T_histoname.Contains("wL_Z")) {
     histos.SetLineColor(kD1600);
     histos.SetLineStyle(L_lineStyle);
     histos.SetMarkerColor(kD1600);
     histos.SetMarkerStyle(0);
     histos.SetMarkerSize(ms4450);
     histos.SetFillStyle(f4312);
     histos.SetFillColor(k4380);
     leg.AddEntry(&histos,"P_{c}(4440)"); 
     histos.DrawCopy("hsame",histname);         
  } 
  
