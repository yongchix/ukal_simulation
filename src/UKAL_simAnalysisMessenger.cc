#include "UKAL_simAnalysisMessenger.hh"
#include "UKAL_simAnalysisManager.hh"
#include "UKAL_simEventAction.hh"

#include "globals.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"

UKAL_simAnalysisMessenger::UKAL_simAnalysisMessenger(UKAL_simAnalysisManager* man) : analysisManager(man) {
    analysisDir = new G4UIdirectory("/UKAL_sim/analysis/"); 
    analysisDir->SetGuidance("output control"); 

    // G4cout << "haha " << G4endl; 

    filenameCmd = new G4UIcmdWithAString("/UKAL_sim/analysis/filename", this); 
    filenameCmd->SetGuidance("Analysis file name");
    filenameCmd->SetParameterName("choice",false);
    filenameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);

    dirnameCmd = new G4UIcmdWithAString("/UKAL_sim/analysis/dirname", this); 
    dirnameCmd->SetGuidance("Directory for output files");
    dirnameCmd->SetParameterName("dirname",false);
    dirnameCmd->AvailableForStates(G4State_PreInit,G4State_Idle);
}

UKAL_simAnalysisMessenger::~UKAL_simAnalysisMessenger() {
    delete filenameCmd; 
    delete dirnameCmd; 
}

void UKAL_simAnalysisMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {

    //G4cout << " >>> Now trying to set new names for output... <<<" << G4endl; 

    if(command == filenameCmd) { //G4cout << "\t\tThis is the filename..." << G4endl; 
        analysisManager->SetFilename(newValue); 
    }

    if(command == dirnameCmd) { //G4cout << "\t\tThis is the directory..." << G4endl; 
        analysisManager->SetDirname(newValue); 
    }
}