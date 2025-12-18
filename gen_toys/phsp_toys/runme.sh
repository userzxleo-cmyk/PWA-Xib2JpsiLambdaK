#latest approach
python run_gentoy.py #automatically use 30 cores to generate toys, save in tuples_formhj
python run_create_angle.py #automatically use 30 cores to calculate angles, save in tuples_formhj

#root -l -q create_angle.C\(0\,50\,400\) >& log/log1 & 
#root -l -q create_angle.C\(50\,100\,400\) >& log/log15 & 
#root -l -q create_angle.C\(100\,150\,400\) >& log/log2 &
#root -l -q create_angle.C\(150\,200\,400\) >& log/log25 &
#root -l -q create_angle.C\(200\,250\,400\) >& log/log3 &
#root -l -q create_angle.C\(250\,300\,400\) >& log/log35 &
#
#root -l -q create_angle.C\(300\,350\,400\) >& log/log4 &
#root -l -q create_angle.C\(350\,400\,400\) >& log/log45 &
#
#
#root -l -q gentoy.C\(2\) >& log/gentoy2 &
#root -l -q gentoy.C\(3\) >& log/gentoy3 &
#root -l -q gentoy.C\(4\) >& log/gentoy4 &
#
#root -l -q gentoy.C\(5\) >& log/gentoy5 &
#root -l -q gentoy.C\(6\) >& log/gentoy6 &
#root -l -q gentoy.C\(7\) >& log/gentoy7 &
#
#root -l -q gentoy.C\(8\) >& log/gentoy8 &
#root -l -q gentoy.C\(9\) >& log/gentoy9 &
#root -l -q gentoy.C\(10\) >& log/gentoy10 &
#
#
#
#
#root -l -q gentoy_formhj.C\(1\) >& log/gentoy1 &
#root -l -q gentoy_formhj.C\(2\) >& log/gentoy2 &
#root -l -q gentoy_formhj.C\(3\) >& log/gentoy3 &
#
#
#root -l -q gentoy_formhj.C\(4\) >& log/gentoy4 &
#
#root -l -q gentoy_formhj.C\(5\) >& log/gentoy5 &
#root -l -q gentoy_formhj.C\(6\) >& log/gentoy6 &
#root -l -q gentoy_formhj.C\(7\) >& log/gentoy7 &
#
#root -l -q gentoy_formhj.C\(8\) >& log/gentoy8 &
#root -l -q gentoy_formhj.C\(9\) >& log/gentoy9 &
#root -l -q gentoy_formhj.C\(10\) >& log/gentoy10 &
