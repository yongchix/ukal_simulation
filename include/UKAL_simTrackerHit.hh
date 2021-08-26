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
/// \file UKAL_simTrackerHit.hh
/// \brief Definition of the UKAL_simTrackerHit class

#ifndef UKAL_simTrackerHit_h
#define UKAL_simTrackerHit_h 1

#include "G4VHit.hh"
#include "G4THitsCollection.hh"
#include "G4Allocator.hh"
#include "G4ThreeVector.hh"
#include "tls.hh"

/// Tracker hit class
///
/// It defines data members to store the trackID, chamberNb, energy deposit,
/// and position of charged particles in a selected volume:
/// - fTrackID, fChamberNB, fEdep, fPos

class UKAL_simTrackerHit : public G4VHit
{
public:
    UKAL_simTrackerHit();
    UKAL_simTrackerHit(const UKAL_simTrackerHit&);
    virtual ~UKAL_simTrackerHit();

    // operators
    const UKAL_simTrackerHit& operator=(const UKAL_simTrackerHit&);
    G4bool operator==(const UKAL_simTrackerHit&) const;

    inline void* operator new(size_t);
    inline void  operator delete(void*);

    // methods from base class
    virtual void Draw();
    virtual void Print();

    // Set methods
    void SetTrackID  (G4int track)      { fTrackID = track; };
    void SetChamberNb(G4int chamb)      { fChamberNb = chamb; };
    void SetEdep     (G4double de)      { fEdep = de; };
    void SetPos      (G4ThreeVector xyz){ fPos = xyz; };

    // Get methods
    G4int GetTrackID() const     { return fTrackID; };
    G4int GetChamberNb() const   { return fChamberNb; };
    G4double GetEdep() const     { return fEdep; };
    G4ThreeVector GetPos() const { return fPos; };

	// by Yongchi - refer to e16032 simulation
	inline void AddEdep(G4double de) { edep += de; }
    inline G4double GetEdep() { return edep; }
	inline G4ThreeVector GetPos() { return pos; }
    inline void SetGtime(G4double gt) {gtime = gt;}
	inline void SetKineticEnergy(G4double ke) {kineticenergy = ke;}
    inline G4double GetGtime() {return gtime;}
	inline void SetStepno(G4int st) {stepno = st;}
    inline G4int GetStepno() {return stepno;}
	inline void SetParentno(G4int pa) {parentno = pa;}
	inline G4int GetParentno() {return parentno;}
	inline void SetParticletype(G4String ty) {particletype = ty;}
	inline G4String GetParticletype() {return particletype;}
	inline void SetProcessname(G4String pr) {processname = pr;}
	inline G4String GetProcessname() {return processname;}
	inline void SetParticlename(G4String nam) {particlename = nam;}
	inline G4String GetParticlename() {return particlename;}
	inline void SetParentName(G4String nam) {parentname = nam;}
	inline G4String GetParentName() {return parentname;}
	inline void SetParentEnergy(G4double paren) {parentenergy = paren;}
	inline G4double GetParentEnergy() {return parentenergy;}
	inline G4double GetKineticEnergy() {return kineticenergy;}
	inline void SetParentMoDir(G4ThreeVector parmodir) {parentmodir = parmodir;}
	inline G4ThreeVector GetParentMoDir() {return parentmodir;}
	
	inline void SetVolName(G4String nam) {volname = nam;}
	inline G4String GetVolName() {return volname;}
	inline void SetVolCopyNo(G4int num) {volcopyno = num;}
	inline G4int GetVolCopyNo() {return volcopyno;}
	inline void SetTrackno(G4int tr) {trackno = tr;}
	inline G4int GetTrackno() {return trackno;}
	inline void SetPrePosition(G4ThreeVector pre) {prepos = pre;};
	inline G4ThreeVector GetPrePosition() {return prepos;};
	inline void SetPostPosition(G4ThreeVector post) {postpos = post;};
	inline G4ThreeVector GetPostPosition() {return postpos;};
	inline void SetDeltaPosition(G4ThreeVector del) {deltapos = del;};
	inline G4ThreeVector GetDeltaPosition() {return deltapos;};

	inline void SetStepLength(G4double slen) {stepleng = slen;};
	inline G4double GetStepLength() {return stepleng;};

	inline void SetDeltaLength(G4double dlen) {fulldis = dlen;};
	inline G4double GetDeltaLength() {return fulldis;};



	

private:

	G4int         fTrackID;
	G4int         fChamberNb;
	G4double      fEdep;
	G4ThreeVector fPos;

private: 
    // by Yongchi - for UKAL_sim
    G4double edep;
    G4ThreeVector pos;
	G4double gtime;
	G4int stepno;
	G4int parentno;
	G4int trackno;
	G4String particletype;
	G4String particlename;
	G4String parentname;
	G4String processname;
	G4double kineticenergy;
	G4String volname;
	G4int volcopyno;
	G4ThreeVector prepos;
	G4ThreeVector postpos;
	G4ThreeVector deltapos;
	G4double fulldis;
	G4double parentenergy;
	G4ThreeVector parentmodir;
	G4double stepleng;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

typedef G4THitsCollection<UKAL_simTrackerHit> UKAL_simTrackerHitsCollection;

extern G4ThreadLocal G4Allocator<UKAL_simTrackerHit>* UKAL_simTrackerHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void* UKAL_simTrackerHit::operator new(size_t)
{
	if(!UKAL_simTrackerHitAllocator)
		UKAL_simTrackerHitAllocator = new G4Allocator<UKAL_simTrackerHit>;
	return (void *) UKAL_simTrackerHitAllocator->MallocSingle();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

inline void UKAL_simTrackerHit::operator delete(void *hit)
{
	UKAL_simTrackerHitAllocator->FreeSingle((UKAL_simTrackerHit*) hit);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
