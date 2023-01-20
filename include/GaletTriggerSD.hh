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

#ifndef GaletTriggerSD_h
#define GaletTriggerSD_h 1

#include "G4VSensitiveDetector.hh"
#include <ostream>
#include <map>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;
class G4LogicalVolume;

//========================================================================
//
// Created by T.Aso
//
// (Modification)
//  20160222 T.Aso Created
//  2017-03-15 T.Aso Threading.
//
//========================================================================

class GaletTriggerSD : public G4VSensitiveDetector {
public:
  GaletTriggerSD(G4String name);
  GaletTriggerSD(const GaletTriggerSD& right);
  ~GaletTriggerSD() override;

  void Initialize(G4HCofThisEvent * HCE) override;
  G4bool ProcessHits(G4Step * aStep, G4TouchableHistory * ROhist) override;
  void EndOfEvent(G4HCofThisEvent * HCE) override;
  void DrawAll() override;
  void PrintAll() override;

  virtual  GaletTriggerSD* Copy() { return new GaletTriggerSD(*this); };

public:
    void SetTriggerId(G4LogicalVolume* log, G4int id);
    void DumpTriggerId(std::ostream& out=G4cout);
    void ClearTriggerId(){ fTrigMap.clear();};

private:
  std::map<G4LogicalVolume*,G4int> fTrigMap;
};

#endif
