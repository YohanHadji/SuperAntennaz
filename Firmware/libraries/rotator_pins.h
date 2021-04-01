/*!
* @file rotator_pins.h
*
* It is a header file for pin mapping.
*
* Licensed under the GPLv3
*
*/

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
