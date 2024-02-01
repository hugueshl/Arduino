//+-------------------------------------------------------------+
//!                                                             !
//!                      Thermomix                              !
//!                                                             !
//+-------------------------------------------------------------+
//!                                                             !
//!  Version: Beta 0.0.0                                        !
//!  Date: 2024/02/01                                           !
//!  Auteurs: Ced/Hug                                           !
//!  Société: APP @peu-près                                     !
//!                                                             !
//+-------------------------------------------------------------+
//!                                                             !
//! Reste @ Programmer:                                         !
//!                                                             !
//! 01 - BP Arrêt   OK                                          !
//! 02 - Csgn de Vitesse Via Téléphone                          !
//! 03 - Wifi Tuya TC: Télécommandes                            !
//! 04 - Voyants Marche/Arrêt/Défauts                           !
//!                                                             !
//+-------------------------------------------------------------+



#include <Stepper.h>
// --- Input ---
const int PIN_NB = 4;
const int PIN_NH = 5;
const int PIN_BP_Bas = 6;
const int PIN_BP_Haut = 7;
const int PIN_BP_Arret = 3;

// Définition des pins

const int pinENA = 8;   // Pin pour activer le driver TB6600
const int pinPULS = 9;  // Pin pour les impulsions du moteur
const int pinDIR = 10;  // Pin pour la direction du moteur


// Sauvegarde Etat des Entrées
  int Save_I_Det_NH = 1;  // Det_NH
  int Save_I_Det_NB = 1;  // Det_NB
  int Save_I_BP_Bas = 1;  // BP_Bas  
  int Save_I_BP_Haut = 1;  // BP_Haut  
  int Save_I_BP_Arret = 1;  // BP_Arret

// Initialize the pushbutton pin as an input:
int I_NH = 0;
int I_NB = 0;
int I_BP_Bas = 0;
int I_BP_Haut = 0;
int I_BP_Arret = 0;


// Initialize the LED pin as an output:
const int PIN_Led_Haut = 12;
const int PIN_Led_Bas = 13;

// Variable
int Cde=0;

const int stepsPerRevolution = 400;  // change this to fit the number of steps per revolution old 200
// for your motor

// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 8, 9, 10, 11);

void setup() {

  // Initialize the pushbutton pin as an input:
  pinMode(PIN_BP_Haut, INPUT_PULLUP);
  pinMode(PIN_BP_Bas, INPUT_PULLUP);
  pinMode(PIN_BP_Arret, INPUT_PULLUP);
  pinMode(PIN_NH, INPUT_PULLUP);
  pinMode(PIN_NB, INPUT_PULLUP);
  
  // Initialize the LED pin as an output:
  pinMode(PIN_Led_Haut, OUTPUT);
  pinMode(PIN_Led_Bas, OUTPUT);

  // set the speed at 60 rpm:
  myStepper.setSpeed(500);  // old 60
  
  // initialize the serial port:
  Serial.begin(9600);
}

void loop() {
	
   // read the state of the pushbutton value:
   I_BP_Haut = digitalRead(PIN_BP_Haut);
   I_BP_Bas = digitalRead(PIN_BP_Bas);
   I_BP_Arret = digitalRead(PIN_BP_Arret);
   I_NH = digitalRead(PIN_NH);
   I_NB = digitalRead(PIN_NB);


   //digitalWrite(PIN_Led, HIGH); // on Allume avec 
 
   if ((Save_I_BP_Arret != I_BP_Arret) ){   //
      Save_I_BP_Arret = I_BP_Arret; // Sauvegarde l'état du BP
      Cde=0; 
      Serial.println("Cde Arrêt");
   }

   if ((Save_I_BP_Haut != I_BP_Haut) ){   //and (Cde == 0)
      Save_I_BP_Haut = I_BP_Haut; // Sauvegarde l'état du BP
      Cde=1;      
      Serial.println("Cde Haut");
   }
   
    if ((Save_I_BP_Bas != I_BP_Bas) ){   //and (Cde == 1)
      Save_I_BP_Bas = I_BP_Bas; // Sauvegarde l'état du BP
      Cde=2; 
      Serial.println("Cde Bas");
   }

 // Serial.println(I_BP_Haut);
 // ------ CDE ARRET ---------
  if (Cde == 0) {
  // step one revolution in the other direction:
  //Serial.println("Arrêt");
  myStepper.step(0);
  // delay(5);
  }

 // ------ CDE MONTEE ---------
  if ((Cde == 1) and (I_NH == 1)){
 // step one revolution  in one direction:
  Serial.println("clockwise");
  myStepper.step(stepsPerRevolution);
 // delay(5);
 }


  // ------ CDE DESCENTE ---------
  if ((Cde == 2) and (I_NB == 1)){
  // step one revolution in the other direction:
  Serial.println("counterclockwise");
  myStepper.step(-stepsPerRevolution);
 // delay(5);
  }


 //---------------- Sorties ----------------
  if (I_NH == 0) {
  digitalWrite(PIN_Led_Haut, 1);
    if (Cde == 1){                    
    Cde=0;
    }
  }else{
   digitalWrite(PIN_Led_Haut, 0);
  }
  
  if (I_NB == 0) {
  digitalWrite(PIN_Led_Bas, 1);
    if (Cde == 2){
    Cde=0;                           
    }
    
  }else{
   digitalWrite(PIN_Led_Bas, 0);
  }

}
