#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "credentials.h"
#include <RCSwitch.h>

#define SERIAL_BAUDRATE 115200

RCSwitch rcSwitch = RCSwitch();
ESP8266WebServer server(80);

void wifiSetup();
void handleSendRequest();
void handleRootRequest();

void setup()
{

  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();
  Serial.println();

  rcSwitch.enableTransmit(D3);

  wifiSetup();

  server.on("/", handleRootRequest);
  server.on("/send", handleSendRequest);
  server.begin();
  Serial.println("Server listening");
}

void loop()
{
  server.handleClient();
}

void handleRootRequest()
{
  server.send(200, "text/plain", "I'm online");
}

/**   
 * endpoint: /send
 * example: /send?code=12345
 * queryParams: 
 *    - code (int): code to send 
 *    - protocol (int): Number of protocol - default = 1, optional 
 **/
void handleSendRequest()
{
  String message = "";

  if (server.arg("protocol") != "")
  {
    rcSwitch.setProtocol(server.arg("protocol").toInt());
  }

  if (server.arg("code") == "")
  {
    message = "Code argument not found";
  }
  else
  {
    // sending code
    rcSwitch.send(server.arg("code").toInt(), 24);
    server.send(200, "text / plain");
    return;
  }

  server.send(400, "text / plain", message);
}

void wifiSetup()
{
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connected!
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}
