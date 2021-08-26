//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file UKAL_simRunAction.cc
/// \brief Implementation of the UKAL_simRunAction class

#include "UKAL_simRunAction.hh"

#include "UKAL_simDetectorConstruction.hh"
#include "UKAL_simAnalysisExample.hh"
#include "UKAL_simAnalysisExampleMessenger.hh"

#include "UKAL_simDetectorHit.hh"

//placing root above G4 so that I stop getting warnings
#if defined (G4ANALYSIS_USE_ROOT)

#include "TROOT.h"
#include "TApplication.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TPad.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TStopwatch.h"
#include "TTree.h"
#include "TRandom.h"

#endif /* defined (G4ANALYSIS_USE_ROOT) */

#include "G4Run.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo....

void UKAL_simAnalysisExample::DetectorInfo(UKAL_simDetectorConstruction* myDet){

  //important information about the setup
  UseHPGeDetector = Int_t(myDet->getUseHPGeDetector());
  UseBGO = Int_t(myDet->getUseBGO());
  UseLeadShield = Int_t(myDet->getUseBGO());
  UseParaffinBlock = Int_t(myDet->getUseBGO());
  UseCopperShields = Int_t(myDet->getUseBGO());
  
  //size of detector
  HPGeDetectorLength = myDet->getHPGeDetectorLength();
  HPGeDetectorRadius = myDet->getHPGeDetectorRadius();
 
  //location of detector
  HPGe_x = myDet->getHPGeDetectorPos().x();
  HPGe_y = myDet->getHPGeDetectorPos().y(); 
  HPGe_z = myDet->getHPGeDetectorPos().z();

  //orientation of detector
  setupRot_x = myDet->getSetupRotationVector().x(); //rotation about the x-axis
  setupRot_y = myDet->getSetupRotationVector().y(); //rotation about the y-axis
  setupRot_z = myDet->getSetupRotationVector().z(); //rotation about the z-axis

  G4double defBins = 6000;
  G4double defRangeLow = 0;
  G4double defRangeHigh = 3000;

  //change values to defaults if desired
  if(GetUseHHPGeDefParam()){
    G4cout << "usehHPGeDefParam is true: " << G4endl;
    hHPGeNumBins = defBins;
    hHPGeRangeLow = defRangeLow;
    hHPGeRangeHigh = defRangeHigh;
  }

  //SVAJDIC 16 Jul 2020
  //this serves only to make the program compile
  max_events = MAX_EVENTS;

  UKAL_siminfo = (TTree *)gROOT->FindObject("UKAL_siminfo");
  if(UKAL_siminfo);
  else{
    UKAL_siminfo = new TTree("UKAL_siminfo","UKAL_siminfo variables");
    //show toggles used SVajdic 22 Jul 2020
    UKAL_siminfo->Branch("UseHPGeDetector",&UseHPGeDetector,"UseHPGeDetector/I");
    UKAL_siminfo->Branch("UseBGO",&UseBGO,"UseBGO/I");
    UKAL_siminfo->Branch("UseLeadShield",&UseLeadShield,"UseLeadShield/I");
    UKAL_siminfo->Branch("UseParaffinBlock",&UseParaffinBlock,"UseParaffinBlock/I");
    UKAL_siminfo->Branch("UseCopperShields",&UseCopperShields,"UseCopperShields/I");

    UKAL_siminfo->Branch("HPGeDetectorLength",&HPGeDetectorLength,"HPGeDetectorLength/D");
    UKAL_siminfo->Branch("HPGeDetectorRadius",&HPGeDetectorRadius,"HPGeDetectorRadius/D");
    UKAL_siminfo->Branch("HPGe_x",&HPGe_x,"HPGe_x/D");
    UKAL_siminfo->Branch("HPGe_y",&HPGe_y,"HPGe_y/D");
    UKAL_siminfo->Branch("HPGe_z",&HPGe_z,"HPGe_z/D");

    //SVajdic 17 Jul 2020
    UKAL_siminfo->Branch("MAX_EVENTS",&max_events,"max_events/I");
    UKAL_siminfo->Branch("totalEventsFromSource",&totalEventsFromSource,"totalEventsFromSource/I");
    UKAL_siminfo->Branch("dummyPosValue",&dummyPosValue,"dummyPosValue/D");
    UKAL_siminfo->Branch("hHPGeNumBins",&hHPGeNumBins,"hHPGeNumBins/D");
    UKAL_siminfo->Branch("hHPGeRangeLow",&hHPGeRangeLow,"hHPGeRangeLow/D");
    UKAL_siminfo->Branch("hHPGeRangeHigh",&hHPGeRangeHigh,"hHPGeRangeHigh/D");
    //SVAJDIC 21 Jan 2021
    UKAL_siminfo->Branch("setupRot_x",&setupRot_x,"setupRot_x/D");
    UKAL_siminfo->Branch("setupRot_y",&setupRot_y,"setupRot_y/D");
    UKAL_siminfo->Branch("setupRot_z",&setupRot_z,"setupRot_z/D");
    
  }
  //fill tree further down after program knows how many events will be processed
  //UKAL_siminfo->Fill();

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UKAL_simRunAction::UKAL_simRunAction()
 : G4UserRunAction()
{ 
      
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UKAL_simRunAction::~UKAL_simRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UKAL_simRunAction::BeginOfRunAction(const G4Run*)
{ 
  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

//getting the total number of events used from beamOn
  totalEventsFromSource = G4RunManager::GetRunManager()->GetCurrentRun()->GetNumberOfEventToBeProcessed();

  G4cout << endl << endl << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << "User set events from source to be: " << totalEventsFromSource << endl;
  G4cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl << endl;
   
  //filling the tree here so that it logs the right number of events to be processed
  UKAL_siminfo->Fill();
  G4cout << "Info tree has been filled" << G4endl;


#if defined (G4ANALYSIS_USE_ROOT)

  stringstream fil;
  fil << dirname << filename << "_" << rank << ".root";
  G4String filenam = fil.str();
  file1 = new TFile(filenam.c_str(),"RECREATE");

  filecount++;
  G4cout << "starting run " << filenam << " " << filecount << G4endl;

  if (gSystem) gSystem->ProcessEvents();


  timer = new TStopwatch();

  /*
   * The main output of the file
   */
  UKAL_simtree = (TTree *)gROOT->FindObject("UKAL_simtree");
  if(UKAL_simtree);
  else
    {
      UKAL_simtree = new TTree("UKAL_simtree","DSSD hit variables");
      //added by SVAJDIC 24 Jul 2020
      UKAL_simtree->Branch("particleNames",&particleNames,Form("particleNames[%i]/S",MAX_EVENTS));
      UKAL_simtree->Branch("energyDeposited",&energyDeposited,Form("energyDeposited[%i]/D",MAX_EVENTS));
      UKAL_simtree->Branch("hitpos_x",&hitpos_x,Form("hitpos_x[%i]/D",MAX_EVENTS));
      UKAL_simtree->Branch("hitpos_y",&hitpos_y,Form("hitpos_y[%i]/D",MAX_EVENTS));
      UKAL_simtree->Branch("hitpos_z",&hitpos_z,Form("hitpos_z[%i]/D",MAX_EVENTS));
      
    }
  

  G4double binwidth = (hHPGeRangeHigh-hHPGeRangeLow)/hHPGeNumBins;
  const char *xaxistitle = "Bin (keV)";
  const char* yaxistitle;
  yaxistitle = Form("Counts per %f keV bin",binwidth);
  
  if(hRawDetector){
    hRawDetector->Reset();
  } else {
    hRawDetector = new TH1D("Raw Detector Signal","Raw Detector Signal",hHPGeNumBins,hHPGeRangeLow,hHPGeRangeHigh);
    hRawDetector->GetXaxis()->SetTitle(xaxistitle); 
    hRawDetector->GetXaxis()->CenterTitle();
    hRawDetector->GetYaxis()->SetTitle(yaxistitle);
    hRawDetector->GetYaxis()->CenterTitle();
  } 

  if(hBGO){
    hBGO->Reset();
  } else {
    hBGO = new TH1D("BGO Signal","BGO Signal",hHPGeNumBins,hHPGeRangeLow,hHPGeRangeHigh);
    hBGO->GetXaxis()->SetTitle(xaxistitle); 
    hBGO->GetXaxis()->CenterTitle();
    hBGO->GetYaxis()->SetTitle(yaxistitle);
    hBGO->GetYaxis()->CenterTitle();
  }

  if(hSuppression){
    hSuppression->Reset();
  } else {
    hSuppression = new TH1D("Compton Suppressed Signal","Compton Suppressed Signal",hHPGeNumBins,hHPGeRangeLow,hHPGeRangeHigh);
    hSuppression->GetXaxis()->SetTitle(xaxistitle); 
    hSuppression->GetXaxis()->CenterTitle();
    hSuppression->GetYaxis()->SetTitle(yaxistitle);
    hSuppression->GetYaxis()->CenterTitle();
 }
 


  eventtimer = new TStopwatch();
 
  OnceAWhileDoIt(true); // do it now

#endif /* defined (G4ANALYSIS_USE_ROOT) */

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UKAL_simRunAction::EndOfRunAction(const G4Run* )
{

//cout<<"EndOfRunAction"<<endl;
#if defined (G4ANALYSIS_USE_ROOT)
  if (gSystem) gSystem->ProcessEvents();

  stringstream fil;
  fil << dirname << filename << "_" << rank << ".root";
  G4String filenam = fil.str();

  file1=gROOT->GetFile(filenam.c_str());

  file1->cd();
  gROOT->SetStyle("Pub");

  UKAL_simtree->Write();
  UKAL_siminfo->Write();

  file1->Write();
  file1->Close();

  timer->Stop();
  timer->Print();

#endif /* defined (G4ANALYSIS_USE_ROOT) */
  
  G4cout << "Number of events where n_hit exceeded the array size: " << too_many_for_arrays << G4endl;

  OnceAWhileDoIt(true); // do it now

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
