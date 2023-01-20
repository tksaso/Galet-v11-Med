//========================================================================
//
// Created by T.Aso
//
// (Modification)
//========================================================================
#include "GaletHit.hh"
#include "G4ios.hh"
#include "G4VVisManager.hh"
#include "G4Colour.hh"
#include "G4VisAttributes.hh"
#include "G4LogicalVolume.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include "G4UnitsTable.hh"
#include "G4UIcommand.hh"


G4ThreadLocal G4Allocator<GaletHit>* GaletHitAllocator=0;

GaletHit::GaletHit(): G4VHit(){
  //
  trigBit.reset();
  StepPosVec.clear();
  StepDEVec.clear();
}

GaletHit::GaletHit(const GaletHit & right):G4VHit() {
  entryPoint = right.entryPoint;
  exitPoint = right.exitPoint;  
  primVertex = right.primVertex;
  incidentKinE = right.incidentKinE;
  momentum = right.momentum;
  electronicLET = right.electronicLET;  
  exitKinE = right.exitKinE;
  PID = right.PID;
  trackID = right.trackID;
  parentTrkID = right.parentTrkID;
  parentPID = right.parentPID;
  unitXID = right.unitXID;
  unitYID = right.unitYID;
  unitZID = right.unitZID;
  unitModID = right.unitModID;
  unitSecID = right.unitSecID;
  EDeposit = right.EDeposit;
  dose    = right.dose;
  slength    = right.slength;
  pType   = right.pType;
  trigBit = right.trigBit;
  stepNumber = right.stepNumber;
  //StepPosVec = right.StepPosVec;
  //StepDEVec = right.StepDEVec;

}

GaletHit::~GaletHit() {
  ;
}

const GaletHit& GaletHit::operator=(const GaletHit &right) {
  entryPoint = right.entryPoint;
  exitPoint = right.exitPoint;  
  primVertex = right.primVertex;
  momentum = right.momentum;
  incidentKinE = right.incidentKinE;
  exitKinE = right.exitKinE;
  PID = right.PID;
  trackID = right.trackID;
  parentTrkID = right.parentTrkID;
  parentPID = right.parentPID;
  unitXID = right.unitXID;
  unitYID = right.unitYID;
  unitZID = right.unitZID;
  unitModID = right.unitModID;
  unitSecID = right.unitSecID;
  EDeposit = right.EDeposit;
  dose    = right.dose;
  slength   = right.slength;
  pType   = right.pType;
  trigBit = right.trigBit;
  //StepPosVec = right.StepPosVec;
  //StepDEVec = right.StepDEVec;

  return *this;
}

G4bool GaletHit::operator==(const GaletHit &right) const {
  return 
    (  (unitXID==right.unitXID)
       &&(unitYID==right.unitYID)
       &&(unitZID==right.unitZID)
       &&(unitModID==right.unitModID)
       &&(unitSecID==right.unitSecID) );
}

void GaletHit::Draw(){
  ;
}

void GaletHit::Print() {
  ;
}

void GaletHit::SetUnitPoint(const G4ThreeVector & _unitPoint) {
  unitPoint = _unitPoint;
}
G4ThreeVector GaletHit::GetUnitPoint() const {
  return unitPoint;
}

void GaletHit::SetPrimaryVertex(const G4ThreeVector & _primVertex) {
  primVertex = _primVertex;
}
G4ThreeVector GaletHit::GetPrimaryVertex() const {
  return primVertex;
}

void GaletHit::SetMomentum(const G4ThreeVector & _momentum) {
  momentum = _momentum;
}
G4ThreeVector GaletHit::GetMomentum() const {
  return momentum;
}

void GaletHit::SetIncidentKinE(const G4double & _incidentKinE) {
  incidentKinE = _incidentKinE;
}
G4double GaletHit::GetIncidentKinE() const {
  return incidentKinE;
}

void GaletHit::SetExitKinE(const G4double & _exitKinE) {
  exitKinE = _exitKinE;
}
G4double GaletHit::GetExitKinE() const {
  return exitKinE;
}

void GaletHit::SetPID(const G4int & _PID) {
  PID = _PID;
}
G4int GaletHit::GetPID() const {
  return PID;
}

void GaletHit::SetTrackID(const G4int & _trackID) {
  trackID = _trackID;
}
G4int GaletHit::GetTrackID() const {
  return trackID;
}

void GaletHit::SetParentTrackID(const G4int & _parentTrkID) {
  parentTrkID = _parentTrkID;
}
G4int GaletHit::GetParentTrackID() const {
  return parentTrkID;
}

void GaletHit::SetParentPID(const G4int & _parentPID) {
  parentPID = _parentPID;
}
G4int GaletHit::GetParentPID() const {
  return parentPID;
}

void GaletHit::SetUnitID(const G4int & _unitXID,
                            const G4int & _unitYID,
                            const G4int & _unitZID) {
  unitXID = _unitXID;
  unitYID = _unitYID;
  unitZID = _unitZID;
}
void GaletHit::SetUnitXID(const G4int & _unitXID) {
  unitXID = _unitXID;
}
G4int GaletHit::GetUnitXID() const {
  return unitXID;
}
void GaletHit::SetUnitYID(const G4int & _unitYID) {
  unitYID = _unitYID;
}
G4int GaletHit::GetUnitYID() const {
  return unitYID;
}
void GaletHit::SetUnitZID(const G4int & _unitZID) {
  unitZID = _unitZID;
}
G4int GaletHit::GetUnitZID() const {
  return unitZID;
}

void GaletHit::SetEnergyDeposit(const G4double & _EDeposit) {
  EDeposit = _EDeposit;
}
G4double GaletHit::GetEnergyDeposit() const {
  return EDeposit;
}
void GaletHit::AddEnergyDeposit(const GaletHit & _aHit) {
  AddEnergyDeposit(_aHit.GetEnergyDeposit());
}
void GaletHit::AddEnergyDeposit(const G4double & _EDeposit) {
  EDeposit += _EDeposit;
}

void GaletHit::SetCreatorProcessType(const G4MProcessType type){
    pType = type;
}

G4MProcessType GaletHit::GetCreatorProcessType() const{
    return pType;
}

G4double GaletHit::GetByScId(G4int scid, const G4double unit, const G4int evno){
  switch  ( scid ){
  case EvnoScId:
    return evno;
  case TrkScId:
    return trackID/unit;
  case PidScId:
    return PID/unit;
  case EdepScId:
    return EDeposit/unit;
  case DoseScId:
    return dose/unit;
  case StepLScId:
    return slength/unit;
  case XidScId:
    return unitXID/unit;
  case YidScId:
    return unitYID/unit;
  case ZidScId:
    return unitZID/unit;
  case ModidScId:
    return unitModID/unit;
  case SecidScId:
    return unitSecID/unit;
  case WeigtScId:
    return weight/unit;
  case KinEScId:
    return incidentKinE/unit;
  case XEntrypointScId:
    return entryPoint.x()/unit;
  case YEntrypointScId:
    return entryPoint.y()/unit;
  case ZEntrypointScId:
    return entryPoint.z()/unit;
  case XExitpointScId:
    return exitPoint.x()/unit;
  case YExitpointScId:
    return exitPoint.y()/unit;
  case ZExitpointScId:
    return exitPoint.z()/unit;
  case PxScId:
    return momentum.x()/unit;
  case PyScId:
    return momentum.y()/unit;
  case PzScId:
    return momentum.z()/unit;
  case TimeScId:
    return time/unit;
  case XvtxScId:
    return primVertex.x()/unit;
  case YvtxScId:
    return primVertex.y()/unit;
  case ZvtxScId:
    return primVertex.z()/unit;
  case TrigPosXScId:
    return trigPos.x()/unit;
  case TrigPosYScId:
    return trigPos.y()/unit;
  case TrigPosZScId:
    return trigPos.z()/unit;
  }
  return 0.0;
}

G4int GaletHit::GetByScIdI(G4int scid, G4int evno){
  switch  ( scid ){
  case EvnoScId:
    return evno;
  case TrkScId:
    return trackID;
  case PidScId:
    return PID;
  case XidScId:
    return unitXID;
  case YidScId:
    return unitYID;
  case ZidScId:
    return unitZID;
  case ModidScId:
    return unitModID;
  case SecidScId:
    return unitSecID;
  case ProcScId:
    return pType;
  case ParentTrkScId:
    return parentTrkID;
  case ParentPidScId:
    return parentPID;
  case TrigbitScId:
    return (G4int)trigBit.to_ulong();
  }
  return 0;
}

