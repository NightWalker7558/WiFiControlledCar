# WiFi-Controlled Car

This project is a WiFi-controlled car that can be remotely operated using a smartphone. It integrates the ESP32 microcontroller for WiFi connectivity and the L293D motor driver for motor control. The car uses the Blynk IoT app for real-time commands and logs its movements to Firebase's Real-time Database, enabling remote tracking and control.

## Components Required

### Hardware
- ESP32 NodeMCU
- L293D Motor Driver (H-Bridge)
- DC Motors (2)
- Chassis
- Wheels (2)
- 1.2 V Ni-MH Batteries (4)
- 7805 Voltage Regulator
- Breadboard
- Jumper Wires
- USB Cable
- Switch Button

### Software
- Arduino IDE
- Blynk IoT App
- Firebase Console

## Packages Required
- Blynk by Volodymyr Shymanskyy
- Firebase Arduino ESP Client by mobizt
- NTPClient by Fabrice Weinberg

## Boards Required
- DOIT ESP32 DevKit V1 ([GitHub Repository](https://github.com/espressif/arduino-esp32))

## Setup

### 1. Install Required Libraries and Boards
1. Install the **ESP32 Board**:
   - Open Arduino IDE.
   - Go to `File > Preferences`.
   - In the "Additional Board Manager URLs" field, add the following URL:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
   - Go to `Tools > Board > Boards Manager`, search for "ESP32" and install the board.

2. Install the **Firebase Arduino ESP Client**:
   - Go to `Sketch > Include Library > Manage Libraries`.
   - Search for **Firebase Arduino ESP Client by mobizt** and install it.

3. Install the **NTP Client Library**:
   - Download the library from [here](https://github.com/taranais/NTPClient/archive/master.zip).
   - Go to `Sketch > Include Library > Add .ZIP Library…` in Arduino IDE.
   - Select the downloaded `.zip` file to install the library.

### 2. Create Firebase Project
1. Go to [Firebase](https://firebase.google.com) and sign in using a Google account.
2. Click **Get Started** and then **Add project** to create a new project.
3. Give a name to your project, e.g., `ESP Firebase Demo`.
4. Disable the option **Enable Google Analytics for this project** as it is not needed, and click **Create project**.
5. After the setup is complete, click **Continue** to be redirected to your project console page.

### 3. Set Authentication Methods
1. On the left sidebar, click on **Authentication** and then on **Get started**.
2. Select the option **Email/Password** and enable it. Click **Save**.
3. Add a user by going to the **Users** tab and clicking **Add User**.
4. Enter an email address and password for the user. Save these credentials for later.

### 4. Get Project API Key
1. On the left sidebar, click on **Project Settings**.
2. Copy the **Web API Key** to a safe place for use in your ESP32 code.

### 5. Set up Realtime Database
1. On the left sidebar, click on **Realtime Database** and then on **Create Database**.
2. Select your database location based on your physical location.
3. Start the database in **test mode**. Click **Enable**.

### 6. ESP32 Datalogging (Firebase Realtime Database)

Program the ESP32 to:

- Authenticate as a user with the email and password created earlier.
- Log data (e.g., joystick positions or movement commands) to the database.
- Use the NTP Client library to retrieve timestamps.

## Wokwi Simulation

The project includes a simulation on Wokwi Cloud, where the ESP32 microcontroller and L293D motor driver are visualized with LEDs representing motor control signals. This simulation provides a real-time representation of the motor outputs.

### Steps for Simulation

1. Open the Wokwi project link.
2. Set the `ssid` variable to `"Wokwi-GUEST"` in the code.
3. Leave the `pass` variable as an empty string.
4. Enter your Firebase auth token in the `auth` variable.
5. Run the simulation to control the car's movements virtually using the Blynk IoT app.

## Features

- **Remote Control**: Uses the Blynk IoT app's joystick to control the car's direction—forward, backward, left, and right.
- **Real-Time Logging**: Firebase Realtime Database logs movements with timestamps for analysis.
- **Energy Optimization**: Implements modem sleep mode on the ESP32 to reduce idle power consumption.
- **Simulation**: Motor outputs visualized via LEDs in the Wokwi simulator.

## Limitations

- Requires manual intervention during peak current scenarios, such as starting the motors.
- Significant power consumption reduces operational time.
- Analytics integration via Firebase's paid services is limited.

## Contributors

- [Muhammad Ashhub Ali](https://github.com/NightWalker7558)
- [Adeel Ahmed Qureshi](https://github.com/itsAdee)
- [Muhammad Zubair Khalid](https://github.com/ZubairKhalid42)
- [Wasif Mehmood](https://github.com/wasif2mehmood)
