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
// $Id: GaletTrackInformation.cc,v 1.1 2004/11/26 07:37:42 asaim Exp $
// GEANT4 tag $Name: geant4-07-01 $
//=====================================================================
//  
//  (Description)
//     Sub class of G4VUserTrackingAction class.
//  2016-03-11 TriggerBit, ParentPID
//  2016-04-12 Trigger Position
//  2017-03-15 Threading.
//=====================================================================
#include "GaletTrackInformation.hh"
#include "G4ios.hh"

G4ThreadLocal G4Allocator<GaletTrackInformation>* aTrackInformationAllocator=0;

GaletTrackInformation::GaletTrackInformation()
{
}

GaletTrackInformation::GaletTrackInformation(const GaletTrackInformation* aTrackInfo)
{
    origProcessType = aTrackInfo->origProcessType;
    fParentPID = aTrackInfo->fParentPID;
    fIsROI = FALSE;
    fPosIn = aTrackInfo->fPosIn;
    fPosOut = aTrackInfo->fPosOut;
    fTriggerBit = aTrackInfo->fTriggerBit;
    //fInherit = aTrackInfo->fInherit;
}

GaletTrackInformation::~GaletTrackInformation()
{;}

GaletTrackInformation& GaletTrackInformation::operator =(const GaletTrackInformation& aTrackInfo)
{
    origProcessType = aTrackInfo.origProcessType;
    fIsROI = aTrackInfo.fIsROI;
    fParentPID = aTrackInfo.fParentPID;
    fPosIn = aTrackInfo.fPosIn;
    fPosOut = aTrackInfo.fPosOut;
    fTriggerBit = aTrackInfo.fTriggerBit;
    return *this;
}
