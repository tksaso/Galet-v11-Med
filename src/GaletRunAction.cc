//
//  GaletRunAction.cc
//
#include "GaletRunAction.hh"
#include "GaletRunActionMessenger.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletRunAction::GaletRunAction()
  : G4UserRunAction()
{ 
  //
  fMessenger = new GaletRunActionMessenger(this);
  //
  // set printing event number per each event
  //G4RunManager::GetRunManager()->SetPrintProgress(50);     

  // Create analysis manager
  // The choice of analysis technology is done via selectin of a namespace
  // in GaletAnalysis.hh
  auto analysisManager = G4AnalysisManager::Instance();
  analysisManager->SetNtupleMerging(true); // Should be false if run size becomes large.
  // Create directories 
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetFirstH1Id(100);
  analysisManager->SetFirstH2Id(200);
  analysisManager->SetFirstH3Id(300);
  analysisManager->SetFirstNtupleId(1000);      
  //
  // Book histograms, ntuple
  //
  // Creating 1-D histograms (HID starts from 100)
  G4int h1Id = 0;
  h1Id = analysisManager->CreateH1("H1_1","Depth-dose in iz", 150, 0., 150);
  G4cout << " H1  Id " << h1Id<<G4endl;

  // Creating 2-D histograms (HID starts from 200)
  G4int h2Id = 0;  
  h2Id = analysisManager->CreateH2("H2_1","x-y dose at iz=10", 150, -150., 150, 150, -150., 150.);
  G4cout << " H2  Id " << h1Id<<G4endl;  
  
  // Creating ntuple  (NtupleId starts from 1000)
  G4int ntupleId = analysisManager->CreateNtuple("Galet", "Galet");
  analysisManager->CreateNtupleIColumn(ntupleId,"evno");
  analysisManager->CreateNtupleIColumn(ntupleId,"ix");
  analysisManager->CreateNtupleIColumn(ntupleId,"iy");
  analysisManager->CreateNtupleIColumn(ntupleId,"iz");
  analysisManager->CreateNtupleFColumn(ntupleId,"x");
  analysisManager->CreateNtupleFColumn(ntupleId,"y");
  analysisManager->CreateNtupleFColumn(ntupleId,"z");
  analysisManager->CreateNtupleFColumn(ntupleId,"de");    
  analysisManager->FinishNtuple(ntupleId);
  G4cout << " Ntuple Id " << ntupleId<<G4endl;
  //
  // Phantom density  (Id 1001)
  ntupleId = analysisManager->CreateNtuple("Phantom", "Phantom dens.");  
  analysisManager->CreateNtupleIColumn(ntupleId,"ix");
  analysisManager->CreateNtupleIColumn(ntupleId,"iy");
  analysisManager->CreateNtupleIColumn(ntupleId,"iz");
  analysisManager->CreateNtupleFColumn(ntupleId,"dens");
  analysisManager->FinishNtuple(ntupleId);//
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletRunAction::~GaletRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletRunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  // Get analysis manager
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  // Open an output file
  //
  analysisManager->OpenFile(fFileName);
  G4cout << "Using " << analysisManager->GetType() << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletRunAction::EndOfRunAction(const G4Run* /*run*/)
{
  // print histogram statistics
  //
  auto analysisManager = G4AnalysisManager::Instance();
  // save histograms & ntuple
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
