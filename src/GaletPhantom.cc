//
//  GaletPhantom
// 
#include "globals.hh"
//
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4Material.hh"
//
#include "GaletPhantom.hh"
#include "GaletNestedPhantomParameterisation.hh"
//
#include "G4VisAttributes.hh"
#include "G4UIcommand.hh"
#include "G4NistManager.hh"
//
#include <fstream>
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaletPhantom::GaletPhantom(const G4String& voxelLVname){
  // Name of Scoring Volume.
  fVoxelLVName = voxelLVname;
  // Assign default materials
  G4NistManager* nist = G4NistManager::Instance();
  fBaseMat  = G4Material::GetMaterial("G4_WATER");    
  fAir = nist->FindOrBuildMaterial("G4_AIR");
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
GaletPhantom::~GaletPhantom(){
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaletPhantom::ReadVTK(const G4String& vtkFile){
  std::ifstream in;
  in.open(vtkFile);
  //
  G4String PName;
  char line[512];
  // Skip.
  in.getline(line,512);
  in.getline(line,512);
  in.getline(line,512);
  in.getline(line,512);  
  //
  // DIMENSION
  in.getline(line,512);
  std::istringstream ss1(line);
  ss1 >> PName >> fNoVoxelX >> fNoVoxelY >> fNoVoxelZ;
  G4cout << PName << " "<<fNoVoxelX <<" "<<fNoVoxelY<<" "<<fNoVoxelZ<<G4endl;
  // ORIGIN
  in.getline(line,512);
  std::istringstream ss2(line);
  G4double orgX, orgY, orgZ;
  ss2 >> PName >> orgX >> orgY >> orgZ;
  G4cout << PName << " "<<orgX <<" "<<orgY<<" "<<orgZ<<G4endl;
  fOrigin.set(orgX*mm,orgY*mm,orgZ*mm);
  // SPACING
  in.getline(line,512);
  std::istringstream ss3(line);  
  ss3 >> PName >> fVoxelSPCX >> fVoxelSPCY >> fVoxelSPCZ;
  G4cout << PName << " "<<fVoxelSPCX <<" "<<fVoxelSPCY<<" "<<fVoxelSPCZ<<G4endl;
  // POINTDATA
  in.getline(line,512);
  std::istringstream ss4(line);  
  size_t nvoxels;
  ss4 >> PName >> nvoxels;
  G4cout << PName << " "<<nvoxels<<G4endl;
  if ( nvoxels != size_t(fNoVoxelX*fNoVoxelY*fNoVoxelZ) ){
    const G4String& msg = "NofVoxel missmatch!! ";
    G4Exception("GaletPhantom::readVTK",
                "GaletPhantom00",FatalException,msg);
  }
  // SCALARS
  in.getline(line,512);
  // LOOKUP_TABLE
  in.getline(line,512);  
  //
  // Density Data => Convert to material index.
  fMaterialIndices.clear();
  G4double density;
  for ( size_t i = 0; i < nvoxels; i++){
    in >> density;
    //G4cout << i <<" "<<density<<G4endl;
    if ( density == 0. ) { // Air(0)
      fMaterialIndices.push_back(0);
    }else{                      // Water(1-)
      density *= g/cm3;
      if ( density > fMaxDens ) {
	density = fMaxDens;
      }else if ( density < fMinDens ){
	density = fMinDens;
      }
      G4int matidx = G4int((density-fMinDens)/fDensReso+1.5);
      fMaterialIndices.push_back(matidx);
      //G4cout << " ivoxel= "<< i <<" matidx= "<<matidx<<G4endl;
    }
  }
  in.close();
  //
  fCenterCoord.set((fOrigin.x()+(fOrigin.x()+fVoxelSPCX*fNoVoxelX))/2.,
		   (fOrigin.y()+(fOrigin.y()+fVoxelSPCY*fNoVoxelY))/2.,
		   (fOrigin.z()+(fOrigin.z()+fVoxelSPCZ*fNoVoxelZ))/2.);

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......  
void GaletPhantom::ReadDummyVTK(G4double dx,G4double dy,G4double dz,
				G4int nx, G4int ny, G4int nz,
				G4bool onSurfIso, G4bool mosaic){
  fVoxelSPCX=dx*2./nx;
  fVoxelSPCY=dy*2./ny;
  fVoxelSPCZ=dz*2./nz;
  fNoVoxelX=nx;
  fNoVoxelY=ny;
  fNoVoxelZ=nz;
  fOrigin.set(-dx+fVoxelSPCX/2.,-dy+fVoxelSPCY/2.,-dz+fVoxelSPCZ/2.);
  // Should be (0,0,0);
  fCenterCoord.set((fOrigin.x()+(fOrigin.x()+fVoxelSPCX*fNoVoxelX))/2.,
		   (fOrigin.y()+(fOrigin.y()+fVoxelSPCY*fNoVoxelY))/2.,
		   (fOrigin.z()+(fOrigin.z()+fVoxelSPCZ*fNoVoxelZ))/2.);
  if (onSurfIso) fCenterCoord.setZ(-dz);
  //
  size_t Nvoxel = fNoVoxelX*fNoVoxelY*fNoVoxelZ;
  //
  std::vector<G4float> densities;
  densities.clear();
  G4double density=0.0;
  for (size_t i=0; i < Nvoxel; i++){
    density = 1.0*g/cm3;
    if ( mosaic && i%2 == 1)  density = 0.5*g/cm3;
    densities.push_back(density);
  }
  //Convert density to material idx.
  fMaterialIndices.clear();
  for (size_t i=0; i < Nvoxel; i++){
    density = densities[i];
    if ( density == 0. ) { // Air(0)
      fMaterialIndices.push_back(0);
    }else{                      // Water(1-)
      if ( density > fMaxDens ) {
	density = fMaxDens;
      }else if ( density < fMinDens ){
	density = fMinDens;
      }
      G4int matidx = G4int((density-fMinDens)/fDensReso+1.5);
      fMaterialIndices.push_back(matidx);
      //G4cout << " ivoxel= "<< i <<" matidx= "<<matidx<<G4endl;
    }
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
void GaletPhantom::CreateMaterialList(){
  //
  //
  fMaterialList.clear();
  fVisAtt.clear();
  //
  size_t Nmat = size_t((fMaxDens - fMinDens)/fDensReso+1) +1; //+1 is for Air
  for ( size_t i = 0; i < Nmat; i++){
    // Material
    G4Material* material=nullptr;
    G4double density=0.0;
    if ( i == 0 ) {
      material = fAir;
    }else{
       density = fMinDens+fDensReso*(i-1);
       material = FindOrCreateMaterial(fBaseMat->GetName(),density);
    }
    /*
    G4cout << " GaletPhantom::CreateMaterialList "
	   << " i = " << i << " " 
	   << density/(g/cm3)
	   << " " << material->GetName() <<G4endl;
    */
    fMaterialList.push_back(material);    
    //
    // VisAtt
    G4double c = ((G4double)i)/Nmat;
    G4VisAttributes* vatt=new G4VisAttributes();
    vatt->SetColour(c,c,c,0.8);
    vatt->SetForceSolid(true);
    vatt->SetVisibility(true);
    fVisAtt.push_back(vatt);
  }
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4LogicalVolume* GaletPhantom::ConstructPhantom(){
  //
  G4String phantomNameS("PhantomS");
  G4VSolid* phantomS = new G4Box(phantomNameS,
				 fNoVoxelX*fVoxelSPCX/2.,
				 fNoVoxelY*fVoxelSPCY/2.,
				 fNoVoxelZ*fVoxelSPCZ/2.);
  G4String phantomNameLV("PhantomLV");  
  fPhantomLV = new G4LogicalVolume(phantomS,fAir,phantomNameLV);
  //
  
    //----- Replication of Water Phantom Volume.
    //--- Y Slice
    G4String yRepName("RepY");
    G4VSolid* solYRep = new G4Box(yRepName,fNoVoxelX*fVoxelSPCX/2.,
                                  fVoxelSPCY/2.,
				  fNoVoxelZ*fVoxelSPCZ/2.);
    G4LogicalVolume* logYRep = new G4LogicalVolume(solYRep,fAir,yRepName);
    new G4PVReplica(yRepName,logYRep,fPhantomLV,kYAxis,
		    fNoVoxelY,fVoxelSPCY);

    logYRep->SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));

    //--- X Slice
    G4String xRepName("RepX");
    G4VSolid* solXRep = new G4Box(xRepName,fVoxelSPCX/2.,fVoxelSPCY/2.,
                                  fNoVoxelZ*fVoxelSPCZ/2.);
    G4LogicalVolume* logXRep = new G4LogicalVolume(solXRep,fAir,xRepName);
    new G4PVReplica(xRepName,logXRep,logYRep,kXAxis,fNoVoxelX,fVoxelSPCX);

    logXRep->SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));
    
    //----- Voxel solid and logical volumes
    //--- Z Slice
    G4VSolid* solVoxel = new G4Box("voxelS",fVoxelSPCX/2.,
				   fVoxelSPCY/2.,fVoxelSPCZ/2.);
    
    G4LogicalVolume* logicVoxel =
      new G4LogicalVolume(solVoxel,fAir,fVoxelLVName);
    if (!fVisOn){
      logicVoxel->
	SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));
    }

    //logicVoxel->
    //SetVisAttributes(new G4VisAttributes(G4VisAttributes::GetInvisible()));

    //
    // Parameterisation for transformation of voxels.
    //  (voxel size is fixed in this example.
    //    e.g. nested parameterisation handles material 
    //    and transfomation of voxels.)
    G4ThreeVector voxelSize(fVoxelSPCX/2.,fVoxelSPCY/2.,fVoxelSPCZ/2.);
    GaletNestedPhantomParameterisation* param =
      new GaletNestedPhantomParameterisation(voxelSize,
					     fMaterialList,
					     fMaterialIndices,
					     fNoVoxelX,fNoVoxelY,fNoVoxelZ);
    param->SetVisAttributes(fVisAtt);
    param->SetVisOn(fVisOn);

    new G4PVParameterised("phantom",    // their name
                          logicVoxel, // their logical volume
                          logXRep,      // Mother logical volume
                          kZAxis,       // Are placed along this axis
                          //kUndefined,
                          // Are placed along this axis
                          fNoVoxelZ,      // Number of cells
                          param);       // Parameterisation.

    // Score volume.
    fScoreVoxelLV = logicVoxel;

    return fPhantomLV;

}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4Material* GaletPhantom::FindOrCreateMaterial(const G4String& baseMatName,
					       G4double density){
  G4NistManager* nist = G4NistManager::Instance();  
  //
  G4Material* baseMat=nullptr;
  baseMat = G4Material::GetMaterial(baseMatName,FALSE);
  if ( baseMat == nullptr ){
    if ( (baseMat = nist->FindOrBuildMaterial(baseMatName)) == nullptr){
    const G4String& msg = "baseMaterial is not found " +baseMatName;
    G4Exception("GaletPhantom::CreateMaterial",
                "GaletPhantom00",FatalException,msg);
    }
  }
  //
  const G4String& givenName = GetMaterialName(baseMatName, density);
  G4double givenDensity = density;
  //
  G4Material* newMat=nullptr;
  newMat = G4Material::GetMaterial(givenName,false);
  if ( newMat ) return newMat;
  newMat =nullptr;
  newMat = nist->FindOrBuildMaterial(givenName);
  if ( newMat ) return newMat;
  //
  newMat = nist->BuildMaterialWithNewDensity(givenName, baseMatName, givenDensity);
  G4double Ipot = (baseMat->GetIonisation())->GetMeanExcitationEnergy();
  G4IonisParamMat* ionisPara = newMat->GetIonisation();
  ionisPara->SetMeanExcitationEnergy(Ipot);
  //
  return newMat;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
const G4String& GaletPhantom::GetMaterialName(const G4String& baseName, G4double density){
  if ( fDensReso == 0. || fMaxDens == 0. ){
    const G4String& msg = "DensReso or MaxDens is not set. ";
    G4Exception("GaletPhantom::GetMaterialName",
                "GaletPhantom00",FatalException,msg);
  }
  //  Air
  if( density == 0. ) return fAir->GetName();
  //
  //  Water
  if( density > fMaxDens ) density = fMaxDens;
  if( density < fMinDens ) density = fMinDens;  
  //
  G4float densBin = G4float(fDensReso)*(G4int((density)/fDensReso+0.5));
  //
  G4String* newName = new G4String(baseName);
  (*newName) += G4UIcommand::ConvertToString(densBin/(g/cm3));
  //
  return (*newName);
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
G4double GaletPhantom::GetDensity(G4int ix, G4int iy, G4int iz){
  G4int index = ix+fNoVoxelX*iy+(fNoVoxelX*fNoVoxelY)*iz;
  if ( index < (G4int)fMaterialIndices.size() ) {
    return fMaterialList[fMaterialIndices[index]]->GetDensity();
  }else{
    G4cerr <<"%%% GaletPhantom::GetDensity() error !! voxel index overflow"<<G4endl;
    exit(-1);
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
