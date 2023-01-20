//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// PhysicsList.cc
//
// [Description]
//   Physics list for Medical Accelerator. (Mainly for hadron therapy)
//
// [Histoy]
//  25 OCT,   2005  T. Aso   : Created
//  18 May,   2009  T. Aso   : Merge with G4 Physics-list.
//  08 Jul,   2010  T. Aso   : RadioactiveDecay
//  2012-01-15      T. Aso   : HadronPhysicsQGSP_INCL_ABLA was replaced to
//                             HadronPhysicsQGSP_INCLXX.
//                             G4IonInclAblaPhysics were replaced to
//                             G4IonINCLXXPhysics.
//  2013-03-27 T.Aso : Add entries for processes in Parallel world geometries.
//  2014-01-08 T.Aso : Replace ParallelWorldProcesses to G4ParallwlWorldPhysics.
//                     Modify according to ver.10 physics.
//                     Exception status was modified to FatalException.
//  2017-03-15 T.Aso : Verbose.
//  2017-03-31 T.Aso : SysttemOfUnits.
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "MACPhysicsList.hh"
#include "G4SystemOfUnits.hh"
#include "MACParticleConstruction.hh"
// ParallelWorld Processes
#include "G4ParallelWorldPhysics.hh"

// EM processes including generic ions.
#include "G4EmDNAPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option1.hh"
#include "G4EmStandardPhysics_option2.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4EmStandardPhysics_option4.hh"
//                                     
//==== Hadrons
// Elastic process for hadrons. 
#include "G4HadronElasticPhysics.hh"
//
// Inelastic processes except for ions. 
#include "G4HadronPhysicsFTFP_BERT.hh"
#include "G4HadronPhysicsFTFP_BERT_HP.hh"
#include "G4HadronPhysicsFTFP_BERT_TRV.hh"
#include "G4HadronPhysicsFTF_BIC.hh"
#include "G4HadronPhysicsQGSP_BERT.hh"
#include "G4HadronPhysicsQGSP_BERT_HP.hh"
#include "G4HadronPhysicsQGSP_BIC.hh"
#include "G4HadronPhysicsQGSP_BIC_HP.hh"
#include "G4HadronPhysicsQGSP_FTFP_BERT.hh"
#include "G4HadronPhysicsQGS_BIC.hh"
#include "G4HadronPhysicsShielding.hh"
//
// Stopping Physics and Radioactive decay
#include "G4StoppingPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
//
//===== Optical Photon
#include "G4OpticalPhysics.hh"
//
//===== Ions including Generic Ions. 
#include "G4IonBinaryCascadePhysics.hh"
#include "G4IonINCLXXPhysics.hh"
#include "G4IonPhysics.hh"
#include "G4IonQMDPhysics.hh"
//
//===== Decay 
#include "G4DecayPhysics.hh"
//                                     
//=====
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4UserLimits.hh"
//=====

#include "G4UImanager.hh"

/******************************************************************************/
MACPhysicsList::MACPhysicsList():G4VModularPhysicsList()
/******************************************************************************/
{   
  SetVerboseLevel(0);
  fPhysListMessenger = new MACPhysicsListMessenger(this);

  // Register particles
  //RegisterPhysics( new MACParticleConstruction() );
  fParticleConstruction = new MACParticleConstruction();

}

/******************************************************************************/
 MACPhysicsList::~MACPhysicsList()
/******************************************************************************/
{
    delete fPhysListMessenger;
}

/******************************************************************************/
void MACPhysicsList::SetCuts()
/******************************************************************************/
{
  // set cut values for gamma at first and for e- second and next for e+,
  //// because some processes for e+/e- need cut values for gamma
  SetCutsWithDefault();
}

/******************************************************************************/
void MACPhysicsList::ConstructParticle()
/******************************************************************************/
{
  if( fParticleConstruction ) fParticleConstruction->ConstructParticle();
  if( fDecayProcess ) fDecayProcess->ConstructParticle();
}

/******************************************************************************/
void MACPhysicsList::ConstructProcess()
/******************************************************************************/
{
  //
  AddTransportation();
  //
  if(fEmProcess) {
    fEmProcess->ConstructProcess();
    em_config.AddModels();
  }

  if ( fHadronElasticProcess) fHadronElasticProcess->ConstructProcess();
  if (fHadronInElasticProcess) fHadronInElasticProcess->ConstructProcess();
  if (fStoppingProcess) fStoppingProcess->ConstructProcess();
  if (fRadioactiveProcess) fRadioactiveProcess->ConstructProcess();
  if (fIonInElasticProcess) fIonInElasticProcess->ConstructProcess();
  if (fDecayProcess) fDecayProcess->ConstructProcess();
  if (fOpticalProcess) fOpticalProcess->ConstructProcess();
  //
  if (fParallelWorldProcess) fParallelWorldProcess->ConstructProcess();  
  
}

/******************************************************************************/
void MACPhysicsList::SetStepLimitForRegion(G4double stepValue,
                                           const G4String&rname)
/******************************************************************************/
{
  G4Region* reg = G4RegionStore::GetInstance()->GetRegion(rname);
  if ( reg ){
    if ( reg->GetUserLimits() ){
      (reg->GetUserLimits())->SetMaxAllowedStep(stepValue);
    }else{
      reg->SetUserLimits(new G4UserLimits(stepValue));
    }
    G4cout << " StepLimits for region " << stepValue/mm << G4endl;
  } else {
    G4cout << " MACPhysicsList::SetUserLimits() region "<<rname<<" not found"
           << G4endl;
  }
}
/******************************************************************************/
void MACPhysicsList::RegisterParallelWorldModule(G4String& namePW,G4bool layered){ 
  G4VPhysicsConstructor* p=nullptr;  
  p = new G4ParallelWorldPhysics(namePW,layered);
  //RegisterPhysics(p);
  fParallelWorldProcess = p;
}
/******************************************************************************/
void MACPhysicsList::RegisterPhysicsModule(G4String& newValue){
  //
  G4int verbose = GetVerboseLevel();

  // Register physics processes
  ///////////////////////////
  // EM processes
  //////////////////////////
  G4VPhysicsConstructor* p=nullptr;
  if ( !fEmProcess ) {
    if( newValue == "G4EmDNAPhysics" ){
      p = new G4EmDNAPhysics(verbose);
    }else if ( newValue == "G4EmLivermorePhysics"){
      p = new G4EmLivermorePhysics(verbose);
    }else if ( newValue == "G4EmPenelopePhysics"){
      p = new G4EmPenelopePhysics(verbose);
    }else if ( newValue == "G4EmStandardPhysics"){
      p = new G4EmStandardPhysics(verbose);
    }else if ( newValue == "G4EmStandardPhysics_option1"){
      p = new G4EmStandardPhysics_option1(verbose);
    }else if ( newValue == "G4EmStandardPhysics_option2"){
      p = new G4EmStandardPhysics_option2(verbose);
    }else if ( newValue == "G4EmStandardPhysics_option3"){
      p = new G4EmStandardPhysics_option3(verbose);
    }else if ( newValue == "G4EmStandardPhysics_option4"){
      p = new G4EmStandardPhysics_option4(verbose);
    }
    if ( p ) {
      //RegisterPhysics(p);
      fEmProcess = p;
      return;
    }
  } 
    ///////////////////////////
    // Hadron Elastic processes
    //////////////////////////
  p = nullptr;
  if ( !fHadronElasticProcess ){
    if ( newValue == "G4HadronElasticPhysics"){
      p = new G4HadronElasticPhysics(verbose);
    }
    if ( p ) {
      //RegisterPhysics(p);
      fHadronElasticProcess = p;
      return;
    }
  }

    ///////////////////////////
    // Hadron Inelastic processes
    //////////////////////////
  p = nullptr;
  if ( !fHadronInElasticProcess ){
    if ( newValue == "G4HadronPhysicsFTFP_BERT"){
      p = new G4HadronPhysicsFTFP_BERT(verbose);
    }else if ( newValue == "G4HadronPhysicsFTFP_BERT_HP"){
      p = new G4HadronPhysicsFTFP_BERT_HP(verbose);
    }else if ( newValue == "G4HadronPhysicsFTFP_BERT_TRV"){
      p = new G4HadronPhysicsFTFP_BERT_TRV(verbose);
    }else if ( newValue == "G4HadronPhysicsFTF_BIC"){
      p = new G4HadronPhysicsFTF_BIC(verbose);
    }else if ( newValue == "G4HadronPhysicsQGSP_BERT"){
      p = new G4HadronPhysicsQGSP_BERT(verbose);
    }else if ( newValue == "G4HadronPhysicsQGSP_BERT_HP"){
      p = new G4HadronPhysicsQGSP_BERT_HP(verbose);
    }else if ( newValue == "G4HadronPhysicsQGSP_BIC"){
      p = new G4HadronPhysicsQGSP_BIC(verbose);
    }else if ( newValue == "G4HadronPhysicsQGSP_BIC_HP"){
      p = new G4HadronPhysicsQGSP_BIC_HP(verbose);
    }else if ( newValue == "G4HadronPhysicsQGSP_FTFP_BERT"){
      p = new G4HadronPhysicsQGSP_FTFP_BERT(verbose);
    }else if ( newValue == "G4HadronPhysicsQGS_BIC"){
      p = new G4HadronPhysicsQGS_BIC(verbose);
    }else if ( newValue == "G4HadronPhysicsShielding"){
      p = new G4HadronPhysicsShielding(verbose);
    }
    if ( p ) {
      //RegisterPhysics(p);
      fHadronInElasticProcess = p;
      return;
    }
  }

    ///////////////////////////
    // Stopping (Capture) processes
    //////////////////////////

  p = nullptr;
  if ( !fStoppingProcess ){
    if ( newValue == "G4StoppingPhysics"){
      p = new G4StoppingPhysics(verbose);      
    }
    if ( p ) {
      //RegisterPhysics(p);
      fStoppingProcess = p;
      return;
    }
  }

    ///////////////////////////
    // Radioactive processes
    //////////////////////////
  p = nullptr;
  if ( !fRadioactiveProcess ){
    if ( newValue == "G4RadioactiveDecayPhysics"){
      p = new G4RadioactiveDecayPhysics(verbose);
    }
    if ( p ) {
      //RegisterPhysics(p);
      fRadioactiveProcess = p;
      return;
    }
  }


    ///////////////////////////
    // Ion InElastic processes
    //////////////////////////
  p = nullptr;
  if ( !fIonInElasticProcess) {
    if ( newValue == "G4IonBinaryCascadePhysics"){
      p = new G4IonBinaryCascadePhysics(verbose);
    }else if ( newValue == "G4IonINCLXXPhysics"){
      p = new G4IonINCLXXPhysics(verbose);
    }else if ( newValue == "G4IonQMDPhysics"){
      p = new G4IonQMDPhysics(verbose);
    }else if ( newValue == "G4IonPhysics"){
      p = new G4IonPhysics(verbose);
    }
    if ( p ) {
      //RegisterPhysics(p);
      fIonInElasticProcess = p;
      return;
    }
  }

    ///////////////////////////
    // Decay processes
    //////////////////////////

  p = nullptr;
  if ( !fDecayProcess ) {
    if ( newValue == "G4DecayPhysics"){
      p = new G4DecayPhysics(verbose);
    }
    if ( p ) {
      //RegisterPhysics(p);
      fDecayProcess = p;
      return;
    }
  }

    ///////////////////////////
    // Optical processes
    //////////////////////////

  p = nullptr;
  if ( !fOpticalProcess ) {
    if ( newValue == "G4OpticalPhysics"){
      p = new G4OpticalPhysics();
    }
    if ( p ) {
      //RegisterPhysics(p);
      fOpticalProcess = p;
      return;
    }
  }
  
  
  //
  // No matching process. 
  // 

  G4String msg = "Process "+newValue+" can not be registed. ";
  G4Exception("MACPhysicsList","RegisterPhysicsModule",FatalException,msg);
}
/******************************************************************************/
