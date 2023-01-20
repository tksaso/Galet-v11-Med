//
// GaletEventAction.cc
// 
#include "GaletEventAction.hh"
#include "GaletSD.hh"
#include "GaletHit.hh"
#include "G4MProcessType.hh"
#include "GaletScoreId.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

#include "Randomize.hh"
#include <iomanip>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletEventAction::GaletEventAction()
 : G4UserEventAction()
{
  fMessenger = new GaletEventActionMessenger(this);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletEventAction::~GaletEventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletHitsCollection* 
GaletEventAction::GetHitsCollection(G4int hcID, const G4Event* event) const
{
  auto hitsCollection 
    = static_cast<GaletHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletEventAction::BeginOfEventAction(const G4Event* /*event*/)
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletEventAction::EndOfEventAction(const G4Event* event)
{  
  // Print per event (modulo n)
  //
  //  UI:  /run/printProgress <n>
  G4int eventID = event->GetEventID();
  G4int printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
  if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     
  }  

  // Get hits collections IDs (only once)
  if ( fHCID == -1 ) {
    fHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("galetSD/GaletHitsCollection");
    G4cout << " fHCID = " << fHCID << G4endl;
  }

  // Skip following code by return if no hits collection
  //
  if ( fHCID < 0 ) return;

  // Get hits collections
  GaletHitsCollection* HC = GetHitsCollection(fHCID, event);

  if ( !HC ) return;

  // get analysis manager
  auto analysisManager = G4AnalysisManager::Instance();
  //
  // Fill histgrams
  for ( size_t i = 0; i < HC->entries() ; i++){
    GaletHit* Hit = (*HC)[i];
    //
    G4double  de  = Hit->GetEnergyDeposit()/keV;
    G4double  dose= Hit->GetDose()/gray;    
    const G4ThreeVector& pos = Hit->GetEntryPoint()/mm;
    G4double  x = pos.x();
    G4double  y = pos.y();
    G4double  z = pos.z();
    G4int    ix = Hit->GetUnitXID();
    G4int    iy = Hit->GetUnitYID();
    G4int    iz = Hit->GetUnitZID();    
    //
    // fill 1D hist
    analysisManager->FillH1(100,iz,dose); // Depth-dose
   
    // fill 2D hist
    if ( iz == 10 ) analysisManager->FillH2(200,x,y,dose); // Lateral at surface
   
    // fill ntuple
    analysisManager->FillNtupleIColumn(1000,0, eventID);
    analysisManager->FillNtupleIColumn(1000,1, ix);
    analysisManager->FillNtupleIColumn(1000,2, iy);
    analysisManager->FillNtupleIColumn(1000,3, iz);
    analysisManager->FillNtupleFColumn(1000,4, x);
    analysisManager->FillNtupleFColumn(1000,5, y);
    analysisManager->FillNtupleFColumn(1000,6, z);
    analysisManager->FillNtupleFColumn(1000,7, de);
    analysisManager->AddNtupleRow(1000);
    //
  } 
  //
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
