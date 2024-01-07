
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
/// \file MagneticField.cc
/// \brief Implementation of the B5::MagneticField class
#include "ML2AcceleratorConstructionMessenger.hh"
#include "magneticfield.hh"

#include "G4GenericMessenger.hh"
#include "G4SystemOfUnits.hh"
#include "globals.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::MagneticField()
{
  // define commands for this class
 //DefineCommands();
  G4cout << "defininiton------------------------------------------------"  << " T" << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

MagneticField::~MagneticField()
{
  //delete fMessenger;
      G4cout << "destructor--------------------------------------------- "  << " T" << G4endl;
}


void MagneticField::GetFieldValue(const G4double position[4],double *bField) const
{
   bField[0] = 0;
  bField[1] = fBy;
  bField[2] = 0.0;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void MagneticField::DefineCommands()
{
  fMessenger = new G4GenericMessenger(this,
                                      "/B5/field/",
                                      "Field control");

  // fieldValue command
  auto& valueCmd
    = fMessenger->DeclareMethodWithUnit("value","tesla",
                                &MagneticField::SetField,
                                "Set field strength.");
  valueCmd.SetParameterName("field", true);
  valueCmd.SetDefaultValue("1.");
  
      G4cout << "commnads--------------------------------------- "  << " T" << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
