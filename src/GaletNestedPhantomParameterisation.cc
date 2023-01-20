//
// GaletNestedPhantomParamterisation
//

#include "GaletNestedPhantomParameterisation.hh"

#include "G4VPhysicalVolume.hh"
#include "G4VTouchable.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4Material.hh"

#include "G4VisAttributes.hh"
#include "G4VVisManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaletNestedPhantomParameterisation::
GaletNestedPhantomParameterisation(const G4ThreeVector& voxelSize,
                                   std::vector<G4Material*>& matlist,
				   std::vector<G4int>& matindices,	   
                                   G4int fnX_, G4int fnY_, G4int fnZ_)
:G4VNestedParameterisation(),
 fdX(voxelSize.x()), fdY(voxelSize.y()), fdZ(voxelSize.z()),
 fnX(fnX_), fnY(fnY_), fnZ(fnZ_), 
 fMaterialList(matlist),fMaterialIndices(matindices){
  //
  fpZ.clear();
  for ( G4int iz = 0; iz < fnZ; iz++){
    G4double zp = (-fnZ*fdZ+fdZ) + iz*(fdZ*2.);
    fpZ.push_back(zp);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaletNestedPhantomParameterisation::~GaletNestedPhantomParameterisation(){
  fpZ.clear();
  for ( size_t i = 0; i < fVisAtt.size(); i++){
    if ( fVisAtt[i] ) delete fVisAtt[i];
  }
  fVisAtt.clear();
  fMaterialIndices.clear();
  fMaterialList.clear();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Material* GaletNestedPhantomParameterisation::
ComputeMaterial(G4VPhysicalVolume* physVol, const G4int iz,
                const G4VTouchable* parentTouch){

    // protection for initialization and vis at idle state
    //
    if(parentTouch == nullptr) return fMaterialList[0];

    // Copy number of voxels.
    // Copy number of X and Y are obtained from replication number.
    // Copy nymber of Z is the copy number of current voxel.
    G4int ix = parentTouch->GetReplicaNumber(0);
    G4int iy = parentTouch->GetReplicaNumber(1);

    G4int copyID = ix + fnX*iy + fnX*fnY*iz;

    G4int matIndex = fMaterialIndices[copyID];
    //G4cout << "copyID "<<copyID<<" matIndex "<< matIndex << G4endl;
    //
    if ( fVisOn && matIndex < (G4int)fVisAtt.size() ){
      physVol->GetLogicalVolume()->SetVisAttributes(fVisAtt[matIndex]);
    }
    //
    return fMaterialList[matIndex];
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
// Number of Materials
// Material scanner is required for preparing physics tables and so on before
// starting simulation, so that G4 has to know number of materials.
//
G4int GaletNestedPhantomParameterisation::GetNumberOfMaterials() const
{
    return G4int(fMaterialList.size());
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// GetMaterial
//  This is needed for material scanner and realizing geometry.
//
G4Material* GaletNestedPhantomParameterisation::GetMaterial(G4int i) const
{
    return fMaterialList[i];
}

//
// Transformation of voxels.
//
void GaletNestedPhantomParameterisation::
ComputeTransformation(const G4int copyNo, G4VPhysicalVolume* physVol) const
{
    // Position of voxels.
    // x and y positions are already defined in DetectorConstruction by using
    // replicated volume. Here only we need to define is z positions of voxels.
    physVol->SetTranslation(G4ThreeVector(0.,0.,fpZ[copyNo]));
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//
// Dimensions are always same in this RE02 example.
//
void GaletNestedPhantomParameterisation::
ComputeDimensions( G4Box& box, const G4int, const G4VPhysicalVolume* ) const
{
    box.SetXHalfLength(fdX);
    box.SetYHalfLength(fdY);
    box.SetZHalfLength(fdZ);
}
