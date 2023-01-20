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
// GaletEventActionMessenger.hh
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
#include "GaletEventActionMessenger.hh"
#include "GaletEventAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4Tokenizer.hh"

GaletEventActionMessenger::
GaletEventActionMessenger(GaletEventAction* eventAction)
  :fEventAction(eventAction) {

  fDir = new G4UIdirectory("/My/eventaction/");
  fDir->SetGuidance("UI commands for eventAction");
  //dummy
  fEventAction->GetHCID();
  
}

GaletEventActionMessenger::~GaletEventActionMessenger() {
  delete fDir;
}

void GaletEventActionMessenger::SetNewValue(G4UIcommand* , G4String) {
}

G4String GaletEventActionMessenger::GetCurrentValue(G4UIcommand*) {
  return G4String("I do not know,");
}

