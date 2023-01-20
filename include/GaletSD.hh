#ifndef GaletSD_HH
#define GaletSD_HH

#include "G4VSensitiveDetector.hh"
#include "GaletHit.hh"
#include "G4MProcessType.hh"
#include "G4ThreeVector.hh"
#include <map>
#include "G4EmCalculator.hh"

class G4Material;
class G4ParticleDefinition;

//========================================================================
//
// (Modification)
// 2017-08-20
//========================================================================

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class GaletSD : public G4VSensitiveDetector {
public:
  GaletSD(const G4String &name,
          const G4String &colname="HitsCollection",
          G4int verb=0);

  ~GaletSD() override;

  void Initialize(G4HCofThisEvent * HCE) override;
  G4bool ProcessHits(G4Step * aStep, G4TouchableHistory * ROhist) override;
  void EndOfEvent(G4HCofThisEvent * HCE) override;
  void clear() override;
  void DrawAll() override;
  void PrintAll() override;

  G4bool IsNewEntry();
  G4bool IsEndOfEntry();

  void SetZeroEdep(G4bool b = true) { bEdep = b; }; 
  G4bool GetZeroEdep() { return bEdep; }; 
  void SetCheckTrackID(G4bool b = true) { bCheckTrackID = b; }; 
  G4bool GetCheckTrackID() { return bCheckTrackID; }; 

  void SetVerbose(G4int v=0) { verbose = v; }; 
  G4int GetVerbose() { return verbose; }; 

  void SetDepth(G4int d_x, G4int d_y, G4int d_z,  G4int d_m=0, G4int d_s=0) 
  { fDepth[0] = d_x; fDepth[1] = d_y;  fDepth[2] = d_z;
    fDepth[3] = d_m; fDepth[4] = d_s; }; 
  G4int GetDepth(G4int i)
  { return fDepth[i];};

  void SetSaveStep(G4bool flg=true){  bStep = flg; }
  G4bool GetSaveStep(){ return  bStep; }

  void SetNxyzMod(G4int nx, G4int ny, G4int nz)
  { NxyzMod[0] = nx; NxyzMod[1] = ny; NxyzMod[2] = nz; };
  G4int GetNxyzMod(G4int i) const { return NxyzMod[i]; };

private:
  G4int    HCID = -1;
  GaletHitsCollection * HCollection = nullptr;

  GaletHit *       currentHit=nullptr;
  G4Track *           track = nullptr;
  G4VPhysicalVolume * currentPV = nullptr;

  G4int fDepth[5]={0,0,0,0,0};

  G4int PID =0;
  G4int theAtomicNumber =0;
  G4int theAtomicMass=0;
  G4int unitXID =0;
  G4int unitYID =0;
  G4int unitZID =0;
  G4int unitModID = 0;
  G4int unitSecID = 0;
  //
  G4int NxyzMod[3]={1,1,1};
  //
  G4int trackID=0;
  G4int parentTrkID=0;

  G4StepPoint *        preStepPoint = nullptr;
  G4StepPoint *        postStepPoint = nullptr;
  G4TouchableHistory * touchable = nullptr;
  G4double             EDeposit = 0.;
  G4double             time = 0.;
  G4ThreeVector        prePoint;
  G4ThreeVector        postPoint;
  G4ThreeVector        unitPoint;
  G4ThreeVector        primVertex;
  G4ThreeVector        momentum;
  G4double             incidentKinE = 0.;
  //
  G4ParticleDefinition* pdef=nullptr;
  G4Material*           mat=nullptr;
  //

  G4double  weight =0.;

  G4double dose =0.;

  G4double stepL =0.;
  G4int    stepNumber=0;

  G4VUserTrackInformation* trackInfo=nullptr;

  std::map<G4String, GaletHit *> hitmap;
  G4String key;

  G4bool bEdep = true;
  G4bool bStep = false;
  G4bool bCheckTrackID = true;
  G4int verbose = 0;
  //
  G4EmCalculator  emCal;
  
private:
  void getStepInfo(G4Step * aStep);
  void createNewHit();
  void updateHit();
  void StoreHit(GaletHit * ahit);

};

#endif
