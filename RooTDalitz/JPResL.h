	int typeR(1);
	TList *list = new TList() ;
	RooRealVar scRe("scRe","",1.0);
	RooRealVar scIm("scIm","",0.0);
	//===Xi(1322) 1/2+
	RooRealVar m0_1322("m0_1322","m0",1.32171) ;
	RooRealVar width_1322("width_1322","width",0.0) ;
  	// a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  	RooRealVar a_p1p1_1322("a_p1p1_1322","",0.2) ;
  	RooRealVar p_p1p1_1322("p_p1p1_1322","",-0.1) ;
  	RooRealVar a_p100_1322("a_p100_1322","",0) ;
  	RooRealVar p_p100_1322("p_p100_1322","",0) ;
  	RooRealVar a_m100_1322("a_m100_1322","",0) ;
  	RooRealVar p_m100_1322("p_m100_1322","",0) ;
  	RooRealVar a_m1m1_1322("a_m1m1_1322","",0) ;
  	RooRealVar p_m1m1_1322("p_m1m1_1322","",0) ;
  	RooRealVar a_p3p1_1322("a_p3p1_1322","",0) ;
  	RooRealVar p_p3p1_1322("p_p3p1_1322","",0) ;
  	RooRealVar a_m3m1_1322("a_m3m1_1322","",0) ;
  	RooRealVar p_m3m1_1322("p_m3m1_1322","",0) ;

  	RooArgList* X_1322 = new RooArgList(a_p1p1_1322,p_p1p1_1322,
					    a_p100_1322,p_p100_1322,
				            a_m100_1322,p_m100_1322,
				            a_m1m1_1322,p_m1m1_1322,"X_1322") ;
  	X_1322->add(a_p3p1_1322) ;
  	X_1322->add(p_p3p1_1322) ;
  	X_1322->add(a_m3m1_1322) ;
  	X_1322->add(p_m3m1_1322) ;
  	X_1322->add(scRe) ;
  	X_1322->add(scIm) ;
  	X_1322->add(m0_1322) ;
  	X_1322->add(width_1322) ;
  	X_1322->add(RooRealConstant::value(typeR)) ;
  	X_1322->add(RooRealConstant::value(1.)) ;//2 x spin
  	X_1322->add(RooRealConstant::value(1)) ; //parity
  


	//===Xi(1530) 3/2+
	RooRealVar m0_1530("m0_1530","m0",1.535) ;
	RooRealVar width_1530("width_1530","width",0.0099) ;
  	// a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  	RooRealVar a_p1p1_1530("a_p1p1_1530","",0.2) ;
  	RooRealVar p_p1p1_1530("p_p1p1_1530","",-0.1) ;
  	RooRealVar a_p100_1530("a_p100_1530","",0) ;
  	RooRealVar p_p100_1530("p_p100_1530","",0) ;
  	RooRealVar a_m100_1530("a_m100_1530","",0) ;
  	RooRealVar p_m100_1530("p_m100_1530","",0) ;
  	RooRealVar a_m1m1_1530("a_m1m1_1530","",0) ;
  	RooRealVar p_m1m1_1530("p_m1m1_1530","",0) ;
  	RooRealVar a_p3p1_1530("a_p3p1_1530","",0) ;
  	RooRealVar p_p3p1_1530("p_p3p1_1530","",0) ;
  	RooRealVar a_m3m1_1530("a_m3m1_1530","",0) ;
  	RooRealVar p_m3m1_1530("p_m3m1_1530","",0) ;

  	RooArgList* X_1530 = new RooArgList(a_p1p1_1530,p_p1p1_1530,
					    a_p100_1530,p_p100_1530,
				            a_m100_1530,p_m100_1530,
				            a_m1m1_1530,p_m1m1_1530,"X_1530") ;
  	X_1530->add(a_p3p1_1530) ;
  	X_1530->add(p_p3p1_1530) ;
  	X_1530->add(a_m3m1_1530) ;
  	X_1530->add(p_m3m1_1530) ;
  	X_1530->add(scRe) ;
  	X_1530->add(scIm) ;
  	X_1530->add(m0_1530) ;
  	X_1530->add(width_1530) ;
  	X_1530->add(RooRealConstant::value(typeR)) ;
  	X_1530->add(RooRealConstant::value(3.)) ;//2 x spin
  	X_1530->add(RooRealConstant::value(1)) ; //parity
  


	//===Xi(1690) ?
	RooRealVar m0_1690("m0_1690","m0",1.69) ;
	RooRealVar width_1690("width_1690","width",0.03) ;
  	// a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  	RooRealVar a_p1p1_1690("a_p1p1_1690","",0.2) ;
  	RooRealVar p_p1p1_1690("p_p1p1_1690","",-0.1) ;
  	RooRealVar a_p100_1690("a_p100_1690","",0) ;
  	RooRealVar p_p100_1690("p_p100_1690","",0) ;
  	RooRealVar a_m100_1690("a_m100_1690","",0) ;
  	RooRealVar p_m100_1690("p_m100_1690","",0) ;
  	RooRealVar a_m1m1_1690("a_m1m1_1690","",0) ;
  	RooRealVar p_m1m1_1690("p_m1m1_1690","",0) ;
  	RooRealVar a_p3p1_1690("a_p3p1_1690","",0) ;
  	RooRealVar p_p3p1_1690("p_p3p1_1690","",0) ;
  	RooRealVar a_m3m1_1690("a_m3m1_1690","",0) ;
  	RooRealVar p_m3m1_1690("p_m3m1_1690","",0) ;

  	RooArgList* X_1690 = new RooArgList(a_p1p1_1690,p_p1p1_1690,
					    a_p100_1690,p_p100_1690,
				            a_m100_1690,p_m100_1690,
				            a_m1m1_1690,p_m1m1_1690,"X_1690") ;
  	X_1690->add(a_p3p1_1690) ;
  	X_1690->add(p_p3p1_1690) ;
  	X_1690->add(a_m3m1_1690) ;
  	X_1690->add(p_m3m1_1690) ;
  	X_1690->add(scRe) ;
  	X_1690->add(scIm) ;
  	X_1690->add(m0_1690) ;
  	X_1690->add(width_1690) ;
  	X_1690->add(RooRealConstant::value(typeR)) ;
  	X_1690->add(RooRealConstant::value( J1690 )) ;//2 x spin
  	X_1690->add(RooRealConstant::value( P1690 )) ; //parity
  


	//===Xi(1820) 3/2-
	RooRealVar m0_1820("m0_1820","m0",1.823) ;
	RooRealVar width_1820("width_1820","width",0.024) ;
  	// a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  	RooRealVar a_p1p1_1820("a_p1p1_1820","",0.2) ;
  	RooRealVar p_p1p1_1820("p_p1p1_1820","",-0.1) ;
  	RooRealVar a_p100_1820("a_p100_1820","",0) ;
  	RooRealVar p_p100_1820("p_p100_1820","",0) ;
  	RooRealVar a_m100_1820("a_m100_1820","",0) ;
  	RooRealVar p_m100_1820("p_m100_1820","",0) ;
  	RooRealVar a_m1m1_1820("a_m1m1_1820","",0) ;
  	RooRealVar p_m1m1_1820("p_m1m1_1820","",0) ;
  	RooRealVar a_p3p1_1820("a_p3p1_1820","",0) ;
  	RooRealVar p_p3p1_1820("p_p3p1_1820","",0) ;
  	RooRealVar a_m3m1_1820("a_m3m1_1820","",0) ;
  	RooRealVar p_m3m1_1820("p_m3m1_1820","",0) ;

  	RooArgList* X_1820 = new RooArgList(a_p1p1_1820,p_p1p1_1820,
					    a_p100_1820,p_p100_1820,
				            a_m100_1820,p_m100_1820,
				            a_m1m1_1820,p_m1m1_1820,"X_1820") ;
  	X_1820->add(a_p3p1_1820) ;
  	X_1820->add(p_p3p1_1820) ;
  	X_1820->add(a_m3m1_1820) ;
  	X_1820->add(p_m3m1_1820) ;
  	X_1820->add(scRe) ;
  	X_1820->add(scIm) ;
  	X_1820->add(m0_1820) ;
  	X_1820->add(width_1820) ;
  	X_1820->add(RooRealConstant::value(typeR)) ;
  	X_1820->add(RooRealConstant::value(3.)) ;//2 x spin
  	X_1820->add(RooRealConstant::value(-1)) ; //parity
  


	//===Xi(1950) ?
	RooRealVar m0_1950("m0_1950","m0",1.95) ;
	RooRealVar width_1950("width_1950","width",0.06) ;
  	// a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  	RooRealVar a_p1p1_1950("a_p1p1_1950","",0.2) ;
  	RooRealVar p_p1p1_1950("p_p1p1_1950","",-0.1) ;
  	RooRealVar a_p100_1950("a_p100_1950","",0) ;
  	RooRealVar p_p100_1950("p_p100_1950","",0) ;
  	RooRealVar a_m100_1950("a_m100_1950","",0) ;
  	RooRealVar p_m100_1950("p_m100_1950","",0) ;
  	RooRealVar a_m1m1_1950("a_m1m1_1950","",0) ;
  	RooRealVar p_m1m1_1950("p_m1m1_1950","",0) ;
  	RooRealVar a_p3p1_1950("a_p3p1_1950","",0) ;
  	RooRealVar p_p3p1_1950("p_p3p1_1950","",0) ;
  	RooRealVar a_m3m1_1950("a_m3m1_1950","",0) ;
  	RooRealVar p_m3m1_1950("p_m3m1_1950","",0) ;

  	RooArgList* X_1950 = new RooArgList(a_p1p1_1950,p_p1p1_1950,
					    a_p100_1950,p_p100_1950,
				            a_m100_1950,p_m100_1950,
				            a_m1m1_1950,p_m1m1_1950,"X_1950") ;
  	X_1950->add(a_p3p1_1950) ;
  	X_1950->add(p_p3p1_1950) ;
  	X_1950->add(a_m3m1_1950) ;
  	X_1950->add(p_m3m1_1950) ;
  	X_1950->add(scRe) ;
  	X_1950->add(scIm) ;
  	X_1950->add(m0_1950) ;
  	X_1950->add(width_1950) ;
  	X_1950->add(RooRealConstant::value(typeR)) ;
  	X_1950->add(RooRealConstant::value( J1950 )) ;//2 x spin
  	X_1950->add(RooRealConstant::value( P1950 )) ; //parity
  


	//===Xi(2030) ?
	RooRealVar m0_2030("m0_2030","m0",2.025) ;
	RooRealVar width_2030("width_2030","width",0.02) ;
  	// a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  	RooRealVar a_p1p1_2030("a_p1p1_2030","",0.2) ;
  	RooRealVar p_p1p1_2030("p_p1p1_2030","",-0.1) ;
  	RooRealVar a_p100_2030("a_p100_2030","",0) ;
  	RooRealVar p_p100_2030("p_p100_2030","",0) ;
  	RooRealVar a_m100_2030("a_m100_2030","",0) ;
  	RooRealVar p_m100_2030("p_m100_2030","",0) ;
  	RooRealVar a_m1m1_2030("a_m1m1_2030","",0) ;
  	RooRealVar p_m1m1_2030("p_m1m1_2030","",0) ;
  	RooRealVar a_p3p1_2030("a_p3p1_2030","",0) ;
  	RooRealVar p_p3p1_2030("p_p3p1_2030","",0) ;
  	RooRealVar a_m3m1_2030("a_m3m1_2030","",0) ;
  	RooRealVar p_m3m1_2030("p_m3m1_2030","",0) ;

  	RooArgList* X_2030 = new RooArgList(a_p1p1_2030,p_p1p1_2030,
					    a_p100_2030,p_p100_2030,
				            a_m100_2030,p_m100_2030,
				            a_m1m1_2030,p_m1m1_2030,"X_2030") ;
  	X_2030->add(a_p3p1_2030) ;
  	X_2030->add(p_p3p1_2030) ;
  	X_2030->add(a_m3m1_2030) ;
  	X_2030->add(p_m3m1_2030) ;
  	X_2030->add(scRe) ;
  	X_2030->add(scIm) ;
  	X_2030->add(m0_2030) ;
  	X_2030->add(width_2030) ;
  	X_2030->add(RooRealConstant::value(typeR)) ;
  	X_2030->add(RooRealConstant::value( J2030 )) ;//2 x spin
  	X_2030->add(RooRealConstant::value( P2030 )) ; //parity
  



	RooRealVar m0_NR("m0_NR","m0",0);
	RooRealVar width_NR("width_NR","width",0.);
//a &phase [1/2,1] [1/2,0] [-1/2,0] [-1/2,-1] [3/2,1] [-3/2,-1]
	RooRealVar a_p1p1_NR("a_p1p1_NR","",0.);
	RooRealVar p_p1p1_NR("p_p1p1_NR","",0.);
	RooRealVar a_p100_NR("a_p100_NR","",0.);
	RooRealVar p_p100_NR("p_p100_NR","",0.);
	RooRealVar a_m100_NR("a_m100_NR","",0.);
	RooRealVar p_m100_NR("p_m100_NR","",0.);
	RooRealVar a_m1m1_NR("a_m1m1_NR","",0.);
	RooRealVar p_m1m1_NR("p_m1m1_NR","",0.);
	RooRealVar a_p3p1_NR("a_p3p1_NR","",0.);
	RooRealVar p_p3p1_NR("p_p3p1_NR","",0.);
	RooRealVar a_m3m1_NR("a_m3m1_NR","",0.);
	RooRealVar p_m3m1_NR("p_m3m1_NR","",0.);

	RooArgList* X_NR = new RooArgList(a_p1p1_NR,p_p1p1_NR,
                                      a_p100_NR,p_p100_NR,
                                      a_m100_NR,p_m100_NR,
                                      a_m1m1_NR,p_m1m1_NR,"X_NR");
	X_NR->add(a_p3p1_NR);
	X_NR->add(p_p3p1_NR);
	X_NR->add(a_m3m1_NR);
	X_NR->add(p_m3m1_NR);
  	X_NR->add(scRe) ;
  	X_NR->add(scIm) ;
	X_NR->add(m0_NR);
	X_NR->add(width_NR);
  	X_NR->add(RooRealConstant::value(typeR)) ;
	X_NR->add(RooRealConstant::value(1.));//2 x spin
	X_NR->add(RooRealConstant::value(-1)); //parity


