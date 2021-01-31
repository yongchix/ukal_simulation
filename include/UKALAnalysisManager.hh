#ifndef UKAL_ANALYSIS_MANAGER_HH
#define UKAL_ANALYSIS_MANAGER_HH

#include "globals.hh"

#include "TFile.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TH2D.h"

class UKALAnalysisManager {
    private: // why private???
        UKALAnalysisManager(); 
    public: 
        ~UKALAnalysisManager(); 
        static UKALAnalysisManager* GetInstance(); 
        void book(); 

    // create the root files and histograms
    // void FillTH2D(G4double x, G4double y, G4double energy); 
    void FillTH1D(G4double energy); 
    // save the result at the end of each run to the ROOT file
    void Save(); 

    private: 
        static UKALAnalysisManager* instance; 
    private: 
        TFile *outroot; 
        TH1D *h1Test; 
}; 






#endif