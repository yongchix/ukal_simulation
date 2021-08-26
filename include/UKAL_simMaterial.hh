#ifndef UKAL_sim_MATERIAL_HH
#define UKAL_sim_MATERIAL_HH
////////////////////////////////////////////////////////////////////////////////
#include "G4Material.hh"
#include "G4NistManager.hh"
#include <vector>
    
class UKAL_simMaterial
{
public:
	UKAL_simMaterial ();
	~UKAL_simMaterial ();  
    
public: 
    G4Material* GetMaterial (G4String name) {return G4Material::GetMaterial(name);};
    // void ListMaterial(); 

private: 
    // by Yongchi - for UKAL_sim
    G4Isotope *isotopeXe136; 
    G4Element *elementXe136; 

    G4Isotope *isotopeF19; 
    G4Element *elementF19; 
    
    // materials
    G4Material *Ge; 
    G4Material *XeF2; 
}; 

#endif