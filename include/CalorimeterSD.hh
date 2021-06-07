// Headers de CalorimeterSD.cc
#ifndef CalorimeterSD_h
#define CalorimeterSD_h 1

#include "G4VSensitiveDetector.hh"

#include "CalorHit.hh"

#include <vector>

class G4Step;
class G4HCofThisEvent;

/// Clase del SensitiveDetector (Calorimeter)
///
/// En Initialize() crea un hit para cada capa del calorimetro y uno mas para el total de capas (cuenta total)
/// Los valores son guardados en ProcessHits() ,funcion que es llamada por el Kernel G4 por cada paso.

class CalorimeterSD : public G4VSensitiveDetector
{
  public:
    CalorimeterSD(const G4String& name, 
                     const G4String& hitsCollectionName, 
                     G4int nofCells);
    virtual ~CalorimeterSD();
  
    // Metodos
    virtual void   Initialize(G4HCofThisEvent* hitCollection);
    virtual G4bool ProcessHits(G4Step* step, G4TouchableHistory* history);
    virtual void   EndOfEvent(G4HCofThisEvent* hitCollection);

  private:
    CalorHitsCollection* fHitsCollection;
    G4int  fNofCells;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

