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
// $Id: GaletTrackingAction.cc,v 1.7 2007/07/05 08:10:24 aso Exp $
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
// 2014-03-18 T.Aso Add process names which are changed in G4.10.0. 
//  2016-04-07 T.Aso Add a flag for the inheritence of trkinfo.
//====================================================================

#include "GaletTrackingAction.hh"
#include "GaletTrackInformation.hh"

#include "G4TrackingManager.hh"
#include "G4Track.hh"

GaletTrackingAction::GaletTrackingAction(const G4String& name)
  :G4UserTrackingAction(),myname(name){
}

void GaletTrackingAction::PreUserTrackingAction(const G4Track* aTrack)
{
  // If the track is primary particle.
  // Define TrackInformation here.
  // Otherwise, for secondary, Define at PostUserTrackingAction().
  //
  if(aTrack->GetUserInformation()==0){ // Primary particle
    GaletTrackInformation* trackInfo = new GaletTrackInformation();
    // Attach to the track.
    G4Track*  theTrack = (G4Track*)aTrack;
    theTrack->SetUserInformation(trackInfo);
  }
}

void GaletTrackingAction::PostUserTrackingAction(const G4Track* aTrack)
{
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(secondaries)
  {
    size_t nSeco = secondaries->size();
    if(nSeco>0)
    {
      for(size_t i=0;i<nSeco;i++)
      { 
        //
        // If This particle is not directry created from Primary particle,
        // let it copy the track information of parent's track information.
        //
        GaletTrackInformation* infoNew =  new GaletTrackInformation();
	//
        infoNew->SetParentPID(aTrack->GetDefinition()->GetPDGEncoding());
        infoNew->SetParentTime(aTrack->GetGlobalTime() - aTrack->GetLocalTime());
        infoNew->SetParentVtx(aTrack->GetVertexPosition()); // created position
	//
	GaletTrackInformation* infoOld = (GaletTrackInformation*)(aTrack->GetUserInformation());
	if ( infoOld ){  // Copy trigger bit and last position at the trigger volume.
	  infoNew->SetTriggerBit(infoOld->GetTriggerBit());
	  infoNew->SetTrgPosIn(infoOld->GetTrgPosIn());
	  infoNew->SetTrgPosOut(infoOld->GetTrgPosOut());
	}
        // Set.
        (*secondaries)[i]->SetUserInformation(infoNew);
      }
    }
  }
}
