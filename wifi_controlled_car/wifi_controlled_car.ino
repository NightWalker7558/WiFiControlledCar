#include "credentials.h" // Include external credentials file

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL60k6nWKVi"
#define BLYNK_TEMPLATE_NAME "WiFi Controlled Car"

#include <Arduino.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Motor control pins
#define ENA 32 // Enable for Motor A
#define IN1 25 // Motor A direction
#define IN2 26 // Motor A direction
#define ENB 33 // Enable for Motor B
#define IN3 27 // Motor B direction
#define IN4 14 // Motor B direction

// Blynk authentication and Wi-Fi credentials from "credentials.h"
#ifndef AUTH_TOKEN
#error "AUTH_TOKEN not defined in credentials.h"
#endif
#ifndef WIFI_SSID
#error "WIFI_SSID not defined in credentials.h"
#endif
#ifndef WIFI_PASS
#error "WIFI_PASS not defined in credentials.h"
#endif

char auth[] = AUTH_TOKEN; 
char ssid[] = WIFI_SSID;  
char pass[] = WIFI_PASS;  

// Variables for joystick values
int x = 50; // Default value for X-axis
int y = 50; // Default value for Y-axis

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  // Configure motor control pins as output
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Set initial states to LOW
  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  // Initialize Blynk connection
  Blynk.begin(auth, ssid, pass);
  Serial.println("Blynk connected. Ready to control the car!");
}

void carForward() {
  digitalWrite(ENA, HIGH); 
  digitalWrite(ENB, HIGH); 
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carBackward() {
  digitalWrite(ENA, HIGH); 
  digitalWrite(ENB, HIGH); 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carLeft() {
  digitalWrite(ENA, LOW); // Stop Motor A
  digitalWrite(ENB, HIGH); // Motor B active
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carRight() {
  digitalWrite(ENA, HIGH); // Motor A active
  digitalWrite(ENB, LOW); // Stop Motor B
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void carStop() {
  digitalWrite(ENA, LOW); 
  digitalWrite(ENB, LOW); 
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

// Blynk virtual pin handlers for joystick values
BLYNK_WRITE(V0) {
  x = constrain(param[0].asInt(), 0, 100); // Constrain X-axis value to [0, 100]
}

BLYNK_WRITE(V1) {
  y = constrain(param[0].asInt(), 0, 100); // Constrain Y-axis value to [0, 100]
}

// Function to control the car based on joystick input
void smartCarControl() {
  if (y > 70) {
    carForward(); 
    Serial.println("Moving Forward");
  } else if (y < 30) {
    carBackward(); 
    Serial.println("Moving Backward");
  } else if (x < 30) {
    carLeft(); 
    Serial.println("Turning Left");
  } else if (x > 70) {
    carRight(); 
    Serial.println("Turning Right");
  } else {
    carStop(); 
    Serial.println("Stopped");
  }
}

void loop() {
  Blynk.run();           // Run the Blynk process
  smartCarControl();     // Call the car control function
}
