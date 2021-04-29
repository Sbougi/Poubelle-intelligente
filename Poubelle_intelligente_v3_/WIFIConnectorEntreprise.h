
// Connection using WPA2 Enterpriser WIFI network


#include <WiFiNINA.h>
//#include <SPI.h>
#include <ArduinoOTA.h>
#include "wifiStatus.h"

//please enter your sensitive data for log on

char ssid[] = "Linksys13617";  // your WPA2 enterprise network SSID (name)
char user[] = "";  // your WPA2 enterprise username
char pass[] = "groupeIOT3";  // your WPA2 enterprise password
/*
char ssid[] = "Chuck and Bailey";
char user[] = "";
char pass[] = "19931993";
*/
int status = WL_IDLE_STATUS;     // the Wifi radio's status



WiFiClient wifiClient; 

void wifiConnect()
{
  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) {
    Serial.println("Please upgrade the firmware");
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA2 enterprise network:
    // - You can optionally provide additional identity and CA cert (string) parameters:
    //      WiFi.beginEnterprise(ssid, user, pass, identity, caCert)
    // . if your network requires them.
    status = WiFi.begin(ssid, pass);
    //status = WiFi.beginEnterprise(ssid, user, pass);
    Serial.println(status);

    // start the WiFi OTA library with internal (flash) based storage
    ArduinoOTA.begin(WiFi.localIP(), "Arduino", "password", InternalStorage);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.print("You're connected to the network");
  printCurrentNet();
  printWifiData();

}
