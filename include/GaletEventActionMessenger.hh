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
//
//---------------------------------------------------------------------
//
#ifndef GaletEventActionMessenger_HH
#define GaletEventActionMessenger_HH

#include "globals.hh"
#include "G4UImessenger.hh"
#include "G4UIcommand.hh"
#include "G4UIdirectory.hh"
#include "G4UIcmdWithAString.hh"
#include "G4UIcmdWithABool.hh"

class GaletEventAction;

//=============================================================
//  Created by ASO_LAB
//=============================================================

class GaletEventActionMessenger : public G4UImessenger{
  public:

    GaletEventActionMessenger(GaletEventAction* eventAction);
    ~GaletEventActionMessenger() override;

    void SetNewValue(G4UIcommand* command, G4String newValue) override;
    G4String GetCurrentValue(G4UIcommand* command) override;

  private:
  G4UIdirectory* fDir = nullptr;

  GaletEventAction* fEventAction = nullptr;
};
#endif
