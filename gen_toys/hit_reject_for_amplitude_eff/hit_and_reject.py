import os,sys
from ROOT import TChain, TFile, TTree, TRandom
from array import array
r = TRandom()
iset = int(sys.argv[2])
component_label = sys.argv[1]
r.SetSeed(iset+1)
max_weight = float(sys.argv[3])

sw = array('d',[0])

f = TFile("output/testKM4Pc-4440m1-4457m3-4312p3-4600p3-"+component_label+"-"+str(iset)+".root","recreate")
chain = TChain("tree")
chain.Add("../../Fit/toy/testKM4Pc-4440m1-4457m3-4312p3-4600p3-"+component_label+"-"+str(iset)+".root")
tree = chain.CloneTree(0)

#if component_label == "phsp_mc":
#  tree.Branch("sw", sw, "sw/D")

nentries = chain.GetEntries()
#loop to get the maximum weight
#max_weight = 0
#for i in range(0, nentries):
#  if i%100000==0:
#    print("get max weight",i,nentries)
#  chain_all.GetEntry(i)
#  if chain.w*chain.eff > max_weight:
#    max_weight = chain.w*chain.eff


for i in range(0, nentries):
  if i%100000==0:
    print("do hit and reject",i,nentries)
  chain.GetEntry(i)
#  if chain.phiK < 0:
#    continue
  random = r.Uniform(0, max_weight)
  if component_label == "sig":
    if chain.w*chain.eff < random:
      continue
  elif component_label == "bkg":
    if chain.eff*chain.sw_old < random:
      continue
#  elif component_label == "phsp_mc":
#    if chain.eff*chain.w < random:
#      continue
#    sw[0] = chain.sw_old/chain.w
  tree.Fill()


print("max weight = "+str(max_weight))
tree.Write()
f.Close()



