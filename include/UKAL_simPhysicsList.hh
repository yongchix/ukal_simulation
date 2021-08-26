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
/// \file medical/electronScattering2/include/PhysicsList.hh
/// \brief Definition of the PhysicsList class

#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "globals.hh"
#include <vector>

class G4VPhysicsConstructor;
class UKAL_simPhysicsListMessenger;
class G4ProductionCuts;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class UKAL_simPhysicsList: public G4VModularPhysicsList
{
public:
  UKAL_simPhysicsList();
  virtual ~UKAL_simPhysicsList();

  virtual void ConstructParticle();
        
  void AddPhysicsList(const G4String& name);
    
  virtual void ConstructProcess();    
  // void AddDecay();
  // void AddStepMax();      

  // migrated from e16032 simulation
  void SetCuts();
  void SetCutForGamma(G4double);
  void SetCutForElectron(G4double);
  void SetCutForPositron(G4double);
  //
  void SetTargetCut(G4double val);
  void SetDetectorCut(G4double val);

private: 
  void AddExtraBuilders(G4bool flagHP);
    
private:
  
  UKAL_simPhysicsListMessenger* fMessenger; 

  G4String fEmName;
  G4VPhysicsConstructor*  fEmPhysicsList;
  // by Yongchi - for UKAL_sim
  G4VPhysicsConstructor*  raddecayList;
  G4VPhysicsConstructor*  particleList;
  G4VPhysicsConstructor*  hadPhysicsList;

  // migrated from e16032 simulation
  std::vector<G4VPhysicsConstructor*> hadronPhys; 
  G4int nhadcomp;
  G4ProductionCuts* DetectorCuts;
  G4ProductionCuts* TargetCuts;

  G4double cutForGamma;
  G4double cutForElectron;
  G4double cutForPositron;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
