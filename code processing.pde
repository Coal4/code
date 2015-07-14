// Code créé par Alex colleux pour le concours INNOVEZ

import de.voidplus.leapmotion.*; // On importe la librairie permettant d'accéder au leap motion
LeapMotion leap; // On appelle leap le leap motion dans le programme

import processing.serial.*; // On importe la librairie gérant la communication série
Serial myPort; // On appel myPort le port série pour communiquer avec l'arduino

int altitude; // Variable stockant la hauteur main - leap motion
int roll; // Variable stockant l'orientation sur l'axe x de la main
int pitch; // Variable stockant l'orientation sur l'axe z de la main
boolean detect = false; // opérateur détecant si la main est au dessus du leap ou non

void setup() {
size(800, 500, P3D); // on crée une fenètre lors du démarrage du programme
background(255); // On met un fond blanc
noStroke(); 
fill(50);
myPort = new Serial(this, Serial.list()[0], 9600); // On ouvre le port série pour envoyer des données
//à l'arduino
leap = new LeapMotion(this); // soit "leap" le leap motion
}

void draw() {
background(255);

// Leap magic
int fps = leap.getFrameRate(); // On récupère le taux de rafraichissement du leap motion
text(fps,750,50); // On affiche le taux de rafraichissement en haut à droite de l'écran

for (Hand hand : leap.getHands()) { //on récupère des données sur la main au dessus du capteur

hand.draw(); // On affiche les mains en 3D sur l'écran
PVector hand_position = hand.getPosition(); 
float   hand_roll = hand.getRoll() -10; 
float   hand_pitch       = hand.getPitch() - 10;


altitude = (int) hand.getPosition().y;//On récupère la distance leap - main
roll = (int)hand_roll;// On récupère l'orientation X
pitch = (int) map(hand_pitch,-30,50,970,1050);// On récupère l'orientation Z

if(altitude >250 && altitude < 430){
myPort.write(str(altitude));
}else if(altitude <280){
  myPort.write(str(250));
}

delay(15); // on attend 15 ms avant le prochain envoi

if(pitch >940 && pitch < 1040){
myPort.write(str(pitch)); // Si la main est dans l'intervalle de variation, on envoi une valeur changeante
}else if(pitch <940){
  myPort.write(str(941));  // si la main est trop à gauche on envoi une valeur statique (minimale)
}else if(pitch >1040){
  myPort.write(str(1039)); // si la main est trop à droite on envoi une valeur statique (maximale)
}

delay(15);

if(roll >-40 && roll < 40){ // Meme principe que précédemment sauf qu'ici on est sur l'axe X
myPort.write(str(roll));
}else if(roll <-40){
  myPort.write(str(-39));
}else if(roll >40){
  myPort.write(str(39));
}
delay(15);

detect = true; // on indique que la main est bien présente au dessus du leap
}

if(detect == false){
  myPort.write(str(0)); // Si pas de main au dessus du LEAp, on envoi "0" à l'arduino
}
detect = false;
}
