//
// GaletMagneticField
//
//

#ifndef GaletMagneticField_H
#define GaletMagneticField_H 1

#include "globals.hh"
#include "G4MagneticField.hh"

#include "G4SystemOfUnits.hh"

class G4GenericMessenger;

/// Magnetic field

class GaletMagneticField : public G4MagneticField
{
  public:
    GaletMagneticField(const G4String& lvname);
    ~GaletMagneticField() override;

    void GetFieldValue(const G4double point[4],double* bField ) const override;

    void SetFieldX(G4double val) { fBx = val; }
    void SetFieldY(G4double val) { fBy = val; }
    void SetFieldZ(G4double val) { fBz = val; }  
    G4double GetFieldX() const { return fBx; }
    G4double GetFieldY() const { return fBy; }
    G4double GetFieldZ() const { return fBz; }  

  private:
    G4String fLVname ="default";
    void DefineCommands();

    G4GenericMessenger* fMessenger = nullptr;
    G4double fBx = 0.*tesla;  
    G4double fBy = 1.0*tesla;
    G4double fBz = 0.*tesla;    
};


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
