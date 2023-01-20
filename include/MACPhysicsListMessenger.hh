//
//
//    MACPhysicsListMessenger
//
//  (Description)
//    Register PhysicsList
//
//    28, OCT, 2005 T.ASO 
//    21. AUG. 2006 T.ASO  Added command for energy fluctuation of proton.
//                         Added command for step limitation for region.
//    2013-03-27 T.Aso Add commands for processes in parallel world geometries.
//    2017-03-15 T.Aso Add verbose command
//
// ----------------------------------------------------------------------------

#ifndef MACPHYSICSLISTMESSENGER_HH
#define MACPHYSICSLISTMESSENGER_HH 1

#include "globals.hh"
#include "G4UImessenger.hh"

class MACPhysicsList;
class G4UIdirectory;
class G4UIcmdWithADoubleAndUnit;
class G4UIcmdWithABool;
class G4UIcmdWithAnInteger;
class G4UIcmdWithAString;
class G4UIcommand;

class MACPhysicsListMessenger: public G4UImessenger {

public:
  MACPhysicsListMessenger(MACPhysicsList* physList);
  
  ~MACPhysicsListMessenger() override;
  
  void SetNewValue(G4UIcommand*, G4String) override;
  
private:
  
  MACPhysicsList*     physicsList = nullptr;   
  G4UIdirectory*      listDir = nullptr;
  G4UIcmdWithAString* physicsListCmd = nullptr;
  G4UIcommand*        stepCmd = nullptr;
  G4UIcommand*        pWorldProcCmd = nullptr;
  G4UIcmdWithAnInteger* verboseCmd = nullptr;
};

#endif








