#define SignalReg_cxx
#include "SignalReg.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <string>
#include <tuple>
#include <fstream>

using namespace std;
//
int main(int argc, char* argv[])
{

  if (argc < 2) {
    cerr << "Please give 3 arguments " << "runList " << " " << "outputFileName" << " " << "dataset" << endl;
    return -1;
  }
  const char *inputFileList = argv[1];
  const char *outFileName   = argv[2];
  const char *data          = argv[3];

  SignalReg ana(inputFileList, outFileName, data);
  cout << "dataset " << data << " " << endl;
  vector<double> mass;
  int idx;
 ana.sortbymass(mass, idx);  
 ana.EventLoop(data,inputFileList);
  
  return 0;
}

void SignalReg::EventLoop(const char *data,const char *inputFileList) {
  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  cout << "nentries " << nentries << endl;
  cout << "Analyzing dataset " << data << " " << endl;
 
  TString s_data=data;
  Long64_t nbytes = 0, nb = 0;
  int decade = 0;

  bool isFastSim = false;
  // float xsec = 0.0, numEvents = 0.0;
  if(s_data.Contains("fastsim")){
    isFastSim = true;
    //   if(s_data.Contains("TChiWZ_1000")){ xsec = 1.34352e-3; numEvents = 28771;}
    //   else if(s_data.Contains("TChiWZ_800")){ xsec = 4.75843e-3; numEvents = 34036;}
    //   cout<<"Assigning xsec as: "<<xsec<<endl;
  }
  Long64_t nEvtSurv = 0;
  int ak8J1Idx = -1;

  h_cutflow->Fill("0",0);
  h_cutflow->Fill("Weighted",0);    
  h_cutflow->Fill("MET>200",0);
  h_cutflow->Fill("dPhiCuts",0);
  h_cutflow->Fill("photonVeto",0);
  h_cutflow->Fill("LVeto",0);
  h_cutflow->Fill("Filters",0);
  h_cutflow->Fill("HTRatioDphi",0);
  h_cutflow->Fill("PassedTrigger",0);
  h_cutflow->Fill("HEMaffected",0);
  h_cutflow->Fill("NEvtsNoWtLeft",0);

  h_filters->Fill("TotEvnts",0);
  h_filters->Fill("globalSuperTightHalo2016Filter",0);
  h_filters->Fill("HBHENoiseFilter",0);
  h_filters->Fill("HBHEIsoNoiseFilter",0);
  h_filters->Fill("eeBadScFilter",0);
  h_filters->Fill("EcalDeadCellTriggerPrimitiveFilter",0);
  h_filters->Fill("BadChargedCandidateFilter",0);
  h_filters->Fill("BadPFMuonFilter",0);
  h_filters->Fill("NVtx>0",0);
  h_filters->Fill("JetID",0);
  h_filters->Fill("(MET/CaloMET<5.)",0);

  int dataRun = 0;
  if(s_data.Contains("MC_2016")){ dataRun = -2016; lumiInfb = 35.815165;}
  else if(s_data.Contains("MC_2017")){ dataRun = -2017; lumiInfb = 41.486136;}
  else if(s_data.Contains("MC_2018")){ dataRun = -2018; lumiInfb = 59.546381;}
  
  else if(s_data.Contains("2016")){ dataRun = 2016; isMC = false;}
  else if(s_data.Contains("2017")){ dataRun = 2017; isMC = false;}
  else if(s_data.Contains("2018")){ dataRun = 2018; isMC = false;}
  
  lumiInfb = 137.0;
  cout<<"!!!! changing intLumi to 137/fb, although you should have used 2018 intLumi...."<<endl;

  if(dataRun>0) cout<<"Processing it as "<<dataRun<<" data"<<endl;
  else if(dataRun<0) cout<<"Processing it as "<<abs(dataRun)<<" MC"<<endl;
  else cout<<"No specific data/MC year"<<endl;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    
    // ==============print number of events done == == == == == == == =
    double progress = 10.0 * jentry / (1.0 * nentries);
    int k = int (progress);
    if (k > decade)
      cout << 10 * k << " %" <<endl;
    decade = k;
    // cout<<"j:"<<jentry<<" fcurrent:"<<fCurrent<<endl;
    // ===============read this entry == == == == == == == == == == == 
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    //    print(jentry);  
    // if(dataRun==-2016 || dataRun==-2017) wt=Weight*1000.0*lumiInfb*NonPrefiringProb;
    // else if(dataRun <=0) wt=Weight*1000.0*lumiInfb;
    //    else wt = 1.0;
    // if(isFastSim && NumEvents==1 && CrossSection==1){
    //   CrossSection = xsec;
    //   NumEvents = numEvents;
    //   Weight = xsec/numEvents;
    // }
    wt=Weight*1000.0*lumiInfb;

    h_cutflow->Fill("0",1);
    h_cutflow->Fill("Weighted",wt);
    //--------------
    //    if(jentry>100) break;

    //------to arrange ak8 jets by decreasing mass-----
    cout<<"sd mass size" << JetsAK8_softDropMass->size()<<endl;
    vector<pair<double, int> > masspair(JetsAK8_softDropMass->size());
    for (int i=0; i< JetsAK8_softDropMass->size(); i++){
      masspair.push_back(make_pair((*JetsAK8_softDropMass)[i], i));
    }
    
    sort(masspair.begin(),masspair.end(), greater<>());
    
    for (int j = 0; j < masspair.size(); j++) { 
      cout << masspair[j].first << "\t"	<< masspair[j].second <<"\n"<< endl; 
      
    } 
    
    vector<double> sorted_mass(masspair.size());
    vector<double> sorted_Pt(masspair.size());
    vector<double> sorted_Eta(masspair.size());
    vector<double> sorted_wDis(masspair.size());
    vector<double> sorted_doubleBdis(masspair.size());

    for(int k=0; k< masspair.size(); k++){
      //int temp_ind = masspair[k].second;
      sorted_mass[k] = masspair[k].first;
      sorted_Pt[k]=(*JetsAK8)[(masspair[k].second)].Pt();
      sorted_Eta[k]=(*JetsAK8)[(masspair[k].second)].Eta();
      sorted_wDis[k] = (*JetsAK8_wDiscriminatorDeep)[(masspair[k].second)];
      sorted_doubleBdis[k] = (*JetsAK8_doubleBDiscriminator)[(masspair[k].second)];
    }
    //-----------end loop to get mass sorted

    //#################### EWK Planned baseline cuts
    if(isFastSim) JetID = true;
    if (JetsAK8->size()<2) continue;                                            
    if(NJets < 2 || HT < 300 || MHT < 200 || MET <250                           // HT, MET, MHT cuts
       || sorted_Pt[0] < 200 || sorted_Pt[1] < 200                  // pT >200
       || abs(sorted_Eta[0]) >2 || abs(sorted_Eta[1]) >2            //  |Eta|<2
       // || !( ((*JetsAK8_NsubjettinessTau2)[0]/(*JetsAK8_NsubjettinessTau1)[0]) >0.35 && ((*JetsAK8_NsubjettinessTau2)[0]/(*JetsAK8_NsubjettinessTau1)[0]) < 0.5)
       //  || !( ((*JetsAK8_NsubjettinessTau2)[1]/(*JetsAK8_NsubjettinessTau1)[1]) >0.35 && ((*JetsAK8_NsubjettinessTau2)[1]/(*JetsAK8_NsubjettinessTau1)[1]) < 0.5)          
       // || !(65< (*JetsAK8_softDropMass)[0] && (*JetsAK8_softDropMass)[0] < 100) // Tight mass cut for J2
       // || !(65< (*JetsAK8_softDropMass)[1] && (*Je                                                            tsAK8_softDropMass)[1] < 100) // Tight mass cut for J2
       || NMuons!=0 || NElectrons!=0                                            // Veto e,muons
       || (MHT/HT > 1.0) || !JetID                                              // MHT>HT and Veto JET ID
       || !(DeltaPhi1 > 1.5 && DeltaPhi2 > 0.5 && DeltaPhi3 > 0.3 && DeltaPhi4 > 0.3)  //Angle cuts
       //|| BTags!=0                                                                //Veto B jets
       || isoMuonTracks!=0 || isoElectronTracks!=0 || isoPionTracks!=0) continue; //Veto isolated tracks
    
   //####################
    //----MET
    //    if(MET < 200) continue;
    //h_cutflow->Fill("MET>200",wt);

    float dphi1=4, dphi2=4, dphi3=4, dphi4=4;
    //if(Jets->size() > 0 && (*Jets)[0].Pt() > 30 && abs((*Jets)[0].Eta()) < 6.0)
    //  dphi1 = (abs(DeltaPhi(METPhi,(*Jets)[0].Phi())));
    if(Jets->size() > 0){
      if ((*Jets)[0].Pt() > 30 && abs((*Jets)[0].Eta()) < 6.0)
	dphi1 = (abs(DeltaPhi(METPhi,(*Jets)[0].Phi())));
    }

    if(Jets->size() > 1 && (*Jets)[1].Pt() > 30 && abs((*Jets)[1].Eta()) < 6.0)
      dphi2 = (abs(DeltaPhi(METPhi,(*Jets)[1].Phi())));

    if(Jets->size() > 2 && (*Jets)[2].Pt() > 30 && abs((*Jets)[2].Eta()) < 6.0)
      dphi3 = (abs(DeltaPhi(METPhi,(*Jets)[2].Phi())));

    if(Jets->size() > 3 && (*Jets)[3].Pt() > 30 && abs((*Jets)[3].Eta()) < 6.0)
      dphi4 = (abs(DeltaPhi(METPhi,(*Jets)[3].Phi())));
    // print(jentry);
    // cout<<"METPhi:"<<METPhi<<" dphi1:"<<dphi1<<" dphi2:"<<dphi2<<" dphi3:"<<dphi3<<" dphi4:"<<dphi4<<endl;
    
    //    if(!(dphi1 > 0.5 && dphi2 > 0.5 && dphi3 > 0.5 && dphi4 > 0.5)) continue;
    //    h_cutflow->Fill("dPhiCuts",wt);
    //----Photon veto
    int nPhotons=0;
    for(int i=0;i<Photons->size();i++){
      if((*Photons)[i].Pt() > 100 && (*Photons_fullID)[i] && (!(*Photons_hasPixelSeed)[i]) ){ nPhotons++; break;}
    }
    if(nPhotons>0) continue;
    //if(Photons->size()!=0) continue;
    h_cutflow->Fill("photonVeto",wt);

    h_filters->Fill("TotEvnts",1);
    h_filters->Fill("globalSuperTightHalo2016Filter",globalSuperTightHalo2016Filter);
    h_filters->Fill("HBHENoiseFilter",HBHENoiseFilter);
    h_filters->Fill("HBHEIsoNoiseFilter",HBHEIsoNoiseFilter);
    h_filters->Fill("eeBadScFilter",eeBadScFilter);
    h_filters->Fill("EcalDeadCellTriggerPrimitiveFilter",EcalDeadCellTriggerPrimitiveFilter);
    h_filters->Fill("BadChargedCandidateFilter",BadChargedCandidateFilter);
    h_filters->Fill("BadPFMuonFilter",BadPFMuonFilter);
    h_filters->Fill("NVtx>0",NVtx > 0);
    h_filters->Fill("JetID",JetID);
    h_filters->Fill("(MET/CaloMET<5.)",(MET/CaloMET < 5.));

    if(!isFastSim){
      if(!(globalSuperTightHalo2016Filter==1 && HBHENoiseFilter==1 && HBHEIsoNoiseFilter==1 && eeBadScFilter==1 && EcalDeadCellTriggerPrimitiveFilter==1 && BadChargedCandidateFilter && BadPFMuonFilter && NVtx > 0 && JetID && (MET/CaloMET < 5.))) continue;
      h_cutflow->Fill("Filters",wt);
      h_cutflow->Fill("HTRatioDphi",wt);
    }
    //--------------------------triggers
    if(!isMC){
      bool trgPass = false;
      TString trgName;
      for(int i=0;i<TriggerPass->size();i++){
	trgName = (*TriggerNames)[i];
	if(!(trgName.Contains("MET"))) continue;
	if((*TriggerPass)[i]==1 && (trgName.Contains("HLT_PFMET100_PFMHT100_IDTight_v") || trgName.Contains("HLT_PFMET110_PFMHT110_IDTight_v") ||
				    trgName.Contains("HLT_PFMET120_PFMHT120_IDTight_v") || trgName.Contains("HLT_PFMET130_PFMHT130_IDTight_v") ||
				    trgName.Contains("HLT_PFMET140_PFMHT140_IDTight_v") || trgName.Contains("HLT_PFMET90_PFMHT90_IDTight_v") || 
				    trgName.Contains("HLT_PFMETNoMu100_PFMHTNoMu100_IDTight_v") || trgName.Contains("HLT_PFMETNoMu110_PFMHTNoMu110_IDTight_v") ||
				    trgName.Contains("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v") || trgName.Contains("HLT_PFMETNoMu130_PFMHTNoMu130_IDTight_v") || 
				    trgName.Contains("HLT_PFMETNoMu140_PFMHTNoMu140_IDTight_v") ||  trgName.Contains("HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_v"))) trgPass = true;
      }
      if(trgPass) h_cutflow->Fill("PassedTrigger",wt);
      else continue;
    }
    bool HEMaffected = false;
    if(dataRun==2018 && RunNum >=319077){
      for(int i=0;i<Jets->size();i++){
	if((*Jets)[i].Pt() < 30) continue;
	if( (*Jets)[i].Eta() >= -3.20 && (*Jets)[i].Eta() <= -1.2 && 
	    (*Jets)[i].Phi() >= -1.77 && (*Jets)[i].Phi() <= -0.67 &&
	    (abs(DeltaPhi(METPhi,(*Jets)[i].Phi())) < 0.5) ){HEMaffected = true; break;}
      }
    }
    //--------------------------end of triggers
    //----MT
    double mt = 0, mt2j = 0;
    if(JetsAK8->size() > 0) mt = sqrt(2*(*JetsAK8)[0].Pt()*MET*(1-cos(DeltaPhi(METPhi,(*JetsAK8)[0].Phi()))));
    if(JetsAK8->size()>=2) mt2j = sqrt(2*(*JetsAK8)[1].Pt()*MET*(1-cos(DeltaPhi(METPhi,(*JetsAK8)[1].Phi()))));

    if(HEMaffected){
      h_cutflow->Fill("HEMaffected",wt);
      continue;
    }
        
    //---------------------------------------------------------------Gen W and AK8 jet matching
    // for (int gen=0; gen < GenParticles_PdgId->size(); gen++){
    //   if( abs((*GenParticles_PdgId)[gen]) == 24){
    // 	cout << "W is here "<< endl;
    // 	if(abs(DeltaR((*GenParticles)[gen].Eta(),(*GenParticles)[gen].Phi(),(*JetsAK8)[0].Eta(),(*JetsAK8)[0].Phi())) < 0.1){
    //---------------------------------------------------------------
    
    

    h_MET->Fill(MET,wt);
    h_MHT->Fill(MHT,wt);
    h_HT->Fill(HT,wt);
    h_NJets->Fill(NJets,wt);
    h_BTags->Fill(BTagsDeepCSV,wt);
    h_dPhi1->Fill(DeltaPhi1,wt);
    h_dPhi2->Fill(DeltaPhi2,wt);
    h_dPhi3->Fill(DeltaPhi3,wt);
    h_dPhi4->Fill(DeltaPhi4,wt);
	  
    cout<<"jets ak8 size "<<JetsAK8->size()<<endl;
    cout<<"z disc size "<<JetsAK8_zDiscriminatorDeep->size()<<endl;
    cout<<"\n";
    
    //case 1: 85<j1<135 and j1->doubleB   and 65<j2<90 and j2->w tag (but not tagged in here)
    if(sorted_mass[0] > 85 && 
       sorted_mass[0] < 135 &&
       sorted_doubleBdis[0] >0.3 &&
       sorted_mass[1] > 65 && 
       sorted_mass[1] < 90 
       // sorted_wDis[1] >0.9
       ){
      if(JetsAK8->size() > 0){
	cout << "mass 1 " << "\t"<< sorted_mass[0] <<"\t"<< "mass 2"<<"\t" << sorted_mass[1] <<endl; 
	h_AK8J1Pt->Fill(sorted_Pt[0],wt);
	h_AK8J1Eta->Fill(sorted_Eta[0],wt);
	h_AK8J1Mass->Fill(sorted_mass[0],wt);
	h_AK8J1Tau21->Fill(((*JetsAK8_NsubjettinessTau2)[0])/((*JetsAK8_NsubjettinessTau1)[0]),wt);
	h_MT->Fill(mt,wt);
	h_MT2J->Fill(mt2j,wt);
	h_dPhiMETAK8->Fill(abs(DeltaPhi(METPhi,(*JetsAK8)[0].Phi())),wt);
          
	// h_dRGenAK8J1->Fill(abs(DeltaR((*GenParticles)[gen].Eta(),(*GenParticles)[gen].Phi(),(*JetsAK8)[0].Eta(),(*JetsAK8)[0].Phi())),wt);
	// h2_DisdRAK8J1->Fill(abs(DeltaR((*GenParticles)[gen].Eta(),(*GenParticles)[gen].Phi(),(*JetsAK8)[0].Eta(),(*JetsAK8)[0].Phi())),((*JetsAK8_wDiscriminatorDeepDecorrel)[0]),wt);
	// h2_Tau21dRAK8J1->Fill(abs(DeltaR((*GenParticles)[gen].Eta(),(*GenParticles)[gen].Phi(),(*JetsAK8)[0].Eta(),(*JetsAK8)[0].Phi())),((*JetsAK8_NsubjettinessTau2)[0])/((*JetsAK8_NsubjettinessTau1)[0]),wt);

	h2_AK8J1Mass_J1Tau21->Fill(((*JetsAK8_softDropMass)[0]),(((*JetsAK8_NsubjettinessTau2)[0])/((*JetsAK8_NsubjettinessTau1)[0])),wt);

	h_AK8J1wDisDC->Fill(((*JetsAK8_wDiscriminatorDeepDecorrel)[0]),wt);
	h_AK8J1zhDisDC->Fill(((*JetsAK8_zhDiscriminatorDeepDecorrel)[0]),wt);
	h_AK8J1wDis->Fill(sorted_wDis[0],wt);
	h_AK8J1doubleBdis->Fill(sorted_doubleBdis[0],wt);
	h_AK8J1zDis->Fill(((*JetsAK8_zDiscriminatorDeep)[0]),wt);
      }
	  
      if(JetsAK8->size() >=2){
     
	h_AK8J2Pt->Fill(sorted_Pt[1],wt);
	h_AK8J2Eta->Fill(sorted_Eta[1],wt);
	h_AK8J2Mass->Fill(sorted_mass[1],wt);
	h_AK8J2Tau21->Fill(((*JetsAK8_NsubjettinessTau2)[1])/((*JetsAK8_NsubjettinessTau1)[1]),wt);
	h_dPhiAK8J1J2->Fill(abs(DeltaPhi((*JetsAK8)[0].Phi(),(*JetsAK8)[1].Phi())),wt);

	// h_dRGenAK8J2->Fill(abs(DeltaR((*GenParticles)[gen].Eta(),(*GenParticles)[gen].Phi(),(*JetsAK8)[1].Eta(),(*JetsAK8)[1].Phi())),wt);
	// h2_DisdRAK8J2->Fill(abs(DeltaR((*GenParticles)[gen].Eta(),(*GenParticles)[gen].Phi(),(*JetsAK8)[1].Eta(),(*JetsAK8)[1].Phi())),((*JetsAK8_wDiscriminatorDeepDecorrel)[1]),wt);
	// h2_Tau21dRAK8J2->Fill(abs(DeltaR((*GenParticles)[gen].Eta(),(*GenParticles)[gen].Phi(),(*JetsAK8)[1].Eta(),(*JetsAK8)[1].Phi())),((*JetsAK8_NsubjettinessTau2)[1])/((*JetsAK8_NsubjettinessTau1)[1]),wt);
	// h2_dRAK8J1J2->Fill(abs(DeltaR((*GenParticles)[gen].Eta(),(*GenParticles)[gen].Phi(),(*JetsAK8)[1].Eta(),(*JetsAK8)[1].Phi())),abs(DeltaR((*GenParticles)[2].Eta(),(*GenParticles)[2].Phi(),(*JetsAK8)[0].Eta(),(*JetsAK8)[0].Phi())),wt);
	h2_AK8J2Mass_J2Tau21->Fill(((*JetsAK8_softDropMass)[1]),(((*JetsAK8_NsubjettinessTau2)[1])/((*JetsAK8_NsubjettinessTau1)[1])),wt);
	    
	h_AK8J2wDisDC->Fill(((*JetsAK8_wDiscriminatorDeepDecorrel)[1]),wt);
	h_AK8J2zhDisDC->Fill(((*JetsAK8_zhDiscriminatorDeepDecorrel)[1]),wt);
	h_AK8J2wDis->Fill(sorted_wDis[1],wt);
	h_AK8J2doubleBdis->Fill(sorted_doubleBdis[1],wt);
	h_AK8J2zDis->Fill(((*JetsAK8_zDiscriminatorDeep)[1]),wt);
	    
	h2_AK8J1J2Tau21->Fill(((*JetsAK8_NsubjettinessTau2)[0])/((*JetsAK8_NsubjettinessTau1)[0]),((*JetsAK8_NsubjettinessTau2)[1])/((*JetsAK8_NsubjettinessTau1)[1]),wt);
      }
    }  // end case 1
    //case1 no2btag
    if(sorted_mass[0] > 85 && 
       sorted_mass[0] < 135 &&
       //sorted_doubleBdis[0] >0.3 &&
       sorted_mass[1] > 65 && 
       sorted_mass[1] < 90 
       // sorted_wDis[1] >0.9
       ){
      h_AK8J1wDisnoBtag->Fill(sorted_wDis[0],wt);
      h_AK8J1doubleBdisnoBtag->Fill(sorted_doubleBdis[0],wt);
      
      h_AK8J2wDisnoBtag->Fill(sorted_wDis[1],wt);
      h_AK8J2doubleBdisnoBtag->Fill(sorted_doubleBdis[1],wt);
    }
    

    //case 2: 85<j1<135 and j1->wtag (but not tagged here)   and 65<j2<90 and j2->doubleBtag
    if(sorted_mass[0] > 85 && 
       sorted_mass[0] < 135 &&
       sorted_doubleBdis[1] >0.3 &&
       sorted_mass[1] > 65 && 
       sorted_mass[1] < 90 
       // sorted_wDis[0] >0.9
       ){
      h_AK8J1wDiscase2->Fill(sorted_wDis[0],wt);
      h_AK8J1doubleBdiscase2->Fill(sorted_doubleBdis[0],wt);
      
      h_AK8J2wDiscase2->Fill(sorted_wDis[1],wt);
      h_AK8J2doubleBdiscase2->Fill(sorted_doubleBdis[1],wt);
    }
    
    //case2 no 2b tag
    if(sorted_mass[0] > 85 && 
       sorted_mass[0] < 135 &&
       // sorted_doubleBdis[1] >0.3 &&
       sorted_mass[1] > 65 && 
       sorted_mass[1] < 90 
       // sorted_wDis[0] >0.9
       ){
      h_AK8J1wDiscase2noBtag->Fill(sorted_wDis[0],wt);
      h_AK8J1doubleBdiscase2noBtag->Fill(sorted_doubleBdis[0],wt);
      
      h_AK8J2wDiscase2noBtag->Fill(sorted_wDis[1],wt);
      h_AK8J2doubleBdiscase2noBtag->Fill(sorted_doubleBdis[1],wt);
    }
  
    
   
  //Uncomment the varibles in header file before using this
    // side band for deep ak8wDisc.
    
    // if(((*JetsAK8_wDiscriminatorDeep)[0]) < 0.9 && ((*JetsAK8_wDiscriminatorDeep)[1]) < 0.9){
    //   if(JetsAK8->size() > 0){  
    // 	h_AK8J1Mass2->Fill((*JetsAK8_softDropMass)[0],wt);
    //   }
    //   if(JetsAK8->size() >=2){  
    // 	h_AK8J2Mass2->Fill((*JetsAK8_softDropMass)[1],wt);
    //   }
    // }
    
    // if(((*JetsAK8_wDiscriminatorDeepDecorrel)[0]) > 0.5 && ((*JetsAK8_wDiscriminatorDeepDecorrel)[1]) > 0.5){
    //   if(JetsAK8->size() > 0){  
    // 	h_AK8J1Mass3->Fill((*JetsAK8_softDropMass)[0],wt);
    //   }
    //   if(JetsAK8->size() >=2){  
    // 	h_AK8J2Mass3->Fill((*JetsAK8_softDropMass)[1],wt);
    //   }
    // }
    
    // if(((*JetsAK8_wDiscriminatorDeepDecorrel)[0]) < 0.5 && ((*JetsAK8_wDiscriminatorDeepDecorrel)[1]) < 0.5){
    //   if(JetsAK8->size() > 0){  
    // 	h_AK8J1Mass4->Fill((*JetsAK8_softDropMass)[0],wt);
    //   }
    //   if(JetsAK8->size() >=2){  
    // 	h_AK8J2Mass4->Fill((*JetsAK8_softDropMass)[1],wt);
    //   }
    // }

    //}  // for ak8 jets gen matched
    
    nEvtSurv++;
    h_cutflow->Fill("NEvtsNoWtLeft",1);
    
    cout<<"--------------------event---------------"<<"\n"<<endl;
  } // loop over entries
  cout<<"No. of entries survived: "<<nEvtSurv<<endl;
}

void SignalReg::sortbymass(vector<double> mass, int idx){
  //--sorting events by decreasing mass 
  vector<pair<double, int> > masspair;
  for (int i=0; i< JetsAK8->size(); i++){
    masspair.push_back(make_pair((*JetsAK8_softDropMass)[i], i));
  }
  sort(masspair.begin(),masspair.end(), greater<>());
  for (int i = 0; i < masspair.size(); i++) { 
    cout << masspair[i].first << "\t"
	 << masspair[i].second << endl; 
  } 
    //sort(arr, arr+5, greater<>());
  // [[m1,pt1,eta1],[m2,pt2,eta2],[]]
  // sortbyMass.push_back(*JetsAK8_softDropMass); // []
  // sortbyMass.push_back(*JetsAK8->Pt()); // vector.at(0) , vector[0]
  //sortbyMass.push_back(*JetsAK8->Eta());
  vector<double> sorted_Pt(masspair.size());
  vector<double> sorted_Eta(masspair.size());  

  // }
}


void SignalReg::print(Long64_t jentry){
  //cout<<endl;
  TLorentzVector v1,photo;
  cout<<"------------------------------------------------------------"<<endl;
  cout<<"MomMass:"<<SusyMotherMass<<" Kid Mass:"<<SusyLSPMass<<endl;
  for(int i=0;i<GenParticles->size();i++){  
    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<<"\tPx :"<<(*GenParticles)[i].Px()<<" Py :"<<(*GenParticles)[i].Py()<<" Pz :"<<(*GenParticles)[i].Pz()<<" E: "<<(*GenParticles)[i].Energy()<<" M:"<<(*GenParticles)[i].M()<<endl;
    cout<<EvtNum<<" "<<jentry<<" "<<GenParticles->size()<<" "<<i<<" PdgId:"<<(*GenParticles_PdgId)[i]<<" parentId:"<<(*GenParticles_ParentId)[i]<<" parentIndx:"<<(*GenParticles_ParentIdx)[i]<<" Status:"<<(*GenParticles_Status)[i]<</*"\tPx:"<<(*GenParticles)[i].Px()<<" Py:"<<(*GenParticles)[i].Py()<<" Pz:"<<(*GenParticles)[i].Pz()<<*/"\tPt:"<<(*GenParticles)[i].Pt()<<" Eta:"<<(*GenParticles)[i].Eta()<<" Phi:"<<(*GenParticles)[i].Phi()<<" E:"<<(*GenParticles)[i].Energy()<<endl;
    
  }
  for(int i=0;i<Photons->size();i++){
    double dR=0;//DeltaR( bestPhoton.Eta(),bestPhoton.Phi(),(*Photons)[i].Eta(),(*Photons)[i].Phi() );
    //cout<<jentry<<" i:"<<i<<" phoSize:"<<Photons->size()<<" Pt:"<<bestPhoton.Pt()<<" eta:"<<bestPhoton.Eta()<<" phi:"<<bestPhoton.Phi()<<" otherP:"<<(*Photons)[i].Pt()<<" eta:"<<(*Photons)[i].Eta()<<" phi:"<<(*Photons)[i].Phi()<<" dR:"<<dR<<endl;
  }
  for(int i=0;i<Jets->size();i++){
    cout<<"JetPt:"<<(*Jets)[i].Pt()<<" JetEta:"<<(*Jets)[i].Eta()<<" JetPhi:"<<(*Jets)[i].Phi()<<endl;
  }
  cout<<"MHTPhi:"<<MHTPhi<<" DPhi1:"<<DeltaPhi1<<" DeltaPhi2:"<<DeltaPhi2<<" DeltaPhi3:"<<DeltaPhi3<<" DeltaPhi4:"<<DeltaPhi4<<endl;
}
