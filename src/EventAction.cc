// Implementacion de la clase EventAction

#include "EventAction.hh"
#include "CalorimeterSD.hh"
#include "CalorHit.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"

#include "Randomize.hh"
#include <iomanip>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::EventAction()
 : G4UserEventAction(),
   fAbsHCID(-1),
   fAbsHCID2(-1),
   fAbsHCID3(-1),
   fAbsHCID4(-1)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

EventAction::~EventAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

CalorHitsCollection* 
EventAction::GetHitsCollection(G4int hcID,
                                  const G4Event* event) const
{
  auto hitsCollection 
    = static_cast<CalorHitsCollection*>(
        event->GetHCofThisEvent()->GetHC(hcID));
  
  if ( ! hitsCollection ) {
    G4ExceptionDescription msg;
    msg << "Cannot access hitsCollection ID " << hcID; 
    G4Exception("EventAction::GetHitsCollection()",
      "MyCode0003", FatalException, msg);
  }         

  return hitsCollection;
}    

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::PrintEventStatistics(G4double absoEdep) const
{
  // Muestra la estadistica del evento
  G4cout
     << "   Absorber: total energy: " 
     << std::setw(7) << G4BestUnit(absoEdep, "Energy")
     << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::BeginOfEventAction(const G4Event* /*event*/)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void EventAction::EndOfEventAction(const G4Event* event)
{  
  // Obtiene los hits collections IDs (solo 1 vez)
  if ( fAbsHCID == -1 ) {
    fAbsHCID 
      = G4SDManager::GetSDMpointer()->GetCollectionID("AbsorberHitsCollection");
  }
  if ( fAbsHCID2 == -1 ) {
    fAbsHCID2 
      = G4SDManager::GetSDMpointer()->GetCollectionID("AbsorberHitsCollection2");
  }
  if ( fAbsHCID3 == -1 ) {
    fAbsHCID3 
      = G4SDManager::GetSDMpointer()->GetCollectionID("AbsorberHitsCollection3");
  }
  if ( fAbsHCID4 == -1 ) {
    fAbsHCID4 
      = G4SDManager::GetSDMpointer()->GetCollectionID("AbsorberHitsCollection4");
  }
  // Obtiene los hits collections individuales
  auto absoHC = GetHitsCollection(fAbsHCID, event);
  auto absoHC2 = GetHitsCollection(fAbsHCID2, event);
  auto absoHC3 = GetHitsCollection(fAbsHCID3, event);
  auto absoHC4 = GetHitsCollection(fAbsHCID4, event);

  // Obtiene los hit con los valores totales
  auto absoHit = (*absoHC)[absoHC->entries()-1];
  auto absoHit2 = (*absoHC2)[absoHC2->entries()-1];
  auto absoHit3 = (*absoHC3)[absoHC3->entries()-1];
  auto absoHit4 = (*absoHC4)[absoHC4->entries()-1];

  // Muestra los eventos
  //
   auto eventID = event->GetEventID();
   auto printModulo = G4RunManager::GetRunManager()->GetPrintProgress();
   if ( ( printModulo > 0 ) && ( eventID % printModulo == 0 ) ) {
    G4cout << "---> End of event: " << eventID << G4endl;     

    PrintEventStatistics(absoHit->GetEdep());
    PrintEventStatistics(absoHit2->GetEdep());
    PrintEventStatistics(absoHit3->GetEdep());
    PrintEventStatistics(absoHit4->GetEdep());
  }  
  
  // Llenado de Histogramas y N-tuples
  //

  // Llamada al analysisManager
  auto analysisManager = G4AnalysisManager::Instance();
 
  // Llenado de histogramas

 for (G4int k=0; k<16; k++) {
    auto abs = (*absoHC)[k];
    analysisManager->FillH1(k, abs->GetEdep());
  }

 for (G4int k=0; k<16; k++) {
    auto abs2 = (*absoHC2)[k];
    analysisManager->FillH1(k+16, abs2->GetEdep());
  }

 for (G4int k=0; k<16; k++) {
    auto abs3 = (*absoHC3)[k];
    analysisManager->FillH1(k+32, abs3->GetEdep());
  }

 for (G4int k=0; k<16; k++) {
    auto abs4 = (*absoHC4)[k];
    analysisManager->FillH1(k+48, abs4->GetEdep());
  }

  //analysisManager->FillH1(0, absoHit->GetEdep());

  
  // fill ntuple
  //analysisManager->FillNtupleDColumn(0, absoHit->GetEdep());
  //analysisManager->AddNtupleRow();  
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
