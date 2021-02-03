#ifndef UKAL_ANALYSIS_MESSENGER_HH
#define UKAL_ANALYSIS_MESSENGER_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

class UKALAnalysisManager; 
class G4UIdirectory;
class G4UIcmdWithAString;
class G4UIcmdWithAnInteger;

class UKALAnalysisMessenger : public G4UImessenger {
    public: 
        UKALAnalysisMessenger(UKALAnalysisManager*); 
        ~UKALAnalysisMessenger(); 
        void SetNewValue(G4UIcommand*, G4String); 
    
    private: 
        UKALAnalysisManager* analysisManager; 
        G4UIdirectory*       analysisDir;

        G4UIcmdWithAString*  filenameCmd; 
        G4UIcmdWithAString*  dirnameCmd; 

}; 





#endif