//            CODE Arduino projet "LEAP' Drone"  Copyright Alex Colleux
// Pour le concours INNOVEZ science et vie junior 2015
#include <SPI.h>

const int POTz = 8;
const int POTx = 9;
const int POTgaz = 10;

int recu; // varible stockant les données reçues
int altitude; // Variable servant à appliquer l'altitude
int pitch; // Variable servant à controler le drone sur l'axe z (gauche - droite)
int roll; // Variable servant à controler le drone sur l'axe x (avant - arrière)

void setup() {
  Serial.begin(9600);      // On défini la vitesse de communication
  Serial.setTimeout(10); // Temps laissé à l'arduino pour recevoir les données
  pinMode (POTz, OUTPUT);
  pinMode (POTgaz, OUTPUT);
  pinMode (POTx, OUTPUT);
  SPI.begin(); // On commence la communication SPI (avec les potentiomètres)
  altitude = 255; // Altitude 255 --> drone au sol
}

void loop() {


  if (Serial.available() > 0)  { // Si des données sont envoyées à l'arduino
    recu = Serial.parseInt();                    // lecture des données reçues
  }
  if (recu == 0) { // Si la main n'est pas au dessus du leap motion le drone n'est plus en vol
    altitude = 255;
    roll = 40;
    pitch = 90;
  }
  if (recu >= 250 && recu <= 430) { // Reception des valeurs de l'altitude
    altitude = int(map(recu, 250, 430, 0, 55)); // On transforme les valeurs reçues pour faire varier le potentiomètre entre 0 et 55
  }

  if (recu >= -40 && recu <= 0) { // Réception des données du "roll" avec la main orientée vers l'avant
    roll = int(map(recu, 0, -40, 35, 0));
  }
  if (recu >= 0 && recu <= 40) { // Réception des données du "roll" avec la main orientée vers l'arrière
    roll = int(map(recu, 0, 40, 35, 255));
  }
  if (recu >= 960 && recu <= 1000) { // Réception des données du "pitch" avec la main orientée vers la gauche
    pitch = int(map(recu, 960, 1000, 255, 80));
  }
  if (recu >= 1000 && recu <= 1040) { // Réception des données du "pitch" avec la main orientée vers la droite
    pitch = int(map(recu, 1000, 1040, 80, 0));
  }
  
  digitalPotWriteGAZ(altitude); // On applique les différentes valeurs reçues aux différentes commandes de la télécommande
  digitalPotWriteZ(pitch);
  digitalPotWriteX(roll);

}
int digitalPotWriteZ(int value) // Contrôle du potentiomètre de l'orientation gauche - droite (axe Z)
{
  digitalWrite(POTz, LOW); // Démarrage de la phase de réception
  SPI.transfer(0x11); // On choisit un protocol de communication spécifique
  SPI.transfer(value); // On écrit la valeur sur le composant
  digitalWrite(POTz, HIGH); // On termine la phase de réception
}
int digitalPotWriteGAZ(int value) // Contrôle du potentiomètre des "gaz"
{
  digitalWrite(POTgaz, LOW);
  SPI.transfer(0x11);
  SPI.transfer(value);
  digitalWrite(POTgaz, HIGH);
}
int digitalPotWriteX(int value) // Contrôle du potentiomètre de l'orientation haut bas (axe X)
{
  digitalWrite(POTx, LOW);
  SPI.transfer(0x11);
  SPI.transfer(value);
  digitalWrite(POTx, HIGH);
}
