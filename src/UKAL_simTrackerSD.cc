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
/// \file UKAL_simTrackerSD.cc
/// \brief Implementation of the UKAL_simTrackerSD class

#include "UKAL_simTrackerSD.hh"
#include "G4HCofThisEvent.hh"
#include "G4Step.hh"
#include "G4ThreeVector.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "UKAL_simAnalysisManager.hh"

using namespace std; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UKAL_simTrackerSD::UKAL_simTrackerSD(const G4String& name,
                         const G4String& hitsCollectionName) 
	: G4VSensitiveDetector(name),
	  fHitsCollection(NULL)
{
	collectionName.insert(hitsCollectionName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

UKAL_simTrackerSD::~UKAL_simTrackerSD() 
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UKAL_simTrackerSD::Initialize(G4HCofThisEvent* hce)
{
	// Create hits collection

	fHitsCollection 
		= new UKAL_simTrackerHitsCollection(SensitiveDetectorName, collectionName[0]); 

	// Add this collection in hce

	G4int hcID 
		= G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);
	hce->AddHitsCollection( hcID, fHitsCollection ); 
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4bool UKAL_simTrackerSD::ProcessHits(G4Step* aStep, 
								G4TouchableHistory*)
{  
	// energy deposit
	G4double edep = aStep->GetTotalEnergyDeposit();

	if (edep==0.) return true; // false; 

	// get this hit
	UKAL_simTrackerHit* aHit = new UKAL_simTrackerHit();
	G4int nHits = fHitsCollection->entries(); 

	// // verbose
	// G4cout << "\t Yongchi: number of hits in this event: " << fHitsCollection->entries() << G4endl;
	// G4cout << "particle type: " << aStep->GetTrack()->GetDefinition()->GetParticleType()
	// 	   << "; particle name: " << aStep->GetTrack()->GetDefinition()->GetParticleName()
	// 	   << "; energy = " << aStep->GetTotalEnergyDeposit()/keV << " keV"
	// 	   << G4endl;


	// process this hit
	aHit->SetTrackID  (aStep->GetTrack()->GetTrackID());
	aHit->SetChamberNb(aStep->GetPreStepPoint()->GetTouchableHandle()
						 ->GetCopyNumber());
	aHit->AddEdep(edep);
	aHit->SetPos (aStep->GetPostStepPoint()->GetPosition());

	// by Yongchi - process this hit for more information - refer to e16032 simulation
	G4double gt = aStep->GetPreStepPoint()->GetGlobalTime();
	aHit->SetGtime( gt );
	G4int st = aStep->GetTrack()->GetCurrentStepNumber();
	aHit->SetStepno( st );
	G4int tr = aStep->GetTrack()->GetTrackID();
	aHit->SetTrackno( tr );
	G4double ke = aStep->GetTrack()->GetKineticEnergy();
	aHit->SetKineticEnergy(ke);
	G4int pa = aStep->GetTrack()->GetParentID();
	aHit->SetParentno( pa );
	// particle
	G4String ty = aStep->GetTrack()->GetDefinition()->GetParticleType();
	aHit->SetParticletype( ty );
	G4String nam = aStep->GetTrack()->GetDefinition()->GetParticleName();
	aHit->SetParticlename( nam );
	G4String volname = aStep->GetTrack()->GetVolume()->GetName();
	aHit->SetVolName( volname );
	// cout << "Paticle = " << aHit->GetParticlename() << endl; // verbose

	G4ThreeVector pre = aStep->GetPreStepPoint()->GetPosition();
	aHit->SetPrePosition( pre );
	G4ThreeVector post =aStep->GetPostStepPoint()->GetPosition();
	aHit->SetPostPosition( post );
	G4ThreeVector del = aStep->GetDeltaPosition();
	aHit->SetDeltaPosition( del );

	G4double dlen = aStep->GetStepLength();
	aHit->SetDeltaLength( dlen );
    
	G4double slen = aStep->GetStepLength();
	aHit->SetStepLength( slen );

	aHit->SetEdep(edep); 


	// processing is done
	fHitsCollection->insert( aHit );

	//aHit->Print();

	// // by Yongchi - for info output
	// UKAL_simAnalysisManager *analysis = UKAL_simAnalysisManager::GetInstance(); 
	// if(aHit->GetParticlename() == "e-") {
	// 	analysis->FillTH1D(edep/keV); 	
	// }
	// G4cout << edep/keV << " keV" << G4endl; 

	return true;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UKAL_simTrackerSD::EndOfEvent(G4HCofThisEvent*)
{
	if ( verboseLevel>1 ) { 
		G4int nofHits = fHitsCollection->entries();
		G4cout << G4endl
			   << "-------->Hits Collection: in this event they are " << nofHits 
			   << " hits in the tracker chambers: " << G4endl;
		for ( G4int i=0; i<nofHits; i++ ) (*fHitsCollection)[i]->Print();
	}
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
