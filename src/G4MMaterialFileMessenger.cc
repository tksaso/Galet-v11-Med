//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//---------------------------------------------------------------------
//
// MaterialFileMessenger.hh
//
// [Description]
//   UIcommand for Material construction from File
//
// [Histoy]
//   30 Aug, 2006, T.Aso and T.Kawaguchi
//   2013-07-03 T.Aso Add property command.
//   2014-01-09 T.Aso SystemOfUnits.
//
//---------------------------------------------------------------------
//
#include "G4MMaterialFileMessenger.hh"
#include "G4MMaterialFileConstruction.hh"
#include "G4SystemOfUnits.hh"

#include "G4Material.hh"

G4MMaterialFileMessenger::
G4MMaterialFileMessenger(G4MMaterialFileConstruction* mConst)
  :mConstruction(mConst) {

  fDirMat = new G4UIdirectory("/G4M/Material/");
  fDirMat->SetGuidance("UI commands for materials");

  fDirElm = new G4UIdirectory("/G4M/Element/");
  fDirElm->SetGuidance("UI commands for element");

  elementPathCmd = new G4UIcmdWithAString("/G4M/Element/path",this);
  materialPathCmd = new G4UIcmdWithAString("/G4M/Material/path",this);

  matCmd = new G4UIcmdWithAString("/G4M/Material/create",this);

  isUseG4ElmCmd = new G4UIcmdWithABool("/G4M/Element/useG4Element",this);

  matPropertyCmd = new G4UIcmdWithAString("/G4M/Material/property",this);

}

G4MMaterialFileMessenger::~G4MMaterialFileMessenger() {
  delete elementPathCmd;
  delete materialPathCmd;
  delete matCmd;
  delete isUseG4ElmCmd;
  delete matPropertyCmd;
  delete fDirMat;
}

void G4MMaterialFileMessenger::SetNewValue(G4UIcommand* command, G4String newValue) {
  if(command == elementPathCmd){
    mConstruction->SetElementPath(newValue);
  }else if(command == materialPathCmd){
    mConstruction->SetMaterialPath(newValue);
  }else if(command == matCmd){
    mConstruction->RegisterMaterial(newValue);
  }else if(command == isUseG4ElmCmd){
    mConstruction->UseG4Element(isUseG4ElmCmd->GetNewBoolValue(newValue));
  }else if(command == matPropertyCmd){
    G4Material* mat = G4Material::GetMaterial(newValue);
    G4cout <<"== Material : "<< newValue <<G4endl;
    G4cout <<" TotNbOfElectPerVolume (/cm3): "
           <<mat->GetTotNbOfElectPerVolume()/cm3
           << G4endl;
  }
}

G4String G4MMaterialFileMessenger::GetCurrentValue(G4UIcommand*) {
  return G4String("I do not know,");
}

