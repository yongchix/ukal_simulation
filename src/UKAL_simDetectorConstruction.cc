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
/// \file UKAL_simDetectorConstruction.cc
/// \brief Implementation of the UKAL_simDetectorConstruction class
 
#include "UKAL_simDetectorConstruction.hh"
#include "UKAL_simDetectorMessenger.hh"
#include "UKAL_simTrackerSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4SDManager.hh"

#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4GeometryTolerance.hh"
#include "G4GeometryManager.hh"

#include "G4UserLimits.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4SystemOfUnits.hh"

#include "UKAL_simMaterial.hh"


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4ThreadLocal 
G4GlobalMagFieldMessenger* UKAL_simDetectorConstruction::fMagFieldMessenger = 0;

UKAL_simDetectorConstruction::UKAL_simDetectorConstruction()
	:G4VUserDetectorConstruction(), 
	 fNbOfChambers(0),
	 fLogicTarget(NULL), fLogicChamber(NULL), 
	 fTargetMaterial(NULL), fChamberMaterial(NULL), 
	 fStepLimit(NULL),
	 fCheckOverlaps(true),
	 hpgePhi(180*degree), hpgePosRadius(10*mm)
{
	fMessenger = new UKAL_simDetectorMessenger(this);

	fNbOfChambers = 5;
	fLogicChamber = new G4LogicalVolume*[fNbOfChambers];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
UKAL_simDetectorConstruction::~UKAL_simDetectorConstruction()
{
	// delete [] fLogicChamber; 
	delete fStepLimit;
	delete fMessenger;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
G4VPhysicalVolume* UKAL_simDetectorConstruction::Construct()
{
	// Define materials
	DefineMaterials();

	// Define volumes
	return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UKAL_simDetectorConstruction::DefineMaterials()
{
	// Material definition 
	// by Yongchi 
	managerUKAL_simMaterial = new UKAL_simMaterial(); 

	G4NistManager* nistManager = G4NistManager::Instance();

	// Air defined using NIST Manager
	nistManager->FindOrBuildMaterial("G4_AIR");
  
	// Lead defined using NIST Manager
	fTargetMaterial  = nistManager->FindOrBuildMaterial("G4_Pb");

	// Xenon gas defined using NIST Manager
	fChamberMaterial = nistManager->FindOrBuildMaterial("G4_Xe");

	// by Yongchi - for UKAL_sim
    sampleMater = managerUKAL_simMaterial->GetMaterial("XeF2"); 
    hpgeMater   = managerUKAL_simMaterial->GetMaterial("Ge");

	// Print materials
	G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* UKAL_simDetectorConstruction::DefineVolumes()
{
	G4Material* air  = G4Material::GetMaterial("G4_AIR");

	// Sizes of the principal geometrical components (solids)
  
	G4double chamberSpacing = 80*cm; // from chamber center to center!

	G4double chamberWidth = 20.0*cm; // width of the chambers
	G4double targetLength =  10.0*cm; // full length of Target
  
	G4double trackerLength = (fNbOfChambers+1)*chamberSpacing;

	G4double worldLength = 1.2 * (2*targetLength + trackerLength);

	G4double targetRadius  = 1.0*targetLength;   // Radius of Target
	targetLength = 0.5*targetLength;             // Half length of the Target  
	G4double trackerSize   = 0.5*trackerLength;  // Half length of the Tracker

	// Definitions of Solids, Logical Volumes, Physical Volumes

	// --------- World ---------

	G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldLength);

	G4cout << "Computed tolerance = "
		   << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance()/mm
		   << " mm" << G4endl;

	G4Box* worldS
		= new G4Box("world",                                    //its name
					worldLength/2,worldLength/2,worldLength/2); //its size
	G4LogicalVolume* worldLV
		= new G4LogicalVolume(
							  worldS,   //its solid
							  air,      //its material
							  "World"); //its name
  
	//  Must place the World Physical volume unrotated at (0,0,0).
	// 
	G4VPhysicalVolume* worldPV
		= new G4PVPlacement(
							0,               // no rotation
							G4ThreeVector(), // at (0,0,0)
							worldLV,         // its logical volume
							"World",         // its name
							0,               // its mother  volume
							false,           // no boolean operations
							0,               // copy number
							fCheckOverlaps); // checking overlaps 



	// // --------- Target ---------
  
	// G4ThreeVector positionTarget = G4ThreeVector(0,0,-2*m); //(targetLength+trackerSize));

	// G4Tubs* targetS
	// 	= new G4Tubs("target",0.,targetRadius,targetLength,0.*deg,360.*deg);
	// fLogicTarget
	// 	= new G4LogicalVolume(targetS, fTargetMaterial,"Target",0,0,0);
	// new G4PVPlacement(0,               // no rotation
	// 				  positionTarget,  // at (x,y,z)
	// 				  fLogicTarget,    // its logical volume
	// 				  "Target",        // its name
	// 				  worldLV,         // its mother volume
	// 				  false,           // no boolean operations
	// 				  0,               // copy number
	// 				  fCheckOverlaps); // checking overlaps 

	// G4cout << "Target is " << 2*targetLength/cm << " cm of "
	// 	   << fTargetMaterial->GetName() << G4endl;



	// // --------- Tracker ---------
 
	// G4ThreeVector positionTracker = G4ThreeVector(0,0,0);

	// G4Tubs* trackerS
	// 	= new G4Tubs("tracker",0,trackerSize,trackerSize, 0.*deg, 360.*deg);
	// G4LogicalVolume* trackerLV
	// 	= new G4LogicalVolume(trackerS, 
	// 						  //air, 
	// 						  hpgeMater, // fill trackers with Ge
	// 						  "Tracker",0,0,0);  
	// new G4PVPlacement(0,               // no rotation
	// 				  positionTracker, // at (x,y,z)
	// 				  trackerLV,       // its logical volume
	// 				  "Tracker",       // its name
	// 				  worldLV,         // its mother  volume
	// 				  false,           // no boolean operations
	// 				  0,               // copy number
	// 				  fCheckOverlaps); // checking overlaps 


	// for UKAL_sim
	// --------- Scattering Sample ---------
	G4double sampleRadius = 9*mm; 
	G4double sampleDz     = 21*mm; 
	// solid
	solidUKAL_simSample = new G4Tubs("solidUKAL_simSample", 0, sampleRadius, sampleDz, 0, 360*degree); 
	// logical 
	logicUKAL_simSample = new G4LogicalVolume(solidUKAL_simSample, 
										  sampleMater, 
										  //hpgeMater,  
										  //fTargetMaterial,
										  //fChamberMaterial,  
										  "logicUKAL_simSample"); 
	// physical
	// placement, see below CeBr3 for refernece, use a G4ThreeVector for xyz position as a whole
	fUKAL_simSample_zpos = -0.5*sampleDz; 
	fUKAL_simSamplePos = G4ThreeVector(0, 0); 
	G4RotationMatrix *rotateUKAL_simSample = new G4RotationMatrix(); 
	rotateUKAL_simSample->rotateX(90.*degree); 
	if(fUseUKAL_simSample) {
		physiUKAL_simSample = new G4PVPlacement(rotateUKAL_simSample, 
											fUKAL_simSamplePos, 
											logicUKAL_simSample, "UKAL_simSample", 
											worldLV, 
											false, 0, true); 
	}
	// --------- HPGe ---------
	// Yongchi - Create the HPGe Detector
	G4double hpgeRadius = 31.5*mm; // 31.5mm
	G4double hpgeDz     = 80*mm; 
	// solid
	solidUKAL_simHPGe = new G4Tubs("solidUKAL_simHPGe", 0, hpgeRadius, hpgeDz, 0, 360*degree); 
	// logical
	logicUKAL_simHPGe = new G4LogicalVolume(solidUKAL_simHPGe, 
										hpgeMater, 
										"logicUKAL_simHPGe"); 
	// physical - placement
	// fUKAL_simHPGe_zpos = 0.5*hpgeDz; 
	if(fUseUKAL_simHPGe) {
		fUKAL_simHPGePos = G4ThreeVector(hpgePosRadius*sin(hpgePhi), 0, hpgePosRadius*cos(hpgePhi)); //-1*fUKAL_simHPGe_zpos - 60*mm, 0, 0); 
		G4RotationMatrix *rotateUKAL_simHPGe = new G4RotationMatrix(); 
		rotateUKAL_simHPGe->rotateY(180*degree-hpgePhi); 	

		physiUKAL_simHPGe = new G4PVPlacement(rotateUKAL_simHPGe, fUKAL_simHPGePos, logicUKAL_simHPGe, "UKAL_simHPGe", 
										  worldLV, 
										  false, 0, true); 
	}










	// Visualization attributes

	G4VisAttributes* boxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
	// G4VisAttributes* chamberVisAtt = new G4VisAttributes(G4Colour(1.0,1.0,0.0));

	worldLV      ->SetVisAttributes(boxVisAtt);
	// fLogicTarget ->SetVisAttributes(boxVisAtt);
	// trackerLV    ->SetVisAttributes(boxVisAtt);

	// // Tracker segments

	// G4cout << "There are " << fNbOfChambers << " chambers in the tracker region. "
	// 	   << G4endl
	// 	   << "The chambers are " << chamberWidth/cm << " cm of "
	// 	   << fChamberMaterial->GetName() << G4endl
	// 	   << "The distance between chamber is " << chamberSpacing/cm << " cm" 
	// 	   << G4endl;
  
	// G4double firstPosition = -trackerSize + chamberSpacing;
	// G4double firstLength   = trackerLength/10;
	// G4double lastLength    = trackerLength;

	// G4double halfWidth = 0.5*chamberWidth;
	// G4double rmaxFirst = 0.5 * firstLength;

	// G4double rmaxIncr = 0.0;
	// if( fNbOfChambers > 0 ){
	// 	rmaxIncr =  0.5 * (lastLength-firstLength)/(fNbOfChambers-1);
	// 	if (chamberSpacing  < chamberWidth) {
	// 		G4Exception("UKAL_simDetectorConstruction::DefineVolumes()",
	// 					"InvalidSetup", FatalException,
	// 					"Width>Spacing");
	// 	}
	// }

	// for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {

	// 	G4double Zposition = firstPosition + copyNo * chamberSpacing;
	// 	G4double rmax =  rmaxFirst + copyNo * rmaxIncr;

	// 	G4Tubs* chamberS
	// 		= new G4Tubs("Chamber_solid", 0, rmax, halfWidth, 0.*deg, 360.*deg);

	// 	fLogicChamber[copyNo] =
	// 		new G4LogicalVolume(chamberS,
	// 						    fChamberMaterial,
	// 							"Chamber_LV",0,0,0);

	// 	fLogicChamber[copyNo]->SetVisAttributes(chamberVisAtt);

	// 	new G4PVPlacement(0,                            // no rotation
	// 					  G4ThreeVector(0,0,Zposition), // at (x,y,z)
	// 					  fLogicChamber[copyNo],        // its logical volume
	// 					  "Chamber_PV",                 // its name
	// 					  trackerLV,                    // its mother  volume
	// 					  false,                        // no boolean operations
	// 					  copyNo,                       // copy number
	// 					  fCheckOverlaps);              // checking overlaps 

	// }

	// Example of User Limits
	//
	// Below is an example of how to set tracking constraints in a given
	// logical volume
	//
	// Sets a max step length in the tracker region, with G4StepLimiter

	G4double maxStep = 0.5*chamberWidth;
	fStepLimit = new G4UserLimits(maxStep);
	// trackerLV->SetUserLimits(fStepLimit);
 
	/// Set additional contraints on the track, with G4UserSpecialCuts
	///
	/// G4double maxLength = 2*trackerLength, maxTime = 0.1*ns, minEkin = 10*MeV;
	/// trackerLV->SetUserLimits(new G4UserLimits(maxStep,
	///                                           maxLength,
	///                                           maxTime,
	///                                           minEkin));

	// Always return the physical world

	return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
void UKAL_simDetectorConstruction::ConstructSDandField()
{
	// Sensitive detectors

	// G4String trackerChamberSDname = "B2/TrackerChamberSD";
	// UKAL_simTrackerSD* aTrackerSD = new UKAL_simTrackerSD(trackerChamberSDname,
	// 										  "TrackerHitsCollection");
	// G4SDManager::GetSDMpointer()->AddNewDetector(aTrackerSD);
	// // Setting aTrackerSD to all logical volumes with the same name 
	// // of "Chamber_LV".
	// SetSensitiveDetector("Chamber_LV", aTrackerSD, true);


	// by Yongchi - do the same thing as above for 
	// scattering samples and hpge detectors
	G4String ukalSampleSDname = "UKAL_sim/ScatteringSampleSD"; 
	UKAL_simTrackerSD* aSampleSD = new UKAL_simTrackerSD(ukalSampleSDname, "ukalSampleHitsCollection"); 
	G4SDManager::GetSDMpointer()->AddNewDetector(aSampleSD); 
	SetSensitiveDetector("logicUKAL_simSample", aSampleSD, true); 
	// 
	G4String ukalHPGeSDname = "UKAL_sim/HPGeSD"; 
	UKAL_simTrackerSD* aHPGeSD = new UKAL_simTrackerSD(ukalHPGeSDname, "ukalHPGeHitsCollection"); 
	G4SDManager::GetSDMpointer()->AddNewDetector(aHPGeSD); 
	SetSensitiveDetector("logicUKAL_simHPGe", aHPGeSD, true); 			







	// // Create global magnetic field messenger.
	// // Uniform magnetic field is then created automatically if
	// // the field value is not zero.
	// G4ThreeVector fieldValue = G4ThreeVector();
	// fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
	// fMagFieldMessenger->SetVerboseLevel(1);
  
	// // Register the field messenger for deleting
	// G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
// void UKAL_simDetectorConstruction::SetTargetMaterial(G4String materialName)
// {
// 	G4NistManager* nistManager = G4NistManager::Instance();

// 	G4Material* pttoMaterial = 
// 		nistManager->FindOrBuildMaterial(materialName);

// 	if (fTargetMaterial != pttoMaterial) {
// 		if ( pttoMaterial ) {
// 			fTargetMaterial = pttoMaterial;
// 			if (fLogicTarget) fLogicTarget->SetMaterial(fTargetMaterial);
// 			G4cout 
// 				<< G4endl 
// 				<< "----> The target is made of " << materialName << G4endl;
// 		} else {
// 			G4cout 
// 				<< G4endl 
// 				<< "-->  WARNING from SetTargetMaterial : "
// 				<< materialName << " not found" << G4endl;
// 		}
// 	}
// }
 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

// void UKAL_simDetectorConstruction::SetChamberMaterial(G4String materialName)
// {
// 	G4NistManager* nistManager = G4NistManager::Instance();

// 	G4Material* pttoMaterial =
// 		nistManager->FindOrBuildMaterial(materialName);

// 	if (fChamberMaterial != pttoMaterial) {
// 		if ( pttoMaterial ) {
// 			fChamberMaterial = pttoMaterial;
// 			for (G4int copyNo=0; copyNo<fNbOfChambers; copyNo++) {
// 				if (fLogicChamber[copyNo]) fLogicChamber[copyNo]->
//                                                SetMaterial(fChamberMaterial);
// 			}
// 			G4cout 
// 				<< G4endl 
// 				<< "----> The chambers are made of " << materialName << G4endl;
// 		} else {
// 			G4cout 
// 				<< G4endl 
// 				<< "-->  WARNING from SetChamberMaterial : "
// 				<< materialName << " not found" << G4endl;
// 		}
// 	}
// }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UKAL_simDetectorConstruction::SetMaxStep(G4double maxStep)
{
	if ((fStepLimit)&&(maxStep>0.)) fStepLimit->SetMaxAllowedStep(maxStep);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void UKAL_simDetectorConstruction::SetCheckOverlaps(G4bool checkOverlaps)
{
	fCheckOverlaps = checkOverlaps;
}  
