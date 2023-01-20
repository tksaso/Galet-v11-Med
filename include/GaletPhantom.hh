//
// GaletPhantom
//
#ifndef GaletPhantom_h
#define GaletPhantom_h 1
//
#include "globals.hh"
#include "G4String.hh"
#include "G4SystemOfUnits.hh"
//
class GaletNestedPhantomParameterisation;
class G4Material;
class G4LogivalVolume;
class G4VisAttributes;
//
class GaletPhantom{
public:
  GaletPhantom(const G4String& voxelLVname);
  ~GaletPhantom();

  void ReadVTK(const G4String& vtkFile);
  void ReadDummyVTK(G4double dx=15.*cm,
		    G4double dy=15.*cm,
		    G4double dz=25.*cm,
		    G4int nx=10,G4int ny=10, G4int nz=10,
		    G4bool onSurfIso=false,
		    G4bool mosaic=false);
  
  G4LogicalVolume* ConstructPhantom();
  G4ThreeVector& GetCenterCoord(){ return fCenterCoord; };
  G4LogicalVolume* GetScoreLV(){ return fScoreVoxelLV;  };
  G4Material* FindOrCreateMaterial(const G4String& baseMatName, G4double density);
  void CreateMaterialList();
  //
  void SetBaseMaterial(G4Material* basemat){ fBaseMat = basemat; };
  void SetDensityResolution(G4double densReso){ fDensReso=densReso;};
  void SetMaxDensity(G4double maxDensity){ fMaxDens = maxDensity; };
  void SetMinDensity(G4double minDensity){ fMinDens = minDensity; };
  void SetVisOn(G4bool on){ fVisOn = on; };
  //
  G4ThreeVector& GetOrigin() { return fOrigin; };
  G4double       GetVoxelSPCX(){ return fVoxelSPCX;} ;
  G4double       GetVoxelSPCY(){ return fVoxelSPCY;} ;
  G4double       GetVoxelSPCZ(){ return fVoxelSPCZ;} ;  
  G4int          GetNoVoxelX(){ return fNoVoxelX;};
  G4int          GetNoVoxelY(){ return fNoVoxelX;};
  G4int          GetNoVoxelZ(){ return fNoVoxelX;};
  G4double       GetDensity(G4int ix, G4int iy, G4int iz);
  
protected:
  const G4String& GetMaterialName(const G4String&baseName, G4double density);

private:
  //
  G4String fVoxelLVName = "";
  //
  G4double fDensReso = 0.01*g/cm3;
  G4double fMinDens  = 0.1*g/cm3;  
  G4double fMaxDens  = 2.8*g/cm3;
  G4Material*  fBaseMat=nullptr;  
  G4Material*  fAir=nullptr;
  std::vector<G4Material*> fMaterialList;
  std::vector<G4int>  fMaterialIndices;
  std::vector<G4VisAttributes*> fVisAtt;
  //
  G4ThreeVector fOrigin{0,0,0};
  G4ThreeVector fCenterCoord{0,0,0};
  G4double fVoxelSPCX=0.;
  G4double fVoxelSPCY=0.;
  G4double fVoxelSPCZ=0.;
  G4int    fNoVoxelX=1;
  G4int    fNoVoxelY=1;
  G4int    fNoVoxelZ=1;
  //
  G4LogicalVolume* fPhantomLV=nullptr;
  G4LogicalVolume* fScoreVoxelLV=nullptr;
  //
  G4bool fVisOn=true;
};
#endif



