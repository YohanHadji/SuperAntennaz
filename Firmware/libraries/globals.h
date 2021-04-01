/*!
* @file globals.h
*
* It is a file to define all global variables
*
* Licensed under the GPLv3
*
*/

#ifndef LIBRARIES_GLOBALS_H_
#define LIBRARIES_GLOBALS_H_

#include <Arduino.h>

/** Rotator status */
enum _rotator_status {
    idle = 1, moving = 2, pointing = 4, error = 8
};
/** Rotator Errors */
enum _rotator_error {
    no_error = 1, sensor_error = 2, homing_error = 4, motor_error = 8,
    over_temperature = 12, wdt_error = 16
};
/** Rotator Control Modes */
enum _control_mode {
    position = 0, speed = 1
};

struct _control{
    double input;          ///< Motor Position feedback in deg
    double input_prv;      ///< T-1 Motor Position feedback in deg
    double speed;          ///< Motor Rotation speed in deg/s
    double setpoint;       ///< Position set point in deg
    double setpoint_speed; ///< Speed set point in deg/s
    uint16_t load;         ///< Motor Load in mA
    double u;              ///< Control signal range 0-255
    double p, i, d;        ///< Control gains
};

struct _rotator{
    volatile enum _rotator_status rotator_status; ///< Rotator status
    volatile enum _rotator_error rotator_error;   ///< Rotator error
    enum _control_mode control_mode;              ///< Control mode
    bool homing_flag;                             ///< Homing flag
    int8_t inside_temperature;                    ///< Inside Temperature
    double park_az, park_el;                      ///< Park position for both axis
    uint8_t fault_az, fault_el;                   ///< Motor drivers fault flag
    bool switch_az, switch_el;                    ///< End-stop vales
};

_control control_az = { .input = 0, .input_prv = 0, .speed=0, .setpoint = 0,
                        .setpoint_speed = 0, .load = 0, .u = 0, .p = 8.0,
                        .i = 0.0, .d = 0.5 };
_control control_el = { .input = 0, .input_prv = 0, .speed=0, .setpoint = 0,
                        .setpoint_speed = 0, .load = 0, .u = 0, .p = 10.0,
                        .i = 0.0, .d = 0.3 };
_rotator rotator = { .rotator_status = idle, .rotator_error = no_error,
                     .control_mode = position, .homing_flag = false,
                     .inside_temperature = 0, .park_az = 0, .park_el = 0,
                     .fault_az = LOW, .fault_el = LOW , .switch_az = false,
                     .switch_el = false};

#endif /* LIBRARIES_GLOBALS_H_ */
