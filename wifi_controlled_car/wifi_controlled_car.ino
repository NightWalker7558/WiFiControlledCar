#include "credentials.h" // Include external credentials file

#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL60k6nWKVi"
#define BLYNK_TEMPLATE_NAME "WiFi Controlled Car"

#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <BlynkSimpleEsp32.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

// Provide the token generation process info.
#include "addons/TokenHelper.h"
// Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Motor control pins
#define ENA 32 // Enable for Motor A
#define IN1 25 // Motor A direction
#define IN2 26 // Motor A direction
#define ENB 33 // Enable for Motor B
#define IN3 27 // Motor B direction
#define IN4 14 // Motor B direction

// Blynk authentication and Wi-Fi credentials from "credentials.h"
char auth[] = AUTH_TOKEN; 
char ssid[] = WIFI_SSID;  
char pass[] = WIFI_PASS;  

int x = 50; // Default joystick X value
int y = 50; // Default joystick Y value

FirebaseData fbdo;
FirebaseAuth firebase_auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool firebaseReady = false; // Flag for Firebase connection
bool signupOK = false;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Movement log structure
struct MovementLog {
  String action;
  String startTime;
  String endTime;
};

String currentAction = "Stopped";
String actionStartTime = "";
#define MAX_LOGS 10
MovementLog logs[MAX_LOGS];
int logCount = 0;

void addMovementLog(String action, String startTime, String endTime) {
  if (logCount < MAX_LOGS) {
    logs[logCount++] = {action, startTime, endTime};
  } else {
    Serial.println("Log storage full. Consider sending data to Firebase.");
  }
}

String formatTimestamp() {
  // Ensure the NTP client has the latest time
  while (!timeClient.update()) {
    timeClient.forceUpdate();  // Force update if the NTP client is out of sync
  }

  // Get the formatted date and time from the NTP client
  String formattedDate = timeClient.getFormattedDate();

  int splitT = formattedDate.indexOf("T");
  String dayStamp = formattedDate.substring(0, splitT);
  String timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 1);


  return dayStamp + " " + timeStamp;
}

void sendLogsToFirebase() {
  if (logCount == 0 || !firebaseReady) return;

  for (int i = 0; i < logCount; i++) {
    FirebaseJson json;
    json.set("action", logs[i].action);
    json.set("startTime", logs[i].startTime);
    json.set("endTime", logs[i].endTime);

    // Push log entry to Firebase RTDB
    if (!Firebase.RTDB.pushJSON(&fbdo, "/movementLogs", &json)) {
      Serial.println("Failed to send log to Firebase.");
      return;
    }
  }

  Serial.println("Logs sent to Firebase successfully.");
  logCount = 0; // Clear logs after sending
}

void updateMovement(String action) {
  if (action != currentAction) {
    if (currentAction != "Stopped") {
      addMovementLog(currentAction, actionStartTime, formatTimestamp()); // Timestamp for both start and end
    }
    currentAction = action;
    actionStartTime = formatTimestamp();
  }
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
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, LOW);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void carRight() {
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, HIGH);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
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

BLYNK_WRITE(V0) {
  x = constrain(param[0].asInt(), 0, 100);
}

BLYNK_WRITE(V1) {
  y = constrain(param[0].asInt(), 0, 100);
}

void smartCarControl() {
  if (y > 70) {
    carForward();
    updateMovement("Forward");
  } else if (y < 30) {
    carBackward();
    updateMovement("Backward");
  } else if (x < 30) {
    carLeft();
    updateMovement("Left");
  } else if (x > 70) {
    carRight();
    updateMovement("Right");
  } else {
    carStop();
    updateMovement("Stopped");
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Initializing...");

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(ENA, LOW);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(ENB, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  WiFi.begin(ssid, pass);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");

  // NTP client initialization
  timeClient.begin();
  timeClient.setTimeOffset(18000);

  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;

  // Set user email and password for Firebase authentication
  firebase_auth.user.email = FIREBASE_EMAIL;
  firebase_auth.user.password = FIREBASE_PASSWORD;

  Firebase.begin(&config, &firebase_auth);
  Firebase.reconnectWiFi(true);

  Blynk.begin(auth, ssid, pass);
  Serial.println("Blynk connected. Ready to control the car!");

  actionStartTime = formatTimestamp();
}

void loop() {
  Blynk.run();
  smartCarControl();

  if (Firebase.ready()) {
    if (!firebaseReady) {
      firebaseReady = true;
      Serial.println("Firebase authenticated successfully!");
    }

    if (millis() - sendDataPrevMillis > 15000) {
      sendDataPrevMillis = millis();
      sendLogsToFirebase();
    }
  } else {
    firebaseReady = false;
    Serial.println("Waiting for Firebase authentication...");
  }
}
