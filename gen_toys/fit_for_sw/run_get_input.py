#use 30 cores together to generate toy samples
#main function given by gentoy_formhj.C
#each subjob generate 10000000 PHSP events

#161953 300 109860 300 testKM4Pc-4440m1-4457m3-4312m1-4600p3

import os,sys
import subprocess
import multiprocessing
import threading
import time
from ROOT import TChain, TTree, TRandom

r = TRandom()
r.SetSeed(1)

nevt_forPHSP = 1646708*5 

nsig_perfile = 161953
nbkg_perfile = 109860
n_file = 150
label = "testKM4Pc-4440m1-4457m3-4312p1-4600p3"

sig_chain = TChain("tree")
sig_chain.Add("../hit_reject_for_amplitude_eff/output/"+label+"*sig*.root")
nsig_perfile_max = sig_chain.GetEntries()
print(nsig_perfile_max)
bkg_chain = TChain("tree")
bkg_chain.Add("../hit_reject_for_amplitude_eff/output/"+label+"*bkg*.root")
nbkg_perfile_max = bkg_chain.GetEntries()
print(nbkg_perfile_max)
if nsig_perfile * n_file > nsig_perfile_max:
  print("nsig_perfile * n_file > nsig_perfile_max, exit")
  exit()
if nbkg_perfile * n_file + nevt_forPHSP > nbkg_perfile_max:
  print("nbkg_perfile * n_file > nbkg_perfile_max, exit")
  exit()

print(nbkg_perfile_max - nbkg_perfile * n_file )

def runFit(command):
  print("run "+command)
  os.system(command)

if __name__ == '__main__':
  joblist = []
  isig_start = 0
  ibkg_start = 0
  for i in range(0, n_file):
    nsig_thisfile = r.Poisson(nsig_perfile)
    nbkg_thisfile = r.Poisson(nbkg_perfile)
    joblist.append("python get_input.py "+str(nsig_thisfile)+" "+str(isig_start)+" "+str(nbkg_thisfile)+" "+str(ibkg_start)+" "+str(i)+" "+label+" > log/"+label+"-"+str(i)+".log")
    isig_start += nsig_thisfile
    ibkg_start += nbkg_thisfile
#  joblist.append("python get_phsp.py "+str(nevt_forPHSP)+" "+str(ibkg_start)+" "+label+" > log/"+label+"-"+str(i)+".log")
  pool = multiprocessing.Pool(processes=80) 
  for job in joblist:
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()
#create phsp mc for intergrating

