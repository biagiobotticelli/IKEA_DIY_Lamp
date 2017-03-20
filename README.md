# IKEA PS 2014 DIY Lamp
During a visit in an IKEA store, I saw the PS 2014 lamp. It's a wonderful manually expandable lamp whose movement was controlled by a twine...
<p align="center">
  <img src="https://github.com/biagiobotticelli/IKEA_DIY_Lamp/blob/master/images/ikea-ps-pendant-lamp.JPG">
</p>
Since I'm a Computer Science Engineer, the first thing that I thought was to find a solution to add remote control to the lamp!

The first step that I made was to hack the hardware: to do this, I replaced the string with a stepper motor with a threaded rod. Then I added a microcontroller: in particular, I've used a **NodeMCU ESP8266** that is an *Open-source*, *Interactive*, *Programmable*, *Low cost* and *WI-FI enabled* integrated development kit.
It can be easily configured and manipulated using the Arduino IDE and it allows to reduce the time to prototype the hardware.
<p align="center">
  <img src="https://github.com/biagiobotticelli/IKEA_DIY_Lamp/blob/master/images/nodemcu_devkit.jpg">
</p>
At the end of the configuration of the harware, I have the circuit that can be represented as:
<p align="center">
  <img src="https://github.com/biagiobotticelli/IKEA_DIY_Lamp/blob/master/images/Circuit.png">
</p>

The last thing to do was to design a firmware for the NodeMCU, using the Arduino IDE.
The two critical parts were:
1. *How to control the lamp;*
2. *How to interact with the stepper motor.*

# The largest heading
## The second largest heading
###### The smallest heading

**This is bold text**

*This text is italicized*

**This text is _extremely_ important**

To format code or text into its own distinct block, use triple backticks.
```
git status
git add
git commit
```

Link:
This site was built using [GitHub Pages](https://pages.github.com/).

List:
- George Washington
- John Adams
- Thomas Jefferson

Numerated:
1. James Madison
2. James Monroe
3. John Quincy Adams

<p align="center">
  <b>Some Links:</b><br>
  <a href="#">Link 1</a> |
  <a href="#">Link 2</a> |
  <a href="#">Link 3</a>
  <br><br>
  <img src="http://s.4cdn.org/image/title/105.gif">
</p>
