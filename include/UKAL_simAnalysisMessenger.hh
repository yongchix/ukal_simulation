#ifndef UKAL_sim_ANALYSIS_MESSENGER_HH
#define UKAL_sim_ANALYSIS_MESSENGER_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

class UKAL_simAnalysisManager; 
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class UKAL_simAnalysisMessenger : public G4UImessenger {
    public: 
        UKAL_simAnalysisMessenger(UKAL_simAnalysisManager*); 
        ~UKAL_simAnalysisMessenger(); 
        void SetNewValue(G4UIcommand*, G4String); 
    
    private: 
        UKAL_simAnalysisManager* analysisManager; 
        G4UIdirectory*       analysisDir;

        G4UIcmdWithAString*  filenameCmd; 
        G4UIcmdWithAString*  dirnameCmd; 

}; 





#endif