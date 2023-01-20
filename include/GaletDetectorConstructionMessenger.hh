// ----------------------------------------------------------------------------
//
//
//    GaletDetectorConstructionMessenger
//
//  (Description)
//    2015 T.ASO 
// ----------------------------------------------------------------------------

#ifndef GaletDetectorConstructionMessenger_HH
#define GaletDetectorConstructionMessenger_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

class GaletDetectorConstruction;
class G4UIdirectory;
class G4UIcmdWithoutParameter;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWithAString;
class G4UIcommand;

class GaletDetectorConstructionMessenger: public G4UImessenger {

public:
  
  GaletDetectorConstructionMessenger(GaletDetectorConstruction* det);
  ~GaletDetectorConstructionMessenger() override;
  
  void SetNewValue(G4UIcommand*, G4String) override;
  
private:
  
  GaletDetectorConstruction*     fDet = nullptr;   
  G4UIdirectory*      listDir = nullptr;
};

#endif








