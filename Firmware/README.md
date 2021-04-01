# SatNOGS Rotator Firmware for Arduino UNO/CNC Shield, for DIRECT CONNECTION TO PC via serial

Firmware [SatNOGS Rotator Controller Based on Arduino UNO and CNC V3 Shield](https://wiki.satnogs.org/SatNOGS_Arduino_Uno/CNC_Shield_Based_Rotator_Controller).

Repository includes all source files for the SatNOGS rotator controller Firmware, based on the Arduino UNO. using the CNC v3 Shield, instead of the custom SatNOGS PCB.

Electronics can be found on [satnogs-rotator-controller](https://wiki.satnogs.org/SatNOGS_Arduino_Uno/CNC_Shield_Based_Rotator_Controller)

## Instructions

In order to use this code, you need to install
 * Arduino IDE
 * [AccelStepper library](http://www.airspayce.com/mikem/arduino/AccelStepper/index.html)
 * [PID_v1 library](https://github.com/br3ttb/Arduino-PID-Library)
 * Wire library
 * Gpredict


## Easycomm implemantation

* AZ, Azimuth, number - 1 decimal place [deg]
* EL, Elevation, number - 1 decimal place [deg]
* SA, Stop azimuth moving
* SE, Stop elevation moving
* RESET, Move to home position
* PARK, Move to park position
* IP, Read an input, number
    * Temperature = 0
    * SW1 = 1
    * SW2 = 2
    * Encoder1 = 3
    * Encoder2 = 4
    * Load of M1/AZ = 5
    * Load of M2/EL = 6
    * Speed of M1/AZ (DPS) = 7
    * Speed of M2/EL (DPS) = 8
* VE, Request Version
* GS, Get status register, number
    * idle = 1
    * moving = 2
    * pointing = 4
    * error = 8
* GE, Get error register, number
    * no_error = 1
    * sensor_error = 2
    * homing_error = 4
    * motor_error = 8
    * over_temperature = 12
    * wdt_error = 16
* VL, Velocity Left ,number [mdeg/s]
* VR, Velocity Right, number [mdeg/s]
* VU, Velocity Up, number [mdeg/s]
* VD, Velocity Down, number [mdeg/s]
* CR, Read config, register [0-x]
    * Gain P for M1/AZ = 1
    * Gain I for M1/AZ = 2
    * Gain D for M1/AZ = 3
    * Gain P for M2/EL = 4
    * Gain I for M2/EL = 5
    * Gain D for M2/EL = 6
    * Azimuth park position = 7
    * Elevation park position = 8
    * Control mode (position = 0, speed = 1) = 9
* CW, Write config, register [0-x]
    * Gain P for M1/AZ = 1
    * Gain I for M1/AZ = 2
    * Gain D for M1/AZ = 3
    * Gain P for M2/EL = 4
    * Gain I for M2/EL = 5
    * Gain D for M2/EL = 6
    * Azimuth park position = 7
    * Elevation park position = 8
    * This reg is set from Vx commands control mode (position = 0, speed = 1) = 9
* RB, custom command to reboot controller

## Controller Configurations

* Stepper Motor
    * Endstops
    * Encoders, optional
    * UART or R485 (For both options the firmware is the same)


## Pins Configuration

```
   #ifndef ROTATOR_PINS_H_
   #define ROTATOR_PINS_H_
   //#define M1IN1 10 ///< Motor 1 PWM pin
   #define M1IN1 2 ///< Motor 1 PWM pin
   #define M1IN2 5  ///< Motor 1 PWM pin
   #define M1SF  7  ///< Motor 1 digital input, status flag for DC Motor Drivers
   #define M1FB  A1 ///< Motor 1 analog input, current/load feedback for DC Motor Drivers
   #define M2IN1 3 ///< Motor 2 PWM pin
   #define M2IN2 6  ///< Motor 2 PWM pin
   #define M2SF  7 ///< Motor 2 digital input, status flag for DC Motor Drivers
   #define M2FB  A0 ///< Motor 2 analog input, current/load feedback for DC Motor Drivers
   #define MOTOR_EN 8 ///< Digital output, to enable the motors
   #define SW1 11 ///< Digital input, to read the status of end-stop for motor 1
   #define SW2 9 ///< Digital input, to read the status of end-stop for motor 2
   #define RS485_DIR 2 ///< Digital output, to set the direction of RS485 communication
   #define SDA_PIN 3 ///< I2C data pin
   #define SCL_PIN 4 ///< I2C clock pin
   #define PIN12 12 ///< General purpose I/O pin
   #define PIN13 13 ///< General purpose I/O pin
   #define A2    A2 ///< General purpose I/O & analog pin
   #define A3    A3 ///< General purpose I/O & analog pin
   #endif /* ROTATOR_PINS_H_ */
```

## Testing with hamlib - rotctl or with Serial Monitor

Software to install
Final pieces to the preparation work are to install a couple of pieces of software. firstly Hamlib. This will be used to communicate between GPredict and the Arduino. Secondly we will install the main prediction software, GPredict. We will not be going into detail about setup of GPredict but assume that you have followed the installation guide and had a play around with it so you can understand what it is and how it works.

Install hamlib

Download the Hamblib software and follow the installation process. this may require you installing it with Administrator rights. using device manager find the port for the Arduino, for the purposes of this we will assume it is COM7

Using a text editor, like Notepad++, to create a file with the code below in it. Note where COM7 is and amend with your own COM port. Save as a batch file (i.e. with th extension .bat file in the same folder as rotctld (Usually found in C:\Program Files (x86)\hamlib-w64-3.2\bin )

rotctld -m 202 -r COM7 -s 9600 -T 127.0.0.1 -t 4533 -C timeout=500 -C retry=0 -vvvvvvvv > pause
Install GPredict

Download and install GPredict. Next we will need to add a rotator. Go to Edit > Preferences > Interfaces > Rotators > Add New

Give it a name. For example Arduino. The port is localhost 4533

That's it for setup. next we will test the system

Testing
We will need to do a few things to get the system running.

Start the .bat file
In GPredict open up the Antenna module and click Engage
You should see that in the batch file output a series of commands being sent to the Arduino
Check that both the Az and Ele work correctly by manually driving the rotator
Occasionally this might not work first time. A restart of both hamlib and Gpredict worked

Tweaks
There is a huge variety in parts that can get bought, here are a few tweaks that may be necessary.

Motors are turning the wrong way - This may need the pins checking

It is only moving half the distance - Change the gear ratio in the main sketch

The limit switches are not working - Change the following code in the main sketch

define DEFAULT_HOME_STATE LOW ///< Change to LOW according to Home sensor
Operation
Operating is as simple as selecting a satellite from GPredict and then selecting track. The rotator will then follow that satellite or object.f



## Contribute

The main repository lives on [Gitlab](https://gitlab.com/librespacefoundation/satnogs/satnogs-rotator-firmware) and all Merge Request should happen there.

## License

[![Libre Space Foundation](https://img.shields.io/badge/%C2%A9%202014--2018-Libre%20Space%20Foundation-6672D8.svg)](https://librespacefoundation.org/)

Licensed under the [GPLv3](LICENSE)
