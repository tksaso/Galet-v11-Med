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
//
// $Id: G4MProcessType.hh,v 1.4 2001/07/11 10:08:17 gunter Exp $
// GEANT4 tag $Name: geant4-07-01 $
//
//
//---------------------------------------------------------------
//
// G4MProcessType.hh
//
// Class Description:
//   This is an enumerator to define process type.
//
//   T.ASO  DEC-01 2005 Created.
//   2013-10-17 T.Aso Add process type for photons and GenelicIons.
//
//
//
//---------------------------------------------------------------

#ifndef G4MProcessType_h
#define G4MProcessType_h 1

enum G4MProcessType
{
  // 0 
  fMUndefined,
  // 1 - 5
  fMPrimary,
  fMQED,
  fMHadron_Elastic,
  fMHadron_Inelastic,
  fMDecay,
  // 6 - 8
  fMPhoto,
  fMCompt,
  fMPair,
  // 9 - 12
  fMIonQED,
  fMIonStopping,
  fMIon_Inelastic,
  fMRadioactiveDecay
};
#endif


