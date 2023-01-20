//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// ParticleConstruction.cc
//
// [Description]
//   Physics list to define particles for the P152 (NIRS) experiment.
//
// [Histoy]
//   8 April, 2003, K. Amako : The 1st version created.
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#include "MACParticleConstruction.hh"
#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4ShortLivedConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"

/******************************************************************************/
 MACParticleConstruction::MACParticleConstruction()
/******************************************************************************/
{}

/******************************************************************************/
 MACParticleConstruction::~MACParticleConstruction()
/******************************************************************************/
{}

/******************************************************************************/
 void MACParticleConstruction::ConstructParticle()
/******************************************************************************/
{
  // Bosons
  G4BosonConstructor bs;
  bs.ConstructParticle();

  // Leptons
  G4LeptonConstructor lp;
  lp.ConstructParticle();

  // Mesons
  G4MesonConstructor mc;
  mc.ConstructParticle();

  // Baryons
  G4BaryonConstructor br;
  br.ConstructParticle();

  // Ions
  G4IonConstructor in;
  in.ConstructParticle();

  // Short Lived
  G4ShortLivedConstructor shortLived;
  shortLived.ConstructParticle();
}

/******************************************************************************/
 void MACParticleConstruction::ConstructProcess()
/******************************************************************************/
{
}



