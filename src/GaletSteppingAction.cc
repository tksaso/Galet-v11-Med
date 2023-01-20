//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file GaletSteppingAction.cc
/// \brief Implementation of the GaletSteppingAction class

#include "GaletSteppingAction.hh"
#include "G4Step.hh"
#include "G4Threading.hh"
#include <fstream>
#include "G4String.hh"
#include "G4TouchableHistory.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"

#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletSteppingAction::GaletSteppingAction()
{
  if ( bDumpStep ) {
    G4cout << " SteppingAction Thread ID " << G4Threading::G4GetThreadId() << G4endl;
    if ( !ofs.is_open() ) {
      G4String name="OutStep"+std::to_string(G4Threading::G4GetThreadId());
      name.append(".txt");
      ofs.open((char*)(name.data()));
    }
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaletSteppingAction::~GaletSteppingAction()
{
  if ( bDumpStep ) {
    if ( ofs.is_open() ){
      ofs.close();
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletSteppingAction::UserSteppingAction(const G4Step* step)
{
  if ( bDumpStep ) {  
    G4int evno = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4StepPoint* preStepPoint = step->GetPreStepPoint();
    G4StepPoint* postStepPoint = step->GetPostStepPoint();
    const G4ThreeVector& prePoint = preStepPoint->GetPosition();
    const G4ThreeVector& postPoint = postStepPoint->GetPosition();
    //
    //const G4ThreeVector& momentum = postStepPoint->GetMomentum();
    //G4double incidentKinE = preStepPoint->GetKineticEnergy();
    //
    //G4TouchableHistory* touchable = (G4TouchableHistory *)preStepPoint->GetTouchable();
    //G4VPhysicalVolume* currentPV  = preStepPoint->GetPhysicalVolume();  
    //
    G4Track* track   = step->GetTrack();
    G4int trackID    = track->GetTrackID();
    G4int parentTrkID    = track->GetParentID();  
    G4int PID        = track->GetDefinition()->GetPDGEncoding();
    //G4int theAtomicMass = track->GetDefinition()->GetAtomicMass();
    //G4int theAtomicNumber = track->GetDefinition()->GetAtomicNumber();
    //const G4ThreeVector& primVertex = track->GetVertexPosition();
    //G4double eDeposit   = step->GetTotalEnergyDeposit();
    //G4double time       = preStepPoint->GetGlobalTime();
    //G4double weight     = preStepPoint->GetWeight();
    //
    ofs << "---- Stepping evno "
	<< evno << " " << trackID <<" " << PID << " "
	<< prePoint.x()/mm << " " << prePoint.y()/mm << " " << prePoint.z()/mm <<" "
	<< postPoint.x()/mm << " " << postPoint.y()/mm << " " << postPoint.z()/mm
	<< G4endl;
    //
    //
    //  Information of primary particle
    if ( parentTrkID == 0 && track->GetCurrentStepNumber() == 1 ){
      // const G4ThreeVector& primPos = preStepPoint->GetPosition()/mm;
      // G4double primKE = preStepPoint->GetKineticEnergy()/MeV;
      // const G4ThreeVector& primDir = preStepPoint->GetMomentumDirection();
    }
  }
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
