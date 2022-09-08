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

using namespace fastjet;
using namespace std;

int main(){

  vector<fastjet::PseudoJet> input_particles;
  
  TFile *myFile = new TFile("genevents.root","READ");
  TTreeReader myReader("events", myFile);
  TTreeReaderArray<Float_t> px(myReader, "px");
  TTreeReaderArray<Float_t> py(myReader, "py");
  TTreeReaderArray<Float_t> pz(myReader, "pz");
  TTreeReaderArray<Float_t> e(myReader, "e");
  
  TFile *jetreco = new TFile("jetsreco.root","RECREATE");
  TTree *jetparam = new TTree("jetparam", "");
  std::vector<Float_t> ej,mj,ptj,pxj,pyj,pzj,rapj,phij;
  jetparam->Branch("energy", "std::vector<Float_t>",  &ej);
  jetparam->Branch("mass", "std::vector<Float_t>",  &mj);
  jetparam->Branch("pt", "std::vector<Float_t>",  &ptj);
  jetparam->Branch("px", "std::vector<Float_t>",  &pxj);
  jetparam->Branch("py", "std::vector<Float_t>",  &pyj);
  jetparam->Branch("pz", "std::vector<Float_t>",  &pzj);
  jetparam->Branch("phi", "std::vector<Float_t>",  &phij);
  jetparam->Branch("rap", "std::vector<Float_t>",  &rapj);
  TH2F *h2 = new TH2F("h2","",100,-50,50,100,0,100);
	TH2F *jvis = new TH2F("jvis","Jet Visualization in the Rapidity-Azimuth plane",100,-50,50,100,0,100);
  
  while (myReader.Next()){
  	for (int i = 0; i < px.GetSize(); i++){
  		input_particles.push_back(fastjet::PseudoJet(px[i],py[i],pz[i],e[i]));
  	}
  }
  
  // choose a jet definition
	double R = 0.7;
	double ptmin = 7.0;
	
	fastjet::Strategy strategy = fastjet::Best;
  fastjet::RecombinationScheme recomb_scheme=fastjet::E_scheme;
  fastjet::JetAlgorithm jet_alg = fastjet::kt_algorithm;
  fastjet::JetDefinition jet_def(jet_alg, R, recomb_scheme, strategy);
	
	// run the clustering, extract the jets
	fastjet::ClusterSequence clust_seq(input_particles, jet_def);
	vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(ptmin));

	//OUTPUT
  cout << "Ran " << jet_def.description() << endl;
	// label the columns
  printf("%5s %15s %15s %15s %15s\n","jet #", "rapidity", "phi", "pt", "energy");
  // print out the details for each jet
  
  for ( int i = 0; i < inclusive_jets.size(); i++) {
 //if (inclusive_jets[i].rap() > -1 && inclusive_jets[i].rap() < 2)
    printf("%5u %15.8f %15.8f %15.8f %15.8f\n",
	   i, inclusive_jets[i].rap(), inclusive_jets[i].phi(), inclusive_jets[i].pt(), inclusive_jets[i].e());
  	ej.push_back(inclusive_jets[i].e());
  	mj.push_back(inclusive_jets[i].m());
  	ptj.push_back(inclusive_jets[i].pt());
  	pxj.push_back(inclusive_jets[i].px());
  	pyj.push_back(inclusive_jets[i].py());
  	pzj.push_back(inclusive_jets[i].pz());
  	phij.push_back(inclusive_jets[i].phi());
  	rapj.push_back(inclusive_jets[i].rap());
  	h2->Fill(inclusive_jets[i].rap(),inclusive_jets[i].phi());
  	
  	jetparam->Fill();
  	ej.clear(); ptj.clear(); mj.clear();pxj.clear();pyj.clear();pzj.clear(); phij.clear(); rapj.clear();
  	jvis->Add(h2);
  	h2->Reset();
  }

	jetreco->Write();
	jetreco->Close();
	delete jetreco;
}
  
 
