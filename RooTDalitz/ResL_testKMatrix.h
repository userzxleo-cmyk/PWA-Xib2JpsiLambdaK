  int typeR(1);
  TList *list = new TList();
  RooRealVar scRe("scRe","",1.0);
  RooRealVar scIm("scIm","",0.0);
  //===Lambda(1520) 3/2- D03
  RooRealVar a_1520("a_1520","",1.0);
  RooRealVar p_1520("p_1520","",0.0);
  RooRealVar m0_1520("m0_1520","m0",1.60);
  RooRealVar width_1520("width_1520","width",0.0001);
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
  RooRealVar m0_1600("m0_1600","m0",1.50);
  RooRealVar width_1600("width_1600","width",0.0001);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1600("a_1600","",1.0);
  RooRealVar p_1600("p_1600","",0.0);
  RooRealVar a_p1p1_P01("a_p1p1_P01","",1.39/100);
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
  L_1600->add(RooRealConstant::value(3.));//2 x spin
  L_1600->add(RooRealConstant::value(-1)); //parity


  //===Lambda(1670) 1/2- S01
  RooRealVar m0_1670("m0_1670","m0",1.60);
  RooRealVar width_1670("width_1670","width",0.0001);
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
  RooRealVar a_1800("a_1800","",1.0);
  RooRealVar p_1800("p_1800","",0.0);

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
  RooRealVar a_1810("a_1810","",0.1);
  RooRealVar p_1810("p_1810","",0.1);

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

  //2110 5/2+ F05
  RooRealVar m0_2110("m0_2110","m0",2.11);
  RooRealVar width_2110("width_2110","width",0.2);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2110("a_2110","",0.2);
  RooRealVar p_2110("p_2110","",0.0);

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
  RooRealVar a_p1p1_F07("a_p1p1_F07","",1.);
  RooRealVar p_p1p1_F07("p_p1p1_F07","",0.);
  RooRealVar a_p100_F07("a_p100_F07","",0.);
  RooRealVar p_p100_F07("p_p100_F07","",0.);
  RooRealVar a_m100_F07("a_m100_F07","",0.);
  RooRealVar p_m100_F07("p_m100_F07","",0.);
  RooRealVar a_m1m1_F07("a_m1m1_F07","",0.);
  RooRealVar p_m1m1_F07("p_m1m1_F07","",0.);
  RooRealVar a_p3p1_F07("a_p3p1_F07","",0.);
  RooRealVar p_p3p1_F07("p_p3p1_F07","",0.);
  RooRealVar a_m3m1_F07("a_m3m1_F07","",0.);
  RooRealVar p_m3m1_F07("p_m3m1_F07","",0.);

  RooArgList* L_2100 = new RooArgList(a_p1p1_F07,p_p1p1_F07,
				      a_p100_F07,p_p100_F07,
				      a_m100_F07,p_m100_F07,
				      a_m1m1_F07,p_m1m1_F07,"L_2100");
  L_2100->add(a_p3p1_F07);
  L_2100->add(p_p3p1_F07);
  L_2100->add(a_m3m1_F07);
  L_2100->add(p_m3m1_F07);
  L_2100->add(a_2100);
  L_2100->add(p_2100);
  L_2100->add(m0_2100);
  L_2100->add(width_2100);
  L_2100->add(RooRealConstant::value(typeR));
  L_2100->add(RooRealConstant::value(7.));//2 x spin
  L_2100->add(RooRealConstant::value(-1)); //parity


  //===Lambda(1405) 1/2- S01
  RooRealVar m0_1405("m0_1405","m0",1.50);
  RooRealVar width_1405("width_1405","width",0.0001);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1405("a_1405","",1.0);
  RooRealVar p_1405("p_1405","",0.0);
    
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
  RooRealVar a_2585("a_2585","",0.1);
  RooRealVar p_2585("p_2585","",0.0);

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



  //L*(2000) 1/2-
  RooRealVar m0_2000("m0_2000","m0",1.95549);
  RooRealVar width_2000("width_2000","width",0.141);
  RooRealVar a_2000("a_2000","",1.0);
  RooRealVar p_2000("p_2000","",0.0);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]

  RooArgList* L_2000 = new RooArgList(a_p1p1_S01,p_p1p1_S01,
                                      a_p100_S01,p_p100_S01,
                                      a_m100_S01,p_m100_S01,
                                      a_m1m1_S01,p_m1m1_S01,"L_2000");
  L_2000->add(a_p3p1_S01);
  L_2000->add(p_p3p1_S01);
  L_2000->add(a_m3m1_S01);
  L_2000->add(p_m3m1_S01);
  L_2000->add(a_2000);
  L_2000->add(p_2000);
  L_2000->add(m0_2000);
  L_2000->add(width_2000);
  L_2000->add(RooRealConstant::value(typeR));
  L_2000->add(RooRealConstant::value(1.));//2 x spin
  L_2000->add(RooRealConstant::value(-1)); //parity




  //===Lambda(1710) 1/2+ P01
  RooRealVar m0_1710("m0_1710","m0",1.81);
  RooRealVar width_1710("width_1710","width",0.15);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1710("a_1710","",0.1);
  RooRealVar p_1710("p_1710","",0.1);

  RooArgList* L_1710 = new RooArgList(a_p1p1_P01,p_p1p1_P01,
                                      a_p100_P01,p_p100_P01,
                                      a_m100_P01,p_m100_P01,
                                      a_m1m1_P01,p_m1m1_P01,"L_1710");
  L_1710->add(a_p3p1_P01);
  L_1710->add(p_p3p1_P01);
  L_1710->add(a_m3m1_P01);
  L_1710->add(p_m3m1_P01);
  L_1710->add(a_1710);
  L_1710->add(p_1710);
  L_1710->add(m0_1710);
  L_1710->add(width_1710);
  L_1710->add(RooRealConstant::value(typeR));
  L_1710->add(RooRealConstant::value(1.));//2 x spin
  L_1710->add(RooRealConstant::value(1)); //parity



  //L*(2020) 7/2+
  RooRealVar m0_2020("m0_2020","m0",2.043);
  RooRealVar width_2020("width_2020","width",0.2);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2020("a_2020","",0.1);
  RooRealVar p_2020("p_2020","",0.1);
  RooRealVar a_p1p1_G07("a_p1p1_G07","",0.);
  RooRealVar p_p1p1_G07("p_p1p1_G07","",0.);
  RooRealVar a_p100_G07("a_p100_G07","",0.);
  RooRealVar p_p100_G07("p_p100_G07","",0.);
  RooRealVar a_m100_G07("a_m100_G07","",0.);
  RooRealVar p_m100_G07("p_m100_G07","",0.);
  RooRealVar a_m1m1_G07("a_m1m1_G07","",0.);
  RooRealVar p_m1m1_G07("p_m1m1_G07","",0.);
  RooRealVar a_p3p1_G07("a_p3p1_G07","",0.);
  RooRealVar p_p3p1_G07("p_p3p1_G07","",0.);
  RooRealVar a_m3m1_G07("a_m3m1_G07","",0.);
  RooRealVar p_m3m1_G07("p_m3m1_G07","",0.);

  RooArgList* L_2020 = new RooArgList(a_p1p1_G07,p_p1p1_G07,
                                      a_p100_G07,p_p100_G07,
                                      a_m100_G07,p_m100_G07,
                                      a_m1m1_G07,p_m1m1_G07,"L_2020");
  L_2020->add(a_p3p1_G07);
  L_2020->add(p_p3p1_G07);
  L_2020->add(a_m3m1_G07);
  L_2020->add(p_m3m1_G07);
  L_2020->add(a_2020);
  L_2020->add(p_2020);
  L_2020->add(m0_2020);
  L_2020->add(width_2020);
  L_2020->add(RooRealConstant::value(typeR));
  L_2020->add(RooRealConstant::value(7.));//2 x spin
  L_2020->add(RooRealConstant::value(1)); //parity


  //L*(2300) 7/2+
  RooRealVar m0_2300("m0_2300","m0",2.3, 1.9, 2.5);
  RooRealVar width_2300("width_2300","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2300("a_2300","",0.000001);
  RooRealVar p_2300("p_2300","",0.000001);

  RooArgList* L_2300 = new RooArgList(a_p1p1_G07,p_p1p1_G07,
                                      a_p100_G07,p_p100_G07,
                                      a_m100_G07,p_m100_G07,
                                      a_m1m1_G07,p_m1m1_G07,"L_2300");
  L_2300->add(a_p3p1_G07);
  L_2300->add(p_p3p1_G07);
  L_2300->add(a_m3m1_G07);
  L_2300->add(p_m3m1_G07);
  L_2300->add(a_2300);
  L_2300->add(p_2300);
  L_2300->add(m0_2300);
  L_2300->add(width_2300);
  L_2300->add(RooRealConstant::value(typeR));
  L_2300->add(RooRealConstant::value(7.));//2 x spin
  L_2300->add(RooRealConstant::value(1)); //parity


  //===Lambda(2050) 3/2- D03
  RooRealVar m0_2050("m0_2050","m0",1.715);
  RooRealVar width_2050("width_2050","width",0.06);
  RooRealVar a_2050("a_2050","",0.5,-10,10);
  RooRealVar p_2050("p_2050","",0.5,-10,10);

  RooArgList* L_2050 = new RooArgList(a_p1p1_D03,p_p1p1_D03,
                                      a_p100_D03,p_p100_D03,
                                      a_m100_D03,p_m100_D03,
                                      a_m1m1_D03,p_m1m1_D03,"L_2050");
  L_2050->add(a_p3p1_D03);
  L_2050->add(p_p3p1_D03);
  L_2050->add(a_m3m1_D03);
  L_2050->add(p_m3m1_D03);
  L_2050->add(a_2050);
  L_2050->add(p_2050);
  L_2050->add(m0_2050);
  L_2050->add(width_2050);
  L_2050->add(RooRealConstant::value(typeR));
  L_2050->add(RooRealConstant::value(3.));//2 x spin
  L_2050->add(RooRealConstant::value(-1)); //parity




//Add more additional states
  //===Lambda(2080) 1/2+ P01
  RooRealVar m0_2080("m0_2080","m0",2.1, 2.0, 2.2);
  RooRealVar width_2080("width_2080","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2080("a_2080","",0.0000001);
  RooRealVar p_2080("p_2080","",0.0000001);

  RooArgList* L_2080 = new RooArgList(a_p1p1_P01,p_p1p1_P01,
                                      a_p100_P01,p_p100_P01,
                                      a_m100_P01,p_m100_P01,
                                      a_m1m1_P01,p_m1m1_P01,"L_2080");
  L_2080->add(a_p3p1_P01);
  L_2080->add(p_p3p1_P01);
  L_2080->add(a_m3m1_P01);
  L_2080->add(p_m3m1_P01);
  L_2080->add(a_2080);
  L_2080->add(p_2080);
  L_2080->add(m0_2080);
  L_2080->add(width_2080);
  L_2080->add(RooRealConstant::value(typeR));
  L_2080->add(RooRealConstant::value(1.));//2 x spin
  L_2080->add(RooRealConstant::value(1)); //parity


  //===Lambda(2320) 1/2+ P01
  RooRealVar m0_2320("m0_2320","m0",2.3, 2.2, 2.4);
  RooRealVar width_2320("width_2320","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2320("a_2320","",0.0000001);
  RooRealVar p_2320("p_2320","",0.0000001);

  RooArgList* L_2320 = new RooArgList(a_p1p1_P01,p_p1p1_P01,
                                      a_p100_P01,p_p100_P01,
                                      a_m100_P01,p_m100_P01,
                                      a_m1m1_P01,p_m1m1_P01,"L_2320");
  L_2320->add(a_p3p1_P01);
  L_2320->add(p_p3p1_P01);
  L_2320->add(a_m3m1_P01);
  L_2320->add(p_m3m1_P01);
  L_2320->add(a_2320);
  L_2320->add(p_2320);
  L_2320->add(m0_2320);
  L_2320->add(width_2320);
  L_2320->add(RooRealConstant::value(typeR));
  L_2320->add(RooRealConstant::value(1.));//2 x spin
  L_2320->add(RooRealConstant::value(1)); //parity


  //===Lambda(2330) 1/2- S01
  RooRealVar m0_2330("m0_2330","m0",2.3, 2.15, 2.4);
  RooRealVar width_2330("width_2330","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2330("a_2330","",0.0000001);
  RooRealVar p_2330("p_2330","",0.0000001);

  RooArgList* L_2330 = new RooArgList(a_p1p1_S01,p_p1p1_S01,
                                      a_p100_S01,p_p100_S01,
                                      a_m100_S01,p_m100_S01,
                                      a_m1m1_S01,p_m1m1_S01,"L_2330");
  L_2330->add(a_p3p1_S01);
  L_2330->add(p_p3p1_S01);
  L_2330->add(a_m3m1_S01);
  L_2330->add(p_m3m1_S01);
  L_2330->add(a_2330);
  L_2330->add(p_2330);
  L_2330->add(m0_2330);
  L_2330->add(width_2330);
  L_2330->add(RooRealConstant::value(typeR));
  L_2330->add(RooRealConstant::value(1.));//2 x spin
  L_2330->add(RooRealConstant::value(-1)); //parity



  //===Lambda(1950) 3/2+ P03
  RooRealVar m0_1950("m0_1950","m0",2.0, 1.9, 2.2);
  RooRealVar width_1950("width_1950","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_1950("a_1950","",0.0000001);
  RooRealVar p_1950("p_1950","",0.0000001);

  RooArgList* L_1950 = new RooArgList(a_p1p1_P03,p_p1p1_P03,
                                      a_p100_P03,p_p100_P03,
                                      a_m100_P03,p_m100_P03,
                                      a_m1m1_P03,p_m1m1_P03,"L_1950");
  L_1950->add(a_p3p1_P03);
  L_1950->add(p_p3p1_P03);
  L_1950->add(a_m3m1_P03);
  L_1950->add(p_m3m1_P03);
  L_1950->add(a_1950);
  L_1950->add(p_1950);
  L_1950->add(m0_1950);
  L_1950->add(width_1950);
  L_1950->add(RooRealConstant::value(typeR));
  L_1950->add(RooRealConstant::value(3.));//2 x spin
  L_1950->add(RooRealConstant::value(1)); //parity

  //===Lambda(2340) 3/2+ P03
  RooRealVar m0_2340("m0_2340","m0",2.3, 2.2, 2.5);
  RooRealVar width_2340("width_2340","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2340("a_2340","",0.0000001);
  RooRealVar p_2340("p_2340","",0.0000001);

  RooArgList* L_2340 = new RooArgList(a_p1p1_P03,p_p1p1_P03,
                                      a_p100_P03,p_p100_P03,
                                      a_m100_P03,p_m100_P03,
                                      a_m1m1_P03,p_m1m1_P03,"L_2340");
  L_2340->add(a_p3p1_P03);
  L_2340->add(p_p3p1_P03);
  L_2340->add(a_m3m1_P03);
  L_2340->add(p_m3m1_P03);
  L_2340->add(a_2340);
  L_2340->add(p_2340);
  L_2340->add(m0_2340);
  L_2340->add(width_2340);
  L_2340->add(RooRealConstant::value(typeR));
  L_2340->add(RooRealConstant::value(3.));//2 x spin
  L_2340->add(RooRealConstant::value(1)); //parity


  //===Lambda(1910) 3/2- D03
  RooRealVar m0_1910("m0_1910","m0",1.9, 1.8, 2.0);
  RooRealVar width_1910("width_1910","width",0.3, 0.05, 0.5);
  RooRealVar a_1910("a_1910","",0.00000001);
  RooRealVar p_1910("p_1910","",0.00000001);

  RooArgList* L_1910 = new RooArgList(a_p1p1_D03,p_p1p1_D03,
                                      a_p100_D03,p_p100_D03,
                                      a_m100_D03,p_m100_D03,
                                      a_m1m1_D03,p_m1m1_D03,"L_1910");
  L_1910->add(a_p3p1_D03);
  L_1910->add(p_p3p1_D03);
  L_1910->add(a_m3m1_D03);
  L_1910->add(p_m3m1_D03);
  L_1910->add(a_1910);
  L_1910->add(p_1910);
  L_1910->add(m0_1910);
  L_1910->add(width_1910);
  L_1910->add(RooRealConstant::value(typeR));
  L_1910->add(RooRealConstant::value(3.));//2 x spin
  L_1910->add(RooRealConstant::value(-1)); //parity



  //===Lambda(2075) 3/2- D03
  RooRealVar m0_2075("m0_2075","m0",2.2, 2.1, 2.4);
  RooRealVar width_2075("width_2075","width",0.3, 0.05, 0.5);
  RooRealVar a_2075("a_2075","",0.000001);
  RooRealVar p_2075("p_2075","",0.000001);

  RooArgList* L_2075 = new RooArgList(a_p1p1_D03,p_p1p1_D03,
                                      a_p100_D03,p_p100_D03,
                                      a_m100_D03,p_m100_D03,
                                      a_m1m1_D03,p_m1m1_D03,"L_2075");
  L_2075->add(a_p3p1_D03);
  L_2075->add(p_p3p1_D03);
  L_2075->add(a_m3m1_D03);
  L_2075->add(p_m3m1_D03);
  L_2075->add(a_2075);
  L_2075->add(p_2075);
  L_2075->add(m0_2075);
  L_2075->add(width_2075);
  L_2075->add(RooRealConstant::value(typeR));
  L_2075->add(RooRealConstant::value(3.));//2 x spin
  L_2075->add(RooRealConstant::value(-1)); //parity




  //2440 5/2+ F05
  RooRealVar m0_2440("m0_2440","m0",2.4, 2.25, 2.5);
  RooRealVar width_2440("width_2440","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2440("a_2440","",0.000001);
  RooRealVar p_2440("p_2440","",0.000001);

  RooArgList* L_2440 = new RooArgList(a_p1p1_F05,p_p1p1_F05,
                                      a_p100_F05,p_p100_F05,
                                      a_m100_F05,p_m100_F05,
                                      a_m1m1_F05,p_m1m1_F05,"L_2440");
  L_2440->add(a_p3p1_F05);
  L_2440->add(p_p3p1_F05);
  L_2440->add(a_m3m1_F05);
  L_2440->add(p_m3m1_F05);
  L_2440->add(a_2440);
  L_2440->add(p_2440);
  L_2440->add(m0_2440);
  L_2440->add(width_2440);
  L_2440->add(RooRealConstant::value(typeR));
  L_2440->add(RooRealConstant::value(5.));//2 x spin
  L_2440->add(RooRealConstant::value(1)); //parity



  //2130 5/2- D05
  RooRealVar m0_2130("m0_2130","m0",2.1, 2.0, 2.2);
  RooRealVar width_2130("width_2130","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2130("a_2130","",0.000001);
  RooRealVar p_2130("p_2130","",0.000001);

  RooArgList* L_2130 = new RooArgList(a_p1p1_D05,p_p1p1_D05,
                                      a_p100_D05,p_p100_D05,
                                      a_m100_D05,p_m100_D05,
                                      a_m1m1_D05,p_m1m1_D05,"L_2130");
  L_2130->add(a_p3p1_D05);
  L_2130->add(p_p3p1_D05);
  L_2130->add(a_m3m1_D05);
  L_2130->add(p_m3m1_D05);
  L_2130->add(a_2130);
  L_2130->add(p_2130);
  L_2130->add(m0_2130);
  L_2130->add(width_2130);
  L_2130->add(RooRealConstant::value(typeR));
  L_2130->add(RooRealConstant::value(5.));//2 x spin
  L_2130->add(RooRealConstant::value(-1)); //parity


  //2310 5/2- D05
  RooRealVar m0_2310("m0_2310","m0",2.3, 2.2, 2.4);
  RooRealVar width_2310("width_2310","width",0.3, 0.05, 0.5);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_2310("a_2310","",0.000001);
  RooRealVar p_2310("p_2310","",0.000001);

  RooArgList* L_2310 = new RooArgList(a_p1p1_D05,p_p1p1_D05,
                                      a_p100_D05,p_p100_D05,
                                      a_m100_D05,p_m100_D05,
                                      a_m1m1_D05,p_m1m1_D05,"L_2310");
  L_2310->add(a_p3p1_D05);
  L_2310->add(p_p3p1_D05);
  L_2310->add(a_m3m1_D05);
  L_2310->add(p_m3m1_D05);
  L_2310->add(a_2310);
  L_2310->add(p_2310);
  L_2310->add(m0_2310);
  L_2310->add(width_2310);
  L_2310->add(RooRealConstant::value(typeR));
  L_2310->add(RooRealConstant::value(5.));//2 x spin
  L_2310->add(RooRealConstant::value(-1)); //parity



  //2305 7/2-
  RooRealVar m0_2305("m0_2305","m0",2.3, 2.2, 2.4);
  RooRealVar width_2305("width_2305","width",0.3, 0.05, 0.5);
    RooRealVar a_2305("a_2305","",0.00000001);
  RooRealVar p_2305("p_2305","",0.00000001);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]

  RooArgList* L_2305 = new RooArgList(a_p1p1_F07,p_p1p1_F07,
                                      a_p100_F07,p_p100_F07,
                                      a_m100_F07,p_m100_F07,
                                      a_m1m1_F07,p_m1m1_F07,"L_2305");
  L_2305->add(a_p3p1_F07);
  L_2305->add(p_p3p1_F07);
  L_2305->add(a_m3m1_F07);
  L_2305->add(p_m3m1_F07);
  L_2305->add(a_2305);
  L_2305->add(p_2305);
  L_2305->add(m0_2305);
  L_2305->add(width_2305);
  L_2305->add(RooRealConstant::value(typeR));
  L_2305->add(RooRealConstant::value(7.));//2 x spin
  L_2305->add(RooRealConstant::value(-1)); //parity



  //===Lambda(2405) 9/2-
  RooRealVar m0_2405("m0_2405","m0",2.4, 2.25, 2.5);
  RooRealVar width_2405("width_2405","width",0.3, 0.05, 0.5);
  RooRealVar a_2405("a_2405","",0.000001);
  RooRealVar p_2405("p_2405","",0.000001);
  // a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
  RooRealVar a_p1p1_2405("a_p1p1_2405","",0.001);
  RooRealVar p_p1p1_2405("p_p1p1_2405","",0.);
  RooRealVar a_p100_2405("a_p100_2405","",0);
  RooRealVar p_p100_2405("p_p100_2405","",0);
  RooRealVar a_m100_2405("a_m100_2405","",0);
  RooRealVar p_m100_2405("p_m100_2405","",0);
  RooRealVar a_m1m1_2405("a_m1m1_2405","",0);
  RooRealVar p_m1m1_2405("p_m1m1_2405","",0);
  RooRealVar a_p3p1_2405("a_p3p1_2405","",0);
  RooRealVar p_p3p1_2405("p_p3p1_2405","",0);
  RooRealVar a_m3m1_2405("a_m3m1_2405","",0);
  RooRealVar p_m3m1_2405("p_m3m1_2405","",0);

  RooArgList* L_2405 = new RooArgList(a_p1p1_2405,p_p1p1_2405,
                                      a_p100_2405,p_p100_2405,
                                      a_m100_2405,p_m100_2405,
                                      a_m1m1_2405,p_m1m1_2405,"L_2405");
  L_2405->add(a_p3p1_2405);
  L_2405->add(p_p3p1_2405);
  L_2405->add(a_m3m1_2405);
  L_2405->add(p_m3m1_2405);
  L_2405->add(a_2405);
  L_2405->add(p_2405);
  L_2405->add(m0_2405);
  L_2405->add(width_2405);
  L_2405->add(RooRealConstant::value(typeR));
  L_2405->add(RooRealConstant::value(9.));//2 x spin
  L_2405->add(RooRealConstant::value(-1)); //parity
