//////////////////////////////////////////////////////////////////////////
//Copyright (2003-2010): Katsuya Amako, Tsukuasa Aso, Go Iwai, Akinori
//Kimura, Koichi Murakami, Takashi Sasaki, Toshiyuki Toshito, Tomohiro
//Yamashita
//
//PTSim Public License
//To use PTSim software, which is distributed under the Apache License
//Version 2.0, you must agree to its license terms and conditions.
//http://www.apache.org/licenses/LICENSE-2.0
//
//PTSim 3rd Party License
//PTSim is developed to use the Geant4 Toolkit distributed under
//the Geant4 Software License.
//http://www.geant4.org/geant4/license/
//////////////////////////////////////////////////////////////////////////
#include "GaletTriggerSD.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4SDManager.hh"
#include "GaletTrackInformation.hh"
#include "G4LogicalVolume.hh"

//#include "GaletTriggerSDMessenger.hh"
//========================================================================
//
// Created by T.Aso
//
// (Modification)
//  2016-02-22
//  2017-03-15 T.Aso Threading.
//========================================================================

GaletTriggerSD::GaletTriggerSD(G4String name)
  :G4VSensitiveDetector(name) {
  fTrigMap.clear();
}

GaletTriggerSD::GaletTriggerSD(const GaletTriggerSD& right)
  :G4VSensitiveDetector(right.SensitiveDetectorName) {
  fTrigMap = right.fTrigMap;
}

GaletTriggerSD::~GaletTriggerSD() {
  fTrigMap.clear();
}

void GaletTriggerSD::Initialize(G4HCofThisEvent* ) {
}

G4bool GaletTriggerSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {

  G4Track* aTrack = aStep->GetTrack();
  GaletTrackInformation* info = 
    (GaletTrackInformation*)(aTrack->GetUserInformation());
  if ( !info ) return false;
  G4LogicalVolume* lv  = 
    aStep->GetPreStepPoint()->GetPhysicalVolume()->GetLogicalVolume();
  
  info->SetTriggerId(fTrigMap[lv]);
  info->SetTrgPosOut(aStep->GetPostStepPoint()->GetPosition());

  return true;
}


void GaletTriggerSD::EndOfEvent(G4HCofThisEvent *) {
}

void GaletTriggerSD::DrawAll() {
  ;
} 

void GaletTriggerSD::PrintAll() {
  ;
}

void GaletTriggerSD::SetTriggerId(G4LogicalVolume* log, G4int id){
  fTrigMap[log] = id;
}

void GaletTriggerSD::DumpTriggerId(std::ostream& out){
  for (std::map<G4LogicalVolume*,G4int>::iterator itr = fTrigMap.begin();
       itr != fTrigMap.end(); itr++){
    G4LogicalVolume* lv = itr->first;
    G4int trigid = itr->second;
    out << lv->GetName() << "¥t" << trigid <<G4endl;
  } 
}

