TList *listX = new TList();
RooRealVar typeX("typeX","",1);

RooRealVar m0_Xplus("m0_Xplus","m0",4.003, 3.500, 5.125);
RooRealVar width_Xplus("width_Xplus","width",0.131, 0.003, 1.000);
// a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
RooRealVar a_Bp_Xplus("a_Bp_Xplus","",0.01,-10, 10);
RooRealVar p_Bp_Xplus("p_Bp_Xplus","",0.01,-10, 10);
RooRealVar a_L0_Xplus("a_L0_Xplus","",0.01,-10,10);
RooRealVar p_L0_Xplus("p_L0_Xplus","",0.01,-10,10);
RooRealVar a_L1_Xplus("a_L1_Xplus","",0.01,-10,10);
RooRealVar p_L1_Xplus("p_L1_Xplus","",0.01,-10,10);
RooRealVar a_L2_Xplus("a_L2_Xplus","",0.01,-10,10);
RooRealVar p_L2_Xplus("p_L2_Xplus","",0.01,-10,10);
RooRealVar a_L3_Xplus("a_L3_Xplus","",0.01,-10,10);
RooRealVar p_L3_Xplus("p_L3_Xplus","",0.01,-10,10);
RooArgList* L_Xplus = new RooArgList(a_L0_Xplus,p_L0_Xplus,
                                 a_L1_Xplus,p_L1_Xplus,
                                 a_L2_Xplus,p_L2_Xplus,
                                 a_L3_Xplus,p_L3_Xplus,
                                 "L_Xplus");
L_Xplus->add(a_Bp_Xplus);
L_Xplus->add(p_Bp_Xplus);                                    
L_Xplus->add(m0_Xplus);
L_Xplus->add(width_Xplus);
L_Xplus->add(typeX);//type
L_Xplus->add(RooRealConstant::value(2));//2 x spin
L_Xplus->add(RooRealConstant::value(1)); //parity

RooRealVar m0_Xminus("m0_Xminus","m0",4.003, 3.500, 5.125);
RooRealVar width_Xminus("width_Xminus","width",0.131, 0.003, 1.000);
// a &phase [1/2,1] [1/2,0] [-1/2, 0] [-1/2, -1] [3/2,1] [-3/2, -1]
RooRealVar a_Bp_Xminus("a_Bp_Xminus","",0);
RooRealVar p_Bp_Xminus("p_Bp_Xminus","",0);
RooRealVar a_L0_Xminus("a_L0_Xminus","",0.1,-10,10);
RooRealVar p_L0_Xminus("p_L0_Xminus","",0.1,-10,10);
RooRealVar a_L1_Xminus("a_L1_Xminus","",0.1,-10,10);
RooRealVar p_L1_Xminus("p_L1_Xminus","",0.1,-10,10);
RooRealVar a_L2_Xminus("a_L2_Xminus","",0.1,-10,10);
RooRealVar p_L2_Xminus("p_L2_Xminus","",0.1,-10,10);
RooRealVar a_L3_Xminus("a_L3_Xminus","",0.1,-10,10);
RooRealVar p_L3_Xminus("p_L3_Xminus","",0.1,-10,10);
RooArgList* L_Xminus = new RooArgList(a_L0_Xminus,p_L0_Xminus,
                                 a_L1_Xminus,p_L1_Xminus,
                                 a_L2_Xminus,p_L2_Xminus,
                                 a_L3_Xminus,p_L3_Xminus,
                                 "L_Xminus");
L_Xminus->add(a_Bp_Xminus);
L_Xminus->add(p_Bp_Xminus);
L_Xminus->add(m0_Xminus);
L_Xminus->add(width_Xminus);
L_Xminus->add(typeX);//type
L_Xminus->add(RooRealConstant::value(2));//2 x spin
L_Xminus->add(RooRealConstant::value(-1)); //parity
