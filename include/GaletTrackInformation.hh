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
// $Id: GaletTrackInformation.hh,v 1.2 2006/11/20 10:13:41 aso Exp $
// GEANT4 tag $Name:  $
//
//=====================================================================
//  
//  (Description)
//     Sub class of G4VUserTrackingAction class.
//
//  T.ASO DEC-01, 2005  Identify production process of secondary
//                      from primary particle.
//  T.Aso Nov-29, 2007  Created Position
//  2016-03-11 TriggerBit, ParentPID
//  2016-04-12 Trigger Position
//  2017-03-15 Threading.
//=====================================================================
//

#ifndef GaletTrackInformation_h
#define GaletTrackInformation_h 1

#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4ParticleDefinition.hh"
#include "G4Track.hh"
#include "G4Allocator.hh"
#include "G4VUserTrackInformation.hh"
#include "G4MProcessType.hh"
#include "G4MTrigBit.hh"

class GaletTrackInformation : public G4VUserTrackInformation 
{
  public:
    GaletTrackInformation();
    GaletTrackInformation(const GaletTrackInformation* aTrackInfo);
    ~GaletTrackInformation() override;

    void Print() const override { G4cout << " " <<origProcessType<<" "<<G4endl;} 
   
    inline void *operator new(size_t);
    inline void operator delete(void *aTrackInfo);
    inline int operator ==(const GaletTrackInformation& right) const
    {return (this==&right);}

    GaletTrackInformation& operator =(const GaletTrackInformation& right);

  private:
    // Information of the track

    //
    // Process type of secondary from primary particles.
    // This is a first interaction for the secondary particle.
    // This type is not changed for the third particle which is produced
    // by secondary particle.
    G4MProcessType  origProcessType = fMUndefined; 

    // Created point
    // End point  G4Track -> Position at end.

    // Parent Track Number -> G4Track::GetParentID()
    G4int fParentPID = 0;
    G4ThreeVector fParentVtxPos;
    G4double      fParentVtxTime = 0.0;

    //
    // Flag for the produced secondary is in ROI.
    //
    G4bool  fIsROI = false;
    //
    //
    G4MTrigBit  fTriggerBit;
    // Trigger Positions
    G4ThreeVector  fPosIn;
    G4ThreeVector  fPosOut;
    //

  public:
  inline G4MProcessType GetOriginalProcessType()  // not used.
    const {return origProcessType;}
    inline void SetOriginalProcessType(G4MProcessType type)  // not used.
         {origProcessType = type;}
    inline void UpdateOriginalProcessType(G4MProcessType type) ; // not used.
    inline void SetROI(G4bool flag=TRUE){ fIsROI = flag; } // not used.
    inline G4bool IsROI(){ return fIsROI; }
    inline void SetParentPID(G4int ppid){ fParentPID=ppid;};
    inline G4int GetParentPID(){return fParentPID;};
    inline void SetParentVtx(const G4ThreeVector& pos){fParentVtxPos = pos;};
    inline G4ThreeVector& GetParentVtx(){return fParentVtxPos;};
    inline void SetParentTime(G4double time){ fParentVtxTime=time;};
    inline G4double GetParentTime(){return fParentVtxTime;};

    inline void ResetTriggerBit(){ fTriggerBit.reset(); };
    inline void SetTriggerId(G4int trigId, G4int val=1){ fTriggerBit.set(trigId,val); };
    inline G4bool TriggerId(G4int trigId){ return fTriggerBit.test(trigId); };
    inline G4int  GetSizeOfTriggerId(){ return fTriggerBit.size(); };
    inline unsigned long  GetTriggerBitByULong(){ return fTriggerBit.to_ulong(); };
    inline void SetTriggerBit(G4MTrigBit& trigbit ){fTriggerBit = trigbit; };  
    inline G4MTrigBit& GetTriggerBit() { return fTriggerBit; };
    inline void SetTrgPosIn(const G4ThreeVector& pos){ fPosIn = pos; };
    inline G4ThreeVector& GetTrgPosIn(){ return fPosIn; };
    inline void SetTrgPosOut(const G4ThreeVector& pos){ fPosOut = pos; };
    inline G4ThreeVector& GetTrgPosOut(){ return fPosOut; };
};

extern G4ThreadLocal G4Allocator<GaletTrackInformation>* aTrackInformationAllocator;

inline void GaletTrackInformation::UpdateOriginalProcessType(G4MProcessType type){
    if ( type > origProcessType ) origProcessType = type;
}


inline void* GaletTrackInformation::operator new(size_t)
{ 
  if ( !aTrackInformationAllocator ){
     aTrackInformationAllocator = new G4Allocator<GaletTrackInformation>;
  }
  void* aTrackInfo;
  aTrackInfo = (void*)aTrackInformationAllocator->MallocSingle();
  return aTrackInfo;
}

inline void GaletTrackInformation::operator delete(void *aTrackInfo)
{ 
  if ( !aTrackInformationAllocator ){
    aTrackInformationAllocator = new G4Allocator<GaletTrackInformation>;
  }
  aTrackInformationAllocator->FreeSingle((GaletTrackInformation*)aTrackInfo);
}

#endif

