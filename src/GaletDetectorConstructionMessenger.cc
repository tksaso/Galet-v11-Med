// ----------------------------------------------------------------------------
//
//
//    GaletDetectorConstructionMessenger
//
//  (Description)
//    2015 T.ASO 
//
// ----------------------------------------------------------------------------
#include "GaletDetectorConstructionMessenger.hh"
#include "GaletDetectorConstruction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"

GaletDetectorConstructionMessenger::
GaletDetectorConstructionMessenger(GaletDetectorConstruction * det)
:fDet(det)
{  
 listDir = new G4UIdirectory("/Galet/det/");
 fDet->Dummy();
}

GaletDetectorConstructionMessenger::~GaletDetectorConstructionMessenger()
{
  delete listDir;
}

void GaletDetectorConstructionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{
  G4cout << command->GetCommandName()<<" " <<newValue<<G4endl;
}
