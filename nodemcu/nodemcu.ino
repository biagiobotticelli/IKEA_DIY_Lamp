//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>
#include <ESP8266WiFi.h>
#include <AccelStepper.h>
#include <MultiStepper.h>
#include <EEPROM.h>
#include <SimpleTimer.h>


// Auth Token
char auth[] = "Your_Auth_Token";

// WiFi credentials
char ssid[] = "Your_WiFi_SSID";
char pass[] = "Your_WiFi_PSW";

// Pins for Stepper Control
const int sleepPin = D0;
const int stepPin = D1;
const int dirPin = D2;

// Pin for the Relay
const int relayPin = D4;

// Pin for the LED
const int ledPin = D6;

// Max Speed and Speed of the Stepper
int sp = 750;
int accel = 80000;

// MIN and MAX values for Lamp Gap control
const int MIN = 0;
const int MAX = 2400;
int current = 0;
int prom_pos = 0;
int light = 0;

// Timer
SimpleTimer timer;
int t = 1600;

// Sectors of flash memory where to store the current position
#define H_ADDRESS 500
#define L_ADDRESS 501
#define LIGHT_ADDRESS 502

// Flag 
bool isFirstConnect = true;
bool isOn = false;

// Stepper Definition
AccelStepper stepper(1, stepPin, dirPin);

void readData() {
  
  // Retrieve from EEPROM the position
  byte high = EEPROM.read(H_ADDRESS);
  byte low = EEPROM.read(L_ADDRESS);
  prom_pos = word(high,low);

}

void saveData() {
    
  // Write the position to EEPROM
  EEPROM.write(H_ADDRESS,highByte(current));
  EEPROM.write(L_ADDRESS,lowByte(current));
  EEPROM.commit();

}

void moveLamp(int pos) {
  
  // Turn ON the Stepper
  digitalWrite(sleepPin, HIGH);
  
  if (pos == MAX) {
    pos = MAX-15;
  }
  else if (pos == MIN) {
    pos = MIN+10;
  }
  stepper.moveTo(pos);
  while (stepper.currentPosition() != pos) {
    stepper.run();
  }

  // Turn OFF the Stepper
  digitalWrite(sleepPin, LOW);
  
}

void moveOn() {
  int in = current;
  if (in >= MAX) {
    in = MAX-15;
  }
  if(current <= MAX) {
    
    if (current<=1600) {
      moveLamp(in+800);
      current = in+800;
    }
    else {
      moveLamp(in);
      current = MAX;
    }
    saveData();
    Blynk.virtualWrite(V0, current);
  }
}

void moveOff() {
  
  int in = current;
  if(in != MIN) {
    if(in > 800) {
      moveLamp(in-800);
      current = in-800;
    }
    else {
      moveLamp(MIN+10);
      current = MIN;
    }
    saveData();
    Blynk.virtualWrite(V0, current);
  }

}

void setup() {
   
  //Serial.begin(115200);
  EEPROM.begin(512);

  // Begin the Blynk session
  Blynk.begin(auth, ssid, pass);
 
  // Put the Stepper in SLEEP mode
  pinMode(sleepPin, OUTPUT);
  digitalWrite(sleepPin, LOW);

  // Turn off the Wi-Fi LED
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Turn ON and OPEN the Lamp as DEFAULT
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, HIGH);
  Blynk.virtualWrite(V1,1);

  if(Blynk.connected()) {
    // Lamp is connected -> LED ON
    digitalWrite(ledPin, HIGH);
    Blynk.virtualWrite(V1,1);
  }
  // Read position from EEPROM
  readData();

  // Set correct position
  stepper.setCurrentPosition(prom_pos);
  current = prom_pos;

  // Set Stepper Speed and Acceleration
  stepper.setMaxSpeed(sp);
  stepper.setSpeed(sp);
  stepper.setAcceleration(accel);

  // Open the Lamp
  if(current != MAX) {
    timer.setTimer(t, moveOn, 3);
  }
  isOn = true;
}

// Sync in case of disconnection
BLYNK_CONNECTED() {
  if (isFirstConnect) {
    Blynk.syncAll();
    isFirstConnect = false;
  }
}

// The STEPPER is connected to VIRTUAL_PIN_0 of Blynk App
BLYNK_WRITE(V0) {
  // Allow the stepper only if the lamp is on
  if (isOn) {
    // read the value of STEPPER
    int input = param.asInt();
    
    moveLamp(input);
    current = input;
    saveData();
  }
  else {
    Blynk.virtualWrite(V0, current);
  }
}

// The BUTTON is connected to VIRTUAL_PIN_1 of Blynk App
BLYNK_WRITE(V1) {
  
  // read the value of button
  int l = param.asInt();
  
  // Light -> ON
  if(l == 1) {
    // Turn ON
    digitalWrite(relayPin, HIGH);
  
    // Read position from EEPROM
    readData();
    
    current = prom_pos;
    if(current != MAX) {
      timer.setTimer(t, moveOn, 3);
    }
    isOn = true;
  }
  
  // Light -> OFF
  else if (l==0 && isOn){
    // Turn OFF
    digitalWrite(relayPin, LOW);
    isOn = false;
  
    // CLOSE the Lamp
    if(current != MIN) {
        timer.setTimer(t, moveOff, 3);
    }
    
  }
  
}

void loop() {
  Blynk.run();
  timer.run();
}
