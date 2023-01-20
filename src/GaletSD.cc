//========================================================================
//
// (Modification)
// 2017-09-20 T.Aso Created
//
//========================================================================

#include "GaletSD.hh"
#include "GaletHit.hh"
#include "G4VUserTrackInformation.hh"
#include "GaletTrackInformation.hh"
#include "G4VPhysicalVolume.hh"
#include "G4LogicalVolume.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4ParticleDefinition.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"
#include "G4ios.hh"
#include "G4SDManager.hh"
#include "G4VSolid.hh"
#include "G4SystemOfUnits.hh"

GaletSD::GaletSD(const G4String &name, 
                 const G4String &colname,
                 G4int verb)
  :G4VSensitiveDetector(name),verbose(verb){

  collectionName.insert(colname);
}

GaletSD::~GaletSD(){
}

void GaletSD::Initialize(G4HCofThisEvent* HCE) {

  HCollection = new GaletHitsCollection(SensitiveDetectorName,
					collectionName[0]); 
  if(HCID < 0){
    G4String colname = SensitiveDetectorName+"/"+collectionName[0];
    HCID = G4SDManager::GetSDMpointer()->GetCollectionID(colname);
    G4cout << colname << " HCID = " << HCID << G4endl;
  }
  HCE->AddHitsCollection(HCID, HCollection);
  
  hitmap.clear();
}

G4bool GaletSD::ProcessHits(G4Step * aStep, G4TouchableHistory *) {

  if (!aStep) return true;
  if ( bEdep && aStep->GetTotalEnergyDeposit() == 0. ) return true;

  getStepInfo(aStep);

  if (IsNewEntry()) createNewHit();
  else              updateHit();
//   if (IsEndOfEntry()) resetID();

  return true;
}


void GaletSD::EndOfEvent(G4HCofThisEvent *) {;}

void GaletSD::clear() {;}

void GaletSD::DrawAll() {;} 

void GaletSD::PrintAll() {;} 

G4bool GaletSD::IsNewEntry()
{
  std::map<G4String, GaletHit *>::iterator p = hitmap.find(key);
  if (p == hitmap.end()) {
    return true;
  }
  currentHit = p->second;
  return false;
}

G4bool GaletSD::IsEndOfEntry(){
  G4StepStatus postStatus = postStepPoint->GetStepStatus();  
  return (postStatus == fGeomBoundary);
}

void GaletSD::getStepInfo(G4Step * aStep) {
  preStepPoint = aStep->GetPreStepPoint();
  postStepPoint = aStep->GetPostStepPoint();
  prePoint = preStepPoint->GetPosition();
  postPoint = postStepPoint->GetPosition();

  momentum = postStepPoint->GetMomentum();
  incidentKinE = preStepPoint->GetKineticEnergy();

  trackInfo = aStep->GetTrack()->GetUserInformation();

  touchable = (G4TouchableHistory *)preStepPoint->GetTouchable();

//
// 20150723 T.Aso 
  unitPoint.set(touchable->GetVolume(fDepth[0])->GetObjectTranslation().x(),
                touchable->GetVolume(fDepth[1])->GetObjectTranslation().y(),
                touchable->GetVolume(fDepth[2])->GetObjectTranslation().z());
//
//
// 20170818 T.Aso
  G4int dID = touchable->GetHistoryDepth();
  unitXID=unitYID=unitZID=unitModID=unitSecID=0;
  if (fDepth[0] <= dID) unitXID = touchable->GetReplicaNumber(fDepth[0]);
  if (fDepth[1] <= dID) unitYID = touchable->GetReplicaNumber(fDepth[1]);
  if (fDepth[2] <= dID) unitZID = touchable->GetReplicaNumber(fDepth[2]);
  if (fDepth[3] <= dID) unitModID = touchable->GetReplicaNumber(fDepth[3]);
  if (fDepth[4] <= dID) unitSecID = touchable->GetReplicaNumber(fDepth[4]);
  //
  //
  track      = aStep->GetTrack();
  pdef = track->GetDefinition();
  mat  = preStepPoint->GetMaterial();
  PID        = pdef->GetPDGEncoding();
  theAtomicMass = pdef->GetAtomicMass();
  theAtomicNumber = pdef->GetAtomicNumber();
  trackID    = track->GetTrackID();
  parentTrkID    = track->GetParentID();
  primVertex = track->GetVertexPosition();
  EDeposit   = aStep->GetTotalEnergyDeposit();
  time       = preStepPoint->GetGlobalTime();
  currentPV  = preStepPoint->GetPhysicalVolume();
  weight     = preStepPoint->GetWeight();
  //

  G4double volume  = preStepPoint->GetPhysicalVolume()
    ->GetLogicalVolume()->GetSolid()->GetCubicVolume();
  G4double density = mat->GetDensity();
  dose       = EDeposit/(density*volume)*weight;

  stepL = aStep->GetStepLength();
  stepNumber = track->GetCurrentStepNumber();

  std::ostringstream ss;
  if ( bCheckTrackID ){
    ss << "X" << unitXID << "Y" << unitYID << "Z" << unitZID 
       << "-" << trackID
       << "M" << unitModID<<"S"<<unitSecID;
  }else{
    ss << "X" << unitXID << "Y" << unitYID << "Z" << unitZID 
       << "M" << unitModID<<"S"<<unitSecID;
  }
  key = ss.str();
}


void GaletSD::createNewHit() {
  if ( verbose > 0 ){
    G4cout << " Create New Hit " 
           << " " << unitXID
           <<" "  << unitYID       
           <<" "  << unitZID <<G4endl;
    G4cout << " Create new Hit: unitPoint "
           << " " << unitPoint.x()/mm
           << " " << unitPoint.y()/mm
           << " " << unitPoint.z()/mm
           << G4endl;
    G4cout << " Create new Hit : EntryPoint"
           << " " << prePoint.x()/mm
           << " " << prePoint.y()/mm
           << " " << prePoint.z()/mm
           << G4endl;
  }
  //
  currentHit = new GaletHit();
  currentHit->SetTrackID(trackID);
  currentHit->SetPID(PID);
  currentHit->SetParentTrackID(parentTrkID);
  currentHit->SetAtomicNumber(theAtomicNumber);
  currentHit->SetAtomicMass(theAtomicMass);
  currentHit->SetGlobalTime(time);
  //
  // Calculate Electronic DEDX for protons
  if ( PID == 2212 ){
    G4double let =
      emCal.ComputeElectronicDEDX(incidentKinE,pdef,mat);
    //     * mat->GetDensity();
    currentHit->SetElectronicLET(let);
    //G4cout << " electronic let =  "<< let/(keV/mm) << " keV/mm "<<G4endl;
  }
  //
  currentHit->SetUnitID(unitXID,unitYID,unitZID);
  currentHit->SetUnitModID(unitModID);
  currentHit->SetUnitSecID(unitSecID);
  currentHit->SetEntryPoint(prePoint);
  currentHit->SetExitPoint(postPoint);
  currentHit->SetUnitPoint(unitPoint);
  currentHit->SetPrimaryVertex(primVertex);
  currentHit->SetMomentum(momentum);
  currentHit->SetIncidentKinE(incidentKinE);
  currentHit->SetEnergyDeposit(EDeposit);  
  currentHit->SetWeight(weight);
  currentHit->SetDose(dose);
  currentHit->SetStepLength(stepL);
  currentHit->SetStepNumber(stepNumber);
  StoreHit(currentHit);
  //
  if ( trackInfo ) { 
    GaletTrackInformation* mTrackInfo= (GaletTrackInformation*)trackInfo;
    currentHit->SetParentPID(mTrackInfo->GetParentPID());
    //currentHit->SetCreatorProcessType(mTrackInfo->GetOriginalProcessType());  
    currentHit->SetTrigBit(mTrackInfo->GetTriggerBit());  
    currentHit->SetTrigPos(mTrackInfo->GetTrgPosOut());  
  }
  //
  
  //
  if ( bStep ){ 
    currentHit->AddStepEntry(prePoint);
    currentHit->AddStepEdep(EDeposit);
  }
  //
  hitmap.insert(std::map<G4String, GaletHit *>::value_type(key, currentHit));
}        

void GaletSD::updateHit(){
  if ( verbose > 0 ){
    G4cout << " Update Hit : Point"
           << " " << postPoint.x()/mm
           << " " << postPoint.y()/mm
           << " " << postPoint.z()/mm
           << G4endl;
  }
  currentHit->AddEnergyDeposit(EDeposit);
  currentHit->AddDose(dose);
  currentHit->AddStepLength(stepL);
  currentHit->SetExitKinE(incidentKinE);
  currentHit->SetExitPoint(postPoint);
  if ( bStep ){ 
    currentHit->AddStepEntry(prePoint);
    currentHit->AddStepEdep(EDeposit);
  }
  if ( verbose > 0 ){
    G4cout << currentHit->GetEnergyDeposit()/keV << G4endl;
  }
} 

void GaletSD::StoreHit(GaletHit* hit){
  if (hit == 0) {
    G4cout << "GaletSD: hit to be stored is NULL !!" <<G4endl;
    return;
  }
  HCollection->insert( hit );

}
