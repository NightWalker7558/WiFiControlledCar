#include "credentials.h"

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL60k6nWKVi"
#define BLYNK_TEMPLATE_NAME "WiFi Controlled Car"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Define motor pins for L293D
#define ENA 25 // PWM pin for Motor A
#define IN1 26 // Motor A Input 1
#define IN2 27 // Motor A Input 2

// Blynk authentication and Wi-Fi credentials
char auth[] = AUTH_TOKEN;
char ssid[] = WIFI_SSID;
char pass[] = WIFI_PASS;

// Variables for joystick values
int x = 50;
int y = 50;

void setup() {
  Serial.begin(115200);

  // Set motor pins as output
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Initialize Blynk
  Blynk.begin(auth, ssid, pass);
}

// Read joystick X-axis values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}

// Read joystick Y-axis values
BLYNK_WRITE(V1) {
  y = param[0].asInt();
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

// Motor control functions
void carForward() {
  analogWrite(ENA, 200); // Adjust PWM value as needed (0-255)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void carBackward() {
  analogWrite(ENA, 200); // Adjust PWM value as needed (0-255)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void carLeft() {
  analogWrite(ENA, 150); // Reduce speed for turning (adjust as needed)
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
}

void carRight() {
  analogWrite(ENA, 150); // Reduce speed for turning (adjust as needed)
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
}

void carStop() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() {
  Blynk.run();   // Run the Blynk function
  smartCarControl(); // Call the car control function
}
