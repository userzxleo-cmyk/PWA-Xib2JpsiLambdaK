import os,sys
from ROOT import TChain, TTree, TFile, TRandom, TH1F, TCanvas
from array import array
from math import exp as exp
r = TRandom()
r.SetSeed(1)

sw = array('d',[0])

nbkg = int(sys.argv[1])
ibkg_start = int(sys.argv[2])
label = sys.argv[3]


#parameterize in XLL
mk = 0.493
mpi = 0.139
mp = 0.938
m_thresh = mk + mp - 0.005

hist_datamc_ratio = TH1F("hist_datamc_ratio", "",2000, 1.6, 2.6)
maxweight = 0
for i in range(1, 2001):
  f = 0.
  mX = hist_datamc_ratio.GetBinCenter(i)
  f += 150.0 * exp(-40.0 * (mX-m_thresh))
  f += 900.0 * exp( -pow((mX -1.52)/0.015,2))
  f += 100.0 * exp( -pow((mX -1.6)/0.1,2))
  f += 100.0 * exp( -pow((mX -1.8)/0.2,2))
  f += 80.0 *exp(-0.6 * (mX-m_thresh))
  f /= 1043.1
  if maxweight < f:
    maxweight = f
  hist_datamc_ratio.SetBinContent(i, f)

canv = TCanvas("canv","",680,600)
hist_datamc_ratio.Draw()
canv.SaveAs("phsp/weight.png")


#chain_data = TChain("DTree")
#chain_data.Add("../../Fit/sPlot.root")
#hist_data = TH1F("hist_data","",100,1.6, 2.6)
#chain_data.Project("hist_data","mkp","nsig_sw")
#print(hist_data.GetMaximum(), hist_data.GetMinimum())

newfile = TFile("phsp/mcsw.root", "recreate")
chain = TChain("tree")
chain.Add("../hit_reject_for_amplitude_eff/output/"+label+"*bkg*.root")
#hist_mc = TH1F("hist_mc","",100,1.6, 2.6)
#chain.Project("hist_mc","mkp")


#maxweight = 0
#for i in range(1, 101):
#  if hist_mc.GetBinContent(i) == 0:
#    hist_datamc_ratio.SetBinContent(i, 0)
#  else:
#    weight = hist_data.GetBinContent(i)/hist_mc.GetBinContent(i)
#    hist_datamc_ratio.SetBinContent(i, weight)
#    if weight > maxweight:
#      maxweight = weight

tree = chain.CloneTree(0)
tree.SetName("h1")
tree.Branch("sw", sw, "sw/D")
istart = ibkg_start
iend = ibkg_start + nbkg
ientry = 0
print("for sig: istart = "+str(istart)+"; iend = "+str(iend))
for ibkg_evt in range(istart, iend):
  chain.GetEntry(ibkg_evt)
  weight = hist_datamc_ratio.GetBinContent(hist_datamc_ratio.FindBin(chain.mkp))
  random = r.Uniform(0, maxweight)
  if weight > random:
    sw[0] = 1./weight 
    tree.Fill()
    ientry += 1
  if ientry > 1646708:
    break
tree.Write()
newfile.Close()

