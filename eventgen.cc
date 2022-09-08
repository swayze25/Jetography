//P2
#include "fastjet/ClusterSequence.hh"
#include "TH1.h"
#include <TF2.h>
#include "TH2F.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"
#include "TStyle.h"
#include "TCanvas.h"
#include <TGraph2D.h>
#include "TTreeReader.h"
#include "TTreeReaderArray.h"
#include "Riostream.h"
#include <TLorentzVector.h>
#include <iostream> 
#include "Pythia8/Pythia.h"

using namespace fastjet;
using namespace Pythia8;
using namespace std;

int main() {
	// Generator.
  Pythia pythia;
	Settings& settings  = pythia.settings;
  //const Info& info = pythia.info;
  pythia.readString("Stat:reset = on");
  pythia.readString("PhaseSpace:Q2Min = 125.0");
  
  // Generator. Process selection. LHC initialization. Histogram.
	pythia.readString("Init:showMultipartonInteractions = on");
  pythia.readString("Beams:eA  = 27.5.");
  pythia.readString("Beams:eB  = 820.");
  pythia.readString("Beams:eCM = 8000.");
  pythia.readString("HardQCD:all = on");
  //pythia.readString("PhaseSpace:pTHatMin = 20.");
  pythia.init();
  
  TFile *genevents = new TFile("genevents.root","RECREATE");
  TTree *T = new TTree("events", "4-Momentum of all Charged Final State Particles");
  std::vector<Float_t> px,py,pz,e;
  T->Branch("px", "std::vector<Float_t>",  &px);
  T->Branch("py", "std::vector<Float_t>",  &py);
  T->Branch("pz", "std::vector<Float_t>",  &pz);
  T->Branch("e", "std::vector<Float_t>",  &e);
  
  
  int nEvents = 10000;
  
  for (int iEvent = 0; iEvent < nEvents; ++iEvent) {
    if (!pythia.next()) continue;
    px.clear(); py.clear(); pz.clear(); e.clear();
    
    for (int i = 0; i < pythia.event.size(); ++i){
      if (pythia.event[i].isFinal() && pythia.event[i].isCharged()){
      
    		px.push_back(pythia.event[i].px());
        py.push_back(pythia.event[i].py());
        pz.push_back(pythia.event[i].pz());
        e.push_back(pythia.event[i].e());
      }
    }
  	T->Fill();
  }
  genevents->Write();
  genevents->Close();
  delete genevents;
}
    
      
      
      	
      	
