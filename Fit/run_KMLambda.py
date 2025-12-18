#Assign amplitude weights to phsp entries 


import os,sys
import subprocess
import multiprocessing
import threading
import time

label = sys.argv[1]

def mp(mp_label):
  if mp_label == "m":
    return "-1"
  elif mp_label == "p":
    return "1"
  else:
    print("mp error")
    exit()

J4440 = label.split("-")[0][4]
J4457 = label.split("-")[1][4]
J4312 = label.split("-")[2][4]
J4600 = label.split("-")[3][4]

P4440 = mp(label.split("-")[0][5])
P4457 = mp(label.split("-")[1][5])
P4312 = mp(label.split("-")[2][5])
P4600 = mp(label.split("-")[3][5])

par = J4440 + " " + P4440 + " " + J4457 + "  "  + P4457 + "  " +  J4312 + "  " + P4312 + "  " + J4600 + " " + P4600 

os.environ['OPENBLAS_NUM_THREADS'] = "1"

def runGauss(year,evttype):
    print("hehe")
    os.system("ls")

def runFit(command):
  print("run "+command)
  os.system(command)

if __name__ == '__main__':
  joblist = []
  joblist.append("../bin/FitKM4Pc_KMLambda_fixPcFirst1 "+par+" > log/FitKM4Pc_KMLambda-fixPcFirst-"+label)
  joblist.append("sleep 10")
  joblist.append("../bin/FitKM4Pc_KMLambda_rndFit1 "+par+" 1 > log/FitKM4Pc_KMLambda-rndFit-seed1-"+label)
  joblist.append("../bin/FitKM4Pc_KMLambda_rndFit1 "+par+" 2 > log/FitKM4Pc_KMLambda-rndFit-seed2-"+label)
  joblist.append("../bin/FitKM4Pc_KMLambda_rndFit1 "+par+" 3 > log/FitKM4Pc_KMLambda-rndFit-seed3-"+label)
  joblist.append("../bin/FitKM4Pc_KMLambda_rndFit1 "+par+" 4 > log/FitKM4Pc_KMLambda-rndFit-seed4-"+label)
  joblist.append("../bin/FitKM4Pc_KMLambda_rndFit1 "+par+" 5 > log/FitKM4Pc_KMLambda-rndFit-seed5-"+label)
  pool = multiprocessing.Pool(processes=1) 
  for job in joblist:
    pool.apply_async(runFit, (job,))
  pool.close()
  pool.join()


#get the best rnd fit
def get_best_FCN():
  best_file = ""
  best_fcn = 0
  best_seed = 999
  for i in range(1, 6):
    filename = "log/FitKM4Pc_KMLambda-rndFit-seed"+str(i)+"-"+label
    myfile = open(filename)
    for line in myfile.readlines():
      if "FCN=" in line:
        fcn_value = float(line.split("FCN=")[1].split(" ")[0])
        if fcn_value < best_fcn:
          best_fcn = fcn_value
          best_file = filename
          best_seed = str(i)
  return [best_seed, best_file, best_fcn]


seed = get_best_FCN()[0]
print(get_best_FCN())
#os.system("../bin/FitKM4Pc_KMLambda_Minos1 "+par+" "+seed+"  > log/FitKM4Pc_KMLambda-Minos-"+label)
