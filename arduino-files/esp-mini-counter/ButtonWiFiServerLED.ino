#ifdef _BWSL

// Secret data in dedicated file to hide from git
#include "credentials.h" 
// Must contain "ssid" and "password" variables with type "char*"
// On Error create file "credentials.h" with content:
// const char* ssid     = "Name of SSID";
// const char* password = "Password of SSID";

//Included libraries
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Assign output variables
const int taster = 0;//D1;
//const int stopp = D2;
const int led_out = 0;//D3;

int val = 0; // variable for reading the pin status

void controller_setup()
{
    // Start Serial for Debugging
    Serial.begin(115200);
}

void output_init()
{
    pinMode(led_out, OUTPUT);
    digitalWrite(led_out, LOW);
}

void sensor_init()
{
    pinMode(taster, INPUT_PULLUP);
    digitalWrite(taster, HIGH);
}

void status_init()
{
}

void network_connect()
{
    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

void status_connected()
{
}

void storage_init()
{
}

void status_ready()
{
}

void sensor_read()
{
    val = digitalRead(taster); // read input value
}

int sensor_is_active()
{
    if(val != HIGH){
      return 1;
    }
    return 0;
}

void sensor_on_active()
{
    digitalWrite(led_out, HIGH); // turn LED ON
    Serial.println(val);
}

void sensor_on_inactive()
{
    digitalWrite(led_out, LOW); // turn LED OFF
}

#endif
