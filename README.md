# esp-ar-drone

Firmware for the ESP8266 designed to control a Parrot AR Drone 2.0 through UDP, using data passed in through its serial port.

This project came into being as one example (thanks, Geoff) of controlling something using my [Power Glove UHID](https://hackaday.io/project/8455-power-glove-uhid) renovation project and largely exists as a just a fun little project. The code is far from perfect, but it currently works and wows! I certainly have not tested it enough nor fully through the Ar Drone's SDK, and warn you to **_use this code at your own risk_**.

## Installation

Add the ESP8266 board config (details found **[here](https://github.com/esp8266/Arduino)**) to your Arduino installation to obtain board support as well the necessary libraries. Then, simply clone this repo to your Arduino sketchbook folder and upload via the Arduino IDE (I'm using an Adafruit Huzzah ESP8266).

You will also have to clone the PGSensorHub repo and upload that to whatever board you may be using to acquire data from your Power Glove (or what have you) (I'm using a Teensy LC).

## Usage

Initially, the ESP will continuously attempt to connect to the AR Drone's AP. After connecting, it waits to receive navdata from the drone before briefly setting the emergency flag using the REF command and calibrating the drone's trim. Afterwards it will continuously send REF and PCMD commands to the drone based on commands received serially.

The ESP expects data coming from the serial port formatted (in ASCII) as follows:

```
K,G,(-)###,(-)###<LF>
```

Where **K** is a single `char` representing the pressed button on the keypad, **G** is a single `char` representing the current gesture being performed using the Power Glove and **(-)###** are pitch and roll values (`int` type), respectively, with optional negative signs. The string is then tokenized in function `debugcommander()`.

Currently the following Power Glove buttons and gestures are recognized:
###Buttons
- 'S' \- The Select button, used to send the emergency command
- 'P' \- The Start button, used to send takeoff/land command when landed/flying, respectively
- '0' \- The 0 or Center buttons, used to calibrate trim (use only when grounded)

###Gestures
- 'R' \- Relaxed gesture, when flying, puts the drone in hover mode.
- 'F' \- Fist gesture, when flying, causes the drone to move forward/back and left/right, according to the level of tilt in the Power Glove
- 'P' \- Pointing gesture, when flying, causes the drone to move up/down and yaw Cw/CCW, according to the level of tilt in the Power Glove

At this time the ESP must be reset anytime the drone is powered down. 

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request :D

## History

Version 0.1: initial upload

## Credits

Inspiration for AT command creation borrowed from [node-ar-drone](https://github.com/felixge/node-ar-drone) by felixge.

Float table and masks.h from [AR.Drone-ESP8266](https://github.com/jorisplusplus/AR.Drone-ESP8266) by jorisplusplus.

## License

Refer to LICENSE file
