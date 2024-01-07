#include "G4SystemOfUnits.hh"
#include "detector.hh"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Detector ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MySensitiveDetector::MySensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

MySensitiveDetector::~MySensitiveDetector()
{}

G4bool MySensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *)
{
    // G4cout << "Que show" << G4endl;
    G4Track *track = aStep->GetTrack();
    G4String particleName = track->GetDynamicParticle()->GetDefinition()->GetParticleName();
    if(particleName=="gamma"){
        G4ThreeVector pos = track->GetPosition();
        G4ThreeVector mom = track->GetMomentum();
        G4double energy = track->GetKineticEnergy();
        if(mom[2]>0){
        	std::ofstream archivo;
        	archivo.open("datos.txt",std::ios::app);
            archivo  << pos  << " " << mom << " " << energy << G4endl;
            archivo.close();
        }
    }
    return true;
}
