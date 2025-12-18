import os,sys
from ROOT import TChain, TTree, TFile, TRandom

r = TRandom()
r.SetSeed(1)

nsig = int(sys.argv[1])
isig_start = int(sys.argv[2])
nbkg = int(sys.argv[3])
ibkg_start = int(sys.argv[4])
ifile = int(sys.argv[5])
label = sys.argv[6]


newfile = TFile("input/"+label+"-sig-"+str(ifile)+".root", "recreate")
chain = TChain("tree")
chain.Add("../hit_reject_for_amplitude_eff/output/"+label+"*sig*.root")
tree = chain.CloneTree(0)
istart = isig_start 
iend = isig_start + nsig 
print("for sig: istart = "+str(istart)+"; iend = "+str(iend))
for isig_evt in range(istart, iend):
  chain.GetEntry(isig_evt)
  tree.Fill()
tree.Write()
newfile.Close()

newfile = TFile("input/"+label+"-bkg-"+str(ifile)+".root", "recreate")
chain = TChain("tree")
chain.Add("../hit_reject_for_amplitude_eff/output/"+label+"*bkg*.root")
tree = chain.CloneTree(0)
istart = ibkg_start
iend = ibkg_start + nbkg
print("for sig: istart = "+str(istart)+"; iend = "+str(iend))
for ibkg_evt in range(istart, iend):
  chain.GetEntry(ibkg_evt)
  tree.Fill()
tree.Write()
newfile.Close()

