// ----------------------------------------------------------------------------
//
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
//    2014-01-08 T.Aso Reduce parameters in pwProcess command.
//    2014-01-08 T.Aso Remove parallel scoring process.
//    2017-02-15 T.Aso Verbose command.
//
// ----------------------------------------------------------------------------
#include "MACPhysicsListMessenger.hh"
#include "MACPhysicsList.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithABool.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"
#include "G4UIcommand.hh"

MACPhysicsListMessenger::MACPhysicsListMessenger(MACPhysicsList * physList)
:physicsList(physList)
{  
 listDir = new G4UIdirectory("/My/physics/");

  // Building modular PhysicsList

 physicsListCmd = new G4UIcmdWithAString("/My/physics/register",this);  
 physicsListCmd->SetGuidance("Register process of PhysicsList.");
 physicsListCmd->SetParameterName("processList",false);
 physicsListCmd->AvailableForStates(G4State_PreInit);

 G4UIparameter* param = 0; 
 stepCmd = new G4UIcommand("/My/physics/stepLimitForRegion",this);  
 stepCmd->SetGuidance("Set Step Limit for a region");
 param = new G4UIparameter("Region",'s',false);
 stepCmd->SetParameter(param);
 param = new G4UIparameter("step",'d',false);
 param->SetParameterRange("step > 0.0");
 stepCmd->SetParameter(param);
 param = new G4UIparameter("Unit",'s',true);
 param->SetDefaultValue("mm");
 param->SetParameterCandidates(stepCmd->UnitsList(stepCmd->CategoryOf("mm")));
 stepCmd->SetParameter(param);
 stepCmd->AvailableForStates(G4State_Idle);

 pWorldProcCmd = new G4UIcommand("/My/physics/pwProcess",this);
 pWorldProcCmd->SetGuidance("ParallelWorldProcess"); 
 param->SetGuidance("ParallelWorld name: The name is applied as the process name");
 param->SetGuidance("True: Layered material,  False: materials are ingored");
 param = new G4UIparameter("ParallelWorldName",'s',false);
 pWorldProcCmd->SetParameter(param);
 param = new G4UIparameter("layered",'b',false);
 pWorldProcCmd->SetParameter(param);
 pWorldProcCmd->AvailableForStates(G4State_PreInit);

 verboseCmd = new G4UIcmdWithAnInteger("/My/physics/verbose",this);  
 verboseCmd->SetGuidance("Set verbose in PhysicsConstructors.");
 verboseCmd->SetParameterName("verbose",false);
 verboseCmd->AvailableForStates(G4State_PreInit);
}

MACPhysicsListMessenger::~MACPhysicsListMessenger()
{
  delete pWorldProcCmd;
  delete physicsListCmd;
  delete stepCmd;
  delete verboseCmd;
  delete listDir;
}

void MACPhysicsListMessenger::SetNewValue(G4UIcommand* command,G4String newValue)
{

 if (command == pWorldProcCmd){
   G4String s[2];
   G4String namePW;
   G4bool layered;
   std::istringstream is(newValue);
   //G4cout << newValue << G4endl;
   is >> s[0] >> s[1] ;
   namePW = s[0];
   layered  = pWorldProcCmd->ConvertToBool(s[1]);
   physicsList->RegisterParallelWorldModule(namePW,layered);

 }else if (command == physicsListCmd){ 
   physicsList->RegisterPhysicsModule(newValue);
 }else if ( command == stepCmd ) { 
   std::istringstream is(newValue);
   char regName[50];
   G4double cVal;
   char uniName[10];
   is >> regName >> cVal >> uniName;
   G4String regN = regName;
   G4String uniN = uniName;
   physicsList->SetStepLimitForRegion(cVal*(stepCmd->ValueOf(uniN)),regN); 
 }else if (  command == verboseCmd ){
   physicsList->SetVerboseLevel(verboseCmd->GetNewIntValue(newValue));
 }

}
