#Assign amplitude weights to phsp entries 


import os,sys
import subprocess
import multiprocessing
import threading
import time

itoy_start = int(sys.argv[1])
itoy_end = int(sys.argv[2])
iGPU = sys.argv[3]
print("itoy_start", itoy_start)
print("itoy_end", itoy_end)
print("iGPU", iGPU)


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
  for itoy in range(itoy_start, itoy_end):
    joblist.append("../bin/FitKM4Pc_fit_JPtoy"+iGPU+" 1 -1 3 -1 3 -1 3 1 \"testKM4Pc-4440m1-4457m3-4312p3-4600p3\" \"testKM4Pc-4440m1-4457m3-4312p3-4600p3-"+str(itoy)+".root\" " + "\"Seed-testKM4Pc-4440m1-4457m3-4312p3-4600p3-"+str(itoy)+"-Fit-testKM4Pc-4440m1-4457m3-4312p3-4600p3.func\"" +"  >  " + "log/Seed-testKM4Pc-4440m1-4457m3-4312p3-4600p3-"+str(itoy)+"-Fit-testKM4Pc-4440m1-4457m3-4312p3-4600p3.log")
    joblist.append("../bin/FitKM4Pc_fit_JPtoy"+iGPU+" 1 -1 3 -1 1 -1 3 1 \"testKM4Pc-4440m1-4457m3-4312m1-4600p3\" \"testKM4Pc-4440m1-4457m3-4312p3-4600p3-"+str(itoy)+".root\"  " + "\"Seed-testKM4Pc-4440m1-4457m3-4312p3-4600p3-"+str(itoy)+"-Fit-testKM4Pc-4440m1-4457m3-4312m1-4600p3.func\" " + " >   " + "log/Seed-testKM4Pc-4440m1-4457m3-4312p3-4600p3-"+str(itoy)+"-Fit-testKM4Pc-4440m1-4457m3-4312m1-4600p3.log")
#    joblist.append("../bin/FitKM4Pc_fit_JPtoy"+iGPU+" 1 -1 3 -1 3 -1 3 1 \"testKM4Pc-4440m1-4457m3-4312m3-4600p3\" \"testKM4Pc-4440m1-4457m3-4312m3-4600p3-"+str(itoy)+".root\" " + "\"Seed-testKM4Pc-4440m1-4457m3-4312m3-4600p3-"+str(itoy)+"-Fit-testKM4Pc-4440m1-4457m3-4312m3-4600p3.func\"" +"  >  " + "log/Seed-testKM4Pc-4440m1-4457m3-4312m3-4600p3-"+str(itoy)+"-Fit-testKM4Pc-4440m1-4457m3-4312m3-4600p3.log")
#    joblist.append("../bin/FitKM4Pc_fit_JPtoy"+iGPU+" 1 -1 3 -1 1 -1 3 1 \"testKM4Pc-4440m1-4457m3-4312m1-4600p3\" \"testKM4Pc-4440m1-4457m3-4312m3-4600p3-"+str(itoy)+".root\"  " + "\"Seed-testKM4Pc-4440m1-4457m3-4312m3-4600p3-"+str(itoy)+"-Fit-testKM4Pc-4440m1-4457m3-4312m1-4600p3.func\" " + " >   " + "log/Seed-testKM4Pc-4440m1-4457m3-4312m3-4600p3-"+str(itoy)+"-Fit-testKM4Pc-4440m1-4457m3-4312m1-4600p3.log")
  pool = multiprocessing.Pool(processes=3) 
  for job in joblist:
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()

