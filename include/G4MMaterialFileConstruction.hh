//
// ********************************************************************
// * DISCLAIMER                                                       *
// *                                                                  *
// * The following disclaimer summarizes all the specific disclaimers *
// * of contributors to this software. The specific disclaimers,which *
// * govern, are listed with their locations in:                      *
// *   http://cern.ch/geant4/license                                  *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.                                                             *
// *                                                                  *
// * This  code  implementation is the  intellectual property  of the *
// * GEANT4 collaboration.                                            *
// * By copying,  distributing  or modifying the Program (or any work *
// * based  on  the Program)  you indicate  your  acceptance of  this *
// * statement, and all its terms.                                    *
// ********************************************************************
//
//---------------------------------------------------------------------
//
// MaterialFileConstruction.hh
//
// [Description]
//   Material construction from File
//
// [Histoy]
//   30 Aug, 2006, T.Aso and T.Kawaguchi
//   10 Sep, 2009, T.Aso and A.Taniuchi  Modify RegisterMaterial.
//  2014-03-11 T.Aso fVerbose.
//
//---------------------------------------------------------------------
//
#ifndef MACMaterialFileConstruction_h
#define MACMaterialFileConstruction_h 1

#include "G4MMaterialFileMessenger.hh"

class G4Element;
class G4Material;

class G4MMaterialFileConstruction
{
  public:

     G4MMaterialFileConstruction();
     G4MMaterialFileConstruction(G4String& elmPATH,G4String& matPATH);
     virtual ~G4MMaterialFileConstruction();

     G4Material* RegisterMaterial(G4String& name,
                                  G4double givenDensity=0.0,
                                  G4String givenName="GivenName");
     G4Element* RegisterElement(G4String& name);

     void SetElementPath(G4String& path){ fElementPath = path; }
     void SetMaterialPath(G4String& path){ fMaterialPath =path; }
     G4String& GetElementPath(){ return fElementPath; }
     G4String& GetMaterialPath(){ return fMaterialPath; }

     void UseG4Element(G4bool flag = TRUE){ fIsUseG4Element = flag; }
     G4bool IsUseG4Element(){ return fIsUseG4Element; }

     void SetVerbose(G4int v ){ fVerbose = v; }

  protected:
     G4Material* CreateSimpleMaterial(G4String& name, 
                                      G4double z, G4double a, 
                                      G4double density, 
                                      G4String& chemicalFormula, 
                                      G4double Ipot );
     void GetElementInfo(G4String& name, G4double& z, G4double& a);
     void GetElementFile(G4String& filename, G4String& name,G4String& symbol,
                         G4double& z, G4double& a);

  private:
     G4String fElementPath="./data/common/element";
     G4String fMaterialPath="./data/common/material";
     G4bool   fIsUseG4Element=true;
     G4MMaterialFileMessenger* fMaterialMessenger = nullptr;

     G4int fVerbose=0;
};

#endif

