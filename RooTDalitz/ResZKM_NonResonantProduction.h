//Z5 4440
//Z2 4500
//Z 4457
//Z4 4312
  int RKMNew=KMATRIX;
  TList *listZ = new TList();
  //===Lambda(Z) 1/2-
  RooRealVar m0_Z("m0_Z","m0",4.440,4.4,4.45);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z("a_Bp_Z","",0.1,-5,5);
  RooRealVar p_Bp_Z("p_Bp_Z","",0.1,-5,5);
  RooRealVar a_L0_Z("a_L0_Z","",1.0);
  RooRealVar p_L0_Z("p_L0_Z","",0.1);
  RooRealVar a_L1_Z("a_L1_Z","",0.01);
  RooRealVar p_L1_Z("p_L1_Z","",0.01);
  RooRealVar a_L2_Z("a_L2_Z","",0.01);
  RooRealVar p_L2_Z("p_L2_Z","",0.01);
  RooArgList* L_Z = new RooArgList(a_Bp_Z,p_Bp_Z,
                                        a_L0_Z,p_L0_Z,
                                        a_L1_Z,p_L1_Z,
                                        a_L2_Z,p_L2_Z,"L_Z");
//four more couplings
  // RooRealVar g1_Z("g1_Z","",0.3,-1,1);
  // RooRealVar rg2_Z("rg2_Z","",0.0);
  // RooRealVar rg3_Z("rg3_Z","",0.);
  // RooRealVar rg4_Z("rg4_Z","",0.01,-50,50);
  // RooFormulaVar g2_Z("g2_Z","@0*@1",RooArgSet(g1_Z,rg2_Z));
  // RooFormulaVar g3_Z("g3_Z","@0*@1",RooArgSet(g1_Z,rg3_Z));
  // RooFormulaVar g4_Z("g4_Z","@0*@1",RooArgSet(g1_Z,rg4_Z));
  RooRealVar g2_Z("g2_Z","",-0.3,-10,10);  //sigma D*
  RooRealVar rg1_Z("rg1_Z","",0.01,-2,2); //jpsi p
  RooRealVar rg3_Z("rg3_Z","",0.); //sigma*D*
  RooRealVar rg4_Z("rg4_Z","",0.0); //sigma D
  RooFormulaVar g1_Z("g1_Z","@0*@1",RooArgSet(g2_Z,rg1_Z));
  RooFormulaVar g3_Z("g3_Z","@0*@1",RooArgSet(g2_Z,rg3_Z));
  RooFormulaVar g4_Z("g4_Z","@0*@1",RooArgSet(g2_Z,rg4_Z));
  L_Z->add(g1_Z);
  L_Z->add(g2_Z);
  L_Z->add(g3_Z);
  L_Z->add(g4_Z);
  L_Z->add(m0_Z);
  L_Z->add(RooRealConstant::value(1.)); // Pc(4457) is close to SigmaC D* channel threshold
  RooRealVar typeZ("typeZ","",RKMNew);
  L_Z->add(typeZ);  //type
  L_Z->add(RooRealConstant::value(ZS));  //2 x spin
  L_Z->add(RooRealConstant::value(ZP));  //parity
  
  
  //===Lambda(Z4) 1/2-
  RooRealVar m0_Z4("m0_Z4","m0",4.312,4.30,4.33);
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
  // RooRealVar g1_Z4("g1_Z4","",0.3,-2,2);  //jpsi p
  // RooRealVar rg2_Z4("rg2_Z4","",0.3,-50,50);  //sigma D*
  // RooRealVar rg3_Z4("rg3_Z4","",0);  //sigma*D*
  // RooRealVar rg4_Z4("rg4_Z4","",0.05,-50,50);  //sigma(*)D
  // RooFormulaVar g2_Z4("g2_Z4","@0*@1",RooArgSet(g1_Z4,rg2_Z4));
  // RooFormulaVar g3_Z4("g3_Z4","@0*@1",RooArgSet(g1_Z4,rg3_Z4));
  // RooFormulaVar g4_Z4("g4_Z4","@0*@1",RooArgSet(g1_Z4,rg4_Z4));
  RooRealVar g4_Z4("g4_Z4","",-0.3,-10,10); //sigma D
  RooRealVar rg1_Z4("rg1_Z4","",0.01,-2,2); //jpsi p
  RooRealVar rg2_Z4("rg2_Z4","",0.);  //sigma D*
  RooRealVar rg3_Z4("rg3_Z4","",0.); //sigma*D*
  RooFormulaVar g1_Z4("g1_Z4","@0*@1",RooArgSet(g4_Z4,rg1_Z4));
  RooFormulaVar g2_Z4("g2_Z4","@0*@1",RooArgSet(g4_Z4,rg2_Z4));
  RooFormulaVar g3_Z4("g3_Z4","@0*@1",RooArgSet(g4_Z4,rg3_Z4));
  L_Z4->add(g1_Z4);
  L_Z4->add(g2_Z4);
  L_Z4->add(g3_Z4);
  L_Z4->add(g4_Z4);
  L_Z4->add(m0_Z4);
  L_Z4->add(RooRealConstant::value(3.)); // Pc(4312) is close to SigmaC D channel threshold
  RooRealVar typeZ4("typeZ4","",RKMNew);
  L_Z4->add(typeZ4);  //type
  L_Z4->add(RooRealConstant::value(ZS4));  //2 x spin
  L_Z4->add(RooRealConstant::value(ZP4));  //parity
 



  // JP = 1/2+ 4150 state. (use in the default model as a RBW structure. JP differ from others)
  RooRealVar m0_Z4150("m0_Z4","m0",4.150);
  RooRealVar a_Bp_Z4150("a_Bp_Z4150","",0.1,-5,5);
  RooRealVar p_Bp_Z4150("p_Bp_Z4150","",0.1,-5,5);
  RooRealVar a_L0_Z4150("a_L0_Z4150","",0.1);
  RooRealVar p_L0_Z4150("p_L0_Z4150","",0.00);
  RooRealVar a_L1_Z4150("a_L1_Z4150","",0.00);
  RooRealVar p_L1_Z4150("p_L1_Z4150","",0.00);
  RooRealVar a_L2_Z4150("a_L2_Z4150","",0.00);
  RooRealVar p_L2_Z4150("p_L2_Z4150","",0.00);
  RooArgList* L_Z4150 = new RooArgList(a_Bp_Z4150,p_Bp_Z4150,
                                      a_L0_Z4150,p_L0_Z4150,
                                      a_L1_Z4150,p_L1_Z4150,
                                      a_L2_Z4150,p_L2_Z4150,"L_Z4150");
  RooRealVar g1_Z4150("g1_Z4150","",0.64500359);  //jpsi p
  RooRealVar rg2_Z4150("rg2_Z4150","",0.0);  //sigma D*
  RooRealVar rg3_Z4150("rg3_Z4150","",0);  //sigma*D*
  RooRealVar rg4_Z4150("rg4_Z4150","",0.0);  //sigma(*)D
  RooFormulaVar g2_Z4150("g2_Z4150","@0*@1",RooArgSet(g1_Z4150,rg2_Z4150));
  RooFormulaVar g3_Z4150("g3_Z4150","@0*@1",RooArgSet(g1_Z4150,rg3_Z4150));
  RooFormulaVar g4_Z4150("g4_Z4150","@0*@1",RooArgSet(g1_Z4150,rg4_Z4150));
  L_Z4150->add(g1_Z4150);
  L_Z4150->add(g2_Z4150);
  L_Z4150->add(g3_Z4150);
  L_Z4150->add(g4_Z4150);
  L_Z4150->add(m0_Z4150);
  L_Z4150->add(RooRealConstant::value(0.)); // Which channel it is coupled to
  RooRealVar typeZ4150("typeZ4150","",RKMNew);
  L_Z4150->add(typeZ4150);  //type
  L_Z4150->add(RooRealConstant::value(1));  //2 x spin
  L_Z4150->add(RooRealConstant::value(1));  //parity

  RooRealVar a_Bp_ZM1SC1("a_Bp_ZM1SC1","",0.1);
  RooRealVar p_Bp_ZM1SC1("p_Bp_ZM1SC1","",0.1);
  RooRealVar a_L0_ZM1SC1("a_L0_ZM1SC1","",0);
  RooRealVar p_L0_ZM1SC1("p_L0_ZM1SC1","",0.00);
  RooRealVar a_L1_ZM1SC1("a_L1_ZM1SC1","",0.00);
  RooRealVar p_L1_ZM1SC1("p_L1_ZM1SC1","",0.00);
  RooRealVar a_L2_ZM1SC1("a_L2_ZM1SC1","",0.00);
  RooRealVar p_L2_ZM1SC1("p_L2_ZM1SC1","",0.00);

  RooArgList* L_ZM1SC1 = new RooArgList(a_Bp_ZM1SC1,p_Bp_ZM1SC1,
                                        a_L0_ZM1SC1,p_L0_ZM1SC1,
                                        a_L1_ZM1SC1,p_L1_ZM1SC1,
                                        a_L2_ZM1SC1,p_L2_ZM1SC1,"L_ZM1SC1");
  RooRealVar fsc_ZM1SC1("fsc_ZM1SC1","",0,-20,20);
  L_ZM1SC1->add(fsc_ZM1SC1);
  L_ZM1SC1->add(RooRealConstant::value(-0.5));
  L_ZM1SC1->add(RooRealConstant::value(0.));
  RooRealVar typeZM1SC1("typeZM1SC1","",RKMNew);
  L_ZM1SC1->add(typeZM1SC1);  //type
  L_ZM1SC1->add(RooRealConstant::value(1));  //2 x spin
  L_ZM1SC1->add(RooRealConstant::value(-1));  //parity
  
  RooRealVar f_prod_ZM1SC2("f_prod_ZM1SC2","",0,-10,10);
  RooFormulaVar a_L0_ZM1SC2("a_L0_ZM1SC2","@0*@1",RooArgSet(f_prod_ZM1SC2,a_L0_ZM1SC1));
  RooFormulaVar p_L0_ZM1SC2("p_L0_ZM1SC2","@0*@1",RooArgSet(f_prod_ZM1SC2,p_L0_ZM1SC1));
  RooFormulaVar a_L1_ZM1SC2("a_L1_ZM1SC2","@0*@1",RooArgSet(f_prod_ZM1SC2,a_L1_ZM1SC1));
  RooFormulaVar p_L1_ZM1SC2("p_L1_ZM1SC2","@0*@1",RooArgSet(f_prod_ZM1SC2,p_L1_ZM1SC1));
  RooFormulaVar a_L2_ZM1SC2("a_L2_ZM1SC2","@0*@1",RooArgSet(f_prod_ZM1SC2,a_L2_ZM1SC1));
  RooFormulaVar p_L2_ZM1SC2("p_L2_ZM1SC2","@0*@1",RooArgSet(f_prod_ZM1SC2,p_L2_ZM1SC1));

  RooArgList* L_ZM1SC2 = new RooArgList(a_Bp_ZM1SC1,p_Bp_ZM1SC1,
                                        a_L0_ZM1SC2,p_L0_ZM1SC2,
                                        a_L1_ZM1SC2,p_L1_ZM1SC2,
                                        a_L2_ZM1SC2,p_L2_ZM1SC2,"L_ZM1SC2");
  RooRealVar fsc_ZM1SC2("fsc_ZM1SC2","",0,-20,20);
  L_ZM1SC2->add(fsc_ZM1SC2);
  L_ZM1SC2->add(RooRealConstant::value(-0.5));
  L_ZM1SC2->add(RooRealConstant::value(0.));
  L_ZM1SC2->add(typeZM1SC1);  //type
  L_ZM1SC2->add(RooRealConstant::value(1));  //2 x spin
  L_ZM1SC2->add(RooRealConstant::value(-1));  //parity

  RooRealVar f_prod_ZM1SC3("f_prod_ZM1SC3","",0,-10,10);
  RooFormulaVar a_L0_ZM1SC3("a_L0_ZM1SC3","@0*@1",RooArgSet(f_prod_ZM1SC3,a_L0_ZM1SC1));
  RooFormulaVar p_L0_ZM1SC3("p_L0_ZM1SC3","@0*@1",RooArgSet(f_prod_ZM1SC3,p_L0_ZM1SC1));
  RooFormulaVar a_L1_ZM1SC3("a_L1_ZM1SC3","@0*@1",RooArgSet(f_prod_ZM1SC3,a_L1_ZM1SC1));
  RooFormulaVar p_L1_ZM1SC3("p_L1_ZM1SC3","@0*@1",RooArgSet(f_prod_ZM1SC3,p_L1_ZM1SC1));
  RooFormulaVar a_L2_ZM1SC3("a_L2_ZM1SC3","@0*@1",RooArgSet(f_prod_ZM1SC3,a_L2_ZM1SC1));
  RooFormulaVar p_L2_ZM1SC3("p_L2_ZM1SC3","@0*@1",RooArgSet(f_prod_ZM1SC3,p_L2_ZM1SC1));

  RooArgList* L_ZM1SC3 = new RooArgList(a_Bp_ZM1SC1,p_Bp_ZM1SC1,
                                        a_L0_ZM1SC3,p_L0_ZM1SC3,
                                        a_L1_ZM1SC3,p_L1_ZM1SC3,
                                        a_L2_ZM1SC3,p_L2_ZM1SC3,"L_ZM1SC3");
  RooRealVar fsc_ZM1SC3("fsc_ZM1SC3","",0,-20,20);
  L_ZM1SC3->add(fsc_ZM1SC3);
  L_ZM1SC3->add(RooRealConstant::value(-0.5));
  L_ZM1SC3->add(RooRealConstant::value(0.));
  L_ZM1SC3->add(typeZM1SC1);  //type
  L_ZM1SC3->add(RooRealConstant::value(1));  //2 x spin
  L_ZM1SC3->add(RooRealConstant::value(-1));  //parity
  
  RooRealVar f_prod_ZM1SC4("f_prod_ZM1SC4","",0,-10,10);
  RooFormulaVar a_L0_ZM1SC4("a_L0_ZM1SC4","@0*@1",RooArgSet(f_prod_ZM1SC4,a_L0_ZM1SC1));
  RooFormulaVar p_L0_ZM1SC4("p_L0_ZM1SC4","@0*@1",RooArgSet(f_prod_ZM1SC4,p_L0_ZM1SC1));
  RooFormulaVar a_L1_ZM1SC4("a_L1_ZM1SC4","@0*@1",RooArgSet(f_prod_ZM1SC4,a_L1_ZM1SC1));
  RooFormulaVar p_L1_ZM1SC4("p_L1_ZM1SC4","@0*@1",RooArgSet(f_prod_ZM1SC4,p_L1_ZM1SC1));
  RooFormulaVar a_L2_ZM1SC4("a_L2_ZM1SC4","@0*@1",RooArgSet(f_prod_ZM1SC4,a_L2_ZM1SC1));
  RooFormulaVar p_L2_ZM1SC4("p_L2_ZM1SC4","@0*@1",RooArgSet(f_prod_ZM1SC4,p_L2_ZM1SC1));

  RooArgList* L_ZM1SC4 = new RooArgList(a_Bp_ZM1SC1,p_Bp_ZM1SC1,
                                        a_L0_ZM1SC4,p_L0_ZM1SC4,
                                        a_L1_ZM1SC4,p_L1_ZM1SC4,
                                        a_L2_ZM1SC4,p_L2_ZM1SC4,"L_ZM1SC4");
  RooRealVar fsc_ZM1SC4("fsc_ZM1SC4","",0,-20,20);
  L_ZM1SC4->add(fsc_ZM1SC4);
  L_ZM1SC4->add(RooRealConstant::value(-0.5));
  L_ZM1SC4->add(RooRealConstant::value(0.));
  L_ZM1SC4->add(typeZM1SC1);  //type
  L_ZM1SC4->add(RooRealConstant::value(1));  //2 x spin
  L_ZM1SC4->add(RooRealConstant::value(-1));  //parity
  
  
  
  //===Lambda(Z) 3/2-
  RooRealVar m0_Z5("m0_Z5","m0",4.312,4.30,4.33);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z5("a_Bp_Z5","",0.1,-5,5);
  RooRealVar p_Bp_Z5("p_Bp_Z5","",0.1,-5,5);
  RooRealVar a_L0_Z5("a_L0_Z5","",1);
  RooRealVar p_L0_Z5("p_L0_Z5","",0.00);
  RooRealVar a_L1_Z5("a_L1_Z5","",0.00);
  RooRealVar p_L1_Z5("p_L1_Z5","",0.00);
  RooRealVar a_L2_Z5("a_L2_Z5","",0.00);
  RooRealVar p_L2_Z5("p_L2_Z5","",0.00);

  RooArgList* L_Z5 = new RooArgList(a_Bp_Z5,p_Bp_Z5,
                                        a_L0_Z5,p_L0_Z5,
                                        a_L1_Z5,p_L1_Z5,
                                        a_L2_Z5,p_L2_Z5,"L_Z5");
  RooRealVar g2_Z5("g2_Z5","",-0.3,-10,10);  //sigma D*
  RooRealVar rg1_Z5("rg1_Z5","",0.01,-2,2); //jpsi p
  RooRealVar rg3_Z5("rg3_Z5","",0.); //sigma*D*
  RooRealVar rg4_Z5("rg4_Z5","",0.0); //sigma D
  RooFormulaVar g1_Z5("g1_Z5","@0*@1",RooArgSet(g2_Z5,rg1_Z5));
  RooFormulaVar g3_Z5("g3_Z5","@0*@1",RooArgSet(g2_Z5,rg3_Z5));
  RooFormulaVar g4_Z5("g4_Z5","@0*@1",RooArgSet(g2_Z5,rg4_Z5));
  L_Z5->add(g1_Z5);
  L_Z5->add(g2_Z5);
  L_Z5->add(g3_Z5);
  L_Z5->add(g4_Z5);
  L_Z5->add(m0_Z5);
  L_Z5->add(RooRealConstant::value(1.)); // Pc(4440) is close to SigmaC D* channel threshold
  RooRealVar typeZ5("typeZ5","",RKMNew);
  L_Z5->add(typeZ5);  //type
  L_Z5->add(RooRealConstant::value(ZS5));  //2 x spin
  L_Z5->add(RooRealConstant::value(ZP5));  //parity


  //===Lambda(Z) 3/2-, couples to Sigmac*D
  RooRealVar m0_Z6("m0_Z6","m0",4.385,4.30,4.45);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z6("a_Bp_Z6","",0.1,-5,5);
  RooRealVar p_Bp_Z6("p_Bp_Z6","",0.1,-5,5);
  RooRealVar a_L0_Z6("a_L0_Z6","",1);
  RooRealVar p_L0_Z6("p_L0_Z6","",0.00);
  RooRealVar a_L1_Z6("a_L1_Z6","",0.00);
  RooRealVar p_L1_Z6("p_L1_Z6","",0.00);
  RooRealVar a_L2_Z6("a_L2_Z6","",0.00);
  RooRealVar p_L2_Z6("p_L2_Z6","",0.00);

  RooArgList* L_Z6 = new RooArgList(a_Bp_Z6,p_Bp_Z6,
              a_L0_Z6,p_L0_Z6,
              a_L1_Z6,p_L1_Z6,
              a_L2_Z6,p_L2_Z6,"L_Z6");
  RooRealVar g4_Z6("g4_Z6","",-0.3,-10,10);  //sigma* D
  RooRealVar rg1_Z6("rg1_Z6","",0.01,-2,2); //jpsi p
  RooRealVar rg2_Z6("rg2_Z6","",0.0); //sigma D*
  RooRealVar rg3_Z6("rg3_Z6","",0.); //sigma*D*
  RooFormulaVar g1_Z6("g1_Z6","@0*@1",RooArgSet(g4_Z6,rg1_Z6));
  RooFormulaVar g2_Z6("g2_Z6","@0*@1",RooArgSet(g4_Z6,rg2_Z6));
  RooFormulaVar g3_Z6("g3_Z6","@0*@1",RooArgSet(g4_Z6,rg3_Z6));
  L_Z6->add(g1_Z6);
  L_Z6->add(g2_Z6);
  L_Z6->add(g3_Z6);
  L_Z6->add(g4_Z6);
  L_Z6->add(m0_Z6);
  L_Z6->add(RooRealConstant::value(2.)); // Pc(4440) is close to SigmaC D* channel threshold
  RooRealVar typeZ6("typeZ6","",RKMNew);
  L_Z6->add(typeZ6);  //type
  L_Z6->add(RooRealConstant::value(ZS6));  //2 x spin
  L_Z6->add(RooRealConstant::value(ZP6));  //parity

  RooRealVar a_Bp_ZM3SC1("a_Bp_ZM3SC1","",0.1);
  RooRealVar p_Bp_ZM3SC1("p_Bp_ZM3SC1","",0.1);
  RooRealVar a_L0_ZM3SC1("a_L0_ZM3SC1","",0);
  RooRealVar p_L0_ZM3SC1("p_L0_ZM3SC1","",0.00);
  RooRealVar a_L1_ZM3SC1("a_L1_ZM3SC1","",0.00);
  RooRealVar p_L1_ZM3SC1("p_L1_ZM3SC1","",0.00);
  RooRealVar a_L2_ZM3SC1("a_L2_ZM3SC1","",0.00);
  RooRealVar p_L2_ZM3SC1("p_L2_ZM3SC1","",0.00);

  RooArgList* L_ZM3SC1 = new RooArgList(a_Bp_ZM3SC1,p_Bp_ZM3SC1,
                                        a_L0_ZM3SC1,p_L0_ZM3SC1,
                                        a_L1_ZM3SC1,p_L1_ZM3SC1,
                                        a_L2_ZM3SC1,p_L2_ZM3SC1,"L_ZM3SC1");
  RooRealVar fsc_ZM3SC1("fsc_ZM3SC1","",0,-20,20);
  L_ZM3SC1->add(fsc_ZM3SC1);
  L_ZM3SC1->add(RooRealConstant::value(-0.5));
  L_ZM3SC1->add(RooRealConstant::value(0.));
  RooRealVar typeZM3SC1("typeZM3SC1","",RKMNew);
  L_ZM3SC1->add(typeZM3SC1);  //type
  L_ZM3SC1->add(RooRealConstant::value(3));  //2 x spin
  L_ZM3SC1->add(RooRealConstant::value(-1));  //parity
  
  RooRealVar f_prod_ZM3SC2("f_prod_ZM3SC2","",0,-10,10);
  RooFormulaVar a_L0_ZM3SC2("a_L0_ZM3SC2","","@0*@1",RooArgSet(f_prod_ZM3SC2,a_L0_ZM3SC1));
  RooFormulaVar p_L0_ZM3SC2("p_L0_ZM3SC2","","@0*@1",RooArgSet(f_prod_ZM3SC2,p_L0_ZM3SC1));
  RooFormulaVar a_L1_ZM3SC2("a_L1_ZM3SC2","","@0*@1",RooArgSet(f_prod_ZM3SC2,a_L1_ZM3SC1));
  RooFormulaVar p_L1_ZM3SC2("p_L1_ZM3SC2","","@0*@1",RooArgSet(f_prod_ZM3SC2,p_L1_ZM3SC1));
  RooFormulaVar a_L2_ZM3SC2("a_L2_ZM3SC2","","@0*@1",RooArgSet(f_prod_ZM3SC2,a_L2_ZM3SC1));
  RooFormulaVar p_L2_ZM3SC2("p_L2_ZM3SC2","","@0*@1",RooArgSet(f_prod_ZM3SC2,p_L2_ZM3SC1));

  RooArgList* L_ZM3SC2 = new RooArgList(a_Bp_ZM3SC1,p_Bp_ZM3SC1,
                                        a_L0_ZM3SC2,p_L0_ZM3SC2,
                                        a_L1_ZM3SC2,p_L1_ZM3SC2,
                                        a_L2_ZM3SC2,p_L2_ZM3SC2,"L_ZM3SC2");
  RooRealVar fsc_ZM3SC2("fsc_ZM3SC2","",0,-20,20);
  L_ZM3SC2->add(fsc_ZM3SC2);
  L_ZM3SC2->add(RooRealConstant::value(-0.5));
  L_ZM3SC2->add(RooRealConstant::value(0.));
  L_ZM3SC2->add(typeZM3SC1);  //type
  L_ZM3SC2->add(RooRealConstant::value(3));  //2 x spin
  L_ZM3SC2->add(RooRealConstant::value(-1));  //parity
  
  RooRealVar f_prod_ZM3SC3("f_prod_ZM3SC3","",0,-10,10);
  RooFormulaVar a_L0_ZM3SC3("a_L0_ZM3SC3","","@0*@1",RooArgSet(f_prod_ZM3SC3,a_L0_ZM3SC1));
  RooFormulaVar p_L0_ZM3SC3("p_L0_ZM3SC3","","@0*@1",RooArgSet(f_prod_ZM3SC3,p_L0_ZM3SC1));
  RooFormulaVar a_L1_ZM3SC3("a_L1_ZM3SC3","","@0*@1",RooArgSet(f_prod_ZM3SC3,a_L1_ZM3SC1));
  RooFormulaVar p_L1_ZM3SC3("p_L1_ZM3SC3","","@0*@1",RooArgSet(f_prod_ZM3SC3,p_L1_ZM3SC1));
  RooFormulaVar a_L2_ZM3SC3("a_L2_ZM3SC3","","@0*@1",RooArgSet(f_prod_ZM3SC3,a_L2_ZM3SC1));
  RooFormulaVar p_L2_ZM3SC3("p_L2_ZM3SC3","","@0*@1",RooArgSet(f_prod_ZM3SC3,p_L2_ZM3SC1));

  RooArgList* L_ZM3SC3 = new RooArgList(a_Bp_ZM3SC1,p_Bp_ZM3SC1,
                                        a_L0_ZM3SC3,p_L0_ZM3SC3,
                                        a_L1_ZM3SC3,p_L1_ZM3SC3,
                                        a_L2_ZM3SC3,p_L2_ZM3SC3,"L_ZM3SC3");
  RooRealVar fsc_ZM3SC3("fsc_ZM3SC3","",0,-20,20);
  L_ZM3SC3->add(fsc_ZM3SC3);
  L_ZM3SC3->add(RooRealConstant::value(-0.5));
  L_ZM3SC3->add(RooRealConstant::value(0.));
  L_ZM3SC3->add(typeZM3SC1);  //type
  L_ZM3SC3->add(RooRealConstant::value(3));  //2 x spin
  L_ZM3SC3->add(RooRealConstant::value(-1));  //parity
  
  RooRealVar f_prod_ZM3SC4("f_prod_ZM3SC4","",0,-10,10);
  RooFormulaVar a_L0_ZM3SC4("a_L0_ZM3SC4","","@0*@1",RooArgSet(f_prod_ZM3SC4,a_L0_ZM3SC1));
  RooFormulaVar p_L0_ZM3SC4("p_L0_ZM3SC4","","@0*@1",RooArgSet(f_prod_ZM3SC4,p_L0_ZM3SC1));
  RooFormulaVar a_L1_ZM3SC4("a_L1_ZM3SC4","","@0*@1",RooArgSet(f_prod_ZM3SC4,a_L1_ZM3SC1));
  RooFormulaVar p_L1_ZM3SC4("p_L1_ZM3SC4","","@0*@1",RooArgSet(f_prod_ZM3SC4,p_L1_ZM3SC1));
  RooFormulaVar a_L2_ZM3SC4("a_L2_ZM3SC4","","@0*@1",RooArgSet(f_prod_ZM3SC4,a_L2_ZM3SC1));
  RooFormulaVar p_L2_ZM3SC4("p_L2_ZM3SC4","","@0*@1",RooArgSet(f_prod_ZM3SC4,p_L2_ZM3SC1));

  RooArgList* L_ZM3SC4 = new RooArgList(a_Bp_ZM3SC1,p_Bp_ZM3SC1,
                                        a_L0_ZM3SC4,p_L0_ZM3SC4,
                                        a_L1_ZM3SC4,p_L1_ZM3SC4,
                                        a_L2_ZM3SC4,p_L2_ZM3SC4,"L_ZM3SC4");
  RooRealVar fsc_ZM3SC4("fsc_ZM3SC4","",0,-20,20);
  L_ZM3SC4->add(fsc_ZM3SC4);
  L_ZM3SC4->add(RooRealConstant::value(-0.5));
  L_ZM3SC4->add(RooRealConstant::value(0.));
  L_ZM3SC4->add(typeZM3SC1);  //type
  L_ZM3SC4->add(RooRealConstant::value(3));  //2 x spin
  L_ZM3SC4->add(RooRealConstant::value(-1));  //parity

  //===Lambda(Z2) 1/2-
  RooRealVar m0_Z2("m0_Z2","m0",4.5,4.48,4.55);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z2("a_Bp_Z2","",0.);
  RooRealVar p_Bp_Z2("p_Bp_Z2","",0.);
  RooRealVar a_L0_Z2("a_L0_Z2","",0);
  RooRealVar p_L0_Z2("p_L0_Z2","",0.00);
  RooRealVar a_L1_Z2("a_L1_Z2","",0.00);
  RooRealVar p_L1_Z2("p_L1_Z2","",0.00);
  RooRealVar a_L2_Z2("a_L2_Z2","",0.00);
  RooRealVar p_L2_Z2("p_L2_Z2","",0.00);

  RooArgList* L_Z2 = new RooArgList(a_Bp_Z2,p_Bp_Z2,
                                        a_L0_Z2,p_L0_Z2,
                                        a_L1_Z2,p_L1_Z2,
                                        a_L2_Z2,p_L2_Z2,"L_Z2");
  RooRealVar g1_Z2("g1_Z2","",0.3,-1,1);  //jpsi p
  RooRealVar rg2_Z2("rg2_Z2","",0.0);  //sigma D*
  RooRealVar rg3_Z2("rg3_Z2","",-50,50);  //sigma*D*
  RooRealVar rg4_Z2("rg4_Z2","",0.0);  //sigma(*)D
  RooFormulaVar g2_Z2("g2_Z2","@0*@1",RooArgSet(g1_Z2,rg2_Z2));
  RooFormulaVar g3_Z2("g3_Z2","@0*@1",RooArgSet(g1_Z2,rg3_Z2));
  RooFormulaVar g4_Z2("g4_Z2","@0*@1",RooArgSet(g1_Z2,rg4_Z2));
  L_Z2->add(g1_Z2);
  L_Z2->add(g2_Z2);
  L_Z2->add(g3_Z2);
  L_Z2->add(g4_Z2);
  L_Z2->add(m0_Z2);
  L_Z2->add(RooRealConstant::value(0.)); // Pc(4500) is not close to any channel threshold
  RooRealVar typeZ2("typeZ2","",RKMNew);
  L_Z2->add(typeZ2);  //type                                        
  L_Z2->add(RooRealConstant::value(ZS2));  //2 x spin
  L_Z2->add(RooRealConstant::value(ZP2));  //parity
  
  int RKM=REVISEDKMATRIX;
  
  RooRealVar width_Z2("width_Z2","width",0.05,0.003,0.3);

  RooArgList* L_Z2BW = new RooArgList(a_Bp_Z2,p_Bp_Z2,
                                      a_L0_Z2,p_L0_Z2,
                                      a_L1_Z2,p_L1_Z2,
                                      a_L2_Z2,p_L2_Z2,"L_Z2BW");
  L_Z2BW->add(m0_Z2);
  L_Z2BW->add(width_Z2);
  RooRealVar typeZ2BW("typeZ2BW","",RKM);
  L_Z2BW->add(typeZ2BW);
//  L_Z2->add(RooRealConstant::value(1));  //  
  L_Z2BW->add(RooRealConstant::value(ZS2));  //2 x spin
  L_Z2BW->add(RooRealConstant::value(ZP2));  //parity
  
  //===Lambda(Z3) 1/2-
  RooRealVar m0_Z3("m0_Z3","m0",4.378,4.34,4.4);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Z3("a_Bp_Z3","",0.1);
  RooRealVar p_Bp_Z3("p_Bp_Z3","",0.1);
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
  RooRealVar g1_Z3("g1_Z3","",0.3,-5,5);  //jpsi p
  RooRealVar rg2_Z3("rg2_Z3","",0.0);  //sigma D*
  RooRealVar rg3_Z3("rg3_Z3","",0);  //sigma*D*
  RooRealVar rg4_Z3("rg4_Z3","",0.0);  //sigma(*)D
  RooFormulaVar g2_Z3("g2_Z3","@0*@1",RooArgSet(g1_Z3,rg2_Z3));
  RooFormulaVar g3_Z3("g3_Z3","@0*@1",RooArgSet(g1_Z3,rg3_Z3));
  RooFormulaVar g4_Z3("g4_Z3","@0*@1",RooArgSet(g1_Z3,rg4_Z3));
  L_Z3->add(g1_Z3);
  L_Z3->add(g2_Z3);
  L_Z3->add(g3_Z3);
  L_Z3->add(g4_Z3);
  L_Z3->add(m0_Z3);
  L_Z3->add(RooRealConstant::value(0.)); // Which channel it is coupled to
  RooRealVar typeZ3("typeZ3","",RKMNew);
  L_Z3->add(typeZ3);  //type
  L_Z3->add(RooRealConstant::value(ZS3));  //2 x spin
  L_Z3->add(RooRealConstant::value(ZP3));  //parity
 


//Add several additional Pc states with fixed JP

  RooRealVar m0_Zm1_1("m0_Zm1_1","m0",4.2,4.0,5.2);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Zm1_1("a_Bp_Zm1_1","",0.001,-5,5);
  RooRealVar p_Bp_Zm1_1("p_Bp_Zm1_1","",0.001,-5,5);
  RooRealVar a_L0_Zm1_1("a_L0_Zm1_1","",0.001);
  RooRealVar p_L0_Zm1_1("p_L0_Zm1_1","",0.001);
  RooRealVar a_L1_Zm1_1("a_L1_Zm1_1","",0.001);
  RooRealVar p_L1_Zm1_1("p_L1_Zm1_1","",0.001);
  RooRealVar a_L2_Zm1_1("a_L2_Zm1_1","",0.001);
  RooRealVar p_L2_Zm1_1("p_L2_Zm1_1","",0.001);

  RooArgList* L_Zm1_1 = new RooArgList(a_Bp_Zm1_1,p_Bp_Zm1_1,
                                      a_L0_Zm1_1,p_L0_Zm1_1,
                                      a_L1_Zm1_1,p_L1_Zm1_1,
                                      a_L2_Zm1_1,p_L2_Zm1_1,"L_Zm1_1");
  RooRealVar g1_Zm1_1("g1_Zm1_1","",0.3,-5,5);  //jpsi p
  RooRealVar rg2_Zm1_1("rg2_Zm1_1","",0);  //sigma D*
  RooRealVar rg3_Zm1_1("rg3_Zm1_1","",0);  //sigma*D*
  RooRealVar rg4_Zm1_1("rg4_Zm1_1","",0);  //sigma(*)D
  RooFormulaVar g2_Zm1_1("g2_Zm1_1","@0*@1",RooArgSet(g1_Zm1_1,rg2_Zm1_1));
  RooFormulaVar g3_Zm1_1("g3_Zm1_1","@0*@1",RooArgSet(g1_Zm1_1,rg3_Zm1_1));
  RooFormulaVar g4_Zm1_1("g4_Zm1_1","@0*@1",RooArgSet(g1_Zm1_1,rg4_Zm1_1));
  L_Zm1_1->add(g1_Zm1_1);
  L_Zm1_1->add(g2_Zm1_1);
  L_Zm1_1->add(g3_Zm1_1);
  L_Zm1_1->add(g4_Zm1_1);
  L_Zm1_1->add(m0_Zm1_1);
  L_Zm1_1->add(RooRealConstant::value(0.));
  RooRealVar typeZm1_1("typeZm1_1","",RKMNew);
  L_Zm1_1->add(typeZm1_1);  //type
  L_Zm1_1->add(RooRealConstant::value(1));  //2 x spin
  L_Zm1_1->add(RooRealConstant::value(-1));  //parity



  RooRealVar m0_Zm1_2("m0_Zm1_2","m0",4.6,4.0,5.2);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Zm1_2("a_Bp_Zm1_2","",0.001,-5,5);
  RooRealVar p_Bp_Zm1_2("p_Bp_Zm1_2","",0.001,-5,5);
  RooRealVar a_L0_Zm1_2("a_L0_Zm1_2","",0.001);
  RooRealVar p_L0_Zm1_2("p_L0_Zm1_2","",0.001);
  RooRealVar a_L1_Zm1_2("a_L1_Zm1_2","",0.001);
  RooRealVar p_L1_Zm1_2("p_L1_Zm1_2","",0.001);
  RooRealVar a_L2_Zm1_2("a_L2_Zm1_2","",0.001);
  RooRealVar p_L2_Zm1_2("p_L2_Zm1_2","",0.001);

  RooArgList* L_Zm1_2 = new RooArgList(a_Bp_Zm1_2,p_Bp_Zm1_2,
                                      a_L0_Zm1_2,p_L0_Zm1_2,
                                      a_L1_Zm1_2,p_L1_Zm1_2,
                                      a_L2_Zm1_2,p_L2_Zm1_2,"L_Zm1_2");
  RooRealVar g1_Zm1_2("g1_Zm1_2","",0.3,-5,5);  //jpsi p
  RooRealVar rg2_Zm1_2("rg2_Zm1_2","",0);  //sigma D*
  RooRealVar rg3_Zm1_2("rg3_Zm1_2","",0);  //sigma*D*
  RooRealVar rg4_Zm1_2("rg4_Zm1_2","",0);  //sigma(*)D
  RooFormulaVar g2_Zm1_2("g2_Zm1_2","@0*@1",RooArgSet(g1_Zm1_2,rg2_Zm1_2));
  RooFormulaVar g3_Zm1_2("g3_Zm1_2","@0*@1",RooArgSet(g1_Zm1_2,rg3_Zm1_2));
  RooFormulaVar g4_Zm1_2("g4_Zm1_2","@0*@1",RooArgSet(g1_Zm1_2,rg4_Zm1_2));
  L_Zm1_2->add(g1_Zm1_2);
  L_Zm1_2->add(g2_Zm1_2);
  L_Zm1_2->add(g3_Zm1_2);
  L_Zm1_2->add(g4_Zm1_2);
  L_Zm1_2->add(m0_Zm1_2);
  L_Zm1_2->add(RooRealConstant::value(0.));
  RooRealVar typeZm1_2("typeZm1_2","",RKMNew);
  L_Zm1_2->add(typeZm1_2);  //type
  L_Zm1_2->add(RooRealConstant::value(1));  //2 x spin
  L_Zm1_2->add(RooRealConstant::value(-1));  //parity


  RooRealVar m0_Zm3_1("m0_Zm3_1","m0",4.2,4.0,5.2);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Zm3_1("a_Bp_Zm3_1","",0.001,-5,5);
  RooRealVar p_Bp_Zm3_1("p_Bp_Zm3_1","",0.001,-5,5);
  RooRealVar a_L0_Zm3_1("a_L0_Zm3_1","",0.001);
  RooRealVar p_L0_Zm3_1("p_L0_Zm3_1","",0.001);
  RooRealVar a_L1_Zm3_1("a_L1_Zm3_1","",0.001);
  RooRealVar p_L1_Zm3_1("p_L1_Zm3_1","",0.001);
  RooRealVar a_L2_Zm3_1("a_L2_Zm3_1","",0.001);
  RooRealVar p_L2_Zm3_1("p_L2_Zm3_1","",0.001);

  RooArgList* L_Zm3_1 = new RooArgList(a_Bp_Zm3_1,p_Bp_Zm3_1,
                                      a_L0_Zm3_1,p_L0_Zm3_1,
                                      a_L1_Zm3_1,p_L1_Zm3_1,
                                      a_L2_Zm3_1,p_L2_Zm3_1,"L_Zm3_1");
  RooRealVar g1_Zm3_1("g1_Zm3_1","",0.3,-5,5);  //jpsi p
  RooRealVar rg2_Zm3_1("rg2_Zm3_1","",0);  //sigma D*
  RooRealVar rg3_Zm3_1("rg3_Zm3_1","",0);  //sigma*D*
  RooRealVar rg4_Zm3_1("rg4_Zm3_1","",0);  //sigma(*)D
  RooFormulaVar g2_Zm3_1("g2_Zm3_1","@0*@1",RooArgSet(g1_Zm3_1,rg2_Zm3_1));
  RooFormulaVar g3_Zm3_1("g3_Zm3_1","@0*@1",RooArgSet(g1_Zm3_1,rg3_Zm3_1));
  RooFormulaVar g4_Zm3_1("g4_Zm3_1","@0*@1",RooArgSet(g1_Zm3_1,rg4_Zm3_1));
  L_Zm3_1->add(g1_Zm3_1);
  L_Zm3_1->add(g2_Zm3_1);
  L_Zm3_1->add(g3_Zm3_1);
  L_Zm3_1->add(g4_Zm3_1);
  L_Zm3_1->add(m0_Zm3_1);
  L_Zm3_1->add(RooRealConstant::value(0.));
  RooRealVar typeZm3_1("typeZm3_1","",RKMNew);
  L_Zm3_1->add(typeZm3_1);  //type
  L_Zm3_1->add(RooRealConstant::value(3));  //2 x spin
  L_Zm3_1->add(RooRealConstant::value(-1));  //parity



  RooRealVar m0_Zm3_2("m0_Zm3_2","m0",4.6,4.0,5.2);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_Bp_Zm3_2("a_Bp_Zm3_2","",0.001,-5,5);
  RooRealVar p_Bp_Zm3_2("p_Bp_Zm3_2","",0.001,-5,5);
  RooRealVar a_L0_Zm3_2("a_L0_Zm3_2","",0.001);
  RooRealVar p_L0_Zm3_2("p_L0_Zm3_2","",0.001);
  RooRealVar a_L1_Zm3_2("a_L1_Zm3_2","",0.001);
  RooRealVar p_L1_Zm3_2("p_L1_Zm3_2","",0.001);
  RooRealVar a_L2_Zm3_2("a_L2_Zm3_2","",0.001);
  RooRealVar p_L2_Zm3_2("p_L2_Zm3_2","",0.001);

  RooArgList* L_Zm3_2 = new RooArgList(a_Bp_Zm3_2,p_Bp_Zm3_2,
                                      a_L0_Zm3_2,p_L0_Zm3_2,
                                      a_L1_Zm3_2,p_L1_Zm3_2,
                                      a_L2_Zm3_2,p_L2_Zm3_2,"L_Zm3_2");
  RooRealVar g1_Zm3_2("g1_Zm3_2","",0.3,-5,5);  //jpsi p
  RooRealVar rg2_Zm3_2("rg2_Zm3_2","",0);  //sigma D*
  RooRealVar rg3_Zm3_2("rg3_Zm3_2","",0);  //sigma*D*
  RooRealVar rg4_Zm3_2("rg4_Zm3_2","",0);  //sigma(*)D
  RooFormulaVar g2_Zm3_2("g2_Zm3_2","@0*@1",RooArgSet(g1_Zm3_2,rg2_Zm3_2));
  RooFormulaVar g3_Zm3_2("g3_Zm3_2","@0*@1",RooArgSet(g1_Zm3_2,rg3_Zm3_2));
  RooFormulaVar g4_Zm3_2("g4_Zm3_2","@0*@1",RooArgSet(g1_Zm3_2,rg4_Zm3_2));
  L_Zm3_2->add(g1_Zm3_2);
  L_Zm3_2->add(g2_Zm3_2);
  L_Zm3_2->add(g3_Zm3_2);
  L_Zm3_2->add(g4_Zm3_2);
  L_Zm3_2->add(m0_Zm3_2);
  L_Zm3_2->add(RooRealConstant::value(0.));
  RooRealVar typeZm3_2("typeZm3_2","",RKMNew);
  L_Zm3_2->add(typeZm3_2);  //type
  L_Zm3_2->add(RooRealConstant::value(3));  //2 x spin
  L_Zm3_2->add(RooRealConstant::value(-1));  //parity

  //===Non-resonant 1/2+
  RooRealVar a_Bp_ZNR1p("a_Bp_ZNR1p","",1.0);
  RooRealVar p_Bp_ZNR1p("p_Bp_ZNR1p","",1.0);
  RooRealVar a_L0_ZNR1p("a_L0_ZNR1p","",0.001);
  RooRealVar p_L0_ZNR1p("p_L0_ZNR1p","",0.001);
  RooRealVar a_L1_ZNR1p("a_L1_ZNR1p","",0.001);
  RooRealVar p_L1_ZNR1p("p_L1_ZNR1p","",0.001);
  RooRealVar a_L2_ZNR1p("a_L2_ZNR1p","",0.000);
  RooRealVar p_L2_ZNR1p("p_L2_ZNR1p","",0.000);

  RooArgList* L_ZNR1p = new RooArgList(a_Bp_ZNR1p,p_Bp_ZNR1p,
              a_L0_ZNR1p,p_L0_ZNR1p,
              a_L1_ZNR1p,p_L1_ZNR1p,
              a_L2_ZNR1p,p_L2_ZNR1p,"L_ZNR1p");
  L_ZNR1p->add(RooRealConstant::value(0.0));
  L_ZNR1p->add(RooRealConstant::value(-1.0));
  L_ZNR1p->add(RooRealConstant::value(-1.0));
  RooRealVar typeZNR1p("typeZNR1p","",NONRESONANT);
  L_ZNR1p->add(typeZNR1p);  //type
  L_ZNR1p->add(RooRealConstant::value(1));  //2 x spin
  L_ZNR1p->add(RooRealConstant::value(1));  //parity

  //===Non-resonant 3/2+
  RooRealVar a_Bp_ZNR3p("a_Bp_ZNR3p","",1.0);
  RooRealVar p_Bp_ZNR3p("p_Bp_ZNR3p","",1.0);
  RooRealVar a_L0_ZNR3p("a_L0_ZNR3p","",0.001);
  RooRealVar p_L0_ZNR3p("p_L0_ZNR3p","",0.001);
  RooRealVar a_L1_ZNR3p("a_L1_ZNR3p","",0.001);
  RooRealVar p_L1_ZNR3p("p_L1_ZNR3p","",0.001);
  RooRealVar a_L2_ZNR3p("a_L2_ZNR3p","",0.001);
  RooRealVar p_L2_ZNR3p("p_L2_ZNR3p","",0.001);

  RooArgList* L_ZNR3p = new RooArgList(a_Bp_ZNR3p,p_Bp_ZNR3p,
              a_L0_ZNR3p,p_L0_ZNR3p,
              a_L1_ZNR3p,p_L1_ZNR3p,
              a_L2_ZNR3p,p_L2_ZNR3p,"L_ZNR3p");
  L_ZNR3p->add(RooRealConstant::value(0.0));
  L_ZNR3p->add(RooRealConstant::value(-1.0));
  L_ZNR3p->add(RooRealConstant::value(-1.0));
  RooRealVar typeZNR3p("typeZNR3p","",NONRESONANT);
  L_ZNR3p->add(typeZNR3p);  //type
  L_ZNR3p->add(RooRealConstant::value(3));  //2 x spin
  L_ZNR3p->add(RooRealConstant::value(1));  //parity

