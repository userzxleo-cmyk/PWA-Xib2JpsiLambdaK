python run_toyfit.py 0 25 0    >& log_bigbrother/log0.log &
python run_toyfit.py 25 50 1   >& log_bigbrother/log1.log &
python run_toyfit.py 50 75 2   >& log_bigbrother/log2_round2.log &  
python run_toyfit.py 75 100 3  >& log_bigbrother/log3_round2.log & 


../bin/FitKM4Pc_KMLambda0 1 -1 3 -1 1 -1 3 1 >& log/FitKM4Pc_KMLambda0-4440m1-4457m3-4312m1-4600p3 & 


../bin/FitKM4Pc_add1Zcs_test3 1 -1 3 -1 1 -1 3 1 >& hehe_Zcs &
/disk402/lhcb/wangmz/tmp/lb2jpsipk_run12_aman/bin/FitKM4Pc_debug3 1 -1 3 -1 1 -1 3 1 >& log_Zcs_ref &


../bin/FitKM8Pc3 1 -1 3 -1 1 -1 3 1 >&  log/FitKM8Pc-4440m1-4457m3-4312m1-4600p3 &
../bin/FitKM8Pc3 1 -1 3 -1 3  1 3 1 >&  log/FitKM8Pc-4440m1-4457m3-4312p3-4600p3 &


../bin/FitKM4Pc_KMLambda_FromZero43123 1 -1 3 -1 3 1 3 1 testKM4Pc-KMLambda-fixPcFirst-4440m1-4457m3-4312p3-4600p3 >& log/FitKM4Pc_KMLambda_FromZero4312-4440m1-4457m3-4312p3-4600p3-round1 &
../bin/FitKM4Pc_KMLambda_FromZero43123 1 -1 3 -1 1 1 3 1 testKM4Pc-KMLambda-fixPcFirst-4440m1-4457m3-4312p3-4600p3 >& log/FitKM4Pc_KMLambda_FromZero4312-4440m1-4457m3-4312p1-4600p3-round1 &
../bin/FitKM4Pc_KMLambda_FromZero43123 1 -1 3 -1 3 -1 3 1 testKM4Pc-KMLambda-fixPcFirst-4440m1-4457m3-4312p3-4600p3 >& log/FitKM4Pc_KMLambda_FromZero4312-4440m1-4457m3-4312m3-4600p3-round1 &
../bin/FitKM4Pc_KMLambda_FromZero43123 1 -1 3 -1 1 -1 3 1 testKM4Pc-KMLambda-fixPcFirst-4440m1-4457m3-4312p3-4600p3 >& log/FitKM4Pc_KMLambda_FromZero4312-4440m1-4457m3-4312m1-4600p3-round1 &



../bin/FitKM4Pc_KMLambda_FromZero43123 1 -1 3 -1 1  1 3 1 testKM4Pc-KMLambda-FromZero4312-4440m1-4457m3-4312p3-4600p3-testKM4Pc-KMLambda-fixPcFirst-4440m1-4457m3-4312p3-4600p3 >& log/FitKM4Pc_KMLambda_FromZero4312-4440m1-4457m3-4312p1-4600p3-round2 &
../bin/FitKM4Pc_KMLambda_FromZero43123 1 -1 3 -1 3  1 3 1 testKM4Pc-KMLambda-FromZero4312-4440m1-4457m3-4312p3-4600p3-testKM4Pc-KMLambda-fixPcFirst-4440m1-4457m3-4312p3-4600p3 >& log/FitKM4Pc_KMLambda_FromZero4312-4440m1-4457m3-4312p3-4600p3-round2 &
../bin/FitKM4Pc_KMLambda_FromZero43123 1 -1 3 -1 1 -1 3 1 testKM4Pc-KMLambda-FromZero4312-4440m1-4457m3-4312p3-4600p3-testKM4Pc-KMLambda-fixPcFirst-4440m1-4457m3-4312p3-4600p3 >& log/FitKM4Pc_KMLambda_FromZero4312-4440m1-4457m3-4312m1-4600p3-round2 &
../bin/FitKM4Pc_KMLambda_FromZero43123 1 -1 3 -1 3 -1 3 1 testKM4Pc-KMLambda-FromZero4312-4440m1-4457m3-4312p3-4600p3-testKM4Pc-KMLambda-fixPcFirst-4440m1-4457m3-4312p3-4600p3 >& log/FitKM4Pc_KMLambda_FromZero4312-4440m1-4457m3-4312m3-4600p3-round2 &


../bin/FitKM4Pc_addZcs4000_genToy_forFigures3 1 -1 3 -1 1 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3 | tee log/hehe


../bin/FitKM4Pc_genToy_forFigures3 1 -1 3 -1 1 -1 3 1 smear_debug_new | tee log/hehe1

../bin/FitKM4Pc_genToy_forFigures0 1 -1 3 -1 1 -1 3 1 smear_debug_old | tee log/hehe2


../bin/FitKM4Pc_ReRunFail2 1 -1 3 -1 1 -1 3 1 smear_debug_old >& log/FitKM4Pc_ReRunFail-smear_debug_old  &


../bin/FitKM4Pc_genToy_forFigures3 1 -1 3 -1 1 -1 3 1 smear_debug_new_10sigma | tee log/hehe1


../bin/FitKM4Pc_genToy_forFigures2 1 -1 3 -1 1 -1 3 1 smear_debug_new_nosmear | tee log/hehe2 




../bin/FitKM4Pc_addZcs40000 1 -1 3 -1 1 -1 3 1  >& log/FitKM4Pc_addZcs4000-4440m1-4457m3-4312m1-4600p3 & 
../bin/FitKM4Pc_addZcs40001 1 -1 3 -1 1  1 3 1  >& log/FitKM4Pc_addZcs4000-4440m1-4457m3-4312p1-4600p3 & 
../bin/FitKM4Pc_addZcs40002 1 -1 3 -1 3 -1 3 1  >& log/FitKM4Pc_addZcs4000-4440m1-4457m3-4312m3-4600p3 & 
../bin/FitKM4Pc_addZcs40003 1 -1 3 -1 3  1 3 1  >& log/FitKM4Pc_addZcs4000-4440m1-4457m3-4312p3-4600p3 & 



../bin/FitKM4Pc_addZcs4000_ReRunFail0 1 -1 3 -1 1 -1 3 1  testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3 >& log/FitKM4Pc_addZcs4000-4440m1-4457m3-4312m1-4600p3_ReRunFail &
../bin/FitKM4Pc_addZcs4000_ReRunFail1 1 -1 3 -1 1  1 3 1  testKM4Pc-addZcs4000-4440m1-4457m3-4312p1-4600p3 >& log/FitKM4Pc_addZcs4000-4440m1-4457m3-4312p1-4600p3_ReRunFail &
../bin/FitKM4Pc_addZcs4000_ReRunFail2 1 -1 3 -1 1 -1 3 1  testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3_v2 >& log/FitKM4Pc_addZcs4000-4440m1-4457m3-4312m1-4600p3_v2_ReRunFail &


../bin/FitKM4Pc_addZcs4000_FromZero43120 1 -1 3 -1 1 -1 3 1 testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m1-4600p3-round1 &
../bin/FitKM4Pc_addZcs4000_FromZero43121 1 -1 3 -1 1  1 3 1 testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p1-4600p3-round1 &
../bin/FitKM4Pc_addZcs4000_FromZero43122 1 -1 3 -1 3 -1 3 1 testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m3-4600p3-round1 &
../bin/FitKM4Pc_addZcs4000_FromZero43123 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p3-4600p3-round1 &


../bin/FitKM4Pc_genToy_forFigures3 3 -1 1 -1 1 -1  3 1 testKM4Pc-4440m3-4457m1-4312m1-4600p3 >& log/FitKM4Pc_genToy_forFigures-4440m3-4457m1-4312m1-4600p3 & 
../bin/FitKM4Pc_genToy_forFigures2 3  1 5  1 1 -1  3 1 testKM4Pc-4440p3-4457p5-4312m1-4600p3 >& log/FitKM4Pc_genToy_forFigures-4440p3-4457p5-4312m1-4600p3 &


../bin/FitKM4Pc_addZcs4000_ReRunFail0 1 -1 3 -1 3  1 3 1  testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p3-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p3-4600p3-round1-ReRunFail & 
../bin/FitKM4Pc_addZcs4000_ReRunFail3 1 -1 3 -1 3 -1 3 1  testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m3-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m3-4600p3-round1-ReRunFail & 
../bin/FitKM4Pc_addZcs4000_ReRunFail2 1 -1 3 -1 1 -1 3 1  testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m1-4600p3-round1-ReRunFail &


../bin/FitKM4Pc_addZcs4000_FromZero43120 1 -1 3 -1 1 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m1-4600p3-round2 &
../bin/FitKM4Pc_addZcs4000_FromZero43121 1 -1 3 -1 1  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p1-4600p3-round2 &
../bin/FitKM4Pc_addZcs4000_FromZero43122 1 -1 3 -1 3 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m3-4600p3-round2 &
../bin/FitKM4Pc_addZcs4000_FromZero43123 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p3-4600p3-round2 &


../bin/FitKM4Pc_addZcs4000_ReRunFail0 1 -1 3 -1 1  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p1-4600p3-round2-ReRunFail &  
../bin/FitKM4Pc_addZcs4000_ReRunFail1 1 -1 3 -1 3 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m3-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m3-4600p3-round2-ReRunFail &  


../bin/FitKM4Pc_addZcs4000_FromZero43120 1 -1 3 -1 1 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m1-4600p3-round3 & 
../bin/FitKM4Pc_addZcs4000_FromZero43121 1 -1 3 -1 1  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p1-4600p3-round3 & 
../bin/FitKM4Pc_addZcs4000_FromZero43122 1 -1 3 -1 3 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m3-4600p3-round3 & 
../bin/FitKM4Pc_addZcs4000_FromZero43123 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p3-4600p3-round3 & 


../bin/FitKM4Pc_addZcs4000_ReRunFail0 1 -1 3 -1 1 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m1-4600p3-round3-ReRunFail & 

../bin/FitKM4Pc_addZcs4000_FromZero43120 1 -1 3 -1 1 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m1-4600p3-round4 & 
../bin/FitKM4Pc_addZcs4000_FromZero43121 1 -1 3 -1 1  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p1-4600p3-round4 & 
../bin/FitKM4Pc_addZcs4000_FromZero43122 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p3-4600p3-round4 & 
../bin/FitKM4Pc_addZcs4000_FromZero43123 1 -1 3 -1 3 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-ReRunFail >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m3-4600p3-round4 & 


../bin/FitKM4Pc_addZcs4000_ReRunFail1 1 -1 3 -1 1 1 3 1 testKM4Pc-addZcs4000-4440m1-4457m3-4312p1-4600p3-beforeRND >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p1-4600p3-round4-ReRunFail & 


python run_Zcs4000_rnd.py 4440m1-4457m3-4312m3-4600p3 testKM4Pc-addZcs4000-4440m1-4457m3-4312m3-4600p3-beforeRND 2 >& log_bigbrother/run_Zcs4000_rnd-4312m3 & 
python run_Zcs4000_rnd.py 4440m1-4457m3-4312p3-4600p3 testKM4Pc-addZcs4000-4440m1-4457m3-4312p3-4600p3-beforeRND 3 >& log_bigbrother/run_Zcs4000_rnd-4312p3 & 
python run_Zcs4000_rnd.py 4440m1-4457m3-4312p1-4600p3 testKM4Pc-addZcs4000-4440m1-4457m3-4312p1-4600p3-beforeRND-ReRunFail 0 >& log_bigbrother/run_Zcs4000_rnd-4312p1 & 
python run_Zcs4000_rnd.py 4440m1-4457m3-4312m1-4600p3 testKM4Pc-addZcs4000-4440m1-4457m3-4312m1-4600p3-beforeRND 1 >& log_bigbrother/run_Zcs4000_rnd-4312m1 & 


../bin/FitKM4Pc_addZcs4000_ReRunFail3 1 -1 3 -1 3 1 3 1  testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed4 >& log/FitKM4Pc_addZcs4000_rndFit-seed4-4440m1-4457m3-4312p3-4600p3-ReRunFail & 
../bin/FitKM4Pc_addZcs4000_ReRunFail2 1 -1 3 -1 3 1 3 1  testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed1 >& log/FitKM4Pc_addZcs4000_rndFit-seed1-4440m1-4457m3-4312p3-4600p3-ReRunFail & 


python run_Zcs4000_rnd.py 4440m1-4457m3-4312m3-4600p3 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312m3-4600p3-seed5 2 round2 >& log_bigbrother/run_Zcs4000_rnd-4312m3-round2 &
python run_Zcs4000_rnd.py 4440m1-4457m3-4312p3-4600p3 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed2 3 round2 >& log_bigbrother/run_Zcs4000_rnd-4312p3-round2 &


../bin/FitKM4Pc_addZcs4000_genToy_forFigures0 1 -1 3 -1 3 -1 3 1 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312m3-4600p3-seed5 >& log/draw-testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312m3-4600p3-seed5 &  
../bin/FitKM4Pc_addZcs4000_genToy_forFigures0 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed2 >& log/draw-testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed2 & 
../bin/FitKM4Pc_addZcs4000_genToy_forFigures0 1 -1 3 -1 1  1 3 1 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p1-4600p3-seed2 >& log/draw-testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p1-4600p3-seed2 & 
../bin/FitKM4Pc_addZcs4000_genToy_forFigures0 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed3 >& log/draw-testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed3 & 
../bin/FitKM4Pc_addZcs4000_genToy_forFigures0 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed4 >& log/draw-testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed4 & 


python run_Zcs4000_rnd.py 4440m1-4457m3-4312p1-4600p3 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p1-4600p3-seed5 0 round2 >& log_bigbrother/run_Zcs4000_rnd-4312p1-round2 &

../bin/FitKM4Pc_addZcs4000_genToy_forFigures_debug0 3 1 3 1 3  1 3 1 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed4 >& log/draw-testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed4-debug & 
../bin/FitKM4Pc_addZcs4000_genToy_forFigures_debug0 3 -1 3 -1 3  -1 3 -1 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed4 >& log/draw-testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed4-debug & 
../bin/FitKM4Pc_addZcs4000_genToy_forFigures_debug0 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed4 >& log/draw-testKM4Pc-addZcs4000-rndFit-4440m1-4457m3-4312p3-4600p3-seed4-debug & 

../bin/FitKM4Pc_addZcs4000_FromZero43120 1 -1 3 -1 1 -1 3 1 testKM4Pc-addZcs4000-4440m1-4457m3-4312p1-4600p3-beforeRND-ReRunFail round5 >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m1-4600p3-round5 & 
../bin/FitKM4Pc_addZcs4000_FromZero43121 1 -1 3 -1 1  1 3 1 testKM4Pc-addZcs4000-4440m1-4457m3-4312p1-4600p3-beforeRND-ReRunFail round5 >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p1-4600p3-round5 & 
../bin/FitKM4Pc_addZcs4000_FromZero43122 1 -1 3 -1 3 -1 3 1 testKM4Pc-addZcs4000-4440m1-4457m3-4312p1-4600p3-beforeRND-ReRunFail round5 >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m3-4600p3-round5 & 
../bin/FitKM4Pc_addZcs4000_FromZero43123 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-4440m1-4457m3-4312p1-4600p3-beforeRND-ReRunFail round5 >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312p3-4600p3-round5 & 



../bin/FitKM4Pc_addZcs4000_cal4312LL0 1 -1 3 -1 1 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-round5 >& logll/testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-round5  &
../bin/FitKM4Pc_addZcs4000_cal4312LL0 1 -1 3 -1 1  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-round5 >& logll/testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-round5  &
../bin/FitKM4Pc_addZcs4000_cal4312LL0 1 -1 3 -1 3 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m3-4600p3-round5 >& logll/testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m3-4600p3-round5  &
../bin/FitKM4Pc_addZcs4000_cal4312LL0 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p3-4600p3-round5 >& logll/testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p3-4600p3-round5  &


../bin/FitKM4Pc_cal4312LL0 1 -1 3 -1 1 -1 3 1 testKM4Pc-4440m1-4457m3-4312m1-4600p3 >& logll/testKM4Pc-4440m1-4457m3-4312m1-4600p3 & 
../bin/FitKM4Pc_cal4312LL0 1 -1 3 -1 1  1 3 1 testKM4Pc-4440m1-4457m3-4312p1-4600p3 >& logll/testKM4Pc-4440m1-4457m3-4312p1-4600p3 & 
../bin/FitKM4Pc_cal4312LL0 1 -1 3 -1 3 -1 3 1 testKM4Pc-4440m1-4457m3-4312m3-4600p3 >& logll/testKM4Pc-4440m1-4457m3-4312m3-4600p3 & 
../bin/FitKM4Pc_cal4312LL0 1 -1 3 -1 3  1 3 1 testKM4Pc-4440m1-4457m3-4312p3-4600p3 >& logll/testKM4Pc-4440m1-4457m3-4312p3-4600p3 &


../bin/FitKM4Pc_addZcs4000_genToy_forFigures0 1 -1 3 -1 1 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-round5 >& log/draw-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-round5 & 
../bin/FitKM4Pc_addZcs4000_genToy_forFigures0 1 -1 3 -1 1  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-round5 >& log/draw-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p1-4600p3-round5 & 
../bin/FitKM4Pc_addZcs4000_genToy_forFigures0 1 -1 3 -1 3 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m3-4600p3-round5 >& log/draw-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m3-4600p3-round5 & 
../bin/FitKM4Pc_addZcs4000_genToy_forFigures0 1 -1 3 -1 3  1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p3-4600p3-round5 >& log/draw-testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312p3-4600p3-round5 & 


../bin/FitKM4Pc_addZcs4000_ReRunFail0 1 -1 3 -1 1 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m1-4600p3-round5 >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m1-4600p3-round5-ReRunFail & 
../bin/FitKM4Pc_addZcs4000_ReRunFail1 1 -1 3 -1 3 -1 3 1 testKM4Pc-addZcs4000-FromZero4312-4440m1-4457m3-4312m3-4600p3-round5 >& log/FitKM4Pc_addZcs4000_FromZero4312-4440m1-4457m3-4312m3-4600p3-round5-ReRunFail & 
