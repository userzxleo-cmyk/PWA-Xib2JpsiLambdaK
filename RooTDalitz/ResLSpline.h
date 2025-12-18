  int typeR(1);
  TList *list = new TList();
  RooRealVar scRe("scRe","",1.0);
  RooRealVar scIm("scIm","",0.0);
  //===Lambda(1520) 3/2- D03
  RooRealVar a_1520("a_1520","",1.0);
  RooRealVar p_1520("p_1520","",0.0);
  RooRealVar m0_1520("m0_1520","m0",1.5195);
  RooRealVar width_1520("width_1520","width",0.0156);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_D03("a_p1p1_D03","",1.);
  RooRealVar p_p1p1_D03("p_p1p1_D03","",0.);
  RooRealVar a_p100_D03("a_p100_D03","",0);
  RooRealVar p_p100_D03("p_p100_D03","",0);
  RooRealVar a_m100_D03("a_m100_D03","",0);
  RooRealVar p_m100_D03("p_m100_D03","",0);
  RooRealVar a_m1m1_D03("a_m1m1_D03","",0);
  RooRealVar p_m1m1_D03("p_m1m1_D03","",0);
  RooRealVar a_p3p1_D03("a_p3p1_D03","",0);
  RooRealVar p_p3p1_D03("p_p3p1_D03","",0);
  RooRealVar a_m3m1_D03("a_m3m1_D03","",0);
  RooRealVar p_m3m1_D03("p_m3m1_D03","",0);

  RooArgList* L_1520 = new RooArgList(a_p1p1_D03,p_p1p1_D03,
				      a_p100_D03,p_p100_D03,
				      a_m100_D03,p_m100_D03,
				      a_m1m1_D03,p_m1m1_D03,"L_1520");
  L_1520->add(a_p3p1_D03);
  L_1520->add(p_p3p1_D03);
  L_1520->add(a_m3m1_D03);
  L_1520->add(p_m3m1_D03);
  L_1520->add(a_1520);
  L_1520->add(p_1520);
  L_1520->add(m0_1520);
  L_1520->add(width_1520);
  L_1520->add(RooRealConstant::value(typeR));
  L_1520->add(RooRealConstant::value(3.));//2 x spin
  L_1520->add(RooRealConstant::value(-1)); //parity

    //===Lambda(1600) 1/2+ P01
  RooRealVar m0_1600("m0_1600","m0",1.6);
  RooRealVar width_1600("width_1600","width",0.15);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1600("a_1600","",1.0);
  RooRealVar p_1600("p_1600","",0.0);
  RooRealVar a_p1p1_P01("a_p1p1_P01","",40.39/100);
  RooRealVar p_p1p1_P01("p_p1p1_P01","",-1.018/100);
  RooRealVar a_p100_P01("a_p100_P01","",0);
  RooRealVar p_p100_P01("p_p100_P01","",0);
  RooRealVar a_m100_P01("a_m100_P01","",0);
  RooRealVar p_m100_P01("p_m100_P01","",0);
  RooRealVar a_m1m1_P01("a_m1m1_P01","",0);
  RooRealVar p_m1m1_P01("p_m1m1_P01","",0);
  RooRealVar a_p3p1_P01("a_p3p1_P01","",0);
  RooRealVar p_p3p1_P01("p_p3p1_P01","",0);
  RooRealVar a_m3m1_P01("a_m3m1_P01","",0);
  RooRealVar p_m3m1_P01("p_m3m1_P01","",0);

  RooArgList* L_1600 = new RooArgList(a_p1p1_P01,p_p1p1_P01,
				      a_p100_P01,p_p100_P01,
				      a_m100_P01,p_m100_P01,
				      a_m1m1_P01,p_m1m1_P01,"L_1600");
  L_1600->add(a_p3p1_P01);
  L_1600->add(p_p3p1_P01);
  L_1600->add(a_m3m1_P01);
  L_1600->add(p_m3m1_P01);
  L_1600->add(a_1600);
  L_1600->add(p_1600);
  L_1600->add(m0_1600);
  L_1600->add(width_1600);
  L_1600->add(RooRealConstant::value(typeR));  
  L_1600->add(RooRealConstant::value(1.));//2 x spin
  L_1600->add(RooRealConstant::value(1)); //parity

  //===Lambda(1890) 3/2+ P03
  RooRealVar m0_1890("m0_1890","m0",1.89);
  RooRealVar width_1890("width_1890","width",0.1);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1890("a_1890","",1.0);
  RooRealVar p_1890("p_1890","",0.0);    
  RooRealVar a_p1p1_P03("a_p1p1_P03","",0.2);
  RooRealVar p_p1p1_P03("p_p1p1_P03","",-10.28/100);
  RooRealVar a_p100_P03("a_p100_P03","",0);
  RooRealVar p_p100_P03("p_p100_P03","",0);
  RooRealVar a_m100_P03("a_m100_P03","",0);
  RooRealVar p_m100_P03("p_m100_P03","",0);
  RooRealVar a_m1m1_P03("a_m1m1_P03","",0);
  RooRealVar p_m1m1_P03("p_m1m1_P03","",0);
  RooRealVar a_p3p1_P03("a_p3p1_P03","",0);
  RooRealVar p_p3p1_P03("p_p3p1_P03","",0);
  RooRealVar a_m3m1_P03("a_m3m1_P03","",0);
  RooRealVar p_m3m1_P03("p_m3m1_P03","",0);

  RooArgList* L_1890 = new RooArgList(a_p1p1_P03,p_p1p1_P03,
				      a_p100_P03,p_p100_P03,
				      a_m100_P03,p_m100_P03,
				      a_m1m1_P03,p_m1m1_P03,"L_1890");
  L_1890->add(a_p3p1_P03);
  L_1890->add(p_p3p1_P03);
  L_1890->add(a_m3m1_P03);
  L_1890->add(p_m3m1_P03);
  L_1890->add(a_1890);
  L_1890->add(p_1890);
  L_1890->add(m0_1890);
  L_1890->add(width_1890);
  L_1890->add(RooRealConstant::value(typeR));
  L_1890->add(RooRealConstant::value(3.));//2 x spin
  L_1890->add(RooRealConstant::value(1)); //parity
  
//  list->Add(L_1890);


  //===Lambda(1670) 1/2- S01
  RooRealVar m0_1670("m0_1670","m0",1.67);
  RooRealVar width_1670("width_1670","width",0.035);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1670("a_1670","",1.0);
  RooRealVar p_1670("p_1670","",0.0);      
  RooRealVar a_p1p1_S01("a_p1p1_S01","",-0.5976/100);
  RooRealVar p_p1p1_S01("p_p1p1_S01","",1.218/100);
  RooRealVar a_p100_S01("a_p100_S01","",0);
  RooRealVar p_p100_S01("p_p100_S01","",0);
  RooRealVar a_m100_S01("a_m100_S01","",0);
  RooRealVar p_m100_S01("p_m100_S01","",0);
  RooRealVar a_m1m1_S01("a_m1m1_S01","",0);
  RooRealVar p_m1m1_S01("p_m1m1_S01","",0);
  RooRealVar a_p3p1_S01("a_p3p1_S01","",0);
  RooRealVar p_p3p1_S01("p_p3p1_S01","",0);
  RooRealVar a_m3m1_S01("a_m3m1_S01","",0);
  RooRealVar p_m3m1_S01("p_m3m1_S01","",0);

  RooArgList* L_1670 = new RooArgList(a_p1p1_S01,p_p1p1_S01,
				      a_p100_S01,p_p100_S01,
				      a_m100_S01,p_m100_S01,
				      a_m1m1_S01,p_m1m1_S01,"L_1670");
  L_1670->add(a_p3p1_S01);
  L_1670->add(p_p3p1_S01);
  L_1670->add(a_m3m1_S01);
  L_1670->add(p_m3m1_S01);
  L_1670->add(a_1670);
  L_1670->add(p_1670);
  L_1670->add(m0_1670);
  L_1670->add(width_1670);
  L_1670->add(RooRealConstant::value(typeR));  
  L_1670->add(RooRealConstant::value(1.));//2 x spin
  L_1670->add(RooRealConstant::value(-1)); //parity


  //1/2- S01
  RooRealVar m0_S01("m0_S01","m0",1.67);
  RooRealVar width_S01("width_S01","width",0.035);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_S01("a_S01","",1.0);
  RooRealVar p_S01("p_S01","",0.0);      

  RooArgList* L_S01 = new RooArgList(a_p1p1_S01,p_p1p1_S01,
				      a_p100_S01,p_p100_S01,
				      a_m100_S01,p_m100_S01,
				      a_m1m1_S01,p_m1m1_S01,"L_S01");
  L_S01->add(a_p3p1_S01);
  L_S01->add(p_p3p1_S01);
  L_S01->add(a_m3m1_S01);
  L_S01->add(p_m3m1_S01);
  
//  static int Nsp=45;
  char Sname[20];
  RooRealVar *S01Re[NSP], *S01Im[NSP];
  for(int i=0; i<NSP; ++i) {
   sprintf(Sname,"S01R_%02d",i);
    S01Re[i] = new RooRealVar(Sname,Sname,0);
    S01Re[i]->setConstant(0);
    L_S01->add(*S01Re[i]);
    sprintf(Sname,"S01I_%02d",i);
    S01Im[i] = new RooRealVar(Sname,Sname,0);
    S01Im[i]->setConstant(0);
    L_S01->add(*S01Im[i]);
  }

  L_S01->add(a_S01);
  L_S01->add(p_S01);
  
  L_S01->add(m0_S01);
  L_S01->add(width_S01);
  L_S01->add(RooRealConstant::value(3));  //spline
  L_S01->add(RooRealConstant::value(1.));//2 x spin
  L_S01->add(RooRealConstant::value(-1)); //parity

  //1/2+ P01
  RooRealVar m0_P01("m0_P01","m0",1.67);
  RooRealVar width_P01("width_P01","width",0.035);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_P01("a_P01","",1.0);
  RooRealVar p_P01("p_P01","",0.0);      

  RooArgList* L_P01 = new RooArgList(a_p1p1_P01,p_p1p1_P01,
				      a_p100_P01,p_p100_P01,
				      a_m100_P01,p_m100_P01,
				      a_m1m1_P01,p_m1m1_P01,"L_P01");
  L_P01->add(a_p3p1_P01);
  L_P01->add(p_p3p1_P01);
  L_P01->add(a_m3m1_P01);
  L_P01->add(p_m3m1_P01);
  
//  static int Nsp=45;
  RooRealVar *P01Re[NSP], *P01Im[NSP];
  for(int i=0; i<NSP; ++i) {
    sprintf(Sname,"P01R_%02d",i);
    P01Re[i] = new RooRealVar(Sname,Sname,0);
    P01Re[i]->setConstant(0);
    L_P01->add(*P01Re[i]);
    sprintf(Sname,"P01I_%02d",i);
    P01Im[i] = new RooRealVar(Sname,Sname,0);
    P01Im[i]->setConstant(0);
    L_P01->add(*P01Im[i]);
  }

  L_P01->add(a_P01);
  L_P01->add(p_P01);
  
  L_P01->add(m0_P01);
  L_P01->add(width_P01);
  L_P01->add(RooRealConstant::value(3));  //spline
  L_P01->add(RooRealConstant::value(1.));//2 x spin
  L_P01->add(RooRealConstant::value(1)); //parity

  //3/2- D03
  RooRealVar m0_D03("m0_D03","m0",1.67);
  RooRealVar width_D03("width_D03","width",0.035);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_D03("a_D03","",1.0);
  RooRealVar p_D03("p_D03","",0.0);      

  RooArgList* L_D03 = new RooArgList(a_p1p1_D03,p_p1p1_D03,
				      a_p100_D03,p_p100_D03,
				      a_m100_D03,p_m100_D03,
				      a_m1m1_D03,p_m1m1_D03,"L_D03");
  L_D03->add(a_p3p1_D03);
  L_D03->add(p_p3p1_D03);
  L_D03->add(a_m3m1_D03);
  L_D03->add(p_m3m1_D03);
  
//  static int Nsp=45;
  RooRealVar *D03Re[NSP], *D03Im[NSP];
  for(int i=0; i<NSP; ++i) {
   sprintf(Sname,"D03R_%02d",i);
    D03Re[i] = new RooRealVar(Sname,Sname,0);
    D03Re[i]->setConstant(0);
    L_D03->add(*D03Re[i]);
    sprintf(Sname,"D03I_%02d",i);
    D03Im[i] = new RooRealVar(Sname,Sname,0);
    D03Im[i]->setConstant(0);
    L_D03->add(*D03Im[i]);
  }

  L_D03->add(a_D03);
  L_D03->add(p_D03);
  
  L_D03->add(m0_D03);
  L_D03->add(width_D03);
  L_D03->add(RooRealConstant::value(3));  //spline
  L_D03->add(RooRealConstant::value(3.));//2 x spin
  L_D03->add(RooRealConstant::value(-1)); //parity


  //3/2+ P03
  RooRealVar m0_P03("m0_P03","m0",1.67);
  RooRealVar width_P03("width_P03","width",0.035);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_P03("a_P03","",1.0);
  RooRealVar p_P03("p_P03","",0.0);      

  RooArgList* L_P03 = new RooArgList(a_p1p1_P03,p_p1p1_P03,
				      a_p100_P03,p_p100_P03,
				      a_m100_P03,p_m100_P03,
				      a_m1m1_P03,p_m1m1_P03,"L_P03");
  L_P03->add(a_p3p1_P03);
  L_P03->add(p_p3p1_P03);
  L_P03->add(a_m3m1_P03);
  L_P03->add(p_m3m1_P03);
  
//  static int Nsp=45;
  RooRealVar *P03Re[NSP], *P03Im[NSP];
  for(int i=0; i<NSP; ++i) {
   sprintf(Sname,"P03R_%02d",i);
    P03Re[i] = new RooRealVar(Sname,Sname,0);
    P03Re[i]->setConstant(0);
    L_P03->add(*P03Re[i]);
    sprintf(Sname,"P03I_%02d",i);
    P03Im[i] = new RooRealVar(Sname,Sname,0);
    P03Im[i]->setConstant(0);
    L_P03->add(*P03Im[i]);
  }

  L_P03->add(a_P03);
  L_P03->add(p_P03);
  
  L_P03->add(m0_P03);
  L_P03->add(width_P03);
  L_P03->add(RooRealConstant::value(3));  //spline
  L_P03->add(RooRealConstant::value(3.));//2 x spin
  L_P03->add(RooRealConstant::value(1)); //parity


  
  //===Lambda(1690) 3/2- D03
  RooRealVar m0_1690("m0_1690","m0",1.715);
  RooRealVar width_1690("width_1690","width",0.06);
  RooRealVar a_1690("a_1690","",0.5,-10,10);
  RooRealVar p_1690("p_1690","",0.5,-10,10);
      
  RooArgList* L_1690 = new RooArgList(a_p1p1_D03,p_p1p1_D03,
				      a_p100_D03,p_p100_D03,
				      a_m100_D03,p_m100_D03,
				      a_m1m1_D03,p_m1m1_D03,"L_1690");
  L_1690->add(a_p3p1_D03);
  L_1690->add(p_p3p1_D03);
  L_1690->add(a_m3m1_D03);
  L_1690->add(p_m3m1_D03);
  L_1690->add(a_1690);
  L_1690->add(p_1690);
  L_1690->add(m0_1690);
  L_1690->add(width_1690);
  L_1690->add(RooRealConstant::value(typeR));
  L_1690->add(RooRealConstant::value(3.));//2 x spin
  L_1690->add(RooRealConstant::value(-1)); //parity
  

  //===Lambda(1800) 1/2- S01
  RooRealVar m0_1800("m0_1800","m0",1.8);
  RooRealVar width_1800("width_1800","width",0.3);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1800("a_1800","",1.0, -10,10);
  RooRealVar p_1800("p_1800","",0.0, -10,10);

  RooArgList* L_1800 = new RooArgList(a_p1p1_S01,p_p1p1_S01,
				      a_p100_S01,p_p100_S01,
				      a_m100_S01,p_m100_S01,
				      a_m1m1_S01,p_m1m1_S01,"L_1800");
  L_1800->add(a_p3p1_S01);
  L_1800->add(p_p3p1_S01);
  L_1800->add(a_m3m1_S01);
  L_1800->add(p_m3m1_S01);
  L_1800->add(a_1800);
  L_1800->add(p_1800);
  L_1800->add(m0_1800);
  L_1800->add(width_1800);
  L_1800->add(RooRealConstant::value(typeR));
  L_1800->add(RooRealConstant::value(1.));//2 x spin
  L_1800->add(RooRealConstant::value(-1)); //parity


  //===Lambda(1810) 1/2+ P01
  RooRealVar m0_1810("m0_1810","m0",1.81);
  RooRealVar width_1810("width_1810","width",0.15);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1810("a_1810","",0.1,-10,10);
  RooRealVar p_1810("p_1810","",0.1,-10,10);

  RooArgList* L_1810 = new RooArgList(a_p1p1_P01,p_p1p1_P01,
				      a_p100_P01,p_p100_P01,
				      a_m100_P01,p_m100_P01,
				      a_m1m1_P01,p_m1m1_P01,"L_1810");
  L_1810->add(a_p3p1_P01);
  L_1810->add(p_p3p1_P01);
  L_1810->add(a_m3m1_P01);
  L_1810->add(p_m3m1_P01);
  L_1810->add(a_1810);
  L_1810->add(p_1810);
  L_1810->add(m0_1810);
  L_1810->add(width_1810);
  L_1810->add(RooRealConstant::value(typeR));
  L_1810->add(RooRealConstant::value(1.));//2 x spin
  L_1810->add(RooRealConstant::value(1)); //parity


  //===Lambda(1820) 5/2+ F05
  RooRealVar m0_1820("m0_1820","m0",1.82);
  RooRealVar width_1820("width_1820","width",0.08);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1820("a_1820","",1.0);
  RooRealVar p_1820("p_1820","",0.0);    
  RooRealVar a_p1p1_F05("a_p1p1_F05","",0.2);
  RooRealVar p_p1p1_F05("p_p1p1_F05","",-10.28/100);
  RooRealVar a_p100_F05("a_p100_F05","",0);
  RooRealVar p_p100_F05("p_p100_F05","",0);
  RooRealVar a_m100_F05("a_m100_F05","",0);
  RooRealVar p_m100_F05("p_m100_F05","",0);
  RooRealVar a_m1m1_F05("a_m1m1_F05","",0);
  RooRealVar p_m1m1_F05("p_m1m1_F05","",0);
  RooRealVar a_p3p1_F05("a_p3p1_F05","",0);
  RooRealVar p_p3p1_F05("p_p3p1_F05","",0);
  RooRealVar a_m3m1_F05("a_m3m1_F05","",0);
  RooRealVar p_m3m1_F05("p_m3m1_F05","",0);

  RooArgList* L_1820 = new RooArgList(a_p1p1_F05,p_p1p1_F05,
				      a_p100_F05,p_p100_F05,
				      a_m100_F05,p_m100_F05,
				      a_m1m1_F05,p_m1m1_F05,"L_1820");
  L_1820->add(a_p3p1_F05);
  L_1820->add(p_p3p1_F05);
  L_1820->add(a_m3m1_F05);
  L_1820->add(p_m3m1_F05);
  L_1820->add(a_1820);
  L_1820->add(p_1820);
  L_1820->add(m0_1820);
  L_1820->add(width_1820);
  L_1820->add(RooRealConstant::value(typeR));
  L_1820->add(RooRealConstant::value(5.));//2 x spin
  L_1820->add(RooRealConstant::value(1)); //parity
  
//  list->Add(L_1820);


  //===Lambda(1830) 5/2- D05
  RooRealVar m0_1830("m0_1830","m0",1.83);
  RooRealVar width_1830("width_1830","width",0.095);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1830("a_1830","",1.0);
  RooRealVar p_1830("p_1830","",0.0);      
  RooRealVar a_p1p1_D05("a_p1p1_D05","",0.2);
  RooRealVar p_p1p1_D05("p_p1p1_D05","",-10.28/100);
  RooRealVar a_p100_D05("a_p100_D05","",0);
  RooRealVar p_p100_D05("p_p100_D05","",0);
  RooRealVar a_m100_D05("a_m100_D05","",0);
  RooRealVar p_m100_D05("p_m100_D05","",0);
  RooRealVar a_m1m1_D05("a_m1m1_D05","",0);
  RooRealVar p_m1m1_D05("p_m1m1_D05","",0);
  RooRealVar a_p3p1_D05("a_p3p1_D05","",0);
  RooRealVar p_p3p1_D05("p_p3p1_D05","",0);
  RooRealVar a_m3m1_D05("a_m3m1_D05","",0);
  RooRealVar p_m3m1_D05("p_m3m1_D05","",0);

  RooArgList* L_1830 = new RooArgList(a_p1p1_D05,p_p1p1_D05,
				      a_p100_D05,p_p100_D05,
				      a_m100_D05,p_m100_D05,
				      a_m1m1_D05,p_m1m1_D05,"L_1830");
  L_1830->add(a_p3p1_D05);
  L_1830->add(p_p3p1_D05);
  L_1830->add(a_m3m1_D05);
  L_1830->add(p_m3m1_D05);
  L_1830->add(a_1830);
  L_1830->add(p_1830);
  L_1830->add(m0_1830);
  L_1830->add(width_1830);
  L_1830->add(RooRealConstant::value(typeR));
  L_1830->add(RooRealConstant::value(5.));//2 x spin
  L_1830->add(RooRealConstant::value(-1)); //parity
  
//  list->Add(L_1830);


  //2110 5/2+ F05
  RooRealVar m0_2110("m0_2110","m0",2.11);
  RooRealVar width_2110("width_2110","width",0.2);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2110("a_2110","",0.2,-10,10);
  RooRealVar p_2110("p_2110","",0.0,-10,10);

  RooArgList* L_2110 = new RooArgList(a_p1p1_F05,p_p1p1_F05,
				      a_p100_F05,p_p100_F05,
				      a_m100_F05,p_m100_F05,
				      a_m1m1_F05,p_m1m1_F05,"L_2110");
  L_2110->add(a_p3p1_F05);
  L_2110->add(p_p3p1_F05);
  L_2110->add(a_m3m1_F05);
  L_2110->add(p_m3m1_F05);
  L_2110->add(a_2110);
  L_2110->add(p_2110);
  L_2110->add(m0_2110);
  L_2110->add(width_2110);
  L_2110->add(RooRealConstant::value(typeR));
  L_2110->add(RooRealConstant::value(5.));//2 x spin
  L_2110->add(RooRealConstant::value(1)); //parity

  //2100 7/2-
  RooRealVar m0_2100("m0_2100","m0",2.10);
  RooRealVar width_2100("width_2100","width",0.2);
    RooRealVar a_2100("a_2100","",1.0);
  RooRealVar p_2100("p_2100","",0.0);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2100("a_p1p1_2100","",1.);
  RooRealVar p_p1p1_2100("p_p1p1_2100","",0.);
  RooRealVar a_p100_2100("a_p100_2100","",0.);
  RooRealVar p_p100_2100("p_p100_2100","",0.);
  RooRealVar a_m100_2100("a_m100_2100","",0.);
  RooRealVar p_m100_2100("p_m100_2100","",0.);
  RooRealVar a_m1m1_2100("a_m1m1_2100","",0.);
  RooRealVar p_m1m1_2100("p_m1m1_2100","",0.);
  RooRealVar a_p3p1_2100("a_p3p1_2100","",0.);
  RooRealVar p_p3p1_2100("p_p3p1_2100","",0.);
  RooRealVar a_m3m1_2100("a_m3m1_2100","",0.);
  RooRealVar p_m3m1_2100("p_m3m1_2100","",0.);

  RooArgList* L_2100 = new RooArgList(a_p1p1_2100,p_p1p1_2100,
				      a_p100_2100,p_p100_2100,
				      a_m100_2100,p_m100_2100,
				      a_m1m1_2100,p_m1m1_2100,"L_2100");
  L_2100->add(a_p3p1_2100);
  L_2100->add(p_p3p1_2100);
  L_2100->add(a_m3m1_2100);
  L_2100->add(p_m3m1_2100);
  L_2100->add(a_2100);
  L_2100->add(p_2100);
  L_2100->add(m0_2100);
  L_2100->add(width_2100);
  L_2100->add(RooRealConstant::value(typeR));
  L_2100->add(RooRealConstant::value(7.));//2 x spin
  L_2100->add(RooRealConstant::value(-1)); //parity


  //===Lambda(1405) 1/2- S01
  RooRealVar m0_1405("m0_1405","m0",1.4051);
  RooRealVar width_1405("width_1405","width",0.0505);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1405("a_1405","",1.0,-10,10);
  RooRealVar p_1405("p_1405","",0.0,-10,10);
    
  RooArgList* L_1405 = new RooArgList(a_p1p1_S01,p_p1p1_S01,
				      a_p100_S01,p_p100_S01,
				      a_m100_S01,p_m100_S01,
				      a_m1m1_S01,p_m1m1_S01,"L_1405");
  L_1405->add(a_p3p1_S01);
  L_1405->add(p_p3p1_S01);
  L_1405->add(a_m3m1_S01);
  L_1405->add(p_m3m1_S01);
  L_1405->add(a_1405);
  L_1405->add(p_1405);
  L_1405->add(m0_1405);
  L_1405->add(width_1405);
  L_1405->add(RooRealConstant::value(typeR));
  L_1405->add(RooRealConstant::value(1.));//2 x spin
  L_1405->add(RooRealConstant::value(-1)); //parity


  //===Lambda(2350) 9/2+
  RooRealVar m0_2350("m0_2350","m0",2.35);
  RooRealVar width_2350("width_2350","width",0.15);
  RooRealVar a_2350("a_2350","",1.0);
  RooRealVar p_2350("p_2350","",0.0);  
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2350("a_p1p1_2350","",0.001);
  RooRealVar p_p1p1_2350("p_p1p1_2350","",0.);
  RooRealVar a_p100_2350("a_p100_2350","",0);
  RooRealVar p_p100_2350("p_p100_2350","",0);
  RooRealVar a_m100_2350("a_m100_2350","",0);
  RooRealVar p_m100_2350("p_m100_2350","",0);
  RooRealVar a_m1m1_2350("a_m1m1_2350","",0);
  RooRealVar p_m1m1_2350("p_m1m1_2350","",0);
  RooRealVar a_p3p1_2350("a_p3p1_2350","",0);
  RooRealVar p_p3p1_2350("p_p3p1_2350","",0);
  RooRealVar a_m3m1_2350("a_m3m1_2350","",0);
  RooRealVar p_m3m1_2350("p_m3m1_2350","",0);

  RooArgList* L_2350 = new RooArgList(a_p1p1_2350,p_p1p1_2350,
				      a_p100_2350,p_p100_2350,
				      a_m100_2350,p_m100_2350,
				      a_m1m1_2350,p_m1m1_2350,"L_2350");
  L_2350->add(a_p3p1_2350);
  L_2350->add(p_p3p1_2350);
  L_2350->add(a_m3m1_2350);
  L_2350->add(p_m3m1_2350);
  L_2350->add(a_2350);
  L_2350->add(p_2350);
  L_2350->add(m0_2350);
  L_2350->add(width_2350);
  L_2350->add(RooRealConstant::value(typeR));
  L_2350->add(RooRealConstant::value(9.));//2 x spin
  L_2350->add(RooRealConstant::value(1)); //parity
  
//  list->Add(L_2350);
  //2585 D05
  RooRealVar m0_2585("m0_2585","m0",2.585);
  RooRealVar width_2585("width_2585","width",0.3);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2585("a_2585","",0.1,-10,10);
  RooRealVar p_2585("p_2585","",0.0,-10,10);

  RooArgList* L_2585 = new RooArgList(a_p1p1_D05,p_p1p1_D05,
				      a_p100_D05,p_p100_D05,
				      a_m100_D05,p_m100_D05,
				      a_m1m1_D05,p_m1m1_D05,"L_2585");
  L_2585->add(a_p3p1_D05);
  L_2585->add(p_p3p1_D05);
  L_2585->add(a_m3m1_D05);
  L_2585->add(p_m3m1_D05);
  L_2585->add(a_2585);
  L_2585->add(p_2585);
  L_2585->add(m0_2585);
  L_2585->add(width_2585);
  L_2585->add(RooRealConstant::value(typeR));
  L_2585->add(RooRealConstant::value(5.));//2 x spin
  L_2585->add(RooRealConstant::value(-1)); //parity

