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
  #sig
  joblist = []
  for i in range(0, njobs_sig):
    joblist.append("root -l -q assign_Lb_mass.C\(\\\"sig\\\"\\,"+str(i)+"\) > log/log"+str(i))
  pool = multiprocessing.Pool(processes=nCPUs) 
  for job in joblist:
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()
  #bkg
  joblist = []
  for i in range(njobs_sig, njobs_bkg+njobs_sig):
    joblist.append("root -l -q assign_Lb_mass.C\(\\\"bkg\\\"\\,"+str(i)+"\) > log/log"+str(i))
  pool = multiprocessing.Pool(processes=nCPUs)
  for job in joblist:
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()


