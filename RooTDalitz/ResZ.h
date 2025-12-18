//Z 4440
//Z2 4380
//Z3 4457
//Z4 4312
  int RKM=REVISEDKMATRIX;
  TList *listZ = new TList();
  //===Lambda(Z) 1/2-
  RooRealVar m0_Z("m0_Z","m0",4.440,4.4,4.45);
  RooRealVar width_Z("width_Z","width",0.01,0,0.05);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z("a_Bp_Z","",0.1,-5,5);
  RooRealVar p_Bp_Z("p_Bp_Z","",0.1,-5,5);
  RooRealVar a_L0_Z("a_L0_Z","",0.01);
  RooRealVar p_L0_Z("p_L0_Z","",0.01);
  RooRealVar a_L1_Z("a_L1_Z","",0.01);
  RooRealVar p_L1_Z("p_L1_Z","",0.01);
  RooRealVar a_L2_Z("a_L2_Z","",0.01);
  RooRealVar p_L2_Z("p_L2_Z","",0.01);

  RooArgList* L_Z = new RooArgList(a_Bp_Z,p_Bp_Z,
				      a_L0_Z,p_L0_Z,
				      a_L1_Z,p_L1_Z,
				      a_L2_Z,p_L2_Z,"L_Z");
  L_Z->add(m0_Z);
  L_Z->add(width_Z);
  RooRealVar typeZ("typeZ","",RKM);
  L_Z->add(typeZ);//type
  L_Z->add(RooRealConstant::value(ZS));//2 x spin
  L_Z->add(RooRealConstant::value(ZP)); //parity
  
//  listZ->Add(L_Z);


  //Define a S-wave NR Jpsip component named as Z0

  RooRealVar m0_Z0("m0_Z0","m0", -2.5);
  RooRealVar width_Z0("width_Z0","width",0.00001,-10, 10);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z0("a_Bp_Z0","",0.1,-5,5);
  RooRealVar p_Bp_Z0("p_Bp_Z0","",0.1,-5,5);
  RooRealVar a_L0_Z0("a_L0_Z0","",0.1);
  RooRealVar p_L0_Z0("p_L0_Z0","",0.1);
  RooRealVar a_L1_Z0("a_L1_Z0","",0.1);
  RooRealVar p_L1_Z0("p_L1_Z0","",0.1);
  RooRealVar a_L2_Z0("a_L2_Z0","",0.1);
  RooRealVar p_L2_Z0("p_L2_Z0","",0.1);

  RooArgList* L_Z0 = new RooArgList(a_Bp_Z0,p_Bp_Z0,
                                      a_L0_Z0,p_L0_Z0,
                                      a_L1_Z0,p_L1_Z0,
                                      a_L2_Z0,p_L2_Z0,"L_Z0");
  L_Z0->add(m0_Z0);
  L_Z0->add(width_Z0);
  RooRealVar typeZ0("typeZ0","",RKM);
  L_Z0->add(typeZ0);//type
  L_Z0->add(RooRealConstant::value(1));//2 x spin
  L_Z0->add(RooRealConstant::value(-1)); //parity



  //===Lambda(Z) 1/2-
  RooRealVar m0_Z3("m0_Z3","m0",4.456,4.2,4.6);
  RooRealVar width_Z3("width_Z3","width",0.2,0,0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z3("a_Bp_Z3","",0.1,-5,5);
  RooRealVar p_Bp_Z3("p_Bp_Z3","",0.1,-5,5);
  RooRealVar a_L0_Z3("a_L0_Z3","",0.1);
  RooRealVar p_L0_Z3("p_L0_Z3","",0.1);
  RooRealVar a_L1_Z3("a_L1_Z3","",0.1);
  RooRealVar p_L1_Z3("p_L1_Z3","",0.1);
  RooRealVar a_L2_Z3("a_L2_Z3","",0.1);
  RooRealVar p_L2_Z3("p_L2_Z3","",0.1);

  RooArgList* L_Z3 = new RooArgList(a_Bp_Z3,p_Bp_Z3,
				      a_L0_Z3,p_L0_Z3,
				      a_L1_Z3,p_L1_Z3,
				      a_L2_Z3,p_L2_Z3,"L_Z3");
  L_Z3->add(m0_Z3);
  L_Z3->add(width_Z3);
  RooRealVar typeZ3("typeZ3","",RKM);
  L_Z3->add(typeZ3);//type
  L_Z3->add(RooRealConstant::value(ZS3));//2 x spin
  L_Z3->add(RooRealConstant::value(ZP3)); //parity
  
//  listZ->Add(L_Z2);

  //===Lambda(Z) 1/2-
  RooRealVar m0_Z2("m0_Z2","m0",4.25,4.2,4.6);
  RooRealVar width_Z2("width_Z2","width",0.168,0,0.3);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z2("a_Bp_Z2","",0.1,-5,5);
  RooRealVar p_Bp_Z2("p_Bp_Z2","",0.1,-5,5);
  RooRealVar a_L0_Z2("a_L0_Z2","",0.1);
  RooRealVar p_L0_Z2("p_L0_Z2","",0.1);
  RooRealVar a_L1_Z2("a_L1_Z2","",0.1);
  RooRealVar p_L1_Z2("p_L1_Z2","",0.1);
  RooRealVar a_L2_Z2("a_L2_Z2","",0.1);
  RooRealVar p_L2_Z2("p_L2_Z2","",0.1);

  RooArgList* L_Z2 = new RooArgList(a_Bp_Z2,p_Bp_Z2,
				      a_L0_Z2,p_L0_Z2,
				      a_L1_Z2,p_L1_Z2,
				      a_L2_Z2,p_L2_Z2,"L_Z2");
  L_Z2->add(m0_Z2);
  L_Z2->add(width_Z2);
  RooRealVar typeZ2("typeZ2","",RKM);
  L_Z2->add(typeZ2);
//  L_Z2->add(RooRealConstant::value(1));//  
  L_Z2->add(RooRealConstant::value(ZS2));//2 x spin
  L_Z2->add(RooRealConstant::value(ZP2)); //parity
  
  
  //===Lambda(Z4) 1/2-
  RooRealVar m0_Z4("m0_Z4","m0",4.312,4.30,4.33);
  RooRealVar width_Z4("width_Z4","width",0.01,0.002,0.04);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z4("a_Bp_Z4","",0.1,-5,5);
  RooRealVar p_Bp_Z4("p_Bp_Z4","",0.1,-5,5);
  RooRealVar a_L0_Z4("a_L0_Z4","",1);
  RooRealVar p_L0_Z4("p_L0_Z4","",0.00);
  RooRealVar a_L1_Z4("a_L1_Z4","",0.00);
  RooRealVar p_L1_Z4("p_L1_Z4","",0.00);
  RooRealVar a_L2_Z4("a_L2_Z4","",0.00);
  RooRealVar p_L2_Z4("p_L2_Z4","",0.00);

  RooArgList* L_Z4 = new RooArgList(a_Bp_Z4,p_Bp_Z4,
				      a_L0_Z4,p_L0_Z4,
				      a_L1_Z4,p_L1_Z4,
				      a_L2_Z4,p_L2_Z4,"L_Z4");
  L_Z4->add(m0_Z4);
  L_Z4->add(width_Z4);
  RooRealVar typeZ4("typeZ4","",RKM);
  L_Z4->add(typeZ4);//type
  L_Z4->add(RooRealConstant::value(ZS4));//2 x spin
  L_Z4->add(RooRealConstant::value(ZP4)); //parity
  
//  listZ->Add(L_Z4);

  //===Lambda(Z4) 1/2-
  RooRealVar m0_Z1P("m0_Z1P","m0",4.312);
  RooRealVar width_Z1P("width_Z1P","width",0.1);
  RooRealVar a_Bp_Z1P("a_Bp_Z1P","",0.5,-5,5);
  RooRealVar p_Bp_Z1P("p_Bp_Z1P","",0.1,-5,5);
  RooRealVar a_L0_Z1P("a_L0_Z1P","",1);
  RooRealVar p_L0_Z1P("p_L0_Z1P","",0.00);
  RooRealVar a_L1_Z1P("a_L1_Z1P","",0.00);
  RooRealVar p_L1_Z1P("p_L1_Z1P","",0.00);
  RooRealVar a_L2_Z1P("a_L2_Z1P","",0.00);
  RooRealVar p_L2_Z1P("p_L2_Z1P","",0.00);

  RooArgList* L_Z1P = new RooArgList(a_Bp_Z1P,p_Bp_Z1P,
				      a_L0_Z1P,p_L0_Z1P,
				      a_L1_Z1P,p_L1_Z1P,
				      a_L2_Z1P,p_L2_Z1P,"L_Z1P");

//  static int Nsp=45;
  char Sname[20];
  RooRealVar *Z1PRe[NUMSP], *Z1PIm[NUMSP];
  for(int i=0; i<NUMSP; ++i) {
   sprintf(Sname,"Z1PR_%02d",i);
    Z1PRe[i] = new RooRealVar(Sname,Sname,0.01);
    Z1PRe[i]->setConstant(0);
    L_Z1P->add(*Z1PRe[i]);
    sprintf(Sname,"Z1PI_%02d",i);
    Z1PIm[i] = new RooRealVar(Sname,Sname,0);
    Z1PIm[i]->setConstant(0);
    L_Z1P->add(*Z1PIm[i]);
  }
  L_Z1P->add(m0_Z1P);
  L_Z1P->add(width_Z1P);				      
  L_Z1P->add(RooRealConstant::value(3));//type
  L_Z1P->add(RooRealConstant::value(1));//2 x spin
  L_Z1P->add(RooRealConstant::value(1)); //parity

//  
  RooRealVar m0_Z1M("m0_Z1M","m0",4.312);
  RooRealVar width_Z1M("width_Z1M","width",0.1);
  RooRealVar a_Bp_Z1M("a_Bp_Z1M","",0.5,-5,5);
  RooRealVar p_Bp_Z1M("p_Bp_Z1M","",0.1,-5,5);
  RooRealVar a_L0_Z1M("a_L0_Z1M","",1);
  RooRealVar p_L0_Z1M("p_L0_Z1M","",0.00);
  RooRealVar a_L1_Z1M("a_L1_Z1M","",0.00);
  RooRealVar p_L1_Z1M("p_L1_Z1M","",0.00);
  RooRealVar a_L2_Z1M("a_L2_Z1M","",0.00);
  RooRealVar p_L2_Z1M("p_L2_Z1M","",0.00);

  RooArgList* L_Z1M = new RooArgList(a_Bp_Z1M,p_Bp_Z1M,
				      a_L0_Z1M,p_L0_Z1M,
				      a_L1_Z1M,p_L1_Z1M,
				      a_L2_Z1M,p_L2_Z1M,"L_Z1M");

//  static int Nsp=45;
//  char Sname[20];
  RooRealVar *Z1MRe[NUMSP], *Z1MIm[NUMSP];
  for(int i=0; i<NUMSP; ++i) {
   sprintf(Sname,"Z1MR_%02d",i);
    Z1MRe[i] = new RooRealVar(Sname,Sname,0.01);
    Z1MRe[i]->setConstant(0);
    L_Z1M->add(*Z1MRe[i]);
    sprintf(Sname,"Z1MI_%02d",i);
    Z1MIm[i] = new RooRealVar(Sname,Sname,0);
    Z1MIm[i]->setConstant(0);
    L_Z1M->add(*Z1MIm[i]);
  }
  L_Z1M->add(m0_Z1M);
  L_Z1M->add(width_Z1M);				      
  L_Z1M->add(RooRealConstant::value(3));//type
  L_Z1M->add(RooRealConstant::value(ZS6));//2 x spin
  L_Z1M->add(RooRealConstant::value(ZP6)); //parity
    
//  listZ->Add(L_Z1M);


  //===Lambda(Z) 1/2-
  RooRealVar m0_Z5("m0_Z5","m0",4.4,4.3,4.5);
  RooRealVar width_Z5("width_Z5","width",0.16,0,0.4);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z5("a_Bp_Z5","",0.01,-5,5);
  RooRealVar p_Bp_Z5("p_Bp_Z5","",0.01,-5,5);
  RooRealVar a_L0_Z5("a_L0_Z5","",0.1);
  RooRealVar p_L0_Z5("p_L0_Z5","",0.1);
  RooRealVar a_L1_Z5("a_L1_Z5","",0.1);
  RooRealVar p_L1_Z5("p_L1_Z5","",0.1);
  RooRealVar a_L2_Z5("a_L2_Z5","",0.1);
  RooRealVar p_L2_Z5("p_L2_Z5","",0.1);

  RooArgList* L_Z5 = new RooArgList(a_Bp_Z5,p_Bp_Z5,
				      a_L0_Z5,p_L0_Z5,
				      a_L1_Z5,p_L1_Z5,
				      a_L2_Z5,p_L2_Z5,"L_Z5");
  L_Z5->add(m0_Z5);
  L_Z5->add(width_Z5);
  RooRealVar typeZ5("typeZ5","",RKM);
  L_Z5->add(typeZ5);//type
  L_Z5->add(RooRealConstant::value(ZS5));//2 x spin
  L_Z5->add(RooRealConstant::value(ZP5)); //parity


  //===Lambda(Z) 1/2-
  RooRealVar m0_Z6("m0_Z6","m0",4.4,4.2,4.6);
  RooRealVar width_Z6("width_Z6","width",0.16,0.01,0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z6("a_Bp_Z6","",0.01,-5,5);
  RooRealVar p_Bp_Z6("p_Bp_Z6","",0.01,-5,5);
  RooRealVar a_L0_Z6("a_L0_Z6","",0.01);
  RooRealVar p_L0_Z6("p_L0_Z6","",0.01);
  RooRealVar a_L1_Z6("a_L1_Z6","",0.01);
  RooRealVar p_L1_Z6("p_L1_Z6","",0.01);
  RooRealVar a_L2_Z6("a_L2_Z6","",0.01);
  RooRealVar p_L2_Z6("p_L2_Z6","",0.01);

  RooArgList* L_Z6 = new RooArgList(a_Bp_Z6,p_Bp_Z6,
				      a_L0_Z6,p_L0_Z6,
				      a_L1_Z6,p_L1_Z6,
				      a_L2_Z6,p_L2_Z6,"L_Z6");
  L_Z6->add(m0_Z6);
  L_Z6->add(width_Z6);
  RooRealVar typeZ6("typeZ6","",RKM);
  L_Z6->add(typeZ6);//type
  L_Z6->add(RooRealConstant::value(ZS6));//2 x spin
  L_Z6->add(RooRealConstant::value(ZP6)); //parity

  //===Lambda(Z) 1/2- fsc
  RooRealVar m0_ZNR1("m0_ZNR1","m0",-1.0);
  RooRealVar width_ZNR1("width_ZNR1","width",0.1,-20,20);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_ZNR1("a_Bp_ZNR1","",0.01,-5,5);
  RooRealVar p_Bp_ZNR1("p_Bp_ZNR1","",0.01,-5,5);
  RooRealVar a_L0_ZNR1("a_L0_ZNR1","",0.01);
  RooRealVar p_L0_ZNR1("p_L0_ZNR1","",0.01);
  RooRealVar a_L1_ZNR1("a_L1_ZNR1","",0.01);
  RooRealVar p_L1_ZNR1("p_L1_ZNR1","",0.01);
  RooRealVar a_L2_ZNR1("a_L2_ZNR1","",0.01);
  RooRealVar p_L2_ZNR1("p_L2_ZNR1","",0.01);

  RooArgList* L_ZNR1 = new RooArgList(a_Bp_ZNR1,p_Bp_ZNR1,
				      a_L0_ZNR1,p_L0_ZNR1,
				      a_L1_ZNR1,p_L1_ZNR1,
				      a_L2_ZNR1,p_L2_ZNR1,"L_ZNR1");
  L_ZNR1->add(m0_ZNR1);
  L_ZNR1->add(width_ZNR1);
  RooRealVar typeZNR1("typeZNR1","",RKM);
  L_ZNR1->add(typeZNR1);//type
  L_ZNR1->add(RooRealConstant::value(1));//2 x spin
  L_ZNR1->add(RooRealConstant::value(-1)); //parity

  //===Lambda(Z) 3/2- fsc
  RooRealVar m0_ZNR3("m0_ZNR3","m0",-1.0);
  RooRealVar width_ZNR3("width_ZNR3","width",0.1,-20,20);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_ZNR3("a_Bp_ZNR3","",0.01,-5,5);
  RooRealVar p_Bp_ZNR3("p_Bp_ZNR3","",0.01,-5,5);
  RooRealVar a_L0_ZNR3("a_L0_ZNR3","",0.01);
  RooRealVar p_L0_ZNR3("p_L0_ZNR3","",0.01);
  RooRealVar a_L1_ZNR3("a_L1_ZNR3","",0.01);
  RooRealVar p_L1_ZNR3("p_L1_ZNR3","",0.01);
  RooRealVar a_L2_ZNR3("a_L2_ZNR3","",0.01);
  RooRealVar p_L2_ZNR3("p_L2_ZNR3","",0.01);

  RooArgList* L_ZNR3 = new RooArgList(a_Bp_ZNR3,p_Bp_ZNR3,
				      a_L0_ZNR3,p_L0_ZNR3,
				      a_L1_ZNR3,p_L1_ZNR3,
				      a_L2_ZNR3,p_L2_ZNR3,"L_ZNR3");
  L_ZNR3->add(m0_ZNR3);
  L_ZNR3->add(width_ZNR3);
  RooRealVar typeZNR3("typeZNR3","",RKM);
  L_ZNR3->add(typeZNR3);//type
  L_ZNR3->add(RooRealConstant::value(3));//2 x spin
  L_ZNR3->add(RooRealConstant::value(-1)); //parity

  //===Lambda(Z) TS
  RooRealVar m0_TZ("m0_TZ","m0",1.872);
  RooRealVar width_TZ("width_TZ","width",0.101);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_TZ("a_Bp_TZ","",0.1,-5,5);
  RooRealVar p_Bp_TZ("p_Bp_TZ","",0.1,-5,5);
  RooRealVar a_L0_TZ("a_L0_TZ","",0.3);
  RooRealVar p_L0_TZ("p_L0_TZ","",0.3);
  RooRealVar a_L1_TZ("a_L1_TZ","",0.3);
  RooRealVar p_L1_TZ("p_L1_TZ","",0.3);
  RooRealVar a_L2_TZ("a_L2_TZ","",0.3);
  RooRealVar p_L2_TZ("p_L2_TZ","",0.3);

  RooArgList* L_TZ = new RooArgList(a_Bp_TZ,p_Bp_TZ,
				      a_L0_TZ,p_L0_TZ,
				      a_L1_TZ,p_L1_TZ,
				      a_L2_TZ,p_L2_TZ,"L_TZ");
  L_TZ->add(m0_TZ);
  L_TZ->add(width_TZ);
  RooRealVar typeTZ("typeTZ","",TRIANGLE);
  L_TZ->add(typeTZ);//type
  L_TZ->add(RooRealConstant::value(3));//2 x spin
  L_TZ->add(RooRealConstant::value(1)); //parity

//Spline Z
  RooRealVar *ZMass[Nsp], *ZRM1[Nsp], *ZIM1[Nsp];
  RooArgList *ZM1wave = new RooArgList("ZM1wave");
  std::ifstream inzwav;
  inzwav.open("toyz1.dat");
 
  iswav=0;
  while(1) {
    inzwav >> xm[iswav];//
    sr0[iswav] =0.01;  sp0[iswav] =0;
    if(!inzwav.good()) break;
    std::cout << xm[iswav] << " " << sr0[iswav] << " " << sp0[iswav] << std::endl;
     iswav++;
   } 
  inzwav.close();
  Npoint = iswav;
  std::cout << "point of ZM1wave " << Npoint << std::endl;
  if(Npoint!=NUMSPZ) {
    std::cout <<  "ZSPline number is wrong " << std::endl;
    assert(0);
  }
//  double sr0[Nsp],  = {};
//  RooDalitzRM1litude dz;
  for(int i=0; i<Npoint; ++i) {
    sprintf(Wname,"ZRM1_%02d",i);
    ZRM1[i] = new RooRealVar(Wname,Wname,sr0[i]);
    ZRM1[i]->setConstant(0);
    ZM1wave->add(*ZRM1[i]);
    sprintf(Wname,"ZIM1_%02d",i);
    ZIM1[i] = new RooRealVar(Wname,Wname,sp0[i]);
    ZIM1[i]->setConstant(0);
    ZM1wave->add(*ZIM1[i]);
    sprintf(Wname,"mZ_%02d",i);
    ZMass[i] = new RooRealVar(Wname,Wname,xm[i]);
    ZM1wave->add(*ZMass[i]);   
    if(ZMass[i]->getVal()<4.2) {
     ZRM1[i]->setVal(0);
     ZIM1[i]->setVal(0);  
     ZRM1[i]->setConstant(1);
     ZIM1[i]->setConstant(1);  
    }     
  }
  ZRM1[NUMSPZ-1]->setVal(0);
  ZIM1[NUMSPZ-1]->setVal(0);  
  
  ZRM1[NUMSPZ-1]->setConstant(1);
  ZIM1[NUMSPZ-1]->setConstant(1);  
  
  RooRealVar a_Bp_ZM1("a_Bp_ZM1","",0.1,-5,5);
  RooRealVar p_Bp_ZM1("p_Bp_ZM1","",0.1,-5,5);
  RooRealVar a_L0_ZM1("a_L0_ZM1","",1.0);
  RooRealVar p_L0_ZM1("p_L0_ZM1","",0.0);
  RooRealVar a_L1_ZM1("a_L1_ZM1","",0.01);
  RooRealVar p_L1_ZM1("p_L1_ZM1","",0.01);
  RooRealVar a_L2_ZM1("a_L2_ZM1","",0.01);
  RooRealVar p_L2_ZM1("p_L2_ZM1","",0.01);

  RooArgList* L_ZM1 = new RooArgList(a_Bp_ZM1,p_Bp_ZM1,
                                      a_L0_ZM1,p_L0_ZM1,
                                      a_L1_ZM1,p_L1_ZM1,
                                      a_L2_ZM1,p_L2_ZM1,"L_ZM1");

  RooRealVar m0_ZM("m0_ZM","m0",-0.5);
  RooRealVar width_ZM("width_ZM","width",0.3);    
  L_ZM1->add(*ZM1wave);
  L_ZM1->add(m0_ZM);
  L_ZM1->add(width_ZM);
  L_ZM1->add(RooRealConstant::value(MODELINDEPENDENT));
  L_ZM1->add(RooRealConstant::value(1.));//2 x spin
  L_ZM1->add(RooRealConstant::value(-1)); //parity
  

  RooRealVar *ZRP1[Nsp], *ZIP1[Nsp];
  RooArgList *ZP1wave = new RooArgList("ZP1wave");

  for(int i=0; i<Npoint; ++i) {
    sprintf(Wname,"ZRP1_%02d",i);
    ZRP1[i] = new RooRealVar(Wname,Wname,sr0[i]);
    ZRP1[i]->setConstant(0);
    ZP1wave->add(*ZRP1[i]);
    sprintf(Wname,"ZIP1_%02d",i);
    ZIP1[i] = new RooRealVar(Wname,Wname,sp0[i]);
    ZIP1[i]->setConstant(0);
    ZP1wave->add(*ZIP1[i]);
    sprintf(Wname,"mZ_%02d",i);
    ZP1wave->add(*ZMass[i]);    
    if(ZMass[i]->getVal()<4.2) {
     ZRP1[i]->setVal(0);
     ZIP1[i]->setVal(0);  
     ZRP1[i]->setConstant(1);
     ZIP1[i]->setConstant(1);  
    }    
  }

  ZRP1[NUMSPZ-1]->setVal(0);
  ZIP1[NUMSPZ-1]->setVal(0);  
  
  ZRP1[NUMSPZ-1]->setConstant(1);
  ZIP1[NUMSPZ-1]->setConstant(1);  
  
  RooRealVar a_Bp_ZP1("a_Bp_ZP1","",0.1,-5,5);
  RooRealVar p_Bp_ZP1("p_Bp_ZP1","",0.1,-5,5);
  RooRealVar a_L0_ZP1("a_L0_ZP1","",1.0);
  RooRealVar p_L0_ZP1("p_L0_ZP1","",0.0);
  RooRealVar a_L1_ZP1("a_L1_ZP1","",0.01);
  RooRealVar p_L1_ZP1("p_L1_ZP1","",0.01);
  RooRealVar a_L2_ZP1("a_L2_ZP1","",0.01);
  RooRealVar p_L2_ZP1("p_L2_ZP1","",0.01);

  RooArgList* L_ZP1 = new RooArgList(a_Bp_ZP1,p_Bp_ZP1,
                                      a_L0_ZP1,p_L0_ZP1,
                                      a_L1_ZP1,p_L1_ZP1,
                                      a_L2_ZP1,p_L2_ZP1,"L_ZP1");

  L_ZP1->add(*ZP1wave);
  L_ZP1->add(m0_ZM);
  L_ZP1->add(width_ZM);
  L_ZP1->add(RooRealConstant::value(MODELINDEPENDENT));
  L_ZP1->add(RooRealConstant::value(1.));//2 x spin
  L_ZP1->add(RooRealConstant::value(1)); //parity

  RooRealVar *ZRP3[Nsp], *ZIP3[Nsp];
  RooArgList *ZP3wave = new RooArgList("ZP3wave");


  for(int i=0; i<Npoint; ++i) {
    sprintf(Wname,"ZRP3_%02d",i);
    ZRP3[i] = new RooRealVar(Wname,Wname,sr0[i]);
    ZRP3[i]->setConstant(0);
    ZP3wave->add(*ZRP3[i]);
    sprintf(Wname,"ZIP3_%02d",i);
    ZIP3[i] = new RooRealVar(Wname,Wname,sp0[i]);
    ZIP3[i]->setConstant(0);
    ZP3wave->add(*ZIP3[i]);
    sprintf(Wname,"mZ_%02d",i);
    ZP3wave->add(*ZMass[i]);    
    if(ZMass[i]->getVal()<4.2) {
     ZRP3[i]->setVal(0);
     ZIP3[i]->setVal(0);  
     ZRP3[i]->setConstant(1);
     ZIP3[i]->setConstant(1);  
    }
  }
  ZRP3[NUMSPZ-1]->setVal(0);
  ZIP3[NUMSPZ-1]->setVal(0);  
  
  ZRP3[NUMSPZ-1]->setConstant(1);
  ZIP3[NUMSPZ-1]->setConstant(1);  
  
  RooRealVar a_Bp_ZP3("a_Bp_ZP3","",0.1,-5,5);
  RooRealVar p_Bp_ZP3("p_Bp_ZP3","",0.1,-5,5);
  RooRealVar a_L0_ZP3("a_L0_ZP3","",1.0);
  RooRealVar p_L0_ZP3("p_L0_ZP3","",0.0);
  RooRealVar a_L1_ZP3("a_L1_ZP3","",0.01);
  RooRealVar p_L1_ZP3("p_L1_ZP3","",0.01);
  RooRealVar a_L2_ZP3("a_L2_ZP3","",0.01);
  RooRealVar p_L2_ZP3("p_L2_ZP3","",0.01);

  RooArgList* L_ZP3 = new RooArgList(a_Bp_ZP3,p_Bp_ZP3,
                                      a_L0_ZP3,p_L0_ZP3,
                                      a_L1_ZP3,p_L1_ZP3,
                                      a_L2_ZP3,p_L2_ZP3,"L_ZP3");

  L_ZP3->add(*ZP3wave);
  L_ZP3->add(m0_ZM);
  L_ZP3->add(width_ZM);
  L_ZP3->add(RooRealConstant::value(MODELINDEPENDENT));
  L_ZP3->add(RooRealConstant::value(3.));//2 x spin
  L_ZP3->add(RooRealConstant::value(1)); //parity

  RooRealVar *ZRM3[Nsp], *ZIM3[Nsp];
  RooArgList *ZM3wave = new RooArgList("ZM3wave");

  for(int i=0; i<Npoint; ++i) {
    sprintf(Wname,"ZRM3_%02d",i);
    ZRM3[i] = new RooRealVar(Wname,Wname,sr0[i]);
    ZRM3[i]->setConstant(0);
    ZM3wave->add(*ZRM3[i]);
    sprintf(Wname,"ZIM3_%02d",i);
    ZIM3[i] = new RooRealVar(Wname,Wname,sp0[i]);
    ZIM3[i]->setConstant(0);
    ZM3wave->add(*ZIM3[i]);
    sprintf(Wname,"mZ_%02d",i);
    ZM3wave->add(*ZMass[i]);    
    if(ZMass[i]->getVal()<4.2) {
     ZRM3[i]->setVal(0);
     ZIM3[i]->setVal(0);  
     ZRM3[i]->setConstant(1);
     ZIM3[i]->setConstant(1);  
    }
  }
  
  ZRM3[NUMSPZ-1]->setVal(0);
  ZIM3[NUMSPZ-1]->setVal(0);  
  ZRM3[NUMSPZ-1]->setConstant(1);
  ZIM3[NUMSPZ-1]->setConstant(1);  
    
  RooRealVar a_Bp_ZM3("a_Bp_ZM3","",0.1,-5,5);
  RooRealVar p_Bp_ZM3("p_Bp_ZM3","",0.1,-5,5);
  RooRealVar a_L0_ZM3("a_L0_ZM3","",1.0);
  RooRealVar p_L0_ZM3("p_L0_ZM3","",0.0);
  RooRealVar a_L1_ZM3("a_L1_ZM3","",0.01);
  RooRealVar p_L1_ZM3("p_L1_ZM3","",0.01);
  RooRealVar a_L2_ZM3("a_L2_ZM3","",0.01);
  RooRealVar p_L2_ZM3("p_L2_ZM3","",0.01);

  RooArgList* L_ZM3 = new RooArgList(a_Bp_ZM3,p_Bp_ZM3,
                                      a_L0_ZM3,p_L0_ZM3,
                                      a_L1_ZM3,p_L1_ZM3,
                                      a_L2_ZM3,p_L2_ZM3,"L_ZM3");

  L_ZM3->add(*ZM3wave);
  L_ZM3->add(m0_ZM);
  L_ZM3->add(width_ZM);
  L_ZM3->add(RooRealConstant::value(MODELINDEPENDENT));
  L_ZM3->add(RooRealConstant::value(3.));//2 x spin
  L_ZM3->add(RooRealConstant::value(-1)); //parity
