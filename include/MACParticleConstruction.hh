//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// ParticleConstruction.hh
//
// [Description]
//   Physics list to define particles for the P152 (NIRS) experiment.
//
// [Histoy]
//   8 April, 2003, K. Amako : The 1st version created.
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef MACParticleConstruction_h
#define MACParticleConstruction_h 1

#include "G4VPhysicsConstructor.hh"
#include "globals.hh"

/******************************************************************************/
 class MACParticleConstruction: public G4VPhysicsConstructor
/******************************************************************************/
{
  public: 
    MACParticleConstruction();
    ~MACParticleConstruction() override;

  public: 
    // Particle constructors should be defined in this methods
    void ConstructParticle() override;
 
    // This method should be dummied because the current class is only
    // for particle constructions
    void ConstructProcess() override;

 };

#endif








