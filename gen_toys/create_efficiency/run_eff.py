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
    joblist.append("cd /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/moments/For_eff/tuples/ && /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/moments/For_eff/tuples/../bin/Addeff_forJPtoy /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/for_Pc4312JP/gen_toys/assign_Lb_mass/output/phsp_withangle_withLbMass_sig_"+str(i)+" > log/log"+str(i) + "  && cd /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/for_Pc4312JP/gen_toys/create_efficiency")
    #joblist.append("cd /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/moments/For_eff/tuples/ && /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/moments/For_eff/tuples/../bin/Addeff /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/for_Pc4312JP/Fit/toy/testKM4Pc-4440m1-4457m3-4312m1-4600p3-sig-"+str(i)+" > log/log"+str(i) + "  && cd /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/for_Pc4312JP/gen_toys/create_efficiency")
  for i in range(njobs_sig, njobs_sig+njobs_bkg):
    joblist.append("cd /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/moments/For_eff/tuples/ && /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/moments/For_eff/tuples/../bin/Addeff_forJPtoy /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/for_Pc4312JP/gen_toys/assign_Lb_mass/output/phsp_withangle_withLbMass_bkg_"+str(i)+" > log/log"+str(i) + "  && cd /nishome/wangmz/workspace/Lb2JpsipK/lb2jpsipk_run12_aman/for_Pc4312JP/gen_toys/create_efficiency")
  pool = multiprocessing.Pool(processes=nCPUs) 
  for job in joblist:
#    log = job.split(">")[1].replace(" ","")
#    if os.path.exists(log):
#      print("WARNING !!! log file " + log + " exist. Move it to " + log+".sav")
#      os.system("mv "+ log+" " + log+".sav")
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()

