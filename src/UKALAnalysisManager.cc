#include "UKALAnalysisManager.hh"

UKALAnalysisManager* UKALAnalysisManager::instance = 0; 

// constructor
UKALAnalysisManager::UKALAnalysisManager() {
    outroot = 0; 
    h1Test = 0; 
}

// desctructor
UKALAnalysisManager::~UKALAnalysisManager() {
    delete outroot; outroot = 0; 
    delete h1Test; h1Test = 0; 
}

UKALAnalysisManager* UKALAnalysisManager::GetInstance() {
    if(instance == 0)
        instance = new UKALAnalysisManager(); 

    return instance; 
}

void UKALAnalysisManager::book() {
    delete outroot; 
    // define the ROOT file
    outroot = new TFile("B2_test.root", "RECREATE"); 
    h1Test = new TH1D("h1Test", "Test", 1024, 0, 1024); 
}

void UKALAnalysisManager::FillTH1D(G4double energy) {
    h1Test->Fill(energy); 
}

void UKALAnalysisManager::Save() {
    if(outroot) {
        outroot->Write(); 
        outroot->Close(); 
    }
}