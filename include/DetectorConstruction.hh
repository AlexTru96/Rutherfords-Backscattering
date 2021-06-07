/// Headers de DetectorConstruction.cc

#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class G4VPhysicalVolume;
class G4GlobalMagFieldMessenger;

/// La clase DetectorConstruction permitira definir materiales y Geometrias.
/// Cada detector esta formado por 16 boxs (capas)de Silicio alineados, los cuales forman un conjunto de 6 detectores alrededor de un target girado 45 grados.
/// Las distancias y dimensiones se definiran en DetectorConstruction.cc
/// Cada detector se le definira como un SensitiveDetector que recoge la informacion del hit que llega a cada capa.

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
    DetectorConstruction();
    virtual ~DetectorConstruction();

  public:
    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();
     
  private:
    // metodos
    //
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();
  
    // Data members(Campo magnetico opcional)
    //
    static G4ThreadLocal G4GlobalMagFieldMessenger*  fMagFieldMessenger; 
                                      // llamada al campo magnetico 

   G4bool  fCheckOverlaps; // Permite verificar que no exista traslapes en las geometrias
   G4int   fNofLayers;     // Numero de capas
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

