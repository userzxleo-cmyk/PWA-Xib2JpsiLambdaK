#Assign amplitude weights to phsp entries 

njobs_sig = 450 
njobs_bkg = 50
nCPUs = 80


import os,sys
from ROOT import TChain, TMath
import subprocess
import multiprocessing
import threading
import time


os.environ['OPENBLAS_NUM_THREADS'] = "1"

chain = TChain("tree")
chain.Add("../../Fit/toy/testKM4Pc-4440m1-4457m3-4312p3-4600p3-*.root")
nentries = chain.GetEntries()
max_to_check = int(TMath.Min(100000., float(nentries)))
print(max_to_check)
#loop to get the maximum weight
max_weight_amw_eff = 0
for i in range(0, max_to_check):
  if i%10000==0:
    print("get max weight",i,max_to_check)
  chain.GetEntry(i)
  if chain.w*chain.eff > max_weight_amw_eff:
    max_weight_amw_eff = chain.w*chain.eff


max_weight_eff_phsp = 0
for i in range(0, max_to_check):
  if i%10000==0:
    print("get max weight",i,max_to_check)
  chain.GetEntry(i)
  if chain.eff*chain.sw_old > max_weight_eff_phsp:
    max_weight_eff_phsp = chain.eff*chain.sw_old

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
    joblist.append("python hit_and_reject.py sig " + str(i) + "  " + str(max_weight_amw_eff) + " > log/log"+str(i))
  for i in range(njobs_sig, njobs_bkg+njobs_sig):
    joblist.append("python hit_and_reject.py bkg " + str(i) + "  " + str(max_weight_eff_phsp) + " > log/log"+str(i))
  pool = multiprocessing.Pool(processes=nCPUs) 
  for job in joblist:
#    log = job.split(">")[1].replace(" ","")
#    if os.path.exists(log):
#      print("WARNING !!! log file " + log + " exist. Move it to " + log+".sav")
#      os.system("mv "+ log+" " + log+".sav")
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()

