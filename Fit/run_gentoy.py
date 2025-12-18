#Assign amplitude weights to phsp entries 

njobs_sig = 450
njobs_bkg = 50 
nCPUs = 80

import os,sys
import subprocess
import multiprocessing
import threading
import time

os.environ['OPENBLAS_NUM_THREADS'] = "1"

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
  for i in range(0, njobs_sig):
#    joblist.append("../bin/FitKM4Pc_genToy 1 -1 3 -1 3 -1 3 1 "+str(i)+" 1 > log_gen/log-4312m3"+str(i))
    joblist.append("../bin/FitKM4Pc_genToy 1 -1 3 -1 3  1 3 1 "+str(i)+" 1 > log_gen/log-4312p3"+str(i))
    joblist.append("../bin/FitKM4Pc_genToy 1 -1 3 -1 1  1 3 1 "+str(i)+" 1 > log_gen/log-4312p1"+str(i))
  for i in range(njobs_sig, njobs_bkg+njobs_sig):
#    joblist.append("../bin/FitKM4Pc_genToy 1 -1 3 -1 3 -1 3 1 "+str(i)+" 0 > log_gen/log-4312m3"+str(i))
    joblist.append("../bin/FitKM4Pc_genToy 1 -1 3 -1 3  1 3 1 "+str(i)+" 0 > log_gen/log-4312p3"+str(i))
    joblist.append("../bin/FitKM4Pc_genToy 1 -1 3 -1 1  1 3 1 "+str(i)+" 0 > log_gen/log-4312p1"+str(i))
  pool = multiprocessing.Pool(processes=nCPUs) 
  for job in joblist:
#    log = job.split(">")[1].replace(" ","")
#    if os.path.exists(log):
#      print("WARNING !!! log file " + log + " exist. Move it to " + log+".sav")
#      os.system("mv "+ log+" " + log+".sav")
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()

