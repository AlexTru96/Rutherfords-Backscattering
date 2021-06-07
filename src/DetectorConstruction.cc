// Implementacion de la clase DetectorConstruction
#include "DetectorConstruction.hh"
#include "CalorimeterSD.hh"
#include "G4Material.hh"
#include "G4NistManager.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4GlobalMagFieldMessenger.hh"
#include "G4AutoDelete.hh"

#include "G4SDManager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal 
G4GlobalMagFieldMessenger* DetectorConstruction::fMagFieldMessenger = 0; 

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::DetectorConstruction()
 : G4VUserDetectorConstruction(),
   fCheckOverlaps(true),
   fNofLayers(-1)
{
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

DetectorConstruction::~DetectorConstruction()
{ 
}  

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::Construct()
{
  // Definicion de los materiales
  DefineMaterials();
  
  // Definicion de los volumenes
  return DefineVolumes();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::DefineMaterials()
{ 
  // Plomo
  auto nistManager = G4NistManager::Instance();
  nistManager->FindOrBuildMaterial("G4_Pb");
  
  // Material Argon Liquido
  G4double a;  // masa de un mol
  G4double z;  // z=numero atomico
  G4double density; // densidad del material
  G4int ncomponents, natoms; //numero de elementos y numero de atomos
  new G4Material("liquidArgon", z=18., a= 39.95*g/mole, density= 1.390*g/cm3);

  // Vacio
  new G4Material("Galactic", z=1., a=1.01*g/mole,density= universe_mean_density,
                  kStateGas, 2.73*kelvin, 3.e-18*pascal);

 //Materiales para el target y detectores
  new G4Material("Gold",     z=79, a=196.97*g/mole, density= 19.32*g/cm3);
  new G4Material("Silicon"  , z=14, a=28.09*g/mole, density= 2.330*g/cm3);
  G4Isotope* c14 = new G4Isotope("c14", 6, 8, 0.0, 0);
  G4Element* ic14 = new G4Element("Carbono14" ,"ic14", ncomponents=1);
  ic14->AddIsotope(c14, 1.0);
  G4Material* C14 = 
  new G4Material("C14",density= 2.1*g/cm3, ncomponents=1);
  C14->AddElement(ic14, natoms=1);

  // Mostrar materiales disponibles
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
  // Parametros de la geometria
  fNofLayers = 16; 
  G4double absoThickness = 30*um; 
  G4double calorSizeX  = 50.*mm; 
  G4double absoY  = 3.*mm; 

  auto layerThickness = absoThickness;
  auto calorThickness = layerThickness;
  auto calorSizeY = fNofLayers*absoY;
  auto worldSizeXY = 26.*cm;
  auto worldSizeZ  = 26.*cm;
  
  // Definiendo materiales para las geometrias
  auto defaultMaterial = G4Material::GetMaterial("Galactic");
  auto absorberMaterial = G4Material::GetMaterial("Silicon");
  auto targetMaterial = G4Material::GetMaterial("Gold");
  
  if ( ! defaultMaterial || ! absorberMaterial) {
    G4ExceptionDescription msg;
    msg << "Cannot retrieve materials already defined."; 
    G4Exception("DetectorConstruction::DefineVolumes()",
      "MyCode0001", FatalException, msg);
  }  
   
  //     
  // World
  //
  auto worldS 
    = new G4Box("World",           // Nombre
                 worldSizeXY/2, worldSizeXY/2, worldSizeZ/2); // Tamaño
                         
  auto worldLV
    = new G4LogicalVolume(
                 worldS,           // Solido a que pertenece
                 defaultMaterial,  // Material
                 "World");         // Nombre
                                   
  auto worldPV
    = new G4PVPlacement(
                 0,                // Rotacion
                 G4ThreeVector(),  // Ubicacion
                 worldLV,          // Volumen logico                         
                 "World",          // Nombre
                 0,                // mother volume
                 false,            // no hay operacion booleana
                 0,                // numero de copias
                 fCheckOverlaps);  // verificando traslapes 
  
  //                               
  // Detector 1
  //  

  auto calorimeterS
    = new G4Box("Calorimeter",     // Nombre
                 calorSizeX/2, calorSizeY/2, calorThickness/2); // Tamaño
                         
  auto calorLV
    = new G4LogicalVolume(
                 calorimeterS,     // Solido a que pertenece
                 defaultMaterial,  // Material
                 "Calorimeter");   // Nombre

  G4RotationMatrix* fieldRot1 = new G4RotationMatrix(); //Definiendo rotacion
  fieldRot1->rotateX(30.*deg); 

  new G4PVPlacement(
		 fieldRot1,                	    // Rotacion
                 G4ThreeVector(0*um,6*cm,10.3923*cm),  // Ubicacion
                 calorLV,          // Volumen logico                         
                 "Calorimeter",    // Nombre
                 worldLV,          // mother volume
                 false,            // no hay operacion booleana
                 0,                // numero de copias
                 fCheckOverlaps);  // verificando traslapes 
  //                               
  // Detector 2
  //  

  auto calorimeterS2
    = new G4Box("Calorimeter2",
                 calorSizeX/2, calorSizeY/2, calorThickness/2);
                         
  auto calorLV2
    = new G4LogicalVolume(
                 calorimeterS2,    
                 defaultMaterial,  
                 "Calorimeter2");   

  G4RotationMatrix* fieldRot2 = new G4RotationMatrix();
  fieldRot2->rotateX(70.*deg); 

  new G4PVPlacement(
		 fieldRot2,                
                 G4ThreeVector(0*um,11.2763*cm,4.1042*cm),  
                 calorLV2,                                  
                 "Calorimeter2",   
                 worldLV,          
                 false,            
                 0,              
                 fCheckOverlaps); 

  //                               
  // Detector 3
  // 

  auto calorimeterS3
    = new G4Box("Calorimeter3", 
                 calorSizeX/2, calorSizeY/2, calorThickness/2); 
                         
  auto calorLV3
    = new G4LogicalVolume(
                 calorimeterS3,    
                 defaultMaterial, 
                 "Calorimeter3");

  G4RotationMatrix* fieldRot3 = new G4RotationMatrix();
  fieldRot3->rotateX(-110.*deg); 

  new G4PVPlacement(
		 fieldRot3,             
                 G4ThreeVector(0*um,-11.2763*cm,-4.1042*cm),  
                 calorLV3,                                 
                 "Calorimeter3",    
                 worldLV,          
                 false,            
                 0,               
                 fCheckOverlaps);


  //                               
  // Detector 4
  // 

  auto calorimeterS4
    = new G4Box("Calorimeter4",    
                 calorSizeX/2, calorSizeY/2, calorThickness/2);
                         
  auto calorLV4
    = new G4LogicalVolume(
                 calorimeterS4,    
                 defaultMaterial,
                 "Calorimeter4");  

  G4RotationMatrix* fieldRot4 = new G4RotationMatrix();
  fieldRot4->rotateX(-150.*deg); 

  new G4PVPlacement(
		 fieldRot4,           
                 G4ThreeVector(0*um,-6*cm,-10.3923*cm),  
                 calorLV4,                                   
                 "Calorimeter4",    
                 worldLV,        
                 false,           
                 0,               
                 fCheckOverlaps); 
 
  //                                 
  // Definiendo replica 1
  //

  auto layerS 
    = new G4Box("Layer",           
                 calorSizeX/2, absoY/2, layerThickness/2);
                         
  auto layerLV
    = new G4LogicalVolume(
                 layerS,           
                 defaultMaterial,  
                 "Layer");         

  new G4PVReplica(
                 "Layer",          // nombre
                 layerLV,          // volumen logico
                 calorLV,          // mother volume
                 kYAxis,           // Eje de replica
                 fNofLayers,       // numero de replicas
                 3.*mm);  	   // tamaño de las replicas
  //                                 
  // Definiendo replica 2
  //

  auto layerS2 
    = new G4Box("Layer2",           
                 calorSizeX/2, absoY/2, layerThickness/2);
                         
  auto layerLV2
    = new G4LogicalVolume(
                 layerS2,           
                 defaultMaterial,  
                 "Layer2");         

  new G4PVReplica(
                 "Layer2",          // nombre
                 layerLV2,          // volumen logico
                 calorLV2,          // mother volume
                 kYAxis,           // Eje de replica
                 fNofLayers,       // numero de replicas
                 3.*mm);  	   // tamaño de las replicas

  //                                 
  // Definiendo replica 3
  //

  auto layerS3 
    = new G4Box("Layer3",           
                 calorSizeX/2, absoY/2, layerThickness/2);
                         
  auto layerLV3
    = new G4LogicalVolume(
                 layerS3,           
                 defaultMaterial,  
                 "Layer3");         

  new G4PVReplica(
                 "Layer3",          // nombre
                 layerLV3,          // volumen logico
                 calorLV3,          // mother volume
                 kYAxis,           // Eje de replica
                 fNofLayers,       // numero de replicas
                 3.*mm);  	   // tamaño de las replicas

  //                                 
  // Definiendo replica 4
  //

  auto layerS4 
    = new G4Box("Layer4",           
                 calorSizeX/2, absoY/2, layerThickness/2);
                         
  auto layerLV4
    = new G4LogicalVolume(
                 layerS4,           
                 defaultMaterial,  
                 "Layer4");         

  new G4PVReplica(
                 "Layer4",          // nombre
                 layerLV4,          // volumen logico
                 calorLV4,         // mother volume
                 kYAxis,           // Eje de replica
                 fNofLayers,       // numero de replicas
                 3.*mm);  	   // tamaño de las replicas

  //
  //TARGET
  //

  auto fSolidAbsorber = new G4Box("Target",        
                       5.*mm, 5.*mm, 2*um);
                          
  auto fLogicAbsorber = new G4LogicalVolume(fSolidAbsorber,   
                                       targetMaterial, 
                                       "Target");      

  G4RotationMatrix* fieldRot = new G4RotationMatrix();
  fieldRot->rotateX(45.*deg);               
                               
  new G4PVPlacement(fieldRot,
                    G4ThreeVector(0*mm,0*mm,0.*cm),
                    fLogicAbsorber,    
                    "Target",         
                    worldLV,        
                    false,              
                    0,		
		    fCheckOverlaps);                 


  //                               
  // Capas Detector1
  //
  auto absorberS 
    = new G4Box("Abso",           
                 calorSizeX/2, absoY/2, absoThickness/2);
                         
  auto absorberLV
    = new G4LogicalVolume(
                 absorberS,        
                 absorberMaterial, 
                 "AbsoLV");        
                                    
   new G4PVPlacement(
                 0,                
                 G4ThreeVector(0., 0., 0.), 
                 absorberLV,                                
                 "Abso",           
                 layerLV,          
                 false,            
                 0,                
                 fCheckOverlaps);  

  //                               
  // Capas Detector2
  //
  auto absorberS2 
    = new G4Box("Abso2",           
                 calorSizeX/2, absoY/2, absoThickness/2);
                         
  auto absorberLV2
    = new G4LogicalVolume(
                 absorberS2,        
                 absorberMaterial, 
                 "AbsoLV2");        
                                    
   new G4PVPlacement(
                 0,                
                 G4ThreeVector(0., 0., 0.), 
                 absorberLV2,                                
                 "Abso2",           
                 layerLV2,          
                 false,            
                 0,                
                 fCheckOverlaps);  
  
  //                               
  // Capas Detector3
  //
  auto absorberS3
    = new G4Box("Abso3",           
                 calorSizeX/2, absoY/2, absoThickness/2);
                         
  auto absorberLV3
    = new G4LogicalVolume(
                 absorberS3,        
                 absorberMaterial, 
                 "AbsoLV3");        
                                    
   new G4PVPlacement(
                 0,                
                 G4ThreeVector(0., 0., 0.), 
                 absorberLV3,                                
                 "Abso3",           
                 layerLV3,          
                 false,            
                 0,                
                 fCheckOverlaps); 

  //                               
  // Capas Detector4
  //
  auto absorberS4 
    = new G4Box("Abso4",           
                 calorSizeX/2, absoY/2, absoThickness/2);
                         
  auto absorberLV4
    = new G4LogicalVolume(
                 absorberS4,        
                 absorberMaterial, 
                 "AbsoLV4");        
                                    
   new G4PVPlacement(
                 0,                
                 G4ThreeVector(0., 0., 0.), 
                 absorberLV4,                                
                 "Abso4",           
                 layerLV4,          
                 false,            
                 0,                
                 fCheckOverlaps); 

  //
  // Muestra los parametros de la geometria global
  //
  G4cout
    << G4endl 
    << "------------------------------------------------------------" << G4endl
    << "---> El detector tiene " << 16 << " capas de: [ "
    << absoThickness/mm << "mm de " << absorberMaterial->GetName()    << G4endl
    << "------------------------------------------------------------" << G4endl;
  
  //                                        
  // Opciones de visualizacion
  //
  //worldLV->SetVisAttributes (G4VisAttributes::GetInvisible());

  auto simpleBoxVisAtt= new G4VisAttributes(G4Colour(1.0,1.0,1.0));
  simpleBoxVisAtt->SetVisibility(true);
  calorLV->SetVisAttributes(simpleBoxVisAtt);

  //
  // Retornar el PhysicalWorld
  //
  return worldPV;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void DetectorConstruction::ConstructSDandField()
{
  // G4SDManager::GetSDMpointer()->SetVerboseLevel(1); (Mayor informacion del 								sensitivedetector)

  // 
  // Sensitive detectors
  //
  auto absoSD 
    = new CalorimeterSD("AbsorberSD", "AbsorberHitsCollection", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(absoSD);
  SetSensitiveDetector("AbsoLV",absoSD);

  auto absoSD2 
    = new CalorimeterSD("AbsorberSD2", "AbsorberHitsCollection2", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(absoSD2);
  SetSensitiveDetector("AbsoLV2",absoSD2);

  auto absoSD3 
    = new CalorimeterSD("AbsorberSD3", "AbsorberHitsCollection3", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(absoSD3);
  SetSensitiveDetector("AbsoLV3",absoSD3);

  auto absoSD4 
    = new CalorimeterSD("AbsorberSD4", "AbsorberHitsCollection4", fNofLayers);
  G4SDManager::GetSDMpointer()->AddNewDetector(absoSD4);
  SetSensitiveDetector("AbsoLV4",absoSD4);

  // 
  // Campo magnetico
  //
  // 
  // Se crea automaticamente si se define el valor del campo diferente a 0

  G4ThreeVector fieldValue;
  fMagFieldMessenger = new G4GlobalMagFieldMessenger(fieldValue);
  fMagFieldMessenger->SetVerboseLevel(1);
  
  // Registra y elimina el campo magnetico
  G4AutoDelete::Register(fMagFieldMessenger);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
