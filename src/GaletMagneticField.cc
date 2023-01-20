//
// GaletMagneticField
//
//

#include "GaletMagneticField.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletMagneticField::GaletMagneticField(const G4String& lvname)
  : fLVname(lvname)
{
  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletMagneticField::~GaletMagneticField()
{
  delete fMessenger;
}

void GaletMagneticField::GetFieldValue(const G4double [4],double *bField) const
{
  bField[0] = fBx;
  bField[1] = fBy;
  bField[2] = fBz;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletMagneticField::DefineCommands()
{
  G4String cmdPath = "/Galet/";
  cmdPath += fLVname;
  cmdPath += "/field/";
  
  // Define /Galet/field command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      cmdPath,
                                      "Field control");

  // fieldValue command
  auto& xvalueCmd
    = fMessenger->DeclareMethodWithUnit("xvalue","tesla",
                                &GaletMagneticField::SetFieldX,
                                "Set x-field strength.");
  xvalueCmd.SetParameterName("Bx", true);
  xvalueCmd.SetDefaultValue("0.");
  
  auto& yvalueCmd
    = fMessenger->DeclareMethodWithUnit("yvalue","tesla",
                                &GaletMagneticField::SetFieldY,
                                "Set y-field strength.");
  yvalueCmd.SetParameterName("By", true);
  yvalueCmd.SetDefaultValue("0.");

  auto& zvalueCmd
    = fMessenger->DeclareMethodWithUnit("zvalue","tesla",
                                &GaletMagneticField::SetFieldZ,
                                "Set z-field strength.");
  zvalueCmd.SetParameterName("By", true);
  zvalueCmd.SetDefaultValue("0.");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
