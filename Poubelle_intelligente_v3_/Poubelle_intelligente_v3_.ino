/* 
 * Auteur      : Sabeg.S
 * Programme   : Poubelle intelligente
 * Description : Ce programme (Version 3) fonctionnel televersé a un arduino permet de :
                  - Ramasser les données pour envoyer le statut d'une poubelle
                  - Ouvrir et fermer la porte de la poubelle en fonction des conditions
                  - Mettre a jour l'utilisateur du statut de la poubelle 
 * Date        : Mardi 23 mars 2021 
 */

// Variable a utiliser pour lire le RPC
String poubelle;
String of;

// Librairies
//#include <Arduino_MKRGPS.h>
#include <Servo.h>
#include <DHT.h>

// Include les fichiera entetes (.h)
#include "WIFIConnectorEntreprise.h"
#include "MQTTConnectorEntreprise.h"

// Definir les servo moteurs
Servo action1;
Servo action2;

// Definir les pins
#define echoPin1 2 // Attacher la pin 2 a la sortie Echo du HC-SR04
#define trigPin1 3 // Attacher la pin 3 a la sortie Trig du HC-SR04
#define echoPin2 6 // Attacher la pin 4 a la sortie Echo du HC-SR04
#define trigPin2 7 // Attacher la pin 5 a la sortie Trig du Hc-SR04
#define DHTPIN 1 // Attacher la pin 1 a la sortie du signal du DHT22
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// Declaration des variables 
long duration1;
int distance1;
int pourcentage;
long duration2;
int distance2;

int chk;
float hum; //Stocke la valeur de l'humidité
float temp; //Stocke la valeur de la temperature 

String statut;

int val = 0;


// Parametre d'initialisation de la carte
void setup() 
{
  // Initialiser le moniteur série
  Serial.end(); 
  Serial.begin(9600); // Démmarer le moniteur série

  // Boucler la lecture serial
  while(!Serial){

    Serial.print("..");
    
  }

  // Initialiser le bouclier GPS
  if(!GPS.begin(GPS_MODE_SHIELD)){

    Serial.println("Failed to initialize GPS!");

    while(1);
  }
  
  // Initialisation des servo moteurs
  action1.attach(9);  // Definir la broche 9 (SCK) comme signal pour le moteur
  action2.attach(10); // Definir la broche 10 comme signal pour le moteur 
  
  // Initialiser le capteur a mouvement
  ultrasonBegin();

  // Commencer le fonctionemment du DHT22
  dht.begin();
  
  // Se connecter au Wifi et le serveur Thingsboard
  wifiConnect();
  MQTTConnect();
  
}


void loop() //Boucle principale, elle s'exécute à l'infini
{
 mqqtClient.loop();


  
  /*// Lecture et affichage de la broche connecté au capteur
  Serial.println(analogRead(A0));
  delay(1000);

      // Condition de detection mouvement
      if (analogRead(A0) > 1000)
      {
        val = val + 1;
        Serial.println(val);
        Serial.println("La porte s'ouvre !");
        val = 0;
      }
     else
      {
        val = val;
        Serial.println(val);
        Serial.println("Mouvement non détecté...");
      }*/
      
  // Lire les données et stocker dans les variables hum et temp
   hum = dht.readHumidity(); // Lire l'humidité
   temp = dht.readTemperature(); // Lire la température

      
  // Préparer la detection de la distance entre le capteur et l'obstacle en face
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  // Activer la TrigPin a HIGH pour 10 Microseconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Lire la vague du son retourné a echoPin
  duration1 = pulseIn(echoPin1, HIGH);  
  // Calculer la distance
  distance1 = duration1*0.034/2; // Vitesse de l'onde sonore divisée par 2 (aller et retour)
  

  // Préparer la detection de la distance entre le capteur et l'obstable en face
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);
  // Activer la TrigPin a HIGH pour 10 Microseconds
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  // Lire la vague du son retourné a echoPin
  duration2 = pulseIn(echoPin2, HIGH);  
  // Calculer la distance
  distance2 = duration2 / 58; // Vitesse de l'onde sonore divisée par 2 (aller et retour)

  pourcentage = 100 - ((distance2 * 100) / 100);
  
  // Affecter les données rammassées dans le tableau a envoyer
  KeyValue[0] = temp;
  KeyValue[1] = hum;
  KeyValue[2] = distance1;
  KeyValue[3] = distance2;
  KeyValue[4] = pourcentage;


  
  
  /*// Afficher la distance sur le moniteur série
  Serial.print("Statut: ");

  // Affecter le statut en changeant le niveau remplie de la poubelle
  if(distance2 <= 100 && distance2 > 90){
    statut = "La poubelle est vide (0%)";  
     
  }
  else if(distance2 < 90 && distance2 >= 75){
    statut = "La poubelle est a niveau d'un quart (25%)";
  }
  else if(distance2 < 75 && distance2 >= 50){
    statut = "La poubelle est presque la motié";
  }
  else if(distance2 < 50 && distance2 >= 25){
    statut = "La poubelle est plus que la motié";
  }
  else if(distance2 < 25 && distance2 >= 1){
    statut = "Vaux mieux vider t'as poubelle";
  }

  
  Serial.println(statut);
  delay (2000);
  Serial.print("Sachant que la temperature et l'humidité est : ");
  Serial.print(temp); 
  Serial.print(", ");
  Serial.print(hum);
  Serial.println(" %");
  delay(2000);
*/


  
  // Si la distance est moins d'un metre le moteur va tourner a ongle suffis d'ouvre la porte de la poubelle
  if(distance1 < 100) 
  {  
    // Tourner le servo moteur
    action1.write(180);
    action2.write(180);
    
  }
  // Sinon le moteur revient a au point depart, qui fait fermer la porte de la poubelle
  else if (distance1 >= 100) 
  {
    // Revenir au point depart 
    action1.write(0);
    action2.write(0);
  }


  if(poubelle = "SMRT")
  {

    if(of == "true")
    {
      action1.write(180);
      action2.write(180);   
    }

    else if (of == "false")
    {
      action1.write(0);
      action2.write(0);
    }
    
  }

 /* // Si la lecture du GPS et disponible
  if(GPS.available()){

  // Lire les valeurs du GPS
  float latitude = GPS.latitude();
  float longitude = GPS.longitude();
  float altitude = GPS.altitude();
  float speed = GPS.speed();
  int satellites = GPS.satellites();

  // Afficher sur le moniteur série les vvleurs du GPS
  
    Serial.print("Location: ");
    Serial.print(latitude, 7);
    
    Serial.print(", ");
    Serial.println(longitude, 7);
    
    Serial.print("Altitude: ");
    Serial.print(altitude);
    Serial.println("m");

    Serial.print("Ground speed: ");
    Serial.print(speed);
    Serial.println(" km/h");

    Serial.print("Number of satellites: ");

    Serial.println(satellites);

    Serial.println();
  
    // Affecter dans le tableau les données du GPS lu
    KeyValue[5] = latitude;
    KeyValue[6] = longitude;
    KeyValue[7] = altitude;
    KeyValue[8] = speed;
    KeyValue[9] = satellites;
  
  }

  else
  {
    KeyValue[5] = 47.62;
    KeyValue[6] = -65.67;
    KeyValue[7] = 38.40;
    KeyValue[8] = 0.23;
    KeyValue[9] = 11;
  }*/
  
  // Envoyer les valeurs
  SendPayload(TelemetryKey, KeyValue);  
 
}



void ultrasonBegin(){
   
    // Initialiser les pins du premier capteur a distance
    pinMode(trigPin1, OUTPUT);
    digitalWrite(trigPin1, LOW);
    pinMode(echoPin1, INPUT);
    
    // Initialiser les pins du deuxieme capteur a distance
    pinMode(trigPin2, OUTPUT);
    digitalWrite(trigPin2, LOW);
    pinMode(echoPin2, INPUT);  
    
}
