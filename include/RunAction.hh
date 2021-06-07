// Headers de RunAction.cc
#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"

class G4Run;

/// Clase tipo RunAction
///
/// Acumula la estadistica y calula energia depositada de las particulas dispersadas por medio de Root.
/// H1D Histograms son creados en BeginOfRunAction() para la energia depositada en cada capa
/// Opcionalmente se puede crear N-tuples, estos datos estadisticos seran arrojados con el nombre prueba.root
///
/// En EndOfRunAction() los datos estadisticos seran mostrados en pantalla como resumen.
///

class RunAction : public G4UserRunAction
{
  public:
    RunAction();
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#endif

