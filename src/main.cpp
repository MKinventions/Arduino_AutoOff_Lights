/**
 * --------------------------------------------------------
 * Project     : Ultrasonic Relay Control with Timeout
 * Author      : Madhan Kumar Chiruguri
 * Brand       : MKinventions
 * Description : Controls a relay and LEDs based on object
 *               detection using an ultrasonic sensor with
 *               a timed auto-off feature.
 * --------------------------------------------------------
 */

 #include <Arduino.h>

 #define DEBUG true  // Set to false to disable debug prints
 //
 #if DEBUG
   #define DEBUG_PRINT(x) Serial.print(x)
   #define DEBUG_PRINTLN(x) Serial.println(x)
 #else
   #define DEBUG_PRINT(x)
   #define DEBUG_PRINTLN(x)
 #endif
 
 // === Pin Definitions ===
 const int trig = 5;
 const int echo = 6;
 const int ultrasonicSensor_led = 4;
 const int relay = 12;
 const int relay_Led = 3;
 
 // === Configuration ===
 const int timer = 180;            // Auto-off time in seconds (3 minutes)
 const int setDistance = 80;       // Trigger distance in cm
 const int hysteresis = 5;         // Hysteresis buffer in cm
 
 // === State ===
 long distance = 0;
 unsigned long lastDetectionTime = 0;
 bool objectPreviouslyDetected = false;
 
 void measureDistance();
 
 void setup() {
     delay(1000); // Wait before setup to allow USB sync
     Serial.begin(9600);
     DEBUG_PRINTLN("System Started");
 
     pinMode(trig, OUTPUT);
     pinMode(echo, INPUT);
     pinMode(ultrasonicSensor_led, OUTPUT);
     pinMode(relay, OUTPUT);
     pinMode(relay_Led, OUTPUT);
 
     digitalWrite(relay, LOW);
     digitalWrite(relay_Led, LOW);
     digitalWrite(ultrasonicSensor_led, LOW);
 }
 
 void loop() {
     measureDistance();
 
     if (distance > 100) distance = 100;
 
     DEBUG_PRINT("Distance (cm): ");
     DEBUG_PRINTLN(distance);
 
     // Visual LED indication
     digitalWrite(ultrasonicSensor_led, (distance <= setDistance) ? HIGH : LOW);
 
     if (!objectPreviouslyDetected && distance <= setDistance) {
         // Object detected
         digitalWrite(relay, HIGH);
         digitalWrite(relay_Led, HIGH);
         lastDetectionTime = millis();
         objectPreviouslyDetected = true;
         DEBUG_PRINTLN("Object Detected: Relay ON");
     }
     else if (objectPreviouslyDetected && distance > (setDistance + hysteresis)) {
         // Object out of range
         if (millis() - lastDetectionTime >= timer * 1000UL) {
             digitalWrite(relay, LOW);
             digitalWrite(relay_Led, LOW);
             objectPreviouslyDetected = false;
             DEBUG_PRINTLN("Timeout Passed: Relay OFF");
         } else {
             DEBUG_PRINT("Waiting... Timer: ");
             DEBUG_PRINT((millis() - lastDetectionTime) / 1000);
             DEBUG_PRINTLN("s");
         }
     }
     else if (objectPreviouslyDetected && distance <= setDistance) {
         // Object still detected — reset timer
         lastDetectionTime = millis();
         DEBUG_PRINTLN("Object Still Detected: Timer Reset");
     }
 
     delay(200); // Sensor read delay
 }
 
 void measureDistance() {
     digitalWrite(trig, LOW);
     delayMicroseconds(2);
     digitalWrite(trig, HIGH);
     delayMicroseconds(10);
     digitalWrite(trig, LOW);
 
     long duration = pulseIn(echo, HIGH, 30000); // Timeout in 30ms
     if (duration == 0) {
         distance = 999;
     } else {
         distance = duration * 0.034 / 2;
     }
 }
 