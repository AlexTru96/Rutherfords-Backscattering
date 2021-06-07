//Implementacion de la clase RunAction 

#include "RunAction.hh"
#include "Analysis.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::RunAction()
 : G4UserRunAction()
{ 
  // Imprime el numero de evento en cada evento
  G4RunManager::GetRunManager()->SetPrintProgress(1);   
  // Creacion del analysis manager (ROOT)

  auto analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << G4endl;

  // Creando directorios 
  //analysisManager->SetHistoDirectoryName("histograms");
  //analysisManager->SetNtupleDirectoryName("ntuple");
  analysisManager->SetVerboseLevel(1);
  analysisManager->SetNtupleMerging(true);


  // Anotando histogramas, n-tuple
  //
  
  // Creando histogramas

  //analysisManager->CreateH1("Eabs","Edep in absorber", 100, 0.01*MeV, 6*MeV);
  const G4String id[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                         "10","11","12","13","14","15","16" 
                        };
                     
  // Valores default (/analysis/h1/set command)
               
  G4int nbins = 250;
  for (G4int k=0; k<16; k++) {
    analysisManager->CreateH1("d1s"+id[k], "S1D"+id[k], nbins, 0.01*MeV, 6*MeV);
  }
  for (G4int k=0; k<16; k++) {
    analysisManager->CreateH1("d2s"+id[k], "S2D"+id[k], nbins, 0.01*MeV, 6*MeV);
  }
  for (G4int k=0; k<16; k++) {
    analysisManager->CreateH1("d3s"+id[k], "S3D"+id[k], nbins, 0.01*MeV, 6*MeV);
  }
  for (G4int k=0; k<16; k++) {
    analysisManager->CreateH1("d4s"+id[k], "S4D"+id[k], nbins, 0.01*MeV, 6*MeV);
  }

  // Creando n-tuple
  //
  //analysisManager->CreateNtuple("B4", "Edep and TrackL");
  //analysisManager->CreateNtupleDColumn("Eabs");
  //analysisManager->CreateNtupleDColumn("Egap");
  //analysisManager->FinishNtuple();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

RunAction::~RunAction()
{
  delete G4AnalysisManager::Instance();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::BeginOfRunAction(const G4Run* /*run*/)
{ 
  //Informa al runManager para conservar la semilla (random seed)

  G4RunManager::GetRunManager()->SetRandomNumberStore(true);
  
  // Analysis manager
  auto analysisManager = G4AnalysisManager::Instance();

  // Abre el archivo root de salida
  // Aqui puedes cambiar el nombre del archivo root de salida, sgte update: si llamas al tercer argumento como el nombre root se modifique automaticamente al iniciar la simulacion :D
  G4String fileName = "prueba";
  analysisManager->OpenFile(fileName);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void RunAction::EndOfRunAction(const G4Run* )
{
  // Muestra las estadisticas de los histogramas
  //
  auto analysisManager = G4AnalysisManager::Instance();
  //if ( analysisManager->GetH1(1) ) {
   // G4cout << G4endl << " ----> print histograms statistic ";
    //if(isMaster) {
    //  G4cout << "for the entire run " << G4endl << G4endl; 
   // }
   // else {
    //  G4cout << "for the local thread " << G4endl << G4endl; 
    //}
    
    //G4cout << " EAbs : mean = " 
      // << G4BestUnit(analysisManager->GetH1(0)->mean(), "Energy") 
      // << " rms = " 
      // << G4BestUnit(analysisManager->GetH1(0)->rms(),  "Energy") << G4endl;


  // guarda los histogramas y n-tuples
  //
  analysisManager->Write();
  analysisManager->CloseFile();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
