#ifndef UKAL_sim_ANALYSIS_MANAGER_HH
#define UKAL_sim_ANALYSIS_MANAGER_HH

#include "globals.hh"

#include "TFile.h"
#include "TROOT.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TF1.h"

class UKAL_simAnalysisMessenger; 

class UKAL_simAnalysisManager {
    private: // why private???
        UKAL_simAnalysisManager(); 
        UKAL_simAnalysisMessenger *messenger; 

    public: 
        ~UKAL_simAnalysisManager(); 
        static UKAL_simAnalysisManager* GetInstance(); 
        void book(); 

    // create the root files and histograms
    // void FillTH2D(G4double x, G4double y, G4double energy); 
    //void FillTH1D(TH1D *h1, G4double energy); 
    //TH1D* GetHistogram(TH1D* h1) {return h1; }
    // save the result at the end of each run to the ROOT file
    void Save(); 

    private: 
        static UKAL_simAnalysisManager* instance; 
    public: 
        TFile *outroot; 
        TH1D *h1Test; 
        TH1D *h1HPGe; 
        TH1D *h1Sample; 

    public: 
        void SetFilename(G4String name); 
        void SetDirname(G4String name); 
    private: 
        G4String filename; 
        G4String dirname; 

    public: 
        TF1 *f1Res; 
}; 






#endif