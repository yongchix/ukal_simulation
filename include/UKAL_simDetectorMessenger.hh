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
/// \file UKAL_simDetectorMessenger.hh
/// \brief Definition of the UKAL_simDetectorMessenger class

#ifndef UKAL_simDetectorMessenger_h
#define UKAL_simDetectorMessenger_h 1

#include "globals.hh"
#include "G4UImessenger.hh"

class UKAL_simDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

/// Messenger class that defines commands for UKAL_simDetectorConstruction.
///
/// It implements commands:
/// - /B2/det/setTargetMaterial name
/// - /B2/det/setChamberMaterial name
/// - /B2/det/stepMax value unit

class UKAL_simDetectorMessenger: public G4UImessenger
{
public:
    UKAL_simDetectorMessenger(UKAL_simDetectorConstruction* );
    virtual ~UKAL_simDetectorMessenger();
    
    virtual void SetNewValue(G4UIcommand*, G4String);
    
private:
    UKAL_simDetectorConstruction*  fDetectorConstruction;

    G4UIdirectory*           fB2Directory;
    G4UIdirectory*           fDetDirectory;

    G4UIcmdWithAString*      fTargMatCmd;
    G4UIcmdWithAString*      fChamMatCmd;

    G4UIcmdWithADoubleAndUnit* fStepMaxCmd;

    // by Yongchi - for UKAL_sim
    // add or remove detectors
    G4UIcmdWithABool*          useUKAL_simSampleCmd; 
    G4UIcmdWithABool*          useUKAL_simHPGeCmd; 
    G4UIcmdWithABool*          useUKAL_simBGOCmd;
    // set parameters of detectors
    G4UIcmdWithADoubleAndUnit* setUKAL_simHPGeAngleCmd; 
    G4UIcmdWithADoubleAndUnit* setUKAL_simHPGePosRadiusCmd; 
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
