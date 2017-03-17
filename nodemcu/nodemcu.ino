//#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

#include <SimpleTimer.h>

#include <Blynk.h>
#include <BlynkSimpleEsp8266.h>

#include <AccelStepper.h>
#include <MultiStepper.h>

#include <ESP8266WiFi.h>

#include <EEPROM.h>

// Auth Token
char auth[] = "Your-Auth-Token";

// WiFi credentials
char ssid[] = "Your-ssid";
char pass[] = "Your-psw";

// Pins for Stepper Control
const int sleepPin = D0;
const int stepPin = D1;
const int dirPin = D2;

// Pin for the Relay
const int relayPin = D4;

// Pin for the LED
const int ledPin = D6;

// Max Speed and Speed of the Stepper
int sp = 780;
int accel = 80000;

// MIN and MAX values for Lamp Gap control
const int MIN = 0;
const int MAX = 2400;
int current = 0;
int prom_pos = 0;
int light = 0;

// Timer
SimpleTimer timerOn;
SimpleTimer timerOff;
int t = 1700;

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
  // DEBUG
  //Serial.print("Reading POSITION from EEPROM...");
  
  // Retrieve from EEPROM the position
  byte high = EEPROM.read(H_ADDRESS);
  byte low = EEPROM.read(L_ADDRESS);
  prom_pos = word(high,low);

  // DEBUG
  //Serial.print("EEPROM POS = ");
  //Serial.print(prom_pos);
  //Serial.println();
}

void saveData() {
  // DEBUG
  //Serial.print("Saving POS= ");
  //Serial.print(current);
  //Serial.println(" to EEPROM...");
  
  // Write the position to EEPROM
  EEPROM.write(H_ADDRESS,highByte(current));
  EEPROM.write(L_ADDRESS,lowByte(current));
  EEPROM.commit();
  
  //Serial.println("DONE!");
  //Serial.println();

}

void moveLamp(int pos) {
  
  // Turn ON the Stepper
  digitalWrite(sleepPin, HIGH);

  if (pos == MAX) {
    pos = 2385;
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
  if (in > MAX) {
    in = 2385;
  }
  if(current <= MAX) {
    
    //Serial.println("*** MOVE_ON ***");
    
    if (current<=1600) {
      //Serial.println("+800");
      moveLamp(in+800);
      current = in+800;
    }
    else {
      
      //Serial.println("MAX!");
      
      moveLamp(2385);
      current = MAX;
    }
    saveData();
    Blynk.virtualWrite(V0, current);
  }
  
}

void moveOff() {
  int in = current;
  if(in != MIN) {
    
    //Serial.println("*** MOVE_OFF ***");
    
    if(in > 800) {
      moveLamp(in-800);
      current = in-800;
    }
    else {
      moveLamp(MIN);
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
    timerOn.setTimer(t, moveOn, 3);
  }

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
    
    //Serial.println("*** STEP ***");
    
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
      timerOn.setTimer(t, moveOn, 3);
    }
    isOn = true;
  }
  
  // Light -> OFF
  else if (l== 0 && isOn){
    // Turn OFF
    digitalWrite(relayPin, LOW);
    isOn = false;
 
    // CLOSE the Lamp
    if(current != MIN) {
        timerOff.setTimer(t, moveOff, 3);
    }
  }
  
}

void loop() {
  Blynk.run();
  timerOn.run();
  timerOff.run();
}
