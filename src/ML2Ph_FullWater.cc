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

#include "ML2Ph_FullWater.hh"
#include "ML2Ph_FullWaterMessenger.hh"

#include "G4SystemOfUnits.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SDManager.hh"
#include "G4PVReplica.hh"
#include "CLHEP/Units/SystemOfUnits.h"


using CLHEP::m;
using CLHEP::cm3;
using CLHEP::mole;
using CLHEP::g;
using CLHEP::mg;
using CLHEP::perCent;

CML2Ph_FullWater::CML2Ph_FullWater()
{
  // phantom size and position
  G4double tamano=275.0;
  halfSize.set(tamano*mm,tamano*mm,tamano*mm);
  // phantom position
  centre.set(0.,0.,0.);
  
  fPhantomSize.setX(2*tamano*mm);
  fPhantomSize.setY(2*tamano*mm);
  fPhantomSize.setZ(2*tamano*mm);
  fullWaterMessenger = new CML2Ph_FullWaterMessenger(this);
}

CML2Ph_FullWater::~CML2Ph_FullWater(void)
{
}

void CML2Ph_FullWater::writeInfo()
{
	G4cout<<"\n\n\tcentre of the phantom: " <<centre/mm<<" [mm]"<< G4endl;
	G4cout<<"\thalf thickness of the phantom: " <<halfSize/mm<<" [mm]\n"<< G4endl;
}
/*
bool CML2Ph_FullWater::Construct(G4VPhysicalVolume *PWorld, G4int nx, G4int ny, G4int nz)
{

 PVWorld=PWorld;

 bool bCreated=false;
 G4Material *WATER=G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
 G4Box *fullWaterPhantomBox = new G4Box("fullWaterPhantomBox", halfSize.getX(), halfSize.getY(), halfSize.getZ());
 G4LogicalVolume *fullWaterPhantomLV = new G4LogicalVolume(fullWaterPhantomBox, WATER, "fullWaterPhantomLV", 0, 0, 0);
 fullWaterPhantomPV = new G4PVPlacement(0, centre, "fullWaterPhantomPV", fullWaterPhantomLV, PVWorld, false, 0);

 
  G4int nxCells = nx;
  G4int nyCells = ny;
  G4int nzCells = nz;

  G4cout << "VoxelX, voxelY, Voxelz = " << nx << ", " << ny <<  ", " << nz << "; " << G4endl;

  G4ThreeVector sensSize;
  sensSize.setX(fPhantomSize.x()/(G4double)nxCells);
  sensSize.setY(fPhantomSize.y()/(G4double)nyCells);
  sensSize.setZ(fPhantomSize.z()/(G4double)nzCells);

  // The phantom is voxelised in 3D

  G4String yRepName("RepY");

  G4VSolid* solYRep = new G4Box(yRepName,fPhantomSize.x()/2.,sensSize.y()/2.,fPhantomSize.z()/2.);

  G4LogicalVolume* logYRep = new G4LogicalVolume(solYRep,WATER,yRepName);
 
  new G4PVReplica(yRepName,logYRep,fullWaterPhantomLV,kYAxis,ny,sensSize.y());

  G4String xRepName("RepX");

  G4VSolid* solXRep = new G4Box(xRepName,sensSize.x()/2.,sensSize.y()/2.,fPhantomSize.z()/2.);
  
  G4LogicalVolume* logXRep = new G4LogicalVolume(solXRep,WATER,xRepName);
  
  new G4PVReplica(xRepName,logXRep,logYRep,kXAxis,nx,sensSize.x());
 
  G4String zVoxName("phantomSens");
 
  G4VSolid* solVoxel = new G4Box(zVoxName,sensSize.x()/2.,sensSize.y()/2.,sensSize.z()/2.);
  
  G4LogicalVolume* LVPhantomSens = new G4LogicalVolume(solVoxel,WATER,zVoxName); // This is the Sensitive Volume

  new G4PVReplica(zVoxName,LVPhantomSens,logXRep,kZAxis,nz,sensSize.z());
//..............................................
  // Phantom segmentation using Parameterisation
  //..............................................
 
  G4cout << "  Water Phantom Size " << fPhantomSize/mm       << G4endl;
  G4cout << "  Segmentation  ("<< nx<<","<<ny<<","<<nz<<")"<< G4endl;
  
  // Region for cuts
  G4Region *regVol= new G4Region("fullWaterPhantomR");
  G4ProductionCuts* cuts = new G4ProductionCuts;
  cuts->SetProductionCut(0.1*mm);
  regVol->SetProductionCuts(cuts);

	fullWaterPhantomLV->SetRegion(regVol);
	regVol->AddRootLogicalVolume(fullWaterPhantomLV);

	// Visibility
	G4VisAttributes* simpleAlSVisAtt= new G4VisAttributes(G4Colour::Red());
	simpleAlSVisAtt->SetVisibility(true);
// 	simpleAlSVisAtt->SetForceSolid(true);
	fullWaterPhantomLV->SetVisAttributes(simpleAlSVisAtt);

	G4MultiFunctionalDetector* myScorer = new G4MultiFunctionalDetector("PhantomSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(myScorer);
	LVPhantomSens->SetSensitiveDetector(myScorer);
       
	G4VPrimitiveScorer * totalDose = new G4PSDoseDeposit3D("TotalDose", nx,ny,nz);
	myScorer->RegisterPrimitive(totalDose);
	G4cout << "scorer registered: totalDose" << G4endl;

	bCreated=true;
	return bCreated;
}*/

bool CML2Ph_FullWater::Construct(G4VPhysicalVolume *PWorld, G4int nx, G4int ny, G4int nz)
{

 PVWorld=PWorld;

G4double A, Z;
	A = 1.01*g/mole;
	G4Element* elH = new G4Element ("Hydrogen","H",Z = 1.,A);

	A = 12.011*g/mole;
	G4Element* elC = new G4Element("Carbon","C",Z = 6.,A);  

	A = 16.00*g/mole;
  G4double d= 1.18*g/cm3;
	G4int natoms, ncomponents;
	G4Element* elO = new G4Element("Oxygen","O",Z = 8.,A);
 G4Material* PMMA = new G4Material("Polimetilmetacrilato",d,ncomponents=3);
	PMMA->AddElement(elC, natoms=5);
	PMMA->AddElement(elH, natoms=8);
	PMMA->AddElement(elO, natoms=2);

    G4String name, symbol;
    G4double z, a, density;
    
    G4Element* elN = new G4Element( name = "Nitrogen",
                                   symbol = "N",
                                   z = 7.0, a = 14.007 * g/mole );
    G4Element* elNa = new G4Element( name = "Sodium",
                                    symbol = "Na",
                                    z= 11.0, a = 22.98977* g/mole );
    G4Element* elMg = new G4Element( name = "Magnesium",
                                    symbol = "Mg",
                                    z = 12.0, a = 24.3050* g/mole );
    G4Element* elP = new G4Element( name = "Phosphorus",
                                   symbol = "P",
                                   z = 15.0, a = 30.973976* g/mole );
    G4Element* elS = new G4Element( name = "Sulfur",
                                   symbol = "S",
                                   z = 16.0,a = 32.065* g/mole );
    G4Element* elCl = new G4Element( name = "Chlorine",
                                    symbol = "P",
                                    z = 17.0, a = 35.453* g/mole );
    G4Element* elK = new G4Element( name = "Potassium",
                                   symbol = "P",
                                   z = 19.0, a = 30.0983* g/mole );

    G4Element* elFe = new G4Element( name = "Iron",
                                    symbol = "Fe",
                                    z = 26, a = 56.845* g/mole );
 
    G4Element* elCa = new G4Element( name="Calcium",
                                    symbol = "Ca",
                                    z = 20.0, a = 40.078* g/mole );

    G4Element* elZn = new G4Element( name = "Zinc",
                                   symbol = "Zn",
                                   z = 30.0,a = 65.382* g/mole );

    // Creating Materials :
    G4int numberofElements;

    // Air


    G4Material* fAir = new G4Material( "Air",
                          1.290*mg/cm3,
                          numberofElements = 2 );
    fAir->AddElement(elN, 0.7);
    fAir->AddElement(elO, 0.3);


   // Soft tissue (ICRP - NIST)
    G4Material* softTissue = new G4Material ("SoftTissue", 1.00*g/cm3, 
                                             numberofElements = 13);
    softTissue->AddElement(elH, 10.4472*perCent);
    softTissue->AddElement(elC, 23.219*perCent);
    softTissue->AddElement(elN, 2.488*perCent);
    softTissue->AddElement(elO, 63.0238*perCent);
    softTissue->AddElement(elNa, 0.113*perCent);
    softTissue->AddElement(elMg, 0.0113*perCent);
    softTissue->AddElement(elP, 0.113*perCent);
    softTissue->AddElement(elS, 0.199*perCent);
    softTissue->AddElement(elCl, 0.134*perCent);
    softTissue->AddElement(elK, 0.199*perCent);
    softTissue->AddElement(elCa, 0.023*perCent);
    softTissue->AddElement(elFe, 0.005*perCent);
    softTissue->AddElement(elZn, 0.003*perCent);


G4Material *mangoTissue = new G4Material ("mangoTissue", 1.00*g/cm3, 
                                             numberofElements = 8);
    mangoTissue->AddElement(elH, 10.18*perCent);
    mangoTissue->AddElement(elC, 7.01*perCent);
    mangoTissue->AddElement(elN, 0.13*perCent);
    mangoTissue->AddElement(elO, 82.40*perCent);
    mangoTissue->AddElement(elMg, 0.01*perCent);
    mangoTissue->AddElement(elP, 0.02*perCent);
    mangoTissue->AddElement(elK, 0.23*perCent);
    mangoTissue->AddElement(elCa, 0.02*perCent);
    




 bool bCreated=false;
 G4Material *WATER=G4NistManager::Instance()->FindOrBuildMaterial("G4_WATER");
 G4Material *aire=G4NistManager::Instance()->FindOrBuildMaterial("G4_AIR");
 
 G4Box *fullWaterPhantomBox = new G4Box("fullWaterPhantomBox", halfSize.getX(), halfSize.getY(), halfSize.getZ());
 G4LogicalVolume *fullWaterPhantomLV = new G4LogicalVolume(fullWaterPhantomBox, aire, "fullWaterPhantomLV", 0, 0, 0);
 fullWaterPhantomPV = new G4PVPlacement(0, centre, "fullWaterPhantomPV", fullWaterPhantomLV, PVWorld, false, 0);

	


  G4int nxCells = nx;
  G4int nyCells = ny;
  G4int nzCells = nz;

  G4cout << "VoxelX, voxelY, Voxelz = " << nx << ", " << ny <<  ", " << nz << "; " << G4endl;

  G4ThreeVector sensSize;
  sensSize.setX(fPhantomSize.x()/(G4double)nxCells);
  sensSize.setY(fPhantomSize.y()/(G4double)nyCells);
  sensSize.setZ(fPhantomSize.z()/(G4double)nzCells);

  // The phantom is voxelised in 3D

  G4String yRepName("RepY");

  G4VSolid* solYRep = new G4Box(yRepName,fPhantomSize.x()/2.,sensSize.y()/2.,fPhantomSize.z()/2.);
  // G4VSolid* solYRep = new G4Orb(yRepName,fPhantomSize.y()/20);

  G4LogicalVolume* logYRep = new G4LogicalVolume(solYRep,WATER,yRepName);
   G4cout << "Antes replicaXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX"<<G4endl;
  new G4PVReplica(yRepName,logYRep,fullWaterPhantomLV,kYAxis,ny,sensSize.y());
   G4cout << "despues replicaXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX "<<fPhantomSize.x()/2<<G4endl;
  G4String xRepName("RepX");

  G4VSolid* solXRep = new G4Box(xRepName,sensSize.x()/2.,sensSize.y()/2.,fPhantomSize.z()/2.);
  //   G4VSolid* solXRep = new G4Orb(xRepName,fPhantomSize.x()/2.5);
  
  G4LogicalVolume* logXRep = new G4LogicalVolume(solXRep,WATER,xRepName);
  
  new G4PVReplica(xRepName,logXRep,logYRep,kXAxis,nx,sensSize.x());
 
  G4String zVoxName("phantomSens");
 
  G4VSolid* solVoxel = new G4Box(zVoxName,sensSize.x()/2.,sensSize.y()/2.,sensSize.z()/2.);
  //G4VSolid* solVoxel = new G4Orb(xRepName,sensSize.x()/4);
  //AQUI CREAS EL VOLUMEN SENSIBLE 
  // G4VSolid*  solVoxel = new G4Ellipsoid(xRepName,sensSize.x()/2,sensSize.x()/4,sensSize.x()/2);
   //G4Ellipsoid (const G4String &pName, G4double pxSemiAxis, G4double pySemiAxis, G4double pzSemiAxis, G4double pzBottomCut=0, G4double pzTopCut=0)
  G4LogicalVolume* LVPhantomSens = new G4LogicalVolume(solVoxel,mangoTissue,zVoxName); // This is the Sensitive Volume

  new G4PVReplica(zVoxName,LVPhantomSens,logXRep,kZAxis,nz,1.0*sensSize.z(),0.01);
//..............................................
  // Phantom segmentation using Parameterisation
  //..............................................
 
  G4cout << "  Water Phantom Size " << fPhantomSize/mm       << G4endl;
  G4cout << "  Segmentation  ("<< nx<<","<<ny<<","<<nz<<")"<< G4endl;
  
  // Region for cuts
  G4Region *regVol= new G4Region("fullWaterPhantomR");
  G4ProductionCuts* cuts = new G4ProductionCuts;
  cuts->SetProductionCut(0.1*mm);
  regVol->SetProductionCuts(cuts);

	fullWaterPhantomLV->SetRegion(regVol);
	regVol->AddRootLogicalVolume(fullWaterPhantomLV);

	// Visibility
	G4VisAttributes* simpleAlSVisAtt= new G4VisAttributes(G4Colour::Red());
	simpleAlSVisAtt->SetVisibility(true);
// 	simpleAlSVisAtt->SetForceSolid(true);
	fullWaterPhantomLV->SetVisAttributes(simpleAlSVisAtt);

	G4MultiFunctionalDetector* myScorer = new G4MultiFunctionalDetector("PhantomSD");
	G4SDManager::GetSDMpointer()->AddNewDetector(myScorer);
	LVPhantomSens->SetSensitiveDetector(myScorer);
       
	G4VPrimitiveScorer * totalDose = new G4PSDoseDeposit3D("TotalDose", nx,ny,nz);
	myScorer->RegisterPrimitive(totalDose);
	G4cout << "scorer registered: totalDose" << G4endl;

	bCreated=true;
	return bCreated;
}


