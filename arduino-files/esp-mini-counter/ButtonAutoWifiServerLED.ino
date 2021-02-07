#ifdef _BAWSL

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

int val = 0; // variable for reading the pin status

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
  pinMode(taster, INPUT_PULLUP); // Sensor Input as PULLUP
  digitalWrite(taster, HIGH); // Sensor on HIGH
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
  http.begin("http://192.168.178.28:3000/api/register/" + macAddress);
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

void loop_action_init()
{
  val = digitalRead(taster); // read input value
  delay(100);
  // HIGH = no input
  // LOW = user input
}

int action_is_active()
{
  if (val != HIGH) { // HIGH = no input
    return 1;
  }
  return 0;
}

void action_on_active()
{
  digitalWrite(led_out_1, HIGH); // turn LED ON
  digitalWrite(led_out_2, HIGH); // turn LED ON
  Serial.println(val);
  http.begin("http://192.168.178.28:3000/api/count/" + macAddress);
  int httpCode = http.GET();
  if (httpCode > 0) { //Check the returning code

    String payload = http.getString();   //Get the request response payload
    Serial.println(payload);             //Print the response payload

  }

  http.end();   //Close connection

}

void action_on_inactive()
{
   status_ready();
}

#endif
