# IKEA PS 2014 DIY Lamp
During a visit in an IKEA store, I saw the PS 2014 lamp. It's a wonderful manually expandable lamp whose movement was controlled by a twine...
<p align="center">
  <img src="https://github.com/biagiobotticelli/IKEA_DIY_Lamp/blob/master/images/ikea-ps-pendant-lamp.JPG">
</p>
Since I'm a Computer Science Engineer, the first thing that I thought was to find a solution to add remote control!!

The first step that I made was to hack the hardware: to do this, I replaced the string with a **stepper motor** with a threaded rod. Then I added a **microcontroller**: in particular, I've used a **NodeMCU ESP8266** that is an *Open-source*, *Interactive*, *Programmable*, *Low cost* and *WI-FI enabled* integrated development kit.
It can be easily configured and manipulated using the Arduino IDE and it allows to reduce the time to prototype the hardware.
<p align="center">
  <img src="https://github.com/biagiobotticelli/IKEA_DIY_Lamp/blob/master/images/nodemcu_devkit.jpg">
</p>
At the end of the configuration of the harware, I have the **circuit diagram** that can be represented as:
<p align="center">
  <img src="https://github.com/biagiobotticelli/IKEA_DIY_Lamp/blob/master/images/Circuit.png">
</p>

The last thing to do was to design a **firmware** for the NodeMCU board, using the *Arduino IDE*.
The two critical parts were:
1. *How to control the lamp;*
2. *How to interact with the stepper motor.*

#### How to control the lamp
Since the **NodeMCU** boards allows to connect the lamp to my home Wi-Fi, I decided to interact with it using the **Blynk** mobile application (it's available both for Android and iOS).
**Blynk** is extremely easy to use with Arduino IDE: once downloaded the *Blynk Library*, to connect the device to a network requires only 3 fields: the *Authentication Token* (given by the mobile app), the *Wi-Fi SSID* and the *Wi-Fi Password*.
A very *simple sketch* could be:
```
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "Your-Auth-Token";
char ssid[] = "Your-WiFi-SSID";
char pass[] = "Your-WiFi-Password";

void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
}
```

#### How to interact with the stepper motor
In order to *open/close* the lamp, I need to control the stepper motor.
To do this, I've chosen the **AccelStepper** library which is an improvement of the basic *Arduino Stepper Library* since it supports *acceleration* and *deceleration*, a large variety of *stepper motors* and it also supports the *DRV8825 Driver Board* that I've chosen for the project. 

## Installation Instructions
The code is a *ready-to-work firmware* **ONLY IF** the project is **implemented _exactly_ as the circuit diagram**.
But it can be easy adapted according to the subjective purpose.
To install the firmware, simply download it, open it using the Arduino IDE and load it into the NodeMCU board.

<a href="http://www.youtube.com/watch?feature=player_embedded&v=37uV71KeJrg
" target="_blank"><img src="http://img.youtube.com/vi/37uV71KeJrg/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

The Demo is uploaded on YouTube: https://www.youtube.com/watch?v=37uV71KeJrg


## Useful Links
- <a href="https://www.linkedin.com/in/biagio-botticelli-444b87105/">My personal LinkedIn Profile;</a>
- <a href="https://nodemcu.readthedocs.io/en/master/">NodeMCU Documentation;</a>
- <a href="https://www.arduino.cc">Arduino website;</a>
- <a href="http://www.blynk.cc">Blynk Web Page;</a>
- <a href="http://www.airspayce.com/mikem/arduino/AccelStepper/">AccelStepper Library Website.</a>
</p>
