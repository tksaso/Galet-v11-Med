// ----------------------------------------------------------------------------
//
//
//    GaletRunActionMessenger
//
//  (Description)
//    2015 T.ASO 
// ----------------------------------------------------------------------------

#ifndef GaletRunActionMessenger_HH
#define GaletRunActionMessenger_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GaletRunAction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcommand;

class GaletRunActionMessenger: public G4UImessenger {

public:
  
  GaletRunActionMessenger(GaletRunAction* runAction);
  ~GaletRunActionMessenger() override;
  
  void SetNewValue(G4UIcommand*, G4String) override;
  
private:
  
  GaletRunAction*     fRunAction = nullptr;   
  G4UIdirectory*      listDir = nullptr;
  G4UIcmdWithAString* fileCmd = nullptr;
};

#endif








