//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
// The code was written by :
//	^Claudio Andenna  claudio.andenna@ispesl.it, claudio.andenna@iss.infn.it
//      *Barbara Caccia barbara.caccia@iss.it
//      with the support of Pablo Cirrone (LNS, INFN Catania Italy)
//	with the contribute of Alessandro Occhigrossi*
//
// ^INAIL DIPIA - ex ISPESL and INFN Roma, gruppo collegato Sanità, Italy
// *Istituto Superiore di Sanità and INFN Roma, gruppo collegato Sanità, Italy
//  Viale Regina Elena 299, 00161 Roma (Italy)
//  tel (39) 06 49902246
//  fax (39) 06 49387075
//
// more information:
// http://g4advancedexamples.lngs.infn.it/Examples/medical-linac
//
//*******************************************************//

#include "G4FieldManager.hh"
#include "ML2AcceleratorConstruction.hh"
#include "ML2AcceleratorConstructionMessenger.hh"
#include "G4GenericMessenger.hh"
//#include "G4MagneticField.hh"
//#include "G4UniformMagField.hh"
//#include "G4FieldManager.hh"
//#include "G4GlobalMagFieldMessenger.hh"
#include "magneticfield.hh"

#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4Mag_UsualEqRhs.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4MaterialTable.hh"
#include "G4NistManager.hh"

#include "G4VSolid.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVParameterised.hh"
#include "G4PVReplica.hh"
#include "G4UserLimits.hh"

#include "G4SDManager.hh"
#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4GenericMessenger.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4ios.hh"
#include "G4SystemOfUnits.hh"

#include "G4MagIntegratorDriver.hh"
#include "G4AutoDelete.hh"


using namespace std;
G4ThreadLocal MagneticField* CML2AcceleratorConstruction::fMagneticField = nullptr;
G4ThreadLocal G4FieldManager* CML2AcceleratorConstruction::fFieldMgr = nullptr;


CML2AcceleratorConstruction::CML2AcceleratorConstruction(void)
{
	acceleratorConstructionMessenger = new CML2AcceleratorConstructionMessenger(this);
	idCurrentRotationX = 0;
}

CML2AcceleratorConstruction::~CML2AcceleratorConstruction(void)
{
	delete acceleratorConstructionMessenger;
}
CML2AcceleratorConstruction* CML2AcceleratorConstruction::instance = 0;

CML2AcceleratorConstruction* CML2AcceleratorConstruction::GetInstance(void)
{
  if (instance == 0)
    {
      instance = new CML2AcceleratorConstruction();
    }
  return instance;
}

void CML2AcceleratorConstruction::resetAccelerator()
{
		accelerator -> reset();
}

bool CML2AcceleratorConstruction::design(void)
{
// switch between different accelerators according to the main macro selection (actually only one accelerator is available)
	G4cout << "I'm building " << AcceleratorName << " accelerator" << G4endl;
	bool bAccExists = false;
	if (AcceleratorName == "acc1")
	{
		accelerator = CML2Acc1::GetInstance();
		bAccExists = true;
	}
	else if (AcceleratorName == "acc2")
	{
		accelerator = CML2Acc2::GetInstance();
		bAccExists = true;
	}
	else if (AcceleratorName == "accSaturn")
	{
		accelerator = CML2AccSaturn::GetInstance();
		bAccExists = true;
	}

	if (bAccExists && AcceleratorMacFileName!="")
	{
		// read the messenger data related to the selected accelerator
		G4UImanager* UI = G4UImanager::GetUIpointer();
		G4String command = "/control/execute ";
		UI->ApplyCommand(command+AcceleratorMacFileName); 
	}

 	if (rotationsX.size() < 1)
 	{
 		addAcceleratorRotationsX(0.);
 	}

	return bAccExists;
}
bool CML2AcceleratorConstruction::Construct(G4VPhysicalVolume *PVWorld, G4bool bOV)
{

	 		// a call to select the right accelerator
	bOnlyVisio = bOV;
	 
	 
	 

	if (design())
	{
//		G4cout << "*** debug *** AcceleratorConstruction::Construct" << G4endl;
		acceleratorConstructionMessenger->SetReferenceWorld(bOnlyVisio);
		// create the accelerator-world box
		
	G4double a;  // atomic mass
	G4double z;  // atomic number
	G4double density;
		G4Material *Vacuum = G4NistManager::Instance()->FindOrBuildMaterial("G4_Galactic");
				G4Material *air= G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
		//auto	Vacuum = new G4Material("Galactic", z=1., a=1.01*g/mole,density= 1.0e-20*g/cm3,	kStateGas, 2.73*kelvin, 3.e-18*pascal);
		 // auto air = G4Material::GetMaterial("G4_AIR");
		G4ThreeVector halfSize;
		initialCentre.set(0.*mm, 0.*mm, -isoCentre);
		halfSize.set(700.*mm, 700.*mm, 700.*mm);
		G4Box *accWorldB = new G4Box("accWorldG", halfSize.getX(), halfSize.getY(), halfSize.getZ());
		 accWorldLV = new G4LogicalVolume(accWorldB, Vacuum, "accWorldL", 0, 0, 0);
		G4VisAttributes* simpleAlSVisAtt = new G4VisAttributes(G4Colour::White());
		simpleAlSVisAtt -> SetVisibility(false);
		accWorldLV -> SetVisAttributes(simpleAlSVisAtt);
	
		PVAccWorld= new G4PVPlacement(0, initialCentre, "acceleratorBox", accWorldLV, PVWorld, false, 0);



 	 auto magneticSolid  = new G4Tubs("magneticTubs",0.,0.1*m,0.1*m,0.,360.*deg);

	 fMagneticLogical  = new G4LogicalVolume(magneticSolid,Vacuum  , "magneticLogical");

  	// placement of Tube
  	G4bool checkOverlaps = true;
  	auto fieldRot = new G4RotationMatrix();
  	fieldRot->rotateY(90.*deg);
	G4double MagPos_x = 0.0*m;
	G4double MagPos_y = 0.0*m;
//	G4double MagPos_z = 0.60055*m;
		G4double MagPos_z = -500.55*mm;
	      G4cout << "XXXXXXYYYYYYYYYYYYYYYValue of B04: "  << " T" << G4endl;
	new G4PVPlacement(fieldRot,G4ThreeVector(MagPos_x,MagPos_y,MagPos_z	),fMagneticLogical,
               "magneticPhysical",accWorldLV ,  false,0,checkOverlaps);
               

  // set step limit in tube with magnetic field
		  auto userLimits = new G4UserLimits(0.1 * m);
			  fMagneticLogical->SetUserLimits(userLimits);




		// create the actual accelerator
		accelerator -> Construct(PVAccWorld, isoCentre);
		Z_Value_PhaseSpaceBeforeJaws = accelerator -> getBeforeJaws_Z_PhaseSpacePosition();
		accelerator -> writeInfo();
		
		fMagneticField = new MagneticField();
	 fMagneticField ->SetField(0.020);
	 	G4cout <<    fMagneticField ->GetField() << " bueno1 "<<G4endl;
	 fFieldMgr = new G4FieldManager();
	      G4cout << "XXXXXXYYYYYYYYYYYYYYYValue of B05: "  << " T" << G4endl;
  fFieldMgr->SetDetectorField(fMagneticField);
  	      G4cout << "XXXXXXYYYYYYYYYYYYYYYValue of B05: "  << " T" << G4endl;
  fFieldMgr->CreateChordFinder(fMagneticField);
    	      G4cout << "XXXXXXYYYYYYYYYYYYYYYValue of B05: "  << " T" << G4endl;
  G4bool forceToAllDaughters = true;
 //  fMagneticField ->SetField(1.0);
//  fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);
  fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);
      	      G4cout << "XXXXXXYYYYYYYYYYYYYYYValue of B06: "  << " T" << G4endl;
      	      G4cout << "AcSSSSSSSSSS" << G4endl;
	
		
		
		

	}
	else
	{
		return false;
	}
	

	 
	
	
	
	return true;
}

void CML2AcceleratorConstruction::writeInfo()
{
	if (!bOnlyVisio)
	{
		G4cout << "Actual rotation: " << idCurrentRotationX << "/" << rotationsX.size() << "  " << G4endl;
	}
	G4cout << "Accelerator angle: " << currentRotationX/deg << " [deg]" << G4endl;
}

G4RotationMatrix * CML2AcceleratorConstruction::rotateAccelerator()
{
	G4RotationMatrix *rmInv=new G4RotationMatrix();
	if (idCurrentRotationX <(int) rotationsX.size())
	{
		currentRotationX = rotationsX[idCurrentRotationX];
		rmInv = rotateAccelerator(currentRotationX);
		idCurrentRotationX++;
	}
	else
	{
		rmInv = 0;
	}
	return rmInv;
}


void CML2AcceleratorConstruction::ConstructSDandField()
{

/*
MagneticField* fMyField = new MagneticField();

G4FieldManager* pFieldManager = new G4FieldManager();
pFieldManager->SetDetectorField(fMyField);


pFieldManager->CreateChordFinder(fMyField);
double epsilon = 1.0e-6;
pFieldManager->SetMaximumEpsilonStep( epsilon );
G4bool forceToContained = true; //overwrites any existing field managers
accWorldLV->SetFieldManager(pFieldManager, forceToContained); // in

G4AutoDelete::Register(fMagneticField);
G4AutoDelete::Register(pFieldManager);*/
  /*

  // magnetic field ----------------------------------------------------------
 fMagneticField = new MagneticField();
	      G4cout << "XXXXXXYYYYYYYYYYYYYYYValue of B04: "  << " T" << G4endl;
 // fMagneticField = new MagneticField();
  //fFieldMgr = new G4FieldManager();
fFieldMgr = new G4FieldManager();
	      G4cout << "XXXXXXYYYYYYYYYYYYYYYValue of B05: "  << " T" << G4endl;
  fFieldMgr->SetDetectorField(fMagneticField);
  	      G4cout << "XXXXXXYYYYYYYYYYYYYYYValue of B05: "  << " T" << G4endl;
  fFieldMgr->CreateChordFinder(fMagneticField);
    	      G4cout << "XXXXXXYYYYYYYYYYYYYYYValue of B05: "  << " T" << G4endl;
  G4bool forceToAllDaughters = true;
 //  fMagneticField ->SetField(1.0);
//  fMagneticLogical->SetFieldManager(fFieldMgr, forceToAllDaughters);
  accWorldLV->SetFieldManager(fFieldMgr, forceToAllDaughters);
      	      G4cout << "XXXXXXYYYYYYYYYYYYYYYValue of B06: "  << " T" << G4endl;
      	      G4cout << "AcSSSSSSSSSS" << G4endl;
      	      
G4AutoDelete::Register(fMagneticField);
G4AutoDelete::Register(fFieldMgr );
G4cout <<    fMagneticField ->GetField() << G4endl;
G4cout << "buenos " << G4endl;*/
}


G4RotationMatrix * CML2AcceleratorConstruction::rotateAccelerator(G4double angleX)
{
	currentRotationX = angleX;
	G4GeometryManager::GetInstance()->OpenGeometry();
	G4ThreeVector NewCentre;
	G4RotationMatrix *rm = new G4RotationMatrix();
	G4RotationMatrix *rmInv = new G4RotationMatrix();
	PVAccWorld->SetTranslation(initialCentre);
	PVAccWorld->SetRotation(rm);
	if (bRotate90Y)
	{
		rm->rotateY(90.*deg);
	}
	rm->rotateX(-angleX);
	PVAccWorld->SetRotation(rm);
	*rmInv=CLHEP::inverseOf(*rm);
	NewCentre=*rmInv*initialCentre;
	PVAccWorld->SetTranslation(NewCentre);
	G4GeometryManager::GetInstance()->CloseGeometry();
	G4RunManager::GetRunManager()->GeometryHasBeenModified();
	return rmInv;
}
G4String CML2AcceleratorConstruction::getCurrentRotationString()
{
	G4int cRI=(G4int)((currentRotationX/deg)+.5);
  G4String rotationName=std::to_string(cRI);

	if (bRotate90Y)
	{rotationName="_Ro90Y"+rotationName;}
	else
	{rotationName="_Ro"+rotationName;}
	return rotationName;
}



