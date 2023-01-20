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
// $Id: GaletDetectorConstruction.hh 75215 2013-10-29 16:07:06Z gcosmo $
// 
/// \file GaletDetectorConstruction.hh
/// \brief Definition of the GaletDetectorConstruction class

#ifndef GaletDetectorConstruction_h
#define GaletDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;
class G4MMaterialFileConstruction;
class GaletDetectorConstructionMessenger;
class GaletMagneticField;

/// Detector construction class to define materials and geometry.

class GaletDetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    GaletDetectorConstruction();
    ~GaletDetectorConstruction() override;

  public:
    G4VPhysicalVolume* Construct() override;
    void ConstructSDandField() override;
  //
    void SetMagneticField(const G4String& fieldname_lv, GaletMagneticField* magfild,
                          G4bool forceToAllDaughters = true); 
    void Dummy(){;};
  //
  private:
    // methods
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
  
    G4bool  fCheckOverlaps = true; // option to activate checking of volumes overlaps
  
    G4MMaterialFileConstruction* materialConstructor = nullptr;
    GaletDetectorConstructionMessenger* messenger = nullptr;
    //
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

