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
// $Id: GaletTrackingAction.hh,v 1.1 2006/08/22 08:09:37 aso Exp $
// GEANT4 tag $Name:  $
//
//====================================================================
// (Class) GaletTrackingAction
//
//  Assign a track information relevant to the process.
//  T.Aso
//
//  11-MAR-07  T.Aso suppress output for debug.
//  06-MAR-09  T.Aso Modify code for removing string operation.
//  2013-10-17 T.Aso Add the process identification for photons/ions.
//  2016-04-07 T.Aso Add a flag for the inheritence of trkinfo.
//
//====================================================================


#ifndef GaletTrackingAction_h
#define GaletTrackingAction_h 1

#include "globals.hh"
#include "G4UserTrackingAction.hh"

class GaletTrackingAction : public G4UserTrackingAction {

  public:
    GaletTrackingAction(const G4String& name="GaletTrackingAction");
    ~GaletTrackingAction() override{};
   
    void PreUserTrackingAction(const G4Track*) override;
    void PostUserTrackingAction(const G4Track*) override;

  private:
    G4String myname;

};

#endif
