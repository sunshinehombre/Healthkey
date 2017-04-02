// ------------------------------------------- //
// Healthkey: The Automatic Keyboard Sanitizer //
// ------------------------------------------- //
//
// Healthkey promotes your well-being by automatically sanitizing your keyboard,
// preventing the transmission of pathogens from your hand to your mouth.
//
// How it works: A Passive Infrared sensor detects if the keyboard has not been used for a period of time.
// This information is fed into an Arduino 101, which then initiates the cleaning procedure.
// In this hackathon, a passive cleaning tool is used but improved methods of sanitizing can be implemented.
// These include the application of isopropyl alcohol in aerosol form and the use of ultraviolet germicidal light.
//
// Written by Jeffry Lew
// April 2, 2017
//

// Pins for motor driver
const int
PWM = 3,
STBY = 11,
AIN1CW = 12,
AIN2CCW = 13;

// Motor timing variables
long startTime;
long elapsedTime;

const int THRESHOLD = 10000; // 10 second observation period

// Pin for PIR sensor
const int PIR = 7;

// Time for noting observation period
long observeTime = 0;

// Flag to keep track if already swept when no motion
boolean sweptAlready = false;

void setup() {
  Serial.begin(9600);
  
  // Initialize pins for motor direction
  pinMode(AIN1CW, OUTPUT);
  pinMode(AIN2CCW, OUTPUT);

  // Initialize pin for PIR sensor
  pinMode(PIR, INPUT);
}

void loop() {
  // Obtain value from PIR sensor
  // LOW = motion detected, HIGH = no motion
  int pirVal = digitalRead(PIR);

  // No motion detected
  if (pirVal == HIGH) {
    // Check if observed time elapsed is greater than threshold
    if (millis() - observeTime > THRESHOLD) {
      // Check if have not swept yet
      if (!sweptAlready) {
        // Turn on motor driver
        digitalWrite(STBY, HIGH);
        
        // Set speed of motor (255 max value)
        analogWrite(PWM, 255);

        // Keep track of how long the motor runs for
        startTime = millis();

        // Sweep from left to right
        digitalWrite(AIN1CW, LOW);
        digitalWrite(AIN2CCW, HIGH);
        delay(5500);

        // Calculate how long the motor ran for
        elapsedTime = millis() - startTime;
        Serial.println("LR Motor ran for");
        Serial.println(elapsedTime);

        // Stop motor
        digitalWrite(AIN1CW, LOW);
        digitalWrite(AIN2CCW, LOW);

        // Sweep from right to left
        digitalWrite(AIN1CW, HIGH);
        digitalWrite(AIN2CCW, LOW);
        delay(elapsedTime);
        Serial.println("RL Motor ran for");
        Serial.println(elapsedTime);

        // Stop motor
        digitalWrite(AIN1CW, LOW);
        digitalWrite(AIN2CCW, LOW);

        // Turn off motor driver
        digitalWrite(STBY, LOW);

        // Now that we have swept, update flag
        sweptAlready = true;

        // Reset observation time tracking after sweeping
        observeTime = millis();
        delay(2000);
        
      } else {
        
        // Even though time elapsed greater than threshold,
        // do not sweep because already did it once

        // sweptAlready is true
      }
    } else {
      
      // No motion detected, but still within threshold
      Serial.println("No motion detected, but within threshold. No sweeping yet.");
       
    }
  } else {
    // Motion detected (i.e. pirVal LOW)
    // Means that someone is using keyboard, so will need to sweep
    Serial.println("Motion detected at");
    Serial.println(millis());
    sweptAlready = false;

    // Reset observation time tracking after motion detected
    observeTime = millis();
    delay(2000);
  }
}
