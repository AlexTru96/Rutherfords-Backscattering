
/// Headers de EventAction.cc

#ifndef EventAction_h
#define EventAction_h 1

#include "G4UserEventAction.hh"

#include "CalorHit.hh"

#include "globals.hh"

/// Clase tipo EventAction
///
/// En EndOfEventAction() imprime las cantidades acumulades de la energia depositada en las capas del detector guardadas en los hits collections.

class EventAction : public G4UserEventAction
{
public:
  EventAction();
  virtual ~EventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);
    
private:
  // metodos
  CalorHitsCollection* GetHitsCollection(G4int hcID,
                                            const G4Event* event) const;
  void PrintEventStatistics(G4double absoEdep) const;
  
  // data members                   
  G4int  fAbsHCID;
  G4int  fAbsHCID2;
  G4int  fAbsHCID3;
  G4int  fAbsHCID4;
};
                     
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
