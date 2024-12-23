// Include necessary libraries
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL60k6nWKVi"
#define BLYNK_TEMPLATE_NAME "WiFi Controlled Car"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// Replace with your network credentials
char ssid[] = "Wokwi-GUEST"; // Wi-Fi SSID
char pass[] = "";            // Wi-Fi password

// Replace with your Blynk Auth Token
char auth[] = "";

// Motor control pins
#define ENA 32 // PWM for Motor A
#define IN1 25 // Motor A direction
#define IN2 26 // Motor A direction
#define ENB 33 // PWM for Motor B
#define IN3 27 // Motor B direction
#define IN4 14 // Motor B direction

// Variables for joystick values
int x = 0;
int y = 0;
int speed = 255; // Speed is fixed for now, but can be adjusted using another slider

// Joystick values from Blynk
BLYNK_WRITE(V0) { x = param[0].asInt(); }
BLYNK_WRITE(V1) { y = param[0].asInt(); }

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Set motor pins as outputs
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Connect to Blynk
  Blynk.begin(auth, ssid, pass);
  Serial.println("Blynk connected. Ready to control motors!");
}

void loop() {
  Blynk.run();
  controlMotors();
}

// Control motors based on joystick values
void controlMotors() {
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
    Serial.println("Stopping");
  }
}

// Movement functions
void carForward() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carBackward() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carLeft() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void carRight() {
  analogWrite(ENA, speed);
  analogWrite(ENB, speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void carStop() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
