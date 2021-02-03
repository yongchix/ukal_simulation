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
    h1Test = new TH1D("h1Test", "Test", 4096*4, 0, 4096); 
    h1Sample = new TH1D("h1Sample", "Energy Deposition in Sample, gated on e-; keV; Counts/0.25 keV", 
                        4096*4, 0, 4096); 
    h1HPGe = new TH1D("h1HPGe", "Energy Deposition in HPGe, gated on e-; keV; Counts/0.25 keV", 
                      4096*4, 0, 4096);                     
}

// void UKALAnalysisManager::FillTH1D(TH1D *h1, G4double energy) {
//     h1->Fill(energy); 
// }

void UKALAnalysisManager::Save() {
    if(outroot) {
        outroot->Write(); 
        outroot->Close(); 
    }
}