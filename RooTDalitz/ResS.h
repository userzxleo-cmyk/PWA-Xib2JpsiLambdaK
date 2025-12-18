    //===Sigma(1660) 1/2+
  RooRealVar m0_S1660("m0_S1660","m0",1.66);
  RooRealVar width_S1660("width_S1660","width",0.1);
  RooRealVar re_0_S1660("re_0_S1660","",1.);
  RooRealVar im_0_S1660("im_0_S1660","",0.);
  RooRealVar re_1_S1660("re_1_S1660","",0);
  RooRealVar im_1_S1660("im_1_S1660","",0);

  RooArgList* S_S1660 = new RooArgList(re_0_S1660,im_0_S1660,
				      re_1_S1660,im_1_S1660,"S_S1660");
  S_S1660->add(m0_S1660);
  S_S1660->add(width_S1660);
  S_S1660->add(RooRealConstant::value(1.));//2 x spin
  S_S1660->add(RooRealConstant::value(1)); //parity


  //===Sigma(S1670) 1/2-
  RooRealVar m0_S1670("m0_S1670","m0",1.67);
  RooRealVar width_S1670("width_S1670","width",0.06);
  RooRealVar re_0_S1670("re_0_S1670","",1.);
  RooRealVar im_0_S1670("im_0_S1670","",0.);
  RooRealVar re_1_S1670("re_1_S1670","",0);
  RooRealVar im_1_S1670("im_1_S1670","",0);

  RooArgList* S_S1670 = new RooArgList(re_0_S1670,im_0_S1670,
				      re_1_S1670,im_1_S1670,"S_S1670");
  S_S1670->add(m0_S1670);
  S_S1670->add(width_S1670);
  S_S1670->add(RooRealConstant::value(3.));//2 x spin
  S_S1670->add(RooRealConstant::value(-1)); //parity
  
  //===Sigma(S1750) 1/2-
  RooRealVar m0_S1750("m0_S1750","m0",1.75);
  RooRealVar width_S1750("width_S1750","width",0.09);
  RooRealVar re_0_S1750("re_0_S1750","",1.);
  RooRealVar im_0_S1750("im_0_S1750","",0.);
  RooRealVar re_1_S1750("re_1_S1750","",0);
  RooRealVar im_1_S1750("im_1_S1750","",0);

  RooArgList* S_S1750 = new RooArgList(re_0_S1750,im_0_S1750,
				      re_1_S1750,im_1_S1750,"S_S1750");
  S_S1750->add(m0_S1750);
  S_S1750->add(width_S1750);
  S_S1750->add(RooRealConstant::value(1.));//2 x spin
  S_S1750->add(RooRealConstant::value(-1)); //parity

  //===Sigma(S1940) 1/2-
  RooRealVar m0_S1940("m0_S1940","m0",1.94);
  RooRealVar width_S1940("width_S1940","width",0.22);
  RooRealVar re_0_S1940("re_0_S1940","",1.);
  RooRealVar im_0_S1940("im_0_S1940","",0.);
  RooRealVar re_1_S1940("re_1_S1940","",0);
  RooRealVar im_1_S1940("im_1_S1940","",0);

  RooArgList* S_S1940 = new RooArgList(re_0_S1940,im_0_S1940,
				      re_1_S1940,im_1_S1940,"S_S1940");
  S_S1940->add(m0_S1940);
  S_S1940->add(width_S1940);
  S_S1940->add(RooRealConstant::value(3.));//2 x spin
  S_S1940->add(RooRealConstant::value(-1)); //parity

