//
// GaletNestedPhantomParameterisation
//

#ifndef GaletNestedPhantomParamterisation_hh
#define GaletNestedPhantomParamterisation_hh

#include <vector>

#include "G4Types.hh"
#include "G4ThreeVector.hh"
#include "G4VNestedParameterisation.hh"

class G4VPhysicalVolume;
class G4VTouchable;
class G4VSolid;
class G4Material;
class G4VisAttributes;

// CSG Entities which may be parameterised/replicated
//
class G4Box;
class G4Tubs;
class G4Trd;
class G4Trap;
class G4Cons;
class G4Sphere;
class G4Ellipsoid;
class G4Orb;
class G4Torus;
class G4Para;
class G4Polycone;
class G4Polyhedra;
class G4Hype;

/// Implements a G4VNestedParameterisation

class GaletNestedPhantomParameterisation : public G4VNestedParameterisation
{
public:

  GaletNestedPhantomParameterisation(const G4ThreeVector& voxelSize,
				     std::vector<G4Material*>& matlist,
				     std::vector<G4int>& matindices,
				     G4int fnX_ = 0, G4int fnY_ = 0, G4int fnZ_ = 0);

  ~GaletNestedPhantomParameterisation();

  void SetVisAttributes(std::vector<G4VisAttributes*>& visAtt)
  { fVisAtt = visAtt; };
  void SetVisOn(G4bool on){ fVisOn = on; };
  
  G4Material* ComputeMaterial(G4VPhysicalVolume *currentVol,
                              const G4int repNo,
                              const G4VTouchable *parentTouch );
  // Must cope with parentTouch for navigator's SetupHierarchy

  G4int       GetNumberOfMaterials() const;
  G4Material* GetMaterial(G4int idx) const;
  // Needed to define materials for instances of Nested Parameterisation
  // Current convention: each call should return the materials
  // of all instances with the same mother/ancestor volume

  void ComputeTransformation(const G4int no,
                             G4VPhysicalVolume *currentPV) const;

  // Additional standard Parameterisation methods,
  // which can be optionally defined, in case solid is used.
  void ComputeDimensions(G4Box &, const G4int,
                         const G4VPhysicalVolume *) const;

private:  // Dummy declarations to get rid of warnings ...

  void ComputeDimensions (G4Trd&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Trap&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Cons&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Sphere&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Ellipsoid&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Orb&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Torus&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Para&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Hype&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Tubs&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Polycone&, const G4int,
                          const G4VPhysicalVolume*) const {}
  void ComputeDimensions (G4Polyhedra&, const G4int,
                          const G4VPhysicalVolume*) const {}

  using G4VNestedParameterisation::ComputeMaterial;

private:

  G4double fdX=0.,fdY=0.,fdZ=0.;
  G4int fnX=0,fnY=0,fnZ=0;
  std::vector<G4Material*> fMaterialList;
  std::vector<G4int> fMaterialIndices; // Index in materials corresponding to each voxel
  std::vector<G4VisAttributes*> fVisAtt;
  std::vector<G4double> fpZ;
  //
  G4bool fVisOn=true;
};

#endif
