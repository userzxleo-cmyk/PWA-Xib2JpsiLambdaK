#Assign amplitude weights to phsp entries 
istart=1
njobs_sig = 0 
njobs_bkg =  40
nCPUs = 40


import os,sys
from ROOT import TChain, TMath
import subprocess
import multiprocessing
import threading
import time


os.environ['OPENBLAS_NUM_THREADS'] = "1"

chain = TChain("tree")
chain.Add("/nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/for_Pc4312JP/gen_toys/assign_Lb_mass/output/phsp_withangle_withLbMass_bkg_*root")
nentries = chain.GetEntries()
max_to_check = int(TMath.Min(100000., float(nentries)))
print(max_to_check)
#loop to get the maximum weight
max_weight_sw_eff = 0
for i in range(0, max_to_check):
  if i%10000==0:
    print("get max weight",i,max_to_check)
  chain.GetEntry(i)
  if chain.sw*chain.eff > max_weight_sw_eff:
    max_weight_sw_eff = chain.sw*chain.eff



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
  for i in range(istart, njobs_sig+istart):
    joblist.append("python hit_and_reject.py sig " + str(i) + "  " + str(max_weight_sw_eff)  + " > log/log"+str(i))
  for i in range(njobs_sig+istart, njobs_bkg+njobs_sig+istart):
    joblist.append("python hit_and_reject.py bkg " + str(i) + "  " + str(max_weight_sw_eff)  +  " > log/log"+str(i))
  pool = multiprocessing.Pool(processes=nCPUs) 
  for job in joblist:
#    log = job.split(">")[1].replace(" ","")
#    if os.path.exists(log):
#      print("WARNING !!! log file " + log + " exist. Move it to " + log+".sav")
#      os.system("mv "+ log+" " + log+".sav")
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()

