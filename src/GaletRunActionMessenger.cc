// ----------------------------------------------------------------------------
//
//
//    GaletRunActionMessenger
//
//  (Description)
//    2015 T.ASO 
//
// ----------------------------------------------------------------------------
#include "GaletRunActionMessenger.hh"
#include "GaletRunAction.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithoutParameter.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"

GaletRunActionMessenger::
GaletRunActionMessenger(GaletRunAction * runAction)
:fRunAction(runAction)
{  
 listDir = new G4UIdirectory("/Galet/runAction/");
 
 fileCmd = new G4UIcmdWithAString("/Galet/runAction/outFile",this);
 fileCmd->SetGuidance("Output file name");
 fileCmd->SetParameterName("outFile",false);
 fileCmd->AvailableForStates(G4State_Idle);

}

GaletRunActionMessenger::~GaletRunActionMessenger()
{
  delete fileCmd;
  delete listDir;
}

void GaletRunActionMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{

 if (command == fileCmd){
   fRunAction->SetOutputFile(newValue);
 }

}
