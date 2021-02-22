#ifdef _IBAWSL

// Secret data in dedicated file to hide from git
#include "credentials.h"
// Must contain "SERVER_ADDRESS" variable with type "char*"
// On Error create file "credentials.h" with content:
// const char* SERVER_ADDRESS = "Path of Server with protocol and port";

//Included libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiManager.h>

// Assign output variables
const int taster = D1;

const int led_out_1 = D2;
const int led_out_2 = D3;

WiFiManager wifimanager;

// HTTP Client objects
HTTPClient http;
String macAddress;

//Volatile counter for ISR
volatile int val = 0; // variable for reading the pin status

void controller_setup()
{
  // Start Serial for Debugging
  Serial.begin(115200);
  //resetEEPROM(true);
}

void output_init()
{
  pinMode(led_out_1, OUTPUT); // LED 1 PIN as Output
  digitalWrite(led_out_1, LOW); // LED 1 Off
  pinMode(led_out_2, OUTPUT); // LED 2 PIN as Output
  digitalWrite(led_out_2, LOW); // LED 2 Off
}

void sensor_init()
{
  pinMode(taster, INPUT_PULLUP); // Sensor Input as PULLUP, HIGH = No input
  attachInterrupt(digitalPinToInterrupt(interruptPin), callISRAction, RISING); //RISING = LOW -> HIGH
}

void status_init()
{
  digitalWrite(led_out_1, HIGH); // turn LED ON
  digitalWrite(led_out_2, LOW); // turn LED ON
}

void network_connect()
{
  // Connect to Wi-Fi network with WiFiManager
  wifimanager.autoConnect();
}

void status_connected()
{
  digitalWrite(led_out_1, HIGH); // turn LED ON
  digitalWrite(led_out_2, HIGH); // turn LED ON
}

void storage_init()
{
  http.begin(SERVER_ADDRESS + "/api/register/" + macAddress);
  int httpCode = http.GET();
  if (httpCode > 0) { //Check the returning code

    String payload = http.getString();   //Get the request response payload
    Serial.println(payload);             //Print the response payload

  }

  http.end();   //Close connection
}

void status_ready()
{
  digitalWrite(led_out_1, LOW); // turn LED OFF
  digitalWrite(led_out_2, HIGH); // turn LED ON
}

void callISRAction()
{
    val += 1;
    Serial.println("ISR Call:");
    Serial.println("New Value: " + val);
}

void loop_action_init()
{
    delay(100);
}

int action_is_active()
{
  return val > 0;
}

void action_on_active()
{
  Serial.println("Action active!");
  digitalWrite(led_out_1, HIGH); // turn LED ON
  digitalWrite(led_out_2, HIGH); // turn LED ON
  Serial.println(val);
  http.begin(SERVER_ADDRESS + "/api/count/" + macAddress + "/" + val);
  int httpCode = http.GET();
  if (httpCode > 0) { //Check the returning code

    String payload = http.getString();   //Get the request response payload
    Serial.println(payload);             //Print the response payload

  }

  http.end();   //Close connection

  val = 0; //Reset after update on Server

  status_ready();

}

void action_on_inactive()
{
   status_ready();
}

#endif
