// // #include <EEPROM.h>
// #include <EEPROM.h>
// #include <Arduino.h>
// #include <Arduino_FreeRTOS.h>
// #include <timers.h>

// #define TRIG_PIN 5
// #define ECHO_PIN 6
// #define PIR_PIN 7
// #define LOAD_LED 11
// #define PIR_LED 3
// #define ULTRASONIC_LED 4
// #define RELAY 12
// #define BUTTON 8

// #define TIMER_DURATION pdMS_TO_TICKS(30000) // Timer duration in ticks (5 seconds)
// #define DISTANCE_ADDRESS 10

// byte distanceValue = 30;

// long distance;
// long duration;

// bool timerExpired = false;
// bool objectDetected = false;


// int lastButtonState = 0;
// int buttonCount = 0;

// TimerHandle_t xTimer;
// unsigned long startTime;

// // Task handles
// TaskHandle_t xHandleButtonTask = NULL;
// TaskHandle_t xHandleSensorTask = NULL;

// // Function Prototypes
// void vButtonTask(void *pvParameters);
// void vSensorTask(void *pvParameters);
// void vTimerCallback(TimerHandle_t xTimer);
// void vULTRASONIC();


// void(* resetFunc) (void) = 0; //declare reset function @ address 0


// /***********testing2*********/
// const char* firmwareVersion = "v1.0.2";

// void setup() {
//   Serial.begin(9600);
//   Serial.println("Restarted......!");

//   // Pin setup
//   pinMode(TRIG_PIN, OUTPUT);
//   pinMode(ECHO_PIN, INPUT);
//   pinMode(LOAD_LED, OUTPUT);
//   pinMode(PIR_LED, OUTPUT);
//   pinMode(ULTRASONIC_LED, OUTPUT);
//   pinMode(RELAY, OUTPUT);
//   pinMode(BUTTON, INPUT_PULLUP);
//   pinMode(PIR_PIN, INPUT_PULLUP);

//   // Initialize LEDs and relay to off
//   digitalWrite(LOAD_LED, LOW);
//   digitalWrite(PIR_LED, LOW);
//   digitalWrite(RELAY, LOW);

//   // Read the stored distance value from EEPROM
//   distanceValue = EEPROM.read(DISTANCE_ADDRESS);
//   Serial.println("Calibrated Distance : " + String(distanceValue) + " CM");

//   // Create tasks
//   xTaskCreate(vButtonTask, "Button Task", 255, NULL, 1, &xHandleButtonTask);
//   xTaskCreate(vSensorTask, "Sensor Task", 255, NULL, 1, &xHandleSensorTask);

//   // Create the timer
//   xTimer = xTimerCreate("Light Timer", TIMER_DURATION, pdFALSE, NULL, vTimerCallback);
// }

// void loop() {
//   // Nothing to do in loop, all tasks are managed by FreeRTOS
// }

// void vButtonTask(void *pvParameters) {

//   while (1) {
//     int currentButtonState = !digitalRead(BUTTON);
//     //  Serial.println("currentButtonState : " + String(currentButtonState));

//     if (currentButtonState != lastButtonState) {
//       lastButtonState = currentButtonState;

//       if (currentButtonState == HIGH) {
//         buttonCount++;
//         Serial.println("Button Count: " + String(buttonCount));

//         if (buttonCount == 2) {
//           vTaskDelay(pdMS_TO_TICKS(2000));//delay 2 seconds

//           // Perform provisioning
//           Serial.println("Provisioning Mode Activated");
//           for (int i = 0; i < 50; i++) { // 50 => 10seconds
//             Serial.println("i: " + String(i));
//             digitalWrite(ULTRASONIC_LED, HIGH);
//             vTaskDelay(pdMS_TO_TICKS(100));
//             digitalWrite(ULTRASONIC_LED, LOW);
//             vTaskDelay(pdMS_TO_TICKS(100));
//           }

//           vULTRASONIC();
//           EEPROM.write(DISTANCE_ADDRESS, distance);
          
//           buttonCount = 0;
//           // Reset the MCU
//           resetFunc();  //call reset
          
//         }
//       }
//     }

//     vTaskDelay(pdMS_TO_TICKS(100));
//   }
// }

// void vSensorTask(void *pvParameters) {
//   while (1) {
//     int pirSensor = digitalRead(PIR_PIN);
//     vULTRASONIC();

//     if (pirSensor == HIGH) {
//       digitalWrite(PIR_LED, HIGH);
//     } else {
//       digitalWrite(PIR_LED, LOW);
//     }

//     // if (distance < distanceValue){
//     //   digitalWrite(ULTRASONIC_LED, HIGH);
//     // }else{
//     //   digitalWrite(ULTRASONIC_LED, LOW);
//     // }

//     if (distance < distanceValue && pirSensor == HIGH) {
//       digitalWrite(LOAD_LED, HIGH);
//       digitalWrite(RELAY, HIGH);
//       digitalWrite(ULTRASONIC_LED, HIGH);
//       objectDetected = true;
//       // Serial.println("Object and Motion Detected!");

//       if (xTimerIsTimerActive(xTimer) == pdFALSE) {
//         xTimerStart(xTimer, 0);  // Start the timer if it's not active
//       }
//     } else if (objectDetected && distance > distanceValue + 5) {
//       objectDetected = false;
//       xTimerReset(xTimer, 0); // Reset and restart the timer if no object detected
//     }

//     // Serial.println("Distance: " + String(distance) + " cm, PIR: " + String(pirSensor));
//     vTaskDelay(pdMS_TO_TICKS(200));
//   }
// }

// void vTimerCallback(TimerHandle_t xTimer) {
//   Serial.println("Timer expired. Turning off lights.");

//   digitalWrite(LOAD_LED, LOW);
//   digitalWrite(RELAY, LOW);
//   digitalWrite(ULTRASONIC_LED, LOW);
//   objectDetected = false;
// }

// void vULTRASONIC() {
//   digitalWrite(TRIG_PIN, LOW);
//   delayMicroseconds(2);
//   digitalWrite(TRIG_PIN, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(TRIG_PIN, LOW);

//   duration = pulseIn(ECHO_PIN, HIGH);
//   distance = duration * 0.034 / 2;
// }