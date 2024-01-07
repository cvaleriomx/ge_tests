#ifndef DETECTOR_HH
#define DETECTOR_HH

#include "G4VSensitiveDetector.hh"
// #include "G4Step.hh"
// #include "G4TouchableHistory.hh"
// #include "run.hh"
#include "G4RunManager.hh"
// #include "construction.hh"
// #include "generator.hh"
#include <iostream>
#include <fstream>
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ SensitiveDetector ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class MySensitiveDetector : public G4VSensitiveDetector
{
public:
    MySensitiveDetector(G4String);
    ~MySensitiveDetector();
private:
    virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
    G4int pID_cont=0, pID_cont_out=0;
};
#endif
