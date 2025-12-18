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

n_file = 150
label = "testKM4Pc-4440m1-4457m3-4312p1-4600p3"

def runFit(command):
  print("run "+command)
  os.system(command)

if __name__ == '__main__':
  joblist = []
  isig_start = 0
  ibkg_start = 0
  for i in range(0, n_file):
    rootname_sig = label+"-sig-"+str(i)
    rootname_bkg = label+"-bkg-"+str(i)
    rootname_output = label+"-"+str(i)
    plotname = label+str(i)
    joblist.append("root -l -q massFit.C\\(\\\""+rootname_sig+"\\\"\\,\\\""+rootname_bkg+"\\\"\\,\\\""+rootname_output+"\\\"\\,\\\""+plotname+"\\\"\) > log/"+label+str(i)+".log")
  pool = multiprocessing.Pool(processes=30) 
  for job in joblist:
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()

