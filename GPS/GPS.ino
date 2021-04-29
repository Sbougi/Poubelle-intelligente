#include <Arduino_MKRGPS.h>
#include "WIFIConnectorEntreprise.h"
#include "MQTTConnectorEntreprise.h"

#define echoPin1 2
#define trigPin1 3

// Declaration des variables 
long duration1;
int distance1;



void setup() {

  // initialize serial communications and wait for port to open:

  Serial.end(); 
  Serial.begin(9600);

  while (!Serial) {

    Serial.print(".."); // wait for serial port to connect. Needed for native USB port only

  }

  // If you are using the MKR GPS as shield, change the next line to pass

  // the GPS_MODE_SHIELD parameter to the GPS.begin(...)

  if (!GPS.begin(GPS_MODE_SHIELD)) {

    Serial.println("Failed to initialize GPS!");

    while (1);

  }

  // Se connecter au Wifi et le serveur Thingsboard
  wifiConnect();
  MQTTConnect();

  //ltrasonBegin(); 
}

void loop() {
mqqtClient.loop();

  /*// Préparer la detection de la distance entre le capteur et l'obstable en face
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(2);
  // Activer la TrigPin a HIGH pour 10 Microseconds
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
  // Lire la vague du son retourné a echoPin
  duration1 = pulseIn(echoPin1, HIGH);  
  // Calculer la distance
  distance1 = duration1*0.034/2; // Vitesse de l'onde sonore divisée par 2 (aller et retour)*/
  

  
  
  // check if there is new GPS data available

  if (GPS.available()) {

    // read GPS values

    float latitude   = GPS.latitude();

    float longitude  = GPS.longitude();

    float altitude   = GPS.altitude();

    float speed      = GPS.speed();

    int   satellites = GPS.satellites();

    // print GPS values

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

    KeyValue[0] = latitude;
    KeyValue[1] = longitude;
    KeyValue[2] = altitude;
    KeyValue[3] = speed;
    KeyValue[4] = satellites;
    //KeyValue[5] = distance1;
   
    SendPayload(TelemetryKey, KeyValue); 

  }


}

  void ultrasonBegin(){
   // Initialiser les pins du premier capteur a distance
    pinMode(trigPin1, OUTPUT);
    digitalWrite(trigPin1, LOW);
    pinMode(echoPin1, INPUT);
  }
