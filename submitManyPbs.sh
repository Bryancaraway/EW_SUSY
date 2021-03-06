#!/bin/sh

export X509_USER_PROXY=/tmp/x509up_u$(id -u)
#voms-proxy-init -valid 192:0 -voms cms
cp $X509_USER_PROXY /cms/data/$USER/.x509_user_proxy

# the last arguement=2 is for pbs. 
root -l -q 'splitRunList.C("TTJets_DiLept_MC_MC2018.txt",1,2)'
root -l -q 'splitRunList.C("TTJets_SingleLeptFromT_MC2018.txt",1,2)'
root -l -q 'splitRunList.C("ZJetsToNuNu_HT_MC2018.txt",1,2)'
root -l -q 'splitRunList.C("WJetsToLNu_HT_MC2018.txt",1,2)'
root -l -q 'splitRunList.C("QCD_HT_MC2018.txt",1,2)'
root -l -q 'splitRunList.C("ST__MC2018.txt",1,2)'
root -l -q 'splitRunList.C("Rare_MC2018.txt",1,2)'

root -l -q 'splitRunList.C("TChiWZ_1000_100_MC2018.txt",1,2)'
root -l -q 'splitRunList.C("TChiWZ_800_100_MC2018.txt",1,2)'
root -l -q 'splitRunList.C("TChiWZ_600_100_MC2018.txt",1,2)'

root -l -q 'splitRunList.C("TChiWH_1000_100_MC2018.txt",1,2)'
root -l -q 'splitRunList.C("TChiWH_800_100_MC2018.txt",1,2)'
root -l -q 'splitRunList.C("TChiWH_600_100_MC2018.txt",1,2)'

#root -l -q 'splitRunList.C("WJetsToLNu_HT_MC2016.txt",1)'
#root -l -q 'splitRunList.C("WJetsToLNu_HT_MC2017.txt",1)'
#root -l -q 'splitRunList.C("WJetsToLNu_HT_MC2018.txt",1)'

#root -l -q 'splitRunList.C("ZJetsToNuNu_HT_MC2016.txt",1)'
#root -l -q 'splitRunList.C("ZJetsToNuNu_HT_MC2017.txt",1)'
#root -l -q 'splitRunList.C("ZJetsToNuNu_HT_MC2018.txt",1)'

#root -l -q 'splitRunList.C("TTJets_DiLept_MC2016.txt",1)'
#root -l -q 'splitRunList.C("TTJets_DiLept_MC2017.txt",1)'
#root -l -q 'splitRunList.C("TTJets_DiLept_MC2018.txt",1)'
##root -l -q 'splitRunList.C("TTJets_HT_MC2016.txt",1)'
##root -l -q 'splitRunList.C("TTJets_HT_MC2017.txt",1)'
##root -l -q 'splitRunList.C("TTJets_HT_MC2018.txt",1)'
#root -l -q 'splitRunList.C("TTJets_SingleLeptFromT_MC2016.txt",1)'
#root -l -q 'splitRunList.C("TTJets_SingleLeptFromT_MC2017.txt",1)'
#root -l -q 'splitRunList.C("TTJets_SingleLeptFromT_MC2018.txt",1)'

#root -l -q 'splitRunList.C("ST__MC2016.txt",1)'
#root -l -q 'splitRunList.C("ST__MC2017.txt",1)'
#root -l -q 'splitRunList.C("ST__MC2018.txt",1)'

#root -l -q 'splitRunList.C("MET_Run2016.txt",1)'
#root -l -q 'splitRunList.C("MET_Run2017.txt",1)'
#root -l -q 'splitRunList.C("MET_Run2018.txt",1)'
