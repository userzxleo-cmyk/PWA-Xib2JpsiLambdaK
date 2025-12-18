#use 30 cores together to calculate angles for tuples generated in run_gentoy.py 
#main function given by tree.C and create_angle_formhj.C
#split the output into 2000 files
#Only create L* and Pc chain angles. For Zcs angle, one can use /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/tuple/phsp_mc/create_angle_Zcs.py. To be implemented soon

njobs = 500 
nCPUs = 80 

import os,sys
import subprocess
import multiprocessing
import threading
import time


def runGauss(year,evttype):
    print("hehe")
    os.system("ls")

def runFit(command):
  print("run "+command)
  os.system(command)
#  os.system("ls")
#  if os.path.exists(log):
#    print("WARNING !!! log file " + log + " exist. Move it to " + log+"_sav")
#    os.system("mv ")
  #print("run "+command)
  #ps = subprocess.Popen("ls")
  #ps = subprocess.Popen("ls", shell=True, stderr=subprocess.STDOUT)
  #return ps

if __name__ == '__main__':
  joblist = []
  for i in range(0, njobs):
    seedm1 = str(i)
    seed = str(i+1)
    joblist.append("root -l -q create_angle_formhj.C\("+seedm1+"\,"+seed+"\,"+str(njobs)+"\) >& log/log"+seedm1)
  #joblist.append("../bin/FitJP3Pc_noZcs_toyJP_Fromm1_Rnd0 3 -1 1 -1   1  1 43 2 1 > log/FitJP3Pc_noZcs_toyJP-4440m3-4457m1-4312p1-toy43-Fromm1-m1Rnd2-Rnd1")
  pool = multiprocessing.Pool(processes=nCPUs) 
  for job in joblist:
#    log = job.split(">")[1].replace(" ","")
#    if os.path.exists(log):
#      print("WARNING !!! log file " + log + " exist. Move it to " + log+".sav")
#      os.system("mv "+ log+" " + log+".sav")
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()

