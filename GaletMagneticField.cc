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
  :fLVname(lvname)  
{
  // define commands for this class
  DefineCommands();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

GaletMagneticField::~GaletMagneticField()
{
  delete fMessenger;
}

void MagneticField::GetFieldValue(const G4double [4],double *bField) const
{
  bField[0] = fBx;
  bField[1] = fBy;
  bField[2] = fBz;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void GaletMagneticField::DefineCommands()
{
  G4String cmdPath ="/Galet";
  cmdPathh.Add
  // Define /B5/field command directory using generic messenger class
  fMessenger = new G4GenericMessenger(this,
                                      "/Galet/field/",
                                      "Field control");

  // fieldValue command
  auto& xvalueCmd
    = fMessenger->DeclareMethodWithUnit("xvalue","tesla",
                                &MagneticField::SetFieldX,
                                "Set x-field strength.");
  xvalueCmd.SetParameterName("Bx", true);
  xvalueCmd.SetDefaultValue("0.");
  
  auto& yvalueCmd
    = fMessenger->DeclareMethodWithUnit("yvalue","tesla",
                                &MagneticField::SetFieldY,
                                "Set y-field strength.");
  yvalueCmd.SetParameterName("By", true);
  yvalueCmd.SetDefaultValue("0.");

  auto& zvalueCmd
    = fMessenger->DeclareMethodWithUnit("zvalue","tesla",
                                &MagneticField::SetFieldZ,
                                "Set z-field strength.");
  zvalueCmd.SetParameterName("By", true);
  zvalueCmd.SetDefaultValue("0.");

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
