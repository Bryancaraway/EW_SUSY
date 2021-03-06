#ifndef SignalReg_H
#define SignalReg_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <tuple>
#include "NtupleVariables.h"
#include "TH1F.h"
#include "TH2.h"
#include <TProfile.h>
#include "TFile.h"
#include "TLorentzVector.h"
#include "TDirectory.h"
#include <bits/stdc++.h> 

class SignalReg : public NtupleVariables{

 public:
  SignalReg(const TString &inputFileList="foo.txt", const char *outFileName="histo.root",const char *dataset="data");
  ~SignalReg();
  Bool_t   FillChain(TChain *chain, const TString &inputFileList);
  Long64_t LoadTree(Long64_t entry);
  void     EventLoop(const char *,const char *);
  void     BookHistogram(const char *);
  void print(Long64_t);
  //vector<tuple<double,double,double> > *; 
  void  sortbymass(vector<double> , int idx);
  //std::vector<std::tuple<double, double, double> >::push_back(std::vector<double>&);

  //Variables defined
  bool isMC=true;
  double wt=0,lumiInfb=35.815165;

  TH1D *h_filters;
  TH1D *h_MET;
  TH1D *h_MHT;
  TH1D *h_HT;
  TH1D *h_NJets;
  TH1D *h_BTags;

  TH1D *h_MT;
  TH1D *h_MT2J;
  TH1D *h_dPhiMETAK8;
  TH1D *h_dPhiAK8J1J2;

  TH1D *h_AK8J1Pt, *h_AK8J1Mass, *h_AK8J1Eta, *h_AK8J1Tau21, *h_AK8J1wDisDC, *h_AK8J1zhDisDC;
  TH1D *h_AK8J2Pt, *h_AK8J2Mass, *h_AK8J2Eta, *h_AK8J2Tau21, *h_AK8J2wDisDC, *h_AK8J2zhDisDC;
  /* TH1D *h_AK8J1Mass1, *h_AK8J2Mass1; */
  /* TH1D *h_AK8J1Mass2, *h_AK8J2Mass2; */
  /* TH1D *h_AK8J1Mass3, *h_AK8J2Mass3; */
  /* TH1D *h_AK8J1Mass4, *h_AK8J2Mass4; */

  TH1D *h_WHAK8J1Pt, *h_WHAK8J1Mass, *h_WHAK8J1Eta, *h_WHAK8J1Tau21, *h_WHAK8J1wDisDC, *h_WHAK8J1zhDisDC; 
  TH1D *h_WHAK8J2Pt, *h_WHAK8J2Mass, *h_WHAK8J2Eta, *h_WHAK8J2Tau21, *h_WHAK8J2wDisDC, *h_WHAK8J2zhDisDC; 
  TH1D *h_WHMT;
  TH1D *h_WHMT2J;
  TH1D *h_WHMET;

  TH1D *h_HWAK8J1Pt, *h_HWAK8J1Mass, *h_HWAK8J1Eta, *h_HWAK8J1Tau21, *h_HWAK8J1wDisDC, *h_HWAK8J1zhDisDC; 
  TH1D *h_HWAK8J2Pt, *h_HWAK8J2Mass, *h_HWAK8J2Eta, *h_HWAK8J2Tau21, *h_HWAK8J2wDisDC, *h_HWAK8J2zhDisDC; 
  TH1D *h_HWMT;
  TH1D *h_HWMT2J;
  TH1D *h_HWMET;

  TH1D *h_AK8J1wDis, *h_AK8J1zDis;
  TH1D *h_AK8J2wDis, *h_AK8J2zDis;  
  TH2D *h2_dRAK8J1J2;
  TH2D *h2_AK8J1J2Tau21;
  TH2D *h2_DisdRAK8J1;
  TH2D *h2_DisdRAK8J2;
  TH2D *h2_Tau21dRAK8J1;
  TH2D *h2_Tau21dRAK8J2;
  TH2D *h2_AK8J1Mass_J1Tau21;
  TH2D *h2_AK8J2Mass_J2Tau21;
  //new for WX
  TH1D *h_WHAK8J1MassNo2bTag, *h_WHAK8J2MassNo2bTag;
  TH1D *h_HWAK8J1MassNo2bTag, *h_HWAK8J2MassNo2bTag;

  TH1D *h_dPhi1;
  TH1D *h_dPhi2;
  TH1D *h_dPhi3;
  TH1D *h_dPhi4;

  TH1F *h_cutflow;
  TFile *oFile;
  
};
#endif

#ifdef SignalReg_cxx

void SignalReg::BookHistogram(const char *outFileName) {

  //  char hname[200], htit[200];
  //  double xlow = 0.0,  xhigh = 2000.0;
  //  int nbins = 2000;
  TString name,title;
 
  oFile = new TFile(outFileName, "recreate");
  TH1::SetDefaultSumw2(1);

  h_cutflow = new TH1F("CutFlow","cut flow",25,0,25);
  h_filters = new TH1D("Filters","Filters: Bin1 : all nEvnts, other bins: filter pass/fail",10,0,10);
  
  h_MET = new TH1D("MET","MET",200,0,2000);
  h_MHT = new TH1D("MHT","MHT",200,0,2000);
  h_HT = new TH1D("HT","HT",100,0,5000);
  h_NJets = new TH1D("NJets","NJets with pT > 30, |eta| < 20.4",20,0,20);  
  h_BTags = new TH1D("BTags","BTags with DeepCSV MedWP",10,0,10);  
  
  h_MT = new TH1D("mT","mT(MET,AK8J)",200,0,2000);
  h_MT2J = new TH1D("mT2J","mT(MET,AK8J2)",200,0,2000);  
  h_dPhiMETAK8 = new TH1D("dPhiMETAK8","dPhi(MET,AK8J)",40,0,4);
  /* h_dRGenAK8J1 = new TH1D("dRGenAK8J1","deltaR(WGen,AK8J1)",40,0,4); */
  /* h_dRGenAK8J2 = new TH1D("dRGenAK8J2","deltaR(WGen,AK8J2)",40,0,4); */
  h_dPhiAK8J1J2 = new TH1D("dPhiAK8J1J2","dPhi(AK8J1,AK8J2)",40,0,4);

  h_AK8J1Pt = new TH1D("AK8Pt","Leading AK8 jets Pt",200,0,2000);
  h_AK8J1Eta = new TH1D("AK8Eta","AK8 Eta",120,-6,6);
  h_AK8J1Mass = new TH1D("AK8Mass","AK8 Mass",60,0,300);
  h_AK8J1Tau21 = new TH1D("AK8Tau21","AK8 Tau21",100,0,1);
  h_AK8J1wDisDC = new TH1D("AK8J1wDisDC","AK8 J1 w Discr.DeepDecorelated",100,0,1);
  h_AK8J1zhDisDC = new TH1D("AK8J1zhDisDC","AK8 J1 zh Discr.DeepDecorelated",100,0,1);
  
  h_AK8J1wDis = new TH1D("AK8J1wDis","AK8 J1 w Discr.Deep Corelated",100,0,1);
  //h_AK8J1zhDis = new TH1D("AK8J1zhDis","AK8 J1 zh Discr.Deep Corelated",20,0,1);
  h_AK8J1zDis = new TH1D("AK8J1zDis","AK8 J1 z Discr.Deep Corelated",100,0,1);


  h_AK8J2Pt = new TH1D("AK8J2Pt","2nd leading AK8 jets Pt",200,0,2000);
  h_AK8J2Eta = new TH1D("AK8J2Eta","AK8J2 Eta",120,-6,6);
  h_AK8J2Mass = new TH1D("AK8J2Mass","AK8J2 Mass",60,0,300);
  h_AK8J2Tau21 = new TH1D("AK8J2Tau21","AK8J2 Tau21",100,0,1);
  h_AK8J2wDisDC = new TH1D("h_AK8J2wDisDC","AK8 J2 w Discr.DeepDecorelated",100,0,1);
  h_AK8J2zhDisDC = new TH1D("h_AK8J2zhDisDC","AK8 J2 zh Discr.DeepDecorelated",100,0,1);
  
  h_AK8J2wDis = new TH1D("AK8J2wDis","AK8 J2 w Discr.Deep Corelated",100,0,1);
  //h_AK8J2zhDis = new TH1D("AK8J2zhDis","AK8 J2 zh Discr.Deep Corelated",20,0,1);
  h_AK8J2zDis = new TH1D("AK8J2zDis","AK8 J2 z Discr.Deep Corelated",100,0,1);
  //other mass histos
  /* h_AK8J1Mass2 = new TH1D("AK8J1Mass2","AK8j1 Mass2",60,0,300);   */
  /* h_AK8J2Mass2 = new TH1D("AK8J2Mass2","AK8j2 Mass2",60,0,300);   */
  /* h_AK8J1Mass3 = new TH1D("AK8J1Mass3","AK8j1 Mass3",60,0,300);   */
  /* h_AK8J2Mass3 = new TH1D("AK8J2Mass3","AK8j2 Mass3",60,0,300);   */
  /* h_AK8J1Mass4 = new TH1D("AK8J1Mass4","AK8j1 Mass4",60,0,300);   */
  /* h_AK8J2Mass4 = new TH1D("AK8J2Mass4","AK8j2 Mass4",60,0,300);   */

  // ---for wh----
  h_WHAK8J1Pt = new TH1D("WHAK8J1Pt","2nd leading AK8 jets Pt",200,0,2000);
  h_WHAK8J1Eta = new TH1D("WHAK8J1Eta","AK8J2 Eta",120,-6,6);
  h_WHAK8J1Mass = new TH1D("WHAK8J1Mass","AK8J2 Mass",60,0,300);
  h_WHAK8J1Tau21 = new TH1D("WHAK8J1Tau21","AK8J2 Tau21",100,0,1);
  h_WHAK8J1wDisDC = new TH1D("WHh_AK8J1wDisDC","AK8 J2 w Discr.DeepDecorelated",100,0,1);
  h_WHAK8J1zhDisDC = new TH1D("WHh_AK8J1zhDisDC","AK8 J2 zh Discr.DeepDecorelated",100,0,1);
  h_WHMET = new TH1D("WHMET","MET",200,0,2000);
  h_WHMT = new TH1D("WHmT","mT(MET,AK8J)",200,0,2000);
  h_WHMT2J = new TH1D("WHmT2J","mT(MET,AK8J2)",200,0,2000);  

  h_WHAK8J2Pt = new TH1D("WHAK8J2Pt","2nd leading AK8 jets Pt",200,0,2000);
  h_WHAK8J2Eta = new TH1D("WHAK8J2Eta","AK8J2 Eta",120,-6,6);
  h_WHAK8J2Mass = new TH1D("WHAK8J2Mass","AK8J2 Mass",60,0,300);
  h_WHAK8J2Tau21 = new TH1D("WHAK8J2Tau21","AK8J2 Tau21",100,0,1);
  h_WHAK8J2wDisDC = new TH1D("WHh_AK8J2wDisDC","AK8 J2 w Discr.DeepDecorelated",100,0,1);
  h_WHAK8J2zhDisDC = new TH1D("WHh_AK8J2zhDisDC","AK8 J2 zh Discr.DeepDecorelated",100,0,1);
  
  // --- for hw
  h_HWAK8J1Pt = new TH1D("HWAK8J1Pt","2nd leading AK8 jets Pt",200,0,2000);
  h_HWAK8J1Eta = new TH1D("HWAK8J1Eta","AK8J2 Eta",120,-6,6);
  h_HWAK8J1Mass = new TH1D("HWAK8J1Mass","AK8J2 Mass",60,0,300);
  h_HWAK8J1Tau21 = new TH1D("HWAK8J1Tau21","AK8J2 Tau21",100,0,1);
  h_HWAK8J1wDisDC = new TH1D("HWh_AK8J1wDisDC","AK8 J2 w Discr.DeepDecorelated",100,0,1);
  h_HWAK8J1zhDisDC = new TH1D("HWh_AK8J1zhDisDC","AK8 J2 zh Discr.DeepDecorelated",100,0,1);
  h_HWMET = new TH1D("HWMET","MET",200,0,2000);
  h_HWMT = new TH1D("HWmT","mT(MET,AK8J)",200,0,2000);
  h_HWMT2J = new TH1D("HWmT2J","mT(MET,AK8J2)",200,0,2000);  

  h_HWAK8J2Pt = new TH1D("HWAK8J2Pt","2nd leading AK8 jets Pt",200,0,2000);
  h_HWAK8J2Eta = new TH1D("HWAK8J2Eta","AK8J2 Eta",120,-6,6);
  h_HWAK8J2Mass = new TH1D("HWAK8J2Mass","AK8J2 Mass",60,0,300);
  h_HWAK8J2Tau21 = new TH1D("HWAK8J2Tau21","AK8J2 Tau21",100,0,1);
  h_HWAK8J2wDisDC = new TH1D("HWh_AK8J2wDisDC","AK8 J2 w Discr.DeepDecorelated",100,0,1);
  h_HWAK8J2zhDisDC = new TH1D("HWh_AK8J2zhDisDC","AK8 J2 zh Discr.DeepDecorelated",100,0,1);

  //-------new WX
  h_WHAK8J1MassNo2bTag = new TH1D("WHAK8J1Massnobtag","WHAK8J1 MassN",60,0,300);
  h_WHAK8J2MassNo2bTag = new TH1D("WHAK8J2Massnobtag","WHAK8J2 MassN",60,0,300);
  h_HWAK8J1MassNo2bTag = new TH1D("HWAK8J1Massnobtag","HWAK8J1 MassN",60,0,300);
  h_HWAK8J2MassNo2bTag = new TH1D("HWAK8J2Massnobtag","HWAK8J2 MassN",60,0,300);
  //

  // ----------
  h2_dRAK8J1J2 = new TH2D("dRAK8J1J2","x:deltaR(GenW,AK8J2) vs y:deltaR(GenW,AK8J2)",50,0,5,50,0,5);
  h2_AK8J1J2Tau21 = new TH2D("AK8J1J2Tau21","x:AK8J1 #tau21 vs y:AK8J2 #tau21",100,0,2,100,0,2);
  h2_DisdRAK8J1 = new TH2D("disdRAK8J1","x:deltaR(GenW,AK8J1) vs y:deepDeCol w discr.",50,0,2,50,0,1);
  h2_DisdRAK8J2 = new TH2D("disdRAK8J2","x:deltaR(GenW,AK8J2) vs y:deepDeCol w discr.",50,0,2,50,0,1);  
  h2_Tau21dRAK8J1 = new TH2D("Tau21dRAK8J1","x:deltaR(GenW,AK8J1) vs y:AK8J1 #tau21",50,0,2,50,0,1);
  h2_Tau21dRAK8J2 = new TH2D("Tau21dRAK8J2","x:deltaR(GenW,AK8J2) vs y:AK8J2 #tau21",50,0,2,50,0,1);
  
  h2_AK8J1Mass_J1Tau21 = new TH2D("h2_AK8J1Mass_J1Tau21","x:SoftdropmassJ1 vs y:AK8J1 #tau21",50,0,200,100,0,1);
  h2_AK8J2Mass_J2Tau21 = new TH2D("h2_AK8J2Mass_J2Tau21","x:SoftdropmassJ2 vs y:AK8J2 #tau21",50,0,200,100,0,1);

  h_dPhi1 = new TH1D("DeltaPhi1","DeltaPhi1",40,0,4);
  h_dPhi2 = new TH1D("DeltaPhi2","DeltaPhi2",40,0,4);
  h_dPhi3 = new TH1D("DeltaPhi3","DeltaPhi3",40,0,4);
  h_dPhi4 = new TH1D("DeltaPhi4","DeltaPhi4",40,0,4);

}

SignalReg::SignalReg(const TString &inputFileList, const char *outFileName, const char* dataset) {
  string nameData=dataset;
  TString nameData2 = nameData;
  TChain *tree = new TChain("tree");
  //  if(nameData2.Contains("TChiWZ")) tree = new TChain("TreeMaker2/PreSelection");
  if( ! FillChain(tree, inputFileList) ) {
    std::cerr << "Cannot get the tree " << std::endl;
  } else {
    std::cout << "Initiating analysis of dataset " << dataset << std::endl;
  }

  cout<<"Treating the input files as "<<nameData<<" for setting tree branches"<<endl;
  NtupleVariables::Init(tree,nameData);

  BookHistogram(outFileName);
  
}

Bool_t SignalReg::FillChain(TChain *chain, const TString &inputFileList) {
  int itr=0;
  TFile *filePointer;
  ifstream infile(inputFileList, ifstream::in);
  std::string buffer;

  if(!infile.is_open()) {
    std::cerr << "** ERROR: Can't open '" << inputFileList << "' for input" << std::endl;
    return kFALSE;
  }

  std::cout << "TreeUtilities : FillChain " << std::endl;
  while(1) {
    infile >> buffer;
    if(!infile.good()) break;
    //    std::cout << "Adding tree from " << buffer.c_str() << std::endl;
    chain->Add(buffer.c_str());
  }
  std::cout << "No. of Entries in this tree : " << chain->GetEntries() << std::endl;
  return kTRUE;
}

Long64_t SignalReg::LoadTree(Long64_t entry) {
  // Set the environment to read one entry                                                                                          
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (!fChain->InheritsFrom(TChain::Class()))  return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    //    Notify();
  }
  return centry;
}

SignalReg::~SignalReg() { 

  if (!fChain) return;
  delete fChain->GetCurrentFile();
  oFile->cd();
  oFile->Write();
  oFile->Close();

}

#endif

