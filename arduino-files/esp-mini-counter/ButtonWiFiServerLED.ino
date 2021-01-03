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
const int taster = D1;
const int led_out_1 = D2;
const int led_out_2 = D3;

int val = 0; // variable for reading the pin status

void controller_setup()
{
    // Start Serial for Debugging
    Serial.begin(115200);
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
    digitalWrite(led_out_1, HIGH); // turn LED ON
    digitalWrite(led_out_2, HIGH); // turn LED ON
}

void storage_init()
{
}

void status_ready()
{
    digitalWrite(led_out_1, LOW); // turn LED ON
    digitalWrite(led_out_2, HIGH); // turn LED ON
}

void sensor_read()
{
    val = digitalRead(taster); // read input value
    // HIGH = no input
    // LOW = user input
}

int sensor_is_active()
{
    if(val != HIGH){ // HIGH = no input
        return 1;
    }
    return 0;
}

void sensor_on_active()
{
    digitalWrite(led_out_2, LOW); // turn LED ON
    Serial.println(val);
}

void sensor_on_inactive()
{
    digitalWrite(led_out_2, HIGH); // turn LED OFF
}

#endif
