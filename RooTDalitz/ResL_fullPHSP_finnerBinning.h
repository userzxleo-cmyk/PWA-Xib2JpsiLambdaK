  int typeR(1);
  TList *list = new TList();
  RooRealVar scRe("scRe","",1.0);
  RooRealVar scIm("scIm","",0.0);
  //===Lambda(1520) 3/2-
  RooRealVar m0_1520("m0_1520","m0",1.5195);
  RooRealVar width_1520("width_1520","width",0.0156);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1520("a_p1p1_1520","",1.);
  RooRealVar p_p1p1_1520("p_p1p1_1520","",0.);
  RooRealVar a_p100_1520("a_p100_1520","",0);
  RooRealVar p_p100_1520("p_p100_1520","",0);
  RooRealVar a_m100_1520("a_m100_1520","",0);
  RooRealVar p_m100_1520("p_m100_1520","",0);
  RooRealVar a_m1m1_1520("a_m1m1_1520","",0);
  RooRealVar p_m1m1_1520("p_m1m1_1520","",0);
  RooRealVar a_p3p1_1520("a_p3p1_1520","",0);
  RooRealVar p_p3p1_1520("p_p3p1_1520","",0);
  RooRealVar a_m3m1_1520("a_m3m1_1520","",0);
  RooRealVar p_m3m1_1520("p_m3m1_1520","",0);

  RooArgList* L_1520 = new RooArgList(a_p1p1_1520,p_p1p1_1520,
				      a_p100_1520,p_p100_1520,
				      a_m100_1520,p_m100_1520,
				      a_m1m1_1520,p_m1m1_1520,"L_1520");
  L_1520->add(a_p3p1_1520);
  L_1520->add(p_p3p1_1520);
  L_1520->add(a_m3m1_1520);
  L_1520->add(p_m3m1_1520);
  L_1520->add(scRe);
  L_1520->add(scIm);
  L_1520->add(m0_1520);
  L_1520->add(width_1520);
  L_1520->add(RooRealConstant::value(typeR));
  L_1520->add(RooRealConstant::value(3.));//2 x spin
  L_1520->add(RooRealConstant::value(-1)); //parity
  


    //===Lambda(1600) 1/2+
  RooRealVar m0_1600("m0_1600","m0",1.6);
  RooRealVar width_1600("width_1600","width",0.15);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1600("a_p1p1_1600","",40.39/100);
  RooRealVar p_p1p1_1600("p_p1p1_1600","",-1.018/100);
  RooRealVar a_p100_1600("a_p100_1600","",0);
  RooRealVar p_p100_1600("p_p100_1600","",0);
  RooRealVar a_m100_1600("a_m100_1600","",0);
  RooRealVar p_m100_1600("p_m100_1600","",0);
  RooRealVar a_m1m1_1600("a_m1m1_1600","",0);
  RooRealVar p_m1m1_1600("p_m1m1_1600","",0);
  RooRealVar a_p3p1_1600("a_p3p1_1600","",0);
  RooRealVar p_p3p1_1600("p_p3p1_1600","",0);
  RooRealVar a_m3m1_1600("a_m3m1_1600","",0);
  RooRealVar p_m3m1_1600("p_m3m1_1600","",0);

  RooArgList* L_1600 = new RooArgList(a_p1p1_1600,p_p1p1_1600,
				      a_p100_1600,p_p100_1600,
				      a_m100_1600,p_m100_1600,
				      a_m1m1_1600,p_m1m1_1600,"L_1600");
  L_1600->add(a_p3p1_1600);
  L_1600->add(p_p3p1_1600);
  L_1600->add(a_m3m1_1600);
  L_1600->add(p_m3m1_1600);
  L_1600->add(scRe);
  L_1600->add(scIm);
  L_1600->add(m0_1600);
  L_1600->add(width_1600);
  L_1600->add(RooRealConstant::value(typeR));  
  L_1600->add(RooRealConstant::value(1.));//2 x spin
  L_1600->add(RooRealConstant::value(1)); //parity


  //===Lambda(1670) 1/2-
  RooRealVar m0_1670("m0_1670","m0",1.67);
  RooRealVar width_1670("width_1670","width",0.035);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1670("a_p1p1_1670","",-0.5976/100);
  RooRealVar p_p1p1_1670("p_p1p1_1670","",1.218/100);
  RooRealVar a_p100_1670("a_p100_1670","",0);
  RooRealVar p_p100_1670("p_p100_1670","",0);
  RooRealVar a_m100_1670("a_m100_1670","",0);
  RooRealVar p_m100_1670("p_m100_1670","",0);
  RooRealVar a_m1m1_1670("a_m1m1_1670","",0);
  RooRealVar p_m1m1_1670("p_m1m1_1670","",0);
  RooRealVar a_p3p1_1670("a_p3p1_1670","",0);
  RooRealVar p_p3p1_1670("p_p3p1_1670","",0);
  RooRealVar a_m3m1_1670("a_m3m1_1670","",0);
  RooRealVar p_m3m1_1670("p_m3m1_1670","",0);

  RooArgList* L_1670 = new RooArgList(a_p1p1_1670,p_p1p1_1670,
				      a_p100_1670,p_p100_1670,
				      a_m100_1670,p_m100_1670,
				      a_m1m1_1670,p_m1m1_1670,"L_1670");
  L_1670->add(a_p3p1_1670);
  L_1670->add(p_p3p1_1670);
  L_1670->add(a_m3m1_1670);
  L_1670->add(p_m3m1_1670);
  L_1670->add(scRe);
  L_1670->add(scIm);
  L_1670->add(m0_1670);
  L_1670->add(width_1670);
  L_1670->add(RooRealConstant::value(typeR));  
  L_1670->add(RooRealConstant::value(1.));//2 x spin
  L_1670->add(RooRealConstant::value(-1)); //parity

  RooArgList* L_1670Flatte = new RooArgList(a_p1p1_1670,p_p1p1_1670,
				      a_p100_1670,p_p100_1670,
				      a_m100_1670,p_m100_1670,
				      a_m1m1_1670,p_m1m1_1670,"L_1670Flatte");
  L_1670Flatte->add(a_p3p1_1670);
  L_1670Flatte->add(p_p3p1_1670);
  L_1670Flatte->add(a_m3m1_1670);
  L_1670Flatte->add(p_m3m1_1670);
  L_1670Flatte->add(scRe);
  L_1670Flatte->add(scIm);
  L_1670Flatte->add(m0_1670);
  L_1670Flatte->add(width_1670);
  L_1670Flatte->add(RooRealConstant::value(2));
  L_1670Flatte->add(RooRealConstant::value(1.));//2 x spin
  L_1670Flatte->add(RooRealConstant::value(-1)); //parity

  
  //===Lambda(1690) 3/2-
  RooRealVar m0_1690("m0_1690","m0",1.715);
  RooRealVar width_1690("width_1690","width",0.06);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1690("a_p1p1_1690","",42.5/100);
  RooRealVar p_p1p1_1690("p_p1p1_1690","",2.471/100);
  RooRealVar a_p100_1690("a_p100_1690","",0);
  RooRealVar p_p100_1690("p_p100_1690","",0);
  RooRealVar a_m100_1690("a_m100_1690","",0);
  RooRealVar p_m100_1690("p_m100_1690","",0);
  RooRealVar a_m1m1_1690("a_m1m1_1690","",0);
  RooRealVar p_m1m1_1690("p_m1m1_1690","",0);
  RooRealVar a_p3p1_1690("a_p3p1_1690","",0);
  RooRealVar p_p3p1_1690("p_p3p1_1690","",0);
  RooRealVar a_m3m1_1690("a_m3m1_1690","",0);
  RooRealVar p_m3m1_1690("p_m3m1_1690","",0);

  RooArgList* L_1690 = new RooArgList(a_p1p1_1690,p_p1p1_1690,
				      a_p100_1690,p_p100_1690,
				      a_m100_1690,p_m100_1690,
				      a_m1m1_1690,p_m1m1_1690,"L_1690");
  L_1690->add(a_p3p1_1690);
  L_1690->add(p_p3p1_1690);
  L_1690->add(a_m3m1_1690);
  L_1690->add(p_m3m1_1690);
  L_1690->add(scRe);
  L_1690->add(scIm);
  L_1690->add(m0_1690);
  L_1690->add(width_1690);
  L_1690->add(RooRealConstant::value(typeR));
  L_1690->add(RooRealConstant::value(3.));//2 x spin
  L_1690->add(RooRealConstant::value(-1)); //parity
 

  //===Lambda(2075) 3/2-
  RooRealVar m0_2075("m0_2075","m0",2.2, 2.1, 2.4);
  RooRealVar width_2075("width_2075","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2075("a_p1p1_2075","",0.1, -10, 10);
  RooRealVar p_p1p1_2075("p_p1p1_2075","",0.1, -10, 10);
  RooRealVar a_p100_2075("a_p100_2075","",0);
  RooRealVar p_p100_2075("p_p100_2075","",0);
  RooRealVar a_m100_2075("a_m100_2075","",0);
  RooRealVar p_m100_2075("p_m100_2075","",0);
  RooRealVar a_m1m1_2075("a_m1m1_2075","",0);
  RooRealVar p_m1m1_2075("p_m1m1_2075","",0);
  RooRealVar a_p3p1_2075("a_p3p1_2075","",0);
  RooRealVar p_p3p1_2075("p_p3p1_2075","",0);
  RooRealVar a_m3m1_2075("a_m3m1_2075","",0);
  RooRealVar p_m3m1_2075("p_m3m1_2075","",0); 

  RooArgList* L_2075 = new RooArgList(a_p1p1_2075,p_p1p1_2075,
                                      a_p100_2075,p_p100_2075,
                                      a_m100_2075,p_m100_2075,
                                      a_m1m1_2075,p_m1m1_2075,"L_2075");
  L_2075->add(a_p3p1_2075);
  L_2075->add(p_p3p1_2075);
  L_2075->add(a_m3m1_2075);
  L_2075->add(p_m3m1_2075);
  L_2075->add(scRe);
  L_2075->add(scIm);
  L_2075->add(m0_2075);
  L_2075->add(width_2075);
  L_2075->add(RooRealConstant::value(typeR));
  L_2075->add(RooRealConstant::value(3.));//2 x spin
  L_2075->add(RooRealConstant::value(-1)); //parity




  //===Lambda(1800) 1/2-
  RooRealVar m0_1800("m0_1800","m0",1.8);
  RooRealVar width_1800("width_1800","width",0.3);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1800("a_p1p1_1800","",0.00);
  RooRealVar p_p1p1_1800("p_p1p1_1800","",0.00);
  RooRealVar a_p100_1800("a_p100_1800","",0);
  RooRealVar p_p100_1800("p_p100_1800","",0);
  RooRealVar a_m100_1800("a_m100_1800","",0);
  RooRealVar p_m100_1800("p_m100_1800","",0);
  RooRealVar a_m1m1_1800("a_m1m1_1800","",0);
  RooRealVar p_m1m1_1800("p_m1m1_1800","",0);
  RooRealVar a_p3p1_1800("a_p3p1_1800","",0);
  RooRealVar p_p3p1_1800("p_p3p1_1800","",0);
  RooRealVar a_m3m1_1800("a_m3m1_1800","",0);
  RooRealVar p_m3m1_1800("p_m3m1_1800","",0);

  RooArgList* L_1800 = new RooArgList(a_p1p1_1800,p_p1p1_1800,
				      a_p100_1800,p_p100_1800,
				      a_m100_1800,p_m100_1800,
				      a_m1m1_1800,p_m1m1_1800,"L_1800");
  L_1800->add(a_p3p1_1800);
  L_1800->add(p_p3p1_1800);
  L_1800->add(a_m3m1_1800);
  L_1800->add(p_m3m1_1800);
  L_1800->add(scRe);
  L_1800->add(scIm);
  L_1800->add(m0_1800);
  L_1800->add(width_1800);
  L_1800->add(RooRealConstant::value(typeR));
  L_1800->add(RooRealConstant::value(1.));//2 x spin
  L_1800->add(RooRealConstant::value(-1)); //parity


  //===Lambda(1810) 1/2+
  RooRealVar m0_1810("m0_1810","m0",1.81);
  RooRealVar width_1810("width_1810","width",0.15);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1810("a_p1p1_1810","",-3.752/100);
  RooRealVar p_p1p1_1810("p_p1p1_1810","",-10.28/100);
  RooRealVar a_p100_1810("a_p100_1810","",0);
  RooRealVar p_p100_1810("p_p100_1810","",0);
  RooRealVar a_m100_1810("a_m100_1810","",0);
  RooRealVar p_m100_1810("p_m100_1810","",0);
  RooRealVar a_m1m1_1810("a_m1m1_1810","",0);
  RooRealVar p_m1m1_1810("p_m1m1_1810","",0);
  RooRealVar a_p3p1_1810("a_p3p1_1810","",0);
  RooRealVar p_p3p1_1810("p_p3p1_1810","",0);
  RooRealVar a_m3m1_1810("a_m3m1_1810","",0);
  RooRealVar p_m3m1_1810("p_m3m1_1810","",0);

  RooArgList* L_1810 = new RooArgList(a_p1p1_1810,p_p1p1_1810,
				      a_p100_1810,p_p100_1810,
				      a_m100_1810,p_m100_1810,
				      a_m1m1_1810,p_m1m1_1810,"L_1810");
  L_1810->add(a_p3p1_1810);
  L_1810->add(p_p3p1_1810);
  L_1810->add(a_m3m1_1810);
  L_1810->add(p_m3m1_1810);
  L_1810->add(scRe);
  L_1810->add(scIm);
  L_1810->add(m0_1810);
  L_1810->add(width_1810);
  L_1810->add(RooRealConstant::value(typeR));
  L_1810->add(RooRealConstant::value(1.));//2 x spin
  L_1810->add(RooRealConstant::value(1)); //parity


  //===Lambda(2080) 1/2+
  // RooRealVar m0_2080("m0_2080","m0",2.1, 2.0, 2.2);
  // RooRealVar width_2080("width_2080","width",0.3, 0.05, 0.5);
  // // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  // RooRealVar a_p1p1_2080("a_p1p1_2080","",0.1, -10, 10);
  // RooRealVar p_p1p1_2080("p_p1p1_2080","",0.1, -10, 10);
  // RooRealVar a_p100_2080("a_p100_2080","",0);
  // RooRealVar p_p100_2080("p_p100_2080","",0);
  // RooRealVar a_m100_2080("a_m100_2080","",0);
  // RooRealVar p_m100_2080("p_m100_2080","",0);
  // RooRealVar a_m1m1_2080("a_m1m1_2080","",0);
  // RooRealVar p_m1m1_2080("p_m1m1_2080","",0);
  // RooRealVar a_p3p1_2080("a_p3p1_2080","",0);
  // RooRealVar p_p3p1_2080("p_p3p1_2080","",0);
  // RooRealVar a_m3m1_2080("a_m3m1_2080","",0);
  // RooRealVar p_m3m1_2080("p_m3m1_2080","",0);

  // RooArgList* L_2080 = new RooArgList(a_p1p1_2080,p_p1p1_2080,
  //                                     a_p100_2080,p_p100_2080,
  //                                     a_m100_2080,p_m100_2080,
  //                                     a_m1m1_2080,p_m1m1_2080,"L_2080");
  // L_2080->add(a_p3p1_2080);
  // L_2080->add(p_p3p1_2080);
  // L_2080->add(a_m3m1_2080);
  // L_2080->add(p_m3m1_2080);
  // L_2080->add(scRe);
  // L_2080->add(scIm);
  // L_2080->add(m0_2080);
  // L_2080->add(width_2080);
  // L_2080->add(RooRealConstant::value(typeR));
  // L_2080->add(RooRealConstant::value(1.));//2 x spin
  // L_2080->add(RooRealConstant::value(1)); //parity

  // ===Lambda(2070) 3/2+
  // From arXiv:1907.13387
  RooRealVar m0_2070("m0_2070","m0",2.044, 2.0, 2.2);
  RooRealVar width_2070("width_2070","width",0.36, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2070("a_p1p1_2070","",0.05, -10, 10);
  RooRealVar p_p1p1_2070("p_p1p1_2070","",0.05, -10, 10);
  RooRealVar a_p100_2070("a_p100_2070","",0);
  RooRealVar p_p100_2070("p_p100_2070","",0);
  RooRealVar a_m100_2070("a_m100_2070","",0);
  RooRealVar p_m100_2070("p_m100_2070","",0);
  RooRealVar a_m1m1_2070("a_m1m1_2070","",0);
  RooRealVar p_m1m1_2070("p_m1m1_2070","",0);
  RooRealVar a_p3p1_2070("a_p3p1_2070","",0);
  RooRealVar p_p3p1_2070("p_p3p1_2070","",0);
  RooRealVar a_m3m1_2070("a_m3m1_2070","",0);
  RooRealVar p_m3m1_2070("p_m3m1_2070","",0);

  RooArgList* L_2070 = new RooArgList(a_p1p1_2070,p_p1p1_2070,
                                      a_p100_2070,p_p100_2070,
                                      a_m100_2070,p_m100_2070,
                                      a_m1m1_2070,p_m1m1_2070,"L_2070");
  L_2070->add(a_p3p1_2070);
  L_2070->add(p_p3p1_2070);
  L_2070->add(a_m3m1_2070);
  L_2070->add(p_m3m1_2070);
  L_2070->add(scRe);
  L_2070->add(scIm);
  L_2070->add(m0_2070);
  L_2070->add(width_2070);
  L_2070->add(RooRealConstant::value(typeR));
  L_2070->add(RooRealConstant::value(3.));//2 x spin
  L_2070->add(RooRealConstant::value(1)); //parity

  // ===Lambda(2080) 5/2-
  // From arXiv:1907.13387
  RooRealVar m0_2080("m0_2080","m0",2.07, 2.0, 2.2);
  RooRealVar width_2080("width_2080","width",0.172, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2080("a_p1p1_2080","",-0.05, -10, 10);
  RooRealVar p_p1p1_2080("p_p1p1_2080","",-0.05, -10, 10);
  RooRealVar a_p100_2080("a_p100_2080","",0);
  RooRealVar p_p100_2080("p_p100_2080","",0);
  RooRealVar a_m100_2080("a_m100_2080","",0);
  RooRealVar p_m100_2080("p_m100_2080","",0);
  RooRealVar a_m1m1_2080("a_m1m1_2080","",0);
  RooRealVar p_m1m1_2080("p_m1m1_2080","",0);
  RooRealVar a_p3p1_2080("a_p3p1_2080","",0);
  RooRealVar p_p3p1_2080("p_p3p1_2080","",0);
  RooRealVar a_m3m1_2080("a_m3m1_2080","",0);
  RooRealVar p_m3m1_2080("p_m3m1_2080","",0);

  RooArgList* L_2080 = new RooArgList(a_p1p1_2080,p_p1p1_2080,
                                      a_p100_2080,p_p100_2080,
                                      a_m100_2080,p_m100_2080,
                                      a_m1m1_2080,p_m1m1_2080,"L_2080");
  L_2080->add(a_p3p1_2080);
  L_2080->add(p_p3p1_2080);
  L_2080->add(a_m3m1_2080);
  L_2080->add(p_m3m1_2080);
  L_2080->add(scRe);
  L_2080->add(scIm);
  L_2080->add(m0_2080);
  L_2080->add(width_2080);
  L_2080->add(RooRealConstant::value(typeR));
  L_2080->add(RooRealConstant::value(5.));//2 x spin
  L_2080->add(RooRealConstant::value(-1)); //parity


  //===Lambda(1820) 5/2+
  RooRealVar m0_1820("m0_1820","m0",1.82);
  RooRealVar width_1820("width_1820","width",0.08);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1820("a_p1p1_1820","",0.2);
  RooRealVar p_p1p1_1820("p_p1p1_1820","",-10.28/100);
  RooRealVar a_p100_1820("a_p100_1820","",0);
  RooRealVar p_p100_1820("p_p100_1820","",0);
  RooRealVar a_m100_1820("a_m100_1820","",0);
  RooRealVar p_m100_1820("p_m100_1820","",0);
  RooRealVar a_m1m1_1820("a_m1m1_1820","",0);
  RooRealVar p_m1m1_1820("p_m1m1_1820","",0);
  RooRealVar a_p3p1_1820("a_p3p1_1820","",0);
  RooRealVar p_p3p1_1820("p_p3p1_1820","",0);
  RooRealVar a_m3m1_1820("a_m3m1_1820","",0);
  RooRealVar p_m3m1_1820("p_m3m1_1820","",0);

  RooArgList* L_1820 = new RooArgList(a_p1p1_1820,p_p1p1_1820,
				      a_p100_1820,p_p100_1820,
				      a_m100_1820,p_m100_1820,
				      a_m1m1_1820,p_m1m1_1820,"L_1820");
  L_1820->add(a_p3p1_1820);
  L_1820->add(p_p3p1_1820);
  L_1820->add(a_m3m1_1820);
  L_1820->add(p_m3m1_1820);
  L_1820->add(scRe);
  L_1820->add(scIm);
  L_1820->add(m0_1820);
  L_1820->add(width_1820);
  L_1820->add(RooRealConstant::value(typeR));
  L_1820->add(RooRealConstant::value(5.));//2 x spin
  L_1820->add(RooRealConstant::value(1)); //parity
  
//  list->Add(L_1820);


  //===Lambda(1830) 5/2-
  RooRealVar m0_1830("m0_1830","m0",1.83);
  RooRealVar width_1830("width_1830","width",0.095);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1830("a_p1p1_1830","",0.2);
  RooRealVar p_p1p1_1830("p_p1p1_1830","",-10.28/100);
  RooRealVar a_p100_1830("a_p100_1830","",0);
  RooRealVar p_p100_1830("p_p100_1830","",0);
  RooRealVar a_m100_1830("a_m100_1830","",0);
  RooRealVar p_m100_1830("p_m100_1830","",0);
  RooRealVar a_m1m1_1830("a_m1m1_1830","",0);
  RooRealVar p_m1m1_1830("p_m1m1_1830","",0);
  RooRealVar a_p3p1_1830("a_p3p1_1830","",0);
  RooRealVar p_p3p1_1830("p_p3p1_1830","",0);
  RooRealVar a_m3m1_1830("a_m3m1_1830","",0);
  RooRealVar p_m3m1_1830("p_m3m1_1830","",0);

  RooArgList* L_1830 = new RooArgList(a_p1p1_1830,p_p1p1_1830,
				      a_p100_1830,p_p100_1830,
				      a_m100_1830,p_m100_1830,
				      a_m1m1_1830,p_m1m1_1830,"L_1830");
  L_1830->add(a_p3p1_1830);
  L_1830->add(p_p3p1_1830);
  L_1830->add(a_m3m1_1830);
  L_1830->add(p_m3m1_1830);
  L_1830->add(scRe);
  L_1830->add(scIm);
  L_1830->add(m0_1830);
  L_1830->add(width_1830);
  L_1830->add(RooRealConstant::value(typeR));
  L_1830->add(RooRealConstant::value(5.));//2 x spin
  L_1830->add(RooRealConstant::value(-1)); //parity
  
//  list->Add(L_1830);

  //===Lambda(1890) 3/2+
  RooRealVar m0_1890("m0_1890","m0",1.89);
  RooRealVar width_1890("width_1890","width",0.1);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1890("a_p1p1_1890","",0.2);
  RooRealVar p_p1p1_1890("p_p1p1_1890","",-10.28/100);
  RooRealVar a_p100_1890("a_p100_1890","",0);
  RooRealVar p_p100_1890("p_p100_1890","",0);
  RooRealVar a_m100_1890("a_m100_1890","",0);
  RooRealVar p_m100_1890("p_m100_1890","",0);
  RooRealVar a_m1m1_1890("a_m1m1_1890","",0);
  RooRealVar p_m1m1_1890("p_m1m1_1890","",0);
  RooRealVar a_p3p1_1890("a_p3p1_1890","",0);
  RooRealVar p_p3p1_1890("p_p3p1_1890","",0);
  RooRealVar a_m3m1_1890("a_m3m1_1890","",0);
  RooRealVar p_m3m1_1890("p_m3m1_1890","",0);

  RooArgList* L_1890 = new RooArgList(a_p1p1_1890,p_p1p1_1890,
				      a_p100_1890,p_p100_1890,
				      a_m100_1890,p_m100_1890,
				      a_m1m1_1890,p_m1m1_1890,"L_1890");
  L_1890->add(a_p3p1_1890);
  L_1890->add(p_p3p1_1890);
  L_1890->add(a_m3m1_1890);
  L_1890->add(p_m3m1_1890);
  L_1890->add(scRe);
  L_1890->add(scIm);
  L_1890->add(m0_1890);
  L_1890->add(width_1890);
  L_1890->add(RooRealConstant::value(typeR));
  L_1890->add(RooRealConstant::value(3.));//2 x spin
  L_1890->add(RooRealConstant::value(1)); //parity


  //===Lambda(1950) 3/2+
  RooRealVar m0_1950("m0_1950","m0",2.0, 1.9, 2.2);
  RooRealVar width_1950("width_1950","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1950("a_p1p1_1950","",0.1, -10, 10);
  RooRealVar p_p1p1_1950("p_p1p1_1950","",0.1, -10, 10);
  RooRealVar a_p100_1950("a_p100_1950","",0);
  RooRealVar p_p100_1950("p_p100_1950","",0);
  RooRealVar a_m100_1950("a_m100_1950","",0);
  RooRealVar p_m100_1950("p_m100_1950","",0);
  RooRealVar a_m1m1_1950("a_m1m1_1950","",0);
  RooRealVar p_m1m1_1950("p_m1m1_1950","",0);
  RooRealVar a_p3p1_1950("a_p3p1_1950","",0);
  RooRealVar p_p3p1_1950("p_p3p1_1950","",0);
  RooRealVar a_m3m1_1950("a_m3m1_1950","",0);
  RooRealVar p_m3m1_1950("p_m3m1_1950","",0);

  RooArgList* L_1950 = new RooArgList(a_p1p1_1950,p_p1p1_1950,
                                      a_p100_1950,p_p100_1950,
                                      a_m100_1950,p_m100_1950,
                                      a_m1m1_1950,p_m1m1_1950,"L_1950");
  L_1950->add(a_p3p1_1950);
  L_1950->add(p_p3p1_1950);
  L_1950->add(a_m3m1_1950);
  L_1950->add(p_m3m1_1950);
  L_1950->add(scRe);
  L_1950->add(scIm);
  L_1950->add(m0_1950);
  L_1950->add(width_1950);
  L_1950->add(RooRealConstant::value(typeR));
  L_1950->add(RooRealConstant::value(3.));//2 x spin
  L_1950->add(RooRealConstant::value(1)); //parity

//  list->Add(L_1890);

  //2110 5/2+
  RooRealVar m0_2110("m0_2110","m0",2.11);
  RooRealVar width_2110("width_2110","width",0.2);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2110("a_p1p1_2110","",1.);
  RooRealVar p_p1p1_2110("p_p1p1_2110","",0.);
  RooRealVar a_p100_2110("a_p100_2110","",0.);
  RooRealVar p_p100_2110("p_p100_2110","",0.);
  RooRealVar a_m100_2110("a_m100_2110","",0.);
  RooRealVar p_m100_2110("p_m100_2110","",0.);
  RooRealVar a_m1m1_2110("a_m1m1_2110","",0.);
  RooRealVar p_m1m1_2110("p_m1m1_2110","",0.);
  RooRealVar a_p3p1_2110("a_p3p1_2110","",0.);
  RooRealVar p_p3p1_2110("p_p3p1_2110","",0.);
  RooRealVar a_m3m1_2110("a_m3m1_2110","",0.);
  RooRealVar p_m3m1_2110("p_m3m1_2110","",0.);

  RooArgList* L_2110 = new RooArgList(a_p1p1_2110,p_p1p1_2110,
				      a_p100_2110,p_p100_2110,
				      a_m100_2110,p_m100_2110,
				      a_m1m1_2110,p_m1m1_2110,"L_2110");
  L_2110->add(a_p3p1_2110);
  L_2110->add(p_p3p1_2110);
  L_2110->add(a_m3m1_2110);
  L_2110->add(p_m3m1_2110);
  L_2110->add(scRe);
  L_2110->add(scIm);
  L_2110->add(m0_2110);
  L_2110->add(width_2110);
  L_2110->add(RooRealConstant::value(typeR));
  L_2110->add(RooRealConstant::value(5.));//2 x spin
  L_2110->add(RooRealConstant::value(1)); //parity

  //2100 7/2-
  RooRealVar m0_2100("m0_2100","m0",2.10);
  RooRealVar width_2100("width_2100","width",0.2);
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
  L_2100->add(scRe);
  L_2100->add(scIm);
  L_2100->add(m0_2100);
  L_2100->add(width_2100);
  L_2100->add(RooRealConstant::value(typeR));
  L_2100->add(RooRealConstant::value(7.));//2 x spin
  L_2100->add(RooRealConstant::value(-1)); //parity


  //===Lambda(1405) 1/2-
  RooRealVar m0_1405("m0_1405","m0",1.4051);
  RooRealVar width_1405("width_1405","width",0.0505);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1405("a_p1p1_1405","",0.001);
  RooRealVar p_p1p1_1405("p_p1p1_1405","",0.);
  RooRealVar a_p100_1405("a_p100_1405","",0);
  RooRealVar p_p100_1405("p_p100_1405","",0);
  RooRealVar a_m100_1405("a_m100_1405","",0);
  RooRealVar p_m100_1405("p_m100_1405","",0);
  RooRealVar a_m1m1_1405("a_m1m1_1405","",0);
  RooRealVar p_m1m1_1405("p_m1m1_1405","",0);
  RooRealVar a_p3p1_1405("a_p3p1_1405","",0);
  RooRealVar p_p3p1_1405("p_p3p1_1405","",0);
  RooRealVar a_m3m1_1405("a_m3m1_1405","",0);
  RooRealVar p_m3m1_1405("p_m3m1_1405","",0);

  RooArgList* L_1405 = new RooArgList(a_p1p1_1405,p_p1p1_1405,
				      a_p100_1405,p_p100_1405,
				      a_m100_1405,p_m100_1405,
				      a_m1m1_1405,p_m1m1_1405,"L_1405");
  L_1405->add(a_p3p1_1405);
  L_1405->add(p_p3p1_1405);
  L_1405->add(a_m3m1_1405);
  L_1405->add(p_m3m1_1405);
  L_1405->add(scRe);
  L_1405->add(scIm);
  L_1405->add(m0_1405);
  L_1405->add(width_1405);
  L_1405->add(RooRealConstant::value(typeR));
  L_1405->add(RooRealConstant::value(1.));//2 x spin
  L_1405->add(RooRealConstant::value(-1)); //parity

  //===Lambda(NR) 1/2-
  RooRealVar m0_NR("m0_NR","m0",-0.1);
  RooRealVar width_NR("width_NR","width",1.0);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_NR("a_p1p1_NR","",0.001);
  RooRealVar p_p1p1_NR("p_p1p1_NR","",0.);
  RooRealVar a_p100_NR("a_p100_NR","",0);
  RooRealVar p_p100_NR("p_p100_NR","",0);
  RooRealVar a_m100_NR("a_m100_NR","",0);
  RooRealVar p_m100_NR("p_m100_NR","",0);
  RooRealVar a_m1m1_NR("a_m1m1_NR","",0);
  RooRealVar p_m1m1_NR("p_m1m1_NR","",0);
  RooRealVar a_p3p1_NR("a_p3p1_NR","",0);
  RooRealVar p_p3p1_NR("p_p3p1_NR","",0);
  RooRealVar a_m3m1_NR("a_m3m1_NR","",0);
  RooRealVar p_m3m1_NR("p_m3m1_NR","",0);

  RooArgList* L_NR = new RooArgList(a_p1p1_NR,p_p1p1_NR,
				      a_p100_NR,p_p100_NR,
				      a_m100_NR,p_m100_NR,
				      a_m1m1_NR,p_m1m1_NR,"L_NR");
  L_NR->add(a_p3p1_NR);
  L_NR->add(p_p3p1_NR);
  L_NR->add(a_m3m1_NR);
  L_NR->add(p_m3m1_NR);
  L_NR->add(scRe);
  L_NR->add(scIm);
  L_NR->add(m0_NR);
  L_NR->add(width_NR);
  L_NR->add(RooRealConstant::value(typeR));
  L_NR->add(RooRealConstant::value(1.));//2 x spin
  L_NR->add(RooRealConstant::value(-1)); //parity



  //===Lambda(2350) 9/2+
  RooRealVar m0_2350("m0_2350","m0",2.35);
  RooRealVar width_2350("width_2350","width",0.15);
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
  L_2350->add(scRe);
  L_2350->add(scIm);
  L_2350->add(m0_2350);
  L_2350->add(width_2350);
  L_2350->add(RooRealConstant::value(typeR));
  L_2350->add(RooRealConstant::value(9.));//2 x spin
  L_2350->add(RooRealConstant::value(1)); //parity
  
//  list->Add(L_2350);
  //2585
  RooRealVar m0_2585("m0_2585","m0",2.585);
  RooRealVar width_2585("width_2585","width",0.3);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2585("a_p1p1_2585","",1.);
  RooRealVar p_p1p1_2585("p_p1p1_2585","",0.);
  RooRealVar a_p100_2585("a_p100_2585","",0.);
  RooRealVar p_p100_2585("p_p100_2585","",0.);
  RooRealVar a_m100_2585("a_m100_2585","",0.);
  RooRealVar p_m100_2585("p_m100_2585","",0.);
  RooRealVar a_m1m1_2585("a_m1m1_2585","",0.);
  RooRealVar p_m1m1_2585("p_m1m1_2585","",0.);
  RooRealVar a_p3p1_2585("a_p3p1_2585","",0.);
  RooRealVar p_p3p1_2585("p_p3p1_2585","",0.);
  RooRealVar a_m3m1_2585("a_m3m1_2585","",0.);
  RooRealVar p_m3m1_2585("p_m3m1_2585","",0.);

  RooArgList* L_2585 = new RooArgList(a_p1p1_2585,p_p1p1_2585,
				      a_p100_2585,p_p100_2585,
				      a_m100_2585,p_m100_2585,
				      a_m1m1_2585,p_m1m1_2585,"L_2585");
  L_2585->add(a_p3p1_2585);
  L_2585->add(p_p3p1_2585);
  L_2585->add(a_m3m1_2585);
  L_2585->add(p_m3m1_2585);
  L_2585->add(scRe);
  L_2585->add(scIm);
  L_2585->add(m0_2585);
  L_2585->add(width_2585);
  L_2585->add(RooRealConstant::value(typeR));
  L_2585->add(RooRealConstant::value(5.));//2 x spin
  L_2585->add(RooRealConstant::value(-1)); //parity

  //L*(2000)
  RooRealVar m0_2000("m0_2000","m0",1.95549);
  RooRealVar width_2000("width_2000","width",0.141);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2000("a_p1p1_2000","",0.);
  RooRealVar p_p1p1_2000("p_p1p1_2000","",0.);
  RooRealVar a_p100_2000("a_p100_2000","",0.);
  RooRealVar p_p100_2000("p_p100_2000","",0.);
  RooRealVar a_m100_2000("a_m100_2000","",0.);
  RooRealVar p_m100_2000("p_m100_2000","",0.);
  RooRealVar a_m1m1_2000("a_m1m1_2000","",0.);
  RooRealVar p_m1m1_2000("p_m1m1_2000","",0.);
  RooRealVar a_p3p1_2000("a_p3p1_2000","",0.);
  RooRealVar p_p3p1_2000("p_p3p1_2000","",0.);
  RooRealVar a_m3m1_2000("a_m3m1_2000","",0.);
  RooRealVar p_m3m1_2000("p_m3m1_2000","",0.);

  RooArgList* L_2000 = new RooArgList(a_p1p1_2000,p_p1p1_2000,
				      a_p100_2000,p_p100_2000,
				      a_m100_2000,p_m100_2000,
				      a_m1m1_2000,p_m1m1_2000,"L_2000");
  L_2000->add(a_p3p1_2000);
  L_2000->add(p_p3p1_2000);
  L_2000->add(a_m3m1_2000);
  L_2000->add(p_m3m1_2000);
  L_2000->add(scRe);
  L_2000->add(scIm);
  L_2000->add(m0_2000);
  L_2000->add(width_2000);
  L_2000->add(RooRealConstant::value(typeR));
  L_2000->add(RooRealConstant::value(1.));//2 x spin
  L_2000->add(RooRealConstant::value(-1)); //parity


  //L*(1710)
  RooRealVar m0_1710("m0_1710","m0",1.713);
  RooRealVar width_1710("width_1710","width",0.18);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1710("a_p1p1_1710","",0.);
  RooRealVar p_p1p1_1710("p_p1p1_1710","",0.);
  RooRealVar a_p100_1710("a_p100_1710","",0.);
  RooRealVar p_p100_1710("p_p100_1710","",0.);
  RooRealVar a_m100_1710("a_m100_1710","",0.);
  RooRealVar p_m100_1710("p_m100_1710","",0.);
  RooRealVar a_m1m1_1710("a_m1m1_1710","",0.);
  RooRealVar p_m1m1_1710("p_m1m1_1710","",0.);
  RooRealVar a_p3p1_1710("a_p3p1_1710","",0.);
  RooRealVar p_p3p1_1710("p_p3p1_1710","",0.);
  RooRealVar a_m3m1_1710("a_m3m1_1710","",0.);
  RooRealVar p_m3m1_1710("p_m3m1_1710","",0.);

  RooArgList* L_1710 = new RooArgList(a_p1p1_1710,p_p1p1_1710,
				      a_p100_1710,p_p100_1710,
				      a_m100_1710,p_m100_1710,
				      a_m1m1_1710,p_m1m1_1710,"L_1710");
  L_1710->add(a_p3p1_1710);
  L_1710->add(p_p3p1_1710);
  L_1710->add(a_m3m1_1710);
  L_1710->add(p_m3m1_1710);
  L_1710->add(scRe);
  L_1710->add(scIm);
  L_1710->add(m0_1710);
  L_1710->add(width_1710);
  L_1710->add(RooRealConstant::value(typeR));
  L_1710->add(RooRealConstant::value(1.));//2 x spin
  L_1710->add(RooRealConstant::value(1)); //parity

  //L*(2020) 7/2+
  RooRealVar m0_2020("m0_2020","m0",2.043);
  RooRealVar width_2020("width_2020","width",0.2);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2020("a_p1p1_2020","",0.);
  RooRealVar p_p1p1_2020("p_p1p1_2020","",0.);
  RooRealVar a_p100_2020("a_p100_2020","",0.);
  RooRealVar p_p100_2020("p_p100_2020","",0.);
  RooRealVar a_m100_2020("a_m100_2020","",0.);
  RooRealVar p_m100_2020("p_m100_2020","",0.);
  RooRealVar a_m1m1_2020("a_m1m1_2020","",0.);
  RooRealVar p_m1m1_2020("p_m1m1_2020","",0.);
  RooRealVar a_p3p1_2020("a_p3p1_2020","",0.);
  RooRealVar p_p3p1_2020("p_p3p1_2020","",0.);
  RooRealVar a_m3m1_2020("a_m3m1_2020","",0.);
  RooRealVar p_m3m1_2020("p_m3m1_2020","",0.);

  RooArgList* L_2020 = new RooArgList(a_p1p1_2020,p_p1p1_2020,
				      a_p100_2020,p_p100_2020,
				      a_m100_2020,p_m100_2020,
				      a_m1m1_2020,p_m1m1_2020,"L_2020");
  L_2020->add(a_p3p1_2020);
  L_2020->add(p_p3p1_2020);
  L_2020->add(a_m3m1_2020);
  L_2020->add(p_m3m1_2020);
  L_2020->add(scRe);
  L_2020->add(scIm);
  L_2020->add(m0_2020);
  L_2020->add(width_2020);
  L_2020->add(RooRealConstant::value(typeR));
  L_2020->add(RooRealConstant::value(7.));//2 x spin
  L_2020->add(RooRealConstant::value(1)); //parity


  //L*(2300)  7/2+
  RooRealVar m0_2300("m0_2300","m0",2.3, 1.9, 2.5);
  RooRealVar width_2300("width_2300","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2300("a_p1p1_2300","",0.1, -10, 10);
  RooRealVar p_p1p1_2300("p_p1p1_2300","",0.1, -10, 10);
  RooRealVar a_p100_2300("a_p100_2300","",0.);
  RooRealVar p_p100_2300("p_p100_2300","",0.);
  RooRealVar a_m100_2300("a_m100_2300","",0.);
  RooRealVar p_m100_2300("p_m100_2300","",0.);
  RooRealVar a_m1m1_2300("a_m1m1_2300","",0.);
  RooRealVar p_m1m1_2300("p_m1m1_2300","",0.);
  RooRealVar a_p3p1_2300("a_p3p1_2300","",0.);
  RooRealVar p_p3p1_2300("p_p3p1_2300","",0.);
  RooRealVar a_m3m1_2300("a_m3m1_2300","",0.);
  RooRealVar p_m3m1_2300("p_m3m1_2300","",0.);

  RooArgList* L_2300 = new RooArgList(a_p1p1_2300,p_p1p1_2300,
                                      a_p100_2300,p_p100_2300,
                                      a_m100_2300,p_m100_2300,
                                      a_m1m1_2300,p_m1m1_2300,"L_2300");
  L_2300->add(a_p3p1_2300);
  L_2300->add(p_p3p1_2300);
  L_2300->add(a_m3m1_2300);
  L_2300->add(p_m3m1_2300);
  L_2300->add(scRe);
  L_2300->add(scIm);
  L_2300->add(m0_2300);
  L_2300->add(width_2300);
  L_2300->add(RooRealConstant::value(typeR));
  L_2300->add(RooRealConstant::value(7.));//2 x spin
  L_2300->add(RooRealConstant::value(1)); //parity


  //L*(2050) 3/2-
  RooRealVar m0_2050("m0_2050","m0",2.056);
  RooRealVar width_2050("width_2050","width",0.493);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2050("a_p1p1_2050","",0.);
  RooRealVar p_p1p1_2050("p_p1p1_2050","",0.);
  RooRealVar a_p100_2050("a_p100_2050","",0.);
  RooRealVar p_p100_2050("p_p100_2050","",0.);
  RooRealVar a_m100_2050("a_m100_2050","",0.);
  RooRealVar p_m100_2050("p_m100_2050","",0.);
  RooRealVar a_m1m1_2050("a_m1m1_2050","",0.);
  RooRealVar p_m1m1_2050("p_m1m1_2050","",0.);
  RooRealVar a_p3p1_2050("a_p3p1_2050","",0.);
  RooRealVar p_p3p1_2050("p_p3p1_2050","",0.);
  RooRealVar a_m3m1_2050("a_m3m1_2050","",0.);
  RooRealVar p_m3m1_2050("p_m3m1_2050","",0.);

  RooArgList* L_2050 = new RooArgList(a_p1p1_2050,p_p1p1_2050,
				      a_p100_2050,p_p100_2050,
				      a_m100_2050,p_m100_2050,
				      a_m1m1_2050,p_m1m1_2050,"L_2050");
  L_2050->add(a_p3p1_2050);
  L_2050->add(p_p3p1_2050);
  L_2050->add(a_m3m1_2050);
  L_2050->add(p_m3m1_2050);
  L_2050->add(scRe);
  L_2050->add(scIm);
  L_2050->add(m0_2050);
  L_2050->add(width_2050);
  L_2050->add(RooRealConstant::value(typeR));
  L_2050->add(RooRealConstant::value(3.));//2 x spin
  L_2050->add(RooRealConstant::value(-1)); //parity


  //2320 1/2+ 
  RooRealVar m0_2320("m0_2320","m0",2.1, 2.0, 2.4);
  RooRealVar width_2320("width_2320","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2320("a_p1p1_2320","",0.1, -10, 10);
  RooRealVar p_p1p1_2320("p_p1p1_2320","",0.1, -10, 10);
  RooRealVar a_p100_2320("a_p100_2320","",0.);
  RooRealVar p_p100_2320("p_p100_2320","",0.);
  RooRealVar a_m100_2320("a_m100_2320","",0.);
  RooRealVar p_m100_2320("p_m100_2320","",0.);
  RooRealVar a_m1m1_2320("a_m1m1_2320","",0.);
  RooRealVar p_m1m1_2320("p_m1m1_2320","",0.);
  RooRealVar a_p3p1_2320("a_p3p1_2320","",0.);
  RooRealVar p_p3p1_2320("p_p3p1_2320","",0.);
  RooRealVar a_m3m1_2320("a_m3m1_2320","",0.);
  RooRealVar p_m3m1_2320("p_m3m1_2320","",0.);

  RooArgList* L_2320 = new RooArgList(a_p1p1_2320,p_p1p1_2320,
                                      a_p100_2320,p_p100_2320,
                                      a_m100_2320,p_m100_2320,
                                      a_m1m1_2320,p_m1m1_2320,"L_2320");
  L_2320->add(a_p3p1_2320);
  L_2320->add(p_p3p1_2320);
  L_2320->add(a_m3m1_2320);
  L_2320->add(p_m3m1_2320);
  L_2320->add(scRe);
  L_2320->add(scIm);
  L_2320->add(m0_2320);
  L_2320->add(width_2320);
  L_2320->add(RooRealConstant::value(typeR));
  L_2320->add(RooRealConstant::value(1.));//2 x spin
  L_2320->add(RooRealConstant::value(1)); //parity



  //2330 1/2- 
  RooRealVar m0_2330("m0_2330","m0",2.3, 2.15, 2.4);
  RooRealVar width_2330("width_2330","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2330("a_p1p1_2330","",0.1, -10, 10);
  RooRealVar p_p1p1_2330("p_p1p1_2330","",0.1, -10, 10);
  RooRealVar a_p100_2330("a_p100_2330","",0.);
  RooRealVar p_p100_2330("p_p100_2330","",0.);
  RooRealVar a_m100_2330("a_m100_2330","",0.);
  RooRealVar p_m100_2330("p_m100_2330","",0.);
  RooRealVar a_m1m1_2330("a_m1m1_2330","",0.);
  RooRealVar p_m1m1_2330("p_m1m1_2330","",0.);
  RooRealVar a_p3p1_2330("a_p3p1_2330","",0.);
  RooRealVar p_p3p1_2330("p_p3p1_2330","",0.);
  RooRealVar a_m3m1_2330("a_m3m1_2330","",0.);
  RooRealVar p_m3m1_2330("p_m3m1_2330","",0.);

  RooArgList* L_2330 = new RooArgList(a_p1p1_2330,p_p1p1_2330,
                                      a_p100_2330,p_p100_2330,
                                      a_m100_2330,p_m100_2330,
                                      a_m1m1_2330,p_m1m1_2330,"L_2330");
  L_2330->add(a_p3p1_2330);
  L_2330->add(p_p3p1_2330);
  L_2330->add(a_m3m1_2330);
  L_2330->add(p_m3m1_2330);
  L_2330->add(scRe);
  L_2330->add(scIm);
  L_2330->add(m0_2330);
  L_2330->add(width_2330);
  L_2330->add(RooRealConstant::value(typeR));
  L_2330->add(RooRealConstant::value(1.));//2 x spin
  L_2330->add(RooRealConstant::value(-1)); //parity



  //2340 3/2+ 
  RooRealVar m0_2340("m0_2340","m0",2.1, 2.0, 2.4);
  RooRealVar width_2340("width_2340","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2340("a_p1p1_2340","",0.05, -100, 100);
  RooRealVar p_p1p1_2340("p_p1p1_2340","",0.05, -100, 100);
  RooRealVar a_p100_2340("a_p100_2340","",0.);
  RooRealVar p_p100_2340("p_p100_2340","",0.);
  RooRealVar a_m100_2340("a_m100_2340","",0.);
  RooRealVar p_m100_2340("p_m100_2340","",0.);
  RooRealVar a_m1m1_2340("a_m1m1_2340","",0.);
  RooRealVar p_m1m1_2340("p_m1m1_2340","",0.);
  RooRealVar a_p3p1_2340("a_p3p1_2340","",0.);
  RooRealVar p_p3p1_2340("p_p3p1_2340","",0.);
  RooRealVar a_m3m1_2340("a_m3m1_2340","",0.);
  RooRealVar p_m3m1_2340("p_m3m1_2340","",0.);

  RooArgList* L_2340 = new RooArgList(a_p1p1_2340,p_p1p1_2340,
                                      a_p100_2340,p_p100_2340,
                                      a_m100_2340,p_m100_2340,
                                      a_m1m1_2340,p_m1m1_2340,"L_2340");
  L_2340->add(a_p3p1_2340);
  L_2340->add(p_p3p1_2340);
  L_2340->add(a_m3m1_2340);
  L_2340->add(p_m3m1_2340);
  L_2340->add(scRe);
  L_2340->add(scIm);
  L_2340->add(m0_2340);
  L_2340->add(width_2340);
  L_2340->add(RooRealConstant::value(typeR));
  L_2340->add(RooRealConstant::value(3.));//2 x spin
  L_2340->add(RooRealConstant::value(1)); //parity


  //1910 3/2- 
  RooRealVar m0_1910("m0_1910","m0",1.9, 1.8, 2.0);
  RooRealVar width_1910("width_1910","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_1910("a_p1p1_1910","",0.05, -100, 100);
  RooRealVar p_p1p1_1910("p_p1p1_1910","",0.05, -100, 100);
  RooRealVar a_p100_1910("a_p100_1910","",0.);
  RooRealVar p_p100_1910("p_p100_1910","",0.);
  RooRealVar a_m100_1910("a_m100_1910","",0.);
  RooRealVar p_m100_1910("p_m100_1910","",0.);
  RooRealVar a_m1m1_1910("a_m1m1_1910","",0.);
  RooRealVar p_m1m1_1910("p_m1m1_1910","",0.);
  RooRealVar a_p3p1_1910("a_p3p1_1910","",0.);
  RooRealVar p_p3p1_1910("p_p3p1_1910","",0.);
  RooRealVar a_m3m1_1910("a_m3m1_1910","",0.);
  RooRealVar p_m3m1_1910("p_m3m1_1910","",0.);

  RooArgList* L_1910 = new RooArgList(a_p1p1_1910,p_p1p1_1910,
                                      a_p100_1910,p_p100_1910,
                                      a_m100_1910,p_m100_1910,
                                      a_m1m1_1910,p_m1m1_1910,"L_1910");
  L_1910->add(a_p3p1_1910);
  L_1910->add(p_p3p1_1910);
  L_1910->add(a_m3m1_1910);
  L_1910->add(p_m3m1_1910);
  L_1910->add(scRe);
  L_1910->add(scIm);
  L_1910->add(m0_1910);
  L_1910->add(width_1910);
  L_1910->add(RooRealConstant::value(typeR));
  L_1910->add(RooRealConstant::value(3.));//2 x spin
  L_1910->add(RooRealConstant::value(-1)); //parity


  //2440 5/2+ 
  RooRealVar m0_2440("m0_2440","m0",2.4, 2.25, 2.5);
  RooRealVar width_2440("width_2440","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2440("a_p1p1_2440","",0.1, -10, 10);
  RooRealVar p_p1p1_2440("p_p1p1_2440","",0.1, -10, 10);
  RooRealVar a_p100_2440("a_p100_2440","",0.);
  RooRealVar p_p100_2440("p_p100_2440","",0.);
  RooRealVar a_m100_2440("a_m100_2440","",0.);
  RooRealVar p_m100_2440("p_m100_2440","",0.);
  RooRealVar a_m1m1_2440("a_m1m1_2440","",0.);
  RooRealVar p_m1m1_2440("p_m1m1_2440","",0.);
  RooRealVar a_p3p1_2440("a_p3p1_2440","",0.);
  RooRealVar p_p3p1_2440("p_p3p1_2440","",0.);
  RooRealVar a_m3m1_2440("a_m3m1_2440","",0.);
  RooRealVar p_m3m1_2440("p_m3m1_2440","",0.);

  RooArgList* L_2440 = new RooArgList(a_p1p1_2440,p_p1p1_2440,
                                      a_p100_2440,p_p100_2440,
                                      a_m100_2440,p_m100_2440,
                                      a_m1m1_2440,p_m1m1_2440,"L_2440");
  L_2440->add(a_p3p1_2440);
  L_2440->add(p_p3p1_2440);
  L_2440->add(a_m3m1_2440);
  L_2440->add(p_m3m1_2440);
  L_2440->add(scRe);
  L_2440->add(scIm);
  L_2440->add(m0_2440);
  L_2440->add(width_2440);
  L_2440->add(RooRealConstant::value(typeR));
  L_2440->add(RooRealConstant::value(5.));//2 x spin
  L_2440->add(RooRealConstant::value(1)); //parity


  //2130 5/2- 
  RooRealVar m0_2130("m0_2130","m0",2.1, 2.0, 2.4);
  RooRealVar width_2130("width_2130","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2130("a_p1p1_2130","",0.05, -100, 100);
  RooRealVar p_p1p1_2130("p_p1p1_2130","",0.05, -100, 100);
  RooRealVar a_p100_2130("a_p100_2130","",0.);
  RooRealVar p_p100_2130("p_p100_2130","",0.);
  RooRealVar a_m100_2130("a_m100_2130","",0.);
  RooRealVar p_m100_2130("p_m100_2130","",0.);
  RooRealVar a_m1m1_2130("a_m1m1_2130","",0.);
  RooRealVar p_m1m1_2130("p_m1m1_2130","",0.);
  RooRealVar a_p3p1_2130("a_p3p1_2130","",0.);
  RooRealVar p_p3p1_2130("p_p3p1_2130","",0.);
  RooRealVar a_m3m1_2130("a_m3m1_2130","",0.);
  RooRealVar p_m3m1_2130("p_m3m1_2130","",0.);

  RooArgList* L_2130 = new RooArgList(a_p1p1_2130,p_p1p1_2130,
                                      a_p100_2130,p_p100_2130,
                                      a_m100_2130,p_m100_2130,
                                      a_m1m1_2130,p_m1m1_2130,"L_2130");
  L_2130->add(a_p3p1_2130);
  L_2130->add(p_p3p1_2130);
  L_2130->add(a_m3m1_2130);
  L_2130->add(p_m3m1_2130);
  L_2130->add(scRe);
  L_2130->add(scIm);
  L_2130->add(m0_2130);
  L_2130->add(width_2130);
  L_2130->add(RooRealConstant::value(typeR));
  L_2130->add(RooRealConstant::value(5.));//2 x spin
  L_2130->add(RooRealConstant::value(-1)); //parity



  //2310 5/2- 
  RooRealVar m0_2310("m0_2310","m0",2.3, 2.2, 2.4);
  RooRealVar width_2310("width_2310","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2310("a_p1p1_2310","",0.1, -10, 10);
  RooRealVar p_p1p1_2310("p_p1p1_2310","",0.1, -10, 10);
  RooRealVar a_p100_2310("a_p100_2310","",0.);
  RooRealVar p_p100_2310("p_p100_2310","",0.);
  RooRealVar a_m100_2310("a_m100_2310","",0.);
  RooRealVar p_m100_2310("p_m100_2310","",0.);
  RooRealVar a_m1m1_2310("a_m1m1_2310","",0.);
  RooRealVar p_m1m1_2310("p_m1m1_2310","",0.);
  RooRealVar a_p3p1_2310("a_p3p1_2310","",0.);
  RooRealVar p_p3p1_2310("p_p3p1_2310","",0.);
  RooRealVar a_m3m1_2310("a_m3m1_2310","",0.);
  RooRealVar p_m3m1_2310("p_m3m1_2310","",0.);

  RooArgList* L_2310 = new RooArgList(a_p1p1_2310,p_p1p1_2310,
                                      a_p100_2310,p_p100_2310,
                                      a_m100_2310,p_m100_2310,
                                      a_m1m1_2310,p_m1m1_2310,"L_2310");
  L_2310->add(a_p3p1_2310);
  L_2310->add(p_p3p1_2310);
  L_2310->add(a_m3m1_2310);
  L_2310->add(p_m3m1_2310);
  L_2310->add(scRe);
  L_2310->add(scIm);
  L_2310->add(m0_2310);
  L_2310->add(width_2310);
  L_2310->add(RooRealConstant::value(typeR));
  L_2310->add(RooRealConstant::value(5.));//2 x spin
  L_2310->add(RooRealConstant::value(-1)); //parity


  //2305 7/2- 
  RooRealVar m0_2305("m0_2305","m0",2.3, 2.2, 2.4);
  RooRealVar width_2305("width_2305","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2305("a_p1p1_2305","",0.1, -10, 10);
  RooRealVar p_p1p1_2305("p_p1p1_2305","",0.1, -10, 10);
  RooRealVar a_p100_2305("a_p100_2305","",0.);
  RooRealVar p_p100_2305("p_p100_2305","",0.);
  RooRealVar a_m100_2305("a_m100_2305","",0.);
  RooRealVar p_m100_2305("p_m100_2305","",0.);
  RooRealVar a_m1m1_2305("a_m1m1_2305","",0.);
  RooRealVar p_m1m1_2305("p_m1m1_2305","",0.);
  RooRealVar a_p3p1_2305("a_p3p1_2305","",0.);
  RooRealVar p_p3p1_2305("p_p3p1_2305","",0.);
  RooRealVar a_m3m1_2305("a_m3m1_2305","",0.);
  RooRealVar p_m3m1_2305("p_m3m1_2305","",0.);

  RooArgList* L_2305 = new RooArgList(a_p1p1_2305,p_p1p1_2305,
                                      a_p100_2305,p_p100_2305,
                                      a_m100_2305,p_m100_2305,
                                      a_m1m1_2305,p_m1m1_2305,"L_2305");
  L_2305->add(a_p3p1_2305);
  L_2305->add(p_p3p1_2305);
  L_2305->add(a_m3m1_2305);
  L_2305->add(p_m3m1_2305);
  L_2305->add(scRe);
  L_2305->add(scIm);
  L_2305->add(m0_2305);
  L_2305->add(width_2305);
  L_2305->add(RooRealConstant::value(typeR));
  L_2305->add(RooRealConstant::value(7.));//2 x spin
  L_2305->add(RooRealConstant::value(-1)); //parity



  //2405 9/2-  
  RooRealVar m0_2405("m0_2405","m0",2.4, 2.25, 2.5);
  RooRealVar width_2405("width_2405","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2405("a_p1p1_2405","",0.1, -10, 10);
  RooRealVar p_p1p1_2405("p_p1p1_2405","",0.1, -10, 10);
  RooRealVar a_p100_2405("a_p100_2405","",0.);
  RooRealVar p_p100_2405("p_p100_2405","",0.);
  RooRealVar a_m100_2405("a_m100_2405","",0.);
  RooRealVar p_m100_2405("p_m100_2405","",0.);
  RooRealVar a_m1m1_2405("a_m1m1_2405","",0.);
  RooRealVar p_m1m1_2405("p_m1m1_2405","",0.);
  RooRealVar a_p3p1_2405("a_p3p1_2405","",0.);
  RooRealVar p_p3p1_2405("p_p3p1_2405","",0.);
  RooRealVar a_m3m1_2405("a_m3m1_2405","",0.);
  RooRealVar p_m3m1_2405("p_m3m1_2405","",0.);

  RooArgList* L_2405 = new RooArgList(a_p1p1_2405,p_p1p1_2405,
                                      a_p100_2405,p_p100_2405,
                                      a_m100_2405,p_m100_2405,
                                      a_m1m1_2405,p_m1m1_2405,"L_2405");
  L_2405->add(a_p3p1_2405);
  L_2405->add(p_p3p1_2405);
  L_2405->add(a_m3m1_2405);
  L_2405->add(p_m3m1_2405);
  L_2405->add(scRe);
  L_2405->add(scIm);
  L_2405->add(m0_2405);
  L_2405->add(width_2405);
  L_2405->add(RooRealConstant::value(typeR));
  L_2405->add(RooRealConstant::value(9.));//2 x spin
  L_2405->add(RooRealConstant::value(-1)); //parity

  //2400 3/2- as additional Ls state
  RooRealVar m0_2400("m0_2400","m0", 2.4, 1.7, 3.0); // Lower limit of 1.7 to avoid L(1910) region
  RooRealVar width_2400("width_2400","width",0.1, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2400("a_p1p1_2400","",0.01, -100, 100);
  RooRealVar p_p1p1_2400("p_p1p1_2400","",0.01, -100, 100);
  RooRealVar a_p100_2400("a_p100_2400","",0.);
  RooRealVar p_p100_2400("p_p100_2400","",0.);
  RooRealVar a_m100_2400("a_m100_2400","",0.);
  RooRealVar p_m100_2400("p_m100_2400","",0.);
  RooRealVar a_m1m1_2400("a_m1m1_2400","",0.);
  RooRealVar p_m1m1_2400("p_m1m1_2400","",0.);
  RooRealVar a_p3p1_2400("a_p3p1_2400","",0.);
  RooRealVar p_p3p1_2400("p_p3p1_2400","",0.);
  RooRealVar a_m3m1_2400("a_m3m1_2400","",0.);
  RooRealVar p_m3m1_2400("p_m3m1_2400","",0.);

  RooArgList* L_2400 = new RooArgList(a_p1p1_2400,p_p1p1_2400,
                                      a_p100_2400,p_p100_2400,
                                      a_m100_2400,p_m100_2400,
                                      a_m1m1_2400,p_m1m1_2400,"L_2400");
  L_2400->add(a_p3p1_2400);
  L_2400->add(p_p3p1_2400);
  L_2400->add(a_m3m1_2400);
  L_2400->add(p_m3m1_2400);
  L_2400->add(scRe);
  L_2400->add(scIm);
  L_2400->add(m0_2400);
  L_2400->add(width_2400);
  L_2400->add(RooRealConstant::value(typeR));
  L_2400->add(RooRealConstant::value(3.));//2 x spin
  L_2400->add(RooRealConstant::value(-1)); //parity

  static int Nsp=30;
  char Wname[20];
  RooRealVar *SMass[Nsp], *SRAm[Nsp], *SIAm[Nsp];
  RooArgList *Swave = new RooArgList("Swave");
  double xm[Nsp], sr0[Nsp], sp0[Nsp];
  std::ifstream inswav;
  inswav.open("toyr_fine_bin.dat");
 
  int iswav=0;
  while(1) {
    inswav >> xm[iswav] >> sr0[iswav] >> sp0[iswav];
    if(!inswav.good()) break;
    std::cout << xm[iswav] << " " << sr0[iswav] << " " << sp0[iswav] << std::endl;
     iswav++;
   } 
  inswav.close();
  int Npoint = iswav;
  std::cout << "point of Swave " << Npoint << std::endl;
  if(Npoint!=NUMSP) {
    std::cout <<  "SPline number is wrong " << std::endl;
    assert(0);
  }
//  double sr0[Nsp],  = {};
//  RooDalitzRAmlitude dz;
  for(int i=0; i<Npoint; ++i) {
    double fac(1.0);
    if(i>0) {
      fac = 1.0;
      std::cout << fac << std::endl;
    }
    sprintf(Wname,"SR_%02d",(i-7>=0 ? (i==7 ? 0 : (i==8 ? 666 : i-8) ) : i+40));
    SRAm[i] = new RooRealVar(Wname,Wname,sr0[i]*fac);
    SRAm[i]->setConstant(0);
    Swave->add(*SRAm[i]);
    sprintf(Wname,"SI_%02d",(i-7>=0 ? (i==7 ? 0 : (i==8 ? 666 : i-8) ) : i+40));
    SIAm[i] = new RooRealVar(Wname,Wname,sp0[i]*fac);
    SIAm[i]->setConstant(0);
    Swave->add(*SIAm[i]);
    sprintf(Wname,"m_%02d",(i-7>=0 ? (i==7 ? 0 : (i==8 ? 666 : i-8) ) : i+40));
    SMass[i] = new RooRealVar(Wname,Wname,xm[i]);
    Swave->add(*SMass[i]);    
  }
  // SRAm[3]->setConstant(1);
  // SIAm[3]->setConstant(1);
  RooArgList* L_S01 = new RooArgList(a_p1p1_2000,p_p1p1_2000,
				      RooRealConstant::value(0),RooRealConstant::value(0),
				      RooRealConstant::value(0),RooRealConstant::value(0),
				      RooRealConstant::value(0),RooRealConstant::value(0),"L_S01");
  L_S01->add(a_p3p1_2000);
  L_S01->add(p_p3p1_2000);
  L_S01->add(a_m3m1_2000);
  L_S01->add(p_m3m1_2000);

  RooRealVar m0_S01("m0_S01","m0",0.95);
  RooRealVar width_S01("width_S01","width",0.3);    
  L_S01->add(*Swave);
  L_S01->add(scRe);
  L_S01->add(scIm);  
  L_S01->add(m0_S01);
  L_S01->add(width_S01);
  L_S01->add(RooRealConstant::value(MODELINDEPENDENT));
  L_S01->add(RooRealConstant::value(1.));//2 x spin
  L_S01->add(RooRealConstant::value(-1)); //parity
  

  RooRealVar *SRAm2[Nsp], *SIAm2[Nsp];
  RooArgList *Swave2 = new RooArgList("Swave2");
  for(int i=0; i<Npoint; ++i) {
    double fac(1.0);
    if(i>0) {
      fac = 1.0;
      std::cout << fac << std::endl;
    }
    sprintf(Wname,"SR2_%02d",(i-7>=0 ? (i==7 ? 0 : (i==8 ? 666 : i-8) ) : i+40));
    SRAm2[i] = new RooRealVar(Wname,Wname,sr0[i]*fac);
    SRAm2[i]->setConstant(0);
    Swave2->add(*SRAm2[i]);
    sprintf(Wname,"SI2_%02d",(i-7>=0 ? (i==7 ? 0 : (i==8 ? 666 : i-8) ) : i+40));
    SIAm2[i] = new RooRealVar(Wname,Wname,sp0[i]*fac);
    SIAm2[i]->setConstant(0);
    Swave2->add(*SIAm2[i]);
    Swave2->add(*SMass[i]);
  }
  // SRAm2[3]->setConstant(1);
  // SIAm2[3]->setConstant(1);
  RooArgList* L_S01_2 = new RooArgList(RooRealConstant::value(0),RooRealConstant::value(0),
				      a_p100_2000,p_p100_2000,
				      a_m100_2000,p_m100_2000,
				      a_m1m1_2000,p_m1m1_2000,"L_S01_2");
  L_S01_2->add(a_p3p1_2000);
  L_S01_2->add(p_p3p1_2000);
  L_S01_2->add(a_m3m1_2000);
  L_S01_2->add(p_m3m1_2000);

  L_S01_2->add(*Swave2);
  L_S01_2->add(scRe);
  L_S01_2->add(scIm);  
  L_S01_2->add(m0_S01);
  L_S01_2->add(width_S01);
  L_S01_2->add(RooRealConstant::value(MODELINDEPENDENT));
  L_S01_2->add(RooRealConstant::value(1.));//2 x spin
  L_S01_2->add(RooRealConstant::value(-1)); //parity

  //===Non-resonant 3/2-
  RooRealVar m0_NR3m("m0_NR3m","m0",-1.);
  RooRealVar width_NR3m("width_NR3m","width",-1.);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_NR3m("a_p1p1_NR3m","",1.);
  RooRealVar p_p1p1_NR3m("p_p1p1_NR3m","",0.);
  RooRealVar a_p100_NR3m("a_p100_NR3m","",0);
  RooRealVar p_p100_NR3m("p_p100_NR3m","",0);
  RooRealVar a_m100_NR3m("a_m100_NR3m","",0);
  RooRealVar p_m100_NR3m("p_m100_NR3m","",0);
  RooRealVar a_m1m1_NR3m("a_m1m1_NR3m","",0);
  RooRealVar p_m1m1_NR3m("p_m1m1_NR3m","",0);
  RooRealVar a_p3p1_NR3m("a_p3p1_NR3m","",0);
  RooRealVar p_p3p1_NR3m("p_p3p1_NR3m","",0);
  RooRealVar a_m3m1_NR3m("a_m3m1_NR3m","",0);
  RooRealVar p_m3m1_NR3m("p_m3m1_NR3m","",0);

  RooArgList* L_NR3m = new RooArgList(a_p1p1_NR3m,p_p1p1_NR3m,
              a_p100_NR3m,p_p100_NR3m,
              a_m100_NR3m,p_m100_NR3m,
              a_m1m1_NR3m,p_m1m1_NR3m,"L_NR3m");
  L_NR3m->add(a_p3p1_NR3m);
  L_NR3m->add(p_p3p1_NR3m);
  L_NR3m->add(a_m3m1_NR3m);
  L_NR3m->add(p_m3m1_NR3m);
  L_NR3m->add(scRe);
  L_NR3m->add(scIm);
  L_NR3m->add(m0_NR3m);
  L_NR3m->add(width_NR3m);
  L_NR3m->add(RooRealConstant::value(NONRESONANT));
  L_NR3m->add(RooRealConstant::value(3.));//2 x spin
  L_NR3m->add(RooRealConstant::value(-1)); //parity

  //===Non-resonant 3/2+
  RooRealVar m0_NR3p("m0_NR3p","m0",-1.);
  RooRealVar width_NR3p("width_NR3p","width",-1.);
  RooRealVar a_p1p1_NR3p("a_p1p1_NR3p","",1.);
  RooRealVar p_p1p1_NR3p("p_p1p1_NR3p","",0.);
  RooRealVar a_p100_NR3p("a_p100_NR3p","",0);
  RooRealVar p_p100_NR3p("p_p100_NR3p","",0);
  RooRealVar a_m100_NR3p("a_m100_NR3p","",0);
  RooRealVar p_m100_NR3p("p_m100_NR3p","",0);
  RooRealVar a_m1m1_NR3p("a_m1m1_NR3p","",0);
  RooRealVar p_m1m1_NR3p("p_m1m1_NR3p","",0);
  RooRealVar a_p3p1_NR3p("a_p3p1_NR3p","",0);
  RooRealVar p_p3p1_NR3p("p_p3p1_NR3p","",0);
  RooRealVar a_m3m1_NR3p("a_m3m1_NR3p","",0);
  RooRealVar p_m3m1_NR3p("p_m3m1_NR3p","",0);

  RooArgList* L_NR3p = new RooArgList(a_p1p1_NR3p,p_p1p1_NR3p,
              a_p100_NR3p,p_p100_NR3p,
              a_m100_NR3p,p_m100_NR3p,
              a_m1m1_NR3p,p_m1m1_NR3p,"L_NR3p");
  L_NR3p->add(a_p3p1_NR3p);
  L_NR3p->add(p_p3p1_NR3p);
  L_NR3p->add(a_m3m1_NR3p);
  L_NR3p->add(p_m3m1_NR3p);
  L_NR3p->add(scRe);
  L_NR3p->add(scIm);
  L_NR3p->add(m0_NR3p);
  L_NR3p->add(width_NR3p);
  L_NR3p->add(RooRealConstant::value(NONRESONANT));
  L_NR3p->add(RooRealConstant::value(3.));//2 x spin
  L_NR3p->add(RooRealConstant::value(1)); //parity

  //===Non-resonant 1/2+
  RooRealVar m0_NR1p("m0_NR1p","m0",-1.);
  RooRealVar width_NR1p("width_NR1p","width",-1.);
  RooRealVar a_p1p1_NR1p("a_p1p1_NR1p","",1.);
  RooRealVar p_p1p1_NR1p("p_p1p1_NR1p","",0.);
  RooRealVar a_p100_NR1p("a_p100_NR1p","",0);
  RooRealVar p_p100_NR1p("p_p100_NR1p","",0);
  RooRealVar a_m100_NR1p("a_m100_NR1p","",0);
  RooRealVar p_m100_NR1p("p_m100_NR1p","",0);
  RooRealVar a_m1m1_NR1p("a_m1m1_NR1p","",0);
  RooRealVar p_m1m1_NR1p("p_m1m1_NR1p","",0);
  RooRealVar a_p3p1_NR1p("a_p3p1_NR1p","",0);
  RooRealVar p_p3p1_NR1p("p_p3p1_NR1p","",0);
  RooRealVar a_m3m1_NR1p("a_m3m1_NR1p","",0);
  RooRealVar p_m3m1_NR1p("p_m3m1_NR1p","",0);

  RooArgList* L_NR1p = new RooArgList(a_p1p1_NR1p,p_p1p1_NR1p,
              a_p100_NR1p,p_p100_NR1p,
              a_m100_NR1p,p_m100_NR1p,
              a_m1m1_NR1p,p_m1m1_NR1p,"L_NR1p");
  L_NR1p->add(a_p3p1_NR1p);
  L_NR1p->add(p_p3p1_NR1p);
  L_NR1p->add(a_m3m1_NR1p);
  L_NR1p->add(p_m3m1_NR1p);
  L_NR1p->add(scRe);
  L_NR1p->add(scIm);
  L_NR1p->add(m0_NR1p);
  L_NR1p->add(width_NR1p);
  L_NR1p->add(RooRealConstant::value(NONRESONANT));
  L_NR1p->add(RooRealConstant::value(1.));//2 x spin
  L_NR1p->add(RooRealConstant::value(1)); //parity
