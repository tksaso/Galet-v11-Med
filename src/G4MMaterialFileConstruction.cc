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
//   17 Feb, 2009, T.Aso Exception for file error.
//   10 Sep, 2009, T.Aso and A.Taniuchi  Modify RegisterMaterial.
//   15 Sep, 2009, T.Aso Bug fix in checking existing materials
//                       in RegisterMaterial.
//   28 Jul, 2011, T.Aso Bug fix in using  NIST elements by default.
//   09 Jul, 2011, T.Aso turn off std. output in RegisterElement().
//   2012-07-24,   T.Aso Base Material (since G4.ver.9.5) is used to
//                      create materials of different density with
//                      same composition. (RegisterMaterial).
//                       The density should be given with the argument.
//  2013-03-30 T.Aso PhysicalConstants/SystemOfUnits.
//                   name->mname in for loop in RegisterMaterial().
// 2014-03-11 T.Aso fVerbose.
//
//---------------------------------------------------------------------
//
#include "G4MMaterialFileConstruction.hh"
#include "G4Material.hh"
#include "G4Element.hh"
#include "G4IonisParamMat.hh"
#include "G4MMaterialFileMessenger.hh"
#include "G4NistManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

#include <sstream>
#include <fstream>

using namespace std;

G4MMaterialFileConstruction::G4MMaterialFileConstruction()
{ 
  fMaterialMessenger = new G4MMaterialFileMessenger(this);
}

G4MMaterialFileConstruction::G4MMaterialFileConstruction(G4String& elmPATH,
                                                         G4String& matPATH)
    :fElementPath(elmPATH),fMaterialPath(matPATH),fIsUseG4Element(FALSE)
{ 
  fMaterialMessenger = new G4MMaterialFileMessenger(this);
}

G4MMaterialFileConstruction::~G4MMaterialFileConstruction()
{ 
  delete fMaterialMessenger;
}

G4Material* G4MMaterialFileConstruction::
CreateSimpleMaterial(G4String& name, 
                     G4double z, G4double a, G4double density,
                     G4String& chemicalFormula, G4double Ipotential ){
  G4Material* material = new G4Material(name,z, a,density);
  material->SetChemicalFormula(chemicalFormula);
  if ( Ipotential > 0. ) {
    material->GetIonisation()->SetMeanExcitationEnergy(Ipotential);
  }
  return material;
}

void G4MMaterialFileConstruction::GetElementInfo(G4String& name, 
                                                 G4double& z, G4double& a){
  G4Element* element = G4Element::GetElement(name,FALSE);
  if ( element ){
    z = element->GetZ();
    a = element->GetA();
  }else{
    if (fIsUseG4Element){
      G4NistManager* nist = G4NistManager::Instance();
      element = nist->FindOrBuildElement(name);
      z = element->GetZ();
      a = element->GetA();
    }else{
      G4String  symbol;
      G4String  fullpath = fElementPath+"/"+name+".dat";
      GetElementFile(fullpath,name,symbol,z,a);
    }
  }
}

G4Element* G4MMaterialFileConstruction::RegisterElement(G4String& name){ 

  //G4cout <<" G4Element FLAG " << fIsUseG4Element << name <<G4endl;

  if (fIsUseG4Element){
    G4NistManager* nist = G4NistManager::Instance();
    return nist->FindOrBuildElement(name);
  }

  G4String filename = fElementPath+"/"+name+".dat";

  G4String  nam, symbol;
  G4double  z, a; 
  GetElementFile(filename,nam,symbol,z, a);
  
  return new G4Element(name, symbol, z, a);
}

void G4MMaterialFileConstruction::
GetElementFile(G4String& filename, 
               G4String& name,G4String& symbol,
               G4double& z, G4double& a){
  char chline[256];
  G4String fname = filename;  //FILE NAME
  std::ifstream ifs;
  ifs.open(fname.c_str());    //file open
  if ( !ifs ) {
    const G4String& msg = "File not found " +fname;
    G4Exception("G4MMaterialFileConstruction::GeElementFile",
                "G4MMatFileConst00",FatalException,msg);
  }

  //+++++Element Name+++++
  ifs.getline(chline,256);  
  std::istringstream iss1(chline);
  iss1 >> name;
  //+++++Symbol Name+++++
  ifs.getline(chline,256);  
  std::istringstream iss2(chline);
  iss2 >> symbol;
  //+++++Z+++++
  ifs.getline(chline,256);  
  std::istringstream iss3(chline);
  iss3 >> z;
  //+++++A+++++
  ifs.getline(chline,256);  
  std::istringstream iss4(chline);
  iss4 >> a;
  a *= (g/mole);
}

G4Material* 
G4MMaterialFileConstruction::RegisterMaterial(G4String& name,
                                              G4double givenDensity,
                                              G4String givenName){
  //
  G4Material* material=NULL;  
  //
  // Existing Material? ( if givenDensity is not specified. )
  //
  G4String CheckName;
  if ( givenDensity <= 0.0 ){
    CheckName = name;
  }else{
    CheckName = givenName;
  }
  G4NistManager* nist = G4NistManager::Instance();
  if ( (material = G4Material::GetMaterial(CheckName,FALSE)) ) {
    G4cerr << "@@@  Material has already existed. SKIP " <<name<<G4endl;
    return material;
  }else if ( (material = nist->FindOrBuildMaterial(CheckName)) ){
    return material;
  }

  // Since G4 ver.9.5 (Using Base Material)
  //  check the Base material is available.
  G4Material* baseMaterial=NULL;
  if ( (baseMaterial = G4Material::GetMaterial(name,FALSE)) ){
    //G4cout << "@@@ Base Material " <<name<<" given name "<<givenName
    //     << " dens " << givenDensity;
    material= nist->BuildMaterialWithNewDensity(givenName,
                                               name,
                                               givenDensity);
    G4double Ipot = (baseMaterial->GetIonisation())->GetMeanExcitationEnergy();
    G4IonisParamMat* ionisPara = material->GetIonisation();
    ionisPara->SetMeanExcitationEnergy(Ipot);
    //G4cout << material <<G4endl;
    return material;
  }

  char chline[256];
  G4String fname = fMaterialPath+"/"+name+".dat";  //FILE NAME
  //
  std::ifstream ifs;
  ifs.open(fname.c_str());  //file open
  if ( !ifs ) {
    const G4String& msg = "File not found " +fname;
    G4Exception("G4MMaterialFileConstruction::GeElementFile",
                "G4MMatFileConst00",FatalException,msg);
  }

  //+++++Material Name+++++
  G4String materialname;
  ifs.getline(chline,256);  //Material Name
  std::istringstream iss1(chline);
  iss1 >> materialname;
  //G4cout << materialname <<  endl;

  //+++++Chemical Formula
  G4String chemicalformula;
  ifs.getline(chline,256);  //Chemical Formula & Material State Flag
  std::istringstream iss2(chline);

  //+++++Material State Flag(Gas:0 or Else:1)
  G4int mat_state_flag;
  iss2 >> chemicalformula >> mat_state_flag;

  //+++++Density+++++
  G4double materialdensity;
  ifs.getline(chline,256);  //Density & Temperature & Pressure
  std::istringstream iss3(chline);
  iss3 >> materialdensity;
  //G4cout << materialdensity << " "  << givenDensity << endl;
  materialdensity*= (g/cm3);
  //G4cout << materialdensity/(g/cm3)  << endl;


  //+++++temperature & pressure+++++
  G4double mat_temperature, mat_pressure;
  if(mat_state_flag == 0){
    iss3 >> mat_temperature >> mat_pressure;
    if ( mat_temperature < 0. ) mat_temperature = STP_Temperature;
    else                        mat_temperature *= kelvin ;
    if ( mat_pressure < 0.    ) mat_pressure = STP_Pressure;
    else                        mat_pressure *= pascal;
  }

  //+++++Excitation Energy++++++++++
  G4double excitE;
  ifs.getline(chline,256);   //Excitation Energy
  std::istringstream iss4(chline);
  iss4 >> excitE;
  excitE *= eV;
  //G4cout << excitE << endl;

  //++++Number of components+++++
  G4int num_of_comp;
  ifs.getline(chline,256);  //Number of components
  std::istringstream iss5(chline);
  iss5 >> num_of_comp;
  //G4cout << num_of_comp << endl;

  //++++Components+++++
  std::vector<G4String> comType;
  std::vector<G4String> comName;
  std::vector<G4double> comFrac;
  for ( G4int i =0; i < num_of_comp; i++){
      ifs.getline(chline,256);  //Number of components
      std::istringstream iss6(chline);
      G4String type;
      G4String mname;
      G4double frac;
      iss6 >> type >> mname >> frac;
      comType.push_back(type);
      comName.push_back(mname);
      comFrac.push_back(frac);
      //G4cout << type << " " << mname << " " << frac <<endl; 
  } 

  // ---------------------------------------------------------------
  // Replace parameter if given density and given name are assigned.
  // ---------------------------------------------------------------
  //
  if(givenDensity>0.0) {
    materialdensity = givenDensity;
    materialname = givenName;
  }

  //  --------------
  //  Create Material
  //  --------------

  //+++++Simple Material+++++
  if(num_of_comp==1 && comType[0] == "E" ){
      G4double z,a;
      GetElementInfo(comName[0],z,a);
      material = CreateSimpleMaterial(materialname, z, a, materialdensity, 
                                      chemicalformula,excitE);
      G4Element* el = G4Element::GetElement(comName[0],FALSE);
      if ( !el )  el = RegisterElement(comName[0]);

  //+++++multi Elements+++++
  }else {

      //+++++ Gas +++++
      if(mat_state_flag==0){
          material = new G4Material(materialname, materialdensity,num_of_comp,
                                    kStateGas,mat_temperature,mat_pressure);

      //+++++solid or liquid+++++
      }else{
          material = new G4Material(materialname,materialdensity,num_of_comp);
          //G4cout << materialname << "  " << materialdensity << " " << num_of_comp << endl;
      }

      //+++++Add Components+++++ 
      for(G4int i=0; i<num_of_comp; i++){
          if ( comType[i] == "E" ){
              G4Element* el = G4Element::GetElement(comName[i],FALSE);
              //G4cout << comType[i] << " " << comName[i] << " " << comFrac[i]<< endl;

              if ( !el ) {
                  el = RegisterElement(comName[i]);
              }
              if ( comFrac[i] >= 1. ) { 
                  // Add substitution  by atom
                  G4int natoms = G4int(comFrac[i]);
                  material->AddElement(el,natoms);
              }else {
                  // Add substitution  by fraction
                  material->AddElement(el,comFrac[i]);
                  //G4cout << el << " " << comFrac[i]<<endl;
              }
          }else if ( comType[i] == "M" ){
              G4Material* elMat = G4Material::GetMaterial(comName[i],FALSE);
              if ( !elMat ) {
                  elMat = RegisterMaterial(comName[i]);
              }
              // Add substitution  by fraction
                  material->AddMaterial(elMat,comFrac[i]);
          }else{
            G4Exception("G4MMaterialFileConstruction::GeElementFile",
                        "G4MMatFileConst00",FatalException,
                        "Type of Material constitution is wrong");
          }
      }
      // G4cout << excitE << endl;
      // Other parameters
      material->SetChemicalFormula(chemicalformula);
      if(excitE > 0.) {
        material->GetIonisation()->SetMeanExcitationEnergy(excitE);
      }
  }
  return material;
}




