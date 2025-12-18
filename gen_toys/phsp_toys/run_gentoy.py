#use 30 cores together to generate toy samples
#main function given by gentoy_formhj.C
#each subjob generate 10000000 PHSP events

#start with istart=5000 for first 100 toys
#then start with istart=7000 for second 100 toys

istart =  1 
njobs =  1000

import os,sys
import subprocess
import multiprocessing
import threading
import time

def runFit(command):
  print("run "+command)
  os.system(command)

if __name__ == '__main__':
  joblist = []
  for i in range(istart, njobs+istart):
    seed = str(i+1)
    joblist.append("root -l -q gentoy_formhj.C\("+seed+"\) > log/gentoy"+seed)
  pool = multiprocessing.Pool(processes=64) 
  for job in joblist:
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()
