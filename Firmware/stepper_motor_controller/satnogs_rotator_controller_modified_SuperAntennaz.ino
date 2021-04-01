/*!
 * @file stepper_motor_controller.ino
 *
 * This is the documentation for satnogs rotator controller firmware
 * for stepper motors configuration. The board (PCB) is placed in
 * <a href="https://gitlab.com/librespacefoundation/satnogs/satnogs-rotator-controller">
 * satnogs-rotator-controller </a> and is for releases:
 * v2.0
 * v2.1
 * v2.2
 * <a href="https://wiki.satnogs.org/SatNOGS_Rotator_Controller"> wiki page </a>
 *
 * @section dependencies Dependencies
 *
 * This firmware depends on <a href="http://www.airspayce.com/mikem/arduino/AccelStepper/index.htmlhttp://www.airspayce.com/mikem/arduino/AccelStepper/index.html">
 * AccelStepper library</a> being present on your system. Please make sure you
 * have installed the latest version before using this firmware.
 *
 * @section license License
 *
 * Licensed under the GPLv3.
 *
 */

#define SAMPLE_TIME        0.1   ///< Control loop in s
#define RATIO              80    ///< Gear ratio of rotator gear box                                 default 54
#define MICROSTEP          2     ///< Set Microstep
#define MIN_PULSE_WIDTH    20    ///< In microsecond for AccelStepper
#define MAX_SPEED          1600  ///< In steps/s, consider the microstep
#define MAX_ACCELERATION   1600  ///< In steps/s^2, consider the microstep
#define SPR                400L ///< Step Per Revolution, consider the microstep
#define MIN_M1_ANGLE       0     ///< Minimum angle of azimuth
#define MAX_M1_ANGLE       360   ///< Maximum angle of azimuth
#define MIN_M2_ANGLE       0     ///< Minimum angle of elevation
#define MAX_M2_ANGLE       180   ///< Maximum angle of elevation
#define DEFAULT_HOME_STATE HIGH  ///< Change to LOW according to Home sensor
#define HOME_DELAY         12000 ///< Time for homing Deceleration in millisecond

#include <AccelStepper.h>
#include <Wire.h>
//#include <globals.h>
#include "easycomm.h"
#include "rotator_pins.h"
//#include <rs485.h>
#include "endstop.h"
//#include <watchdog.h>

uint32_t t_run = 0; // run time of uC
easycomm comm;
AccelStepper stepper_az(1, 3, 2);
AccelStepper stepper_el(1, 5, 4);
endstop switch_az(SW1, DEFAULT_HOME_STATE), switch_el(SW2, DEFAULT_HOME_STATE);
//wdt_timer wdt;

enum _rotator_error homing(int32_t seek_az, int32_t seek_el);
int32_t deg2step(float deg);
float step2deg(int32_t step);

void setup() {
    // Homing switch
    switch_az.init();
    switch_el.init();

    // Serial Communication
    comm.easycomm_init();

    // Stepper Motor setup
    stepper_az.setEnablePin(MOTOR_EN);
    stepper_az.setPinsInverted(false, false, true);
    stepper_az.enableOutputs();
    stepper_az.setMaxSpeed(MAX_SPEED);
    stepper_az.setAcceleration(MAX_ACCELERATION);
    stepper_az.setMinPulseWidth(MIN_PULSE_WIDTH);
    stepper_el.setPinsInverted(false, false, true);
    stepper_el.enableOutputs();
    stepper_el.setMaxSpeed(MAX_SPEED);
    stepper_el.setAcceleration(MAX_ACCELERATION);
    stepper_el.setMinPulseWidth(MIN_PULSE_WIDTH);

    // Initialize WDT
   // wdt.watchdog_init();
}

void loop() {
    // Update WDT
   // wdt.watchdog_reset();

    // Get end stop status
    rotator.switch_az = switch_az.get_state();
    rotator.switch_el = switch_el.get_state();

    // Run easycomm implementation
    comm.easycomm_proc();

    // Get position of both axis
    control_az.input = step2deg(stepper_az.currentPosition());
    control_el.input = step2deg(stepper_el.currentPosition());

    // Check rotator status
    if (rotator.rotator_status != error) {
        if (rotator.homing_flag == false) {
            // Check home flag
            rotator.control_mode = position;
            // Homing
            rotator.rotator_error = homing(deg2step(-MAX_M1_ANGLE),
                                           deg2step(-MAX_M2_ANGLE));
            if (rotator.rotator_error == no_error) {
                // No error
                rotator.rotator_status = idle;
                rotator.homing_flag = true;
            } else {
                // Error
                rotator.rotator_status = error;
                rotator.rotator_error = homing_error;
            }
        } else {
            // Control Loop
            stepper_az.moveTo(deg2step(control_az.setpoint));
            stepper_el.moveTo(deg2step(control_el.setpoint));
            rotator.rotator_status = pointing;
            // Move azimuth and elevation motors
            stepper_az.run();
            stepper_el.run();
            // Idle rotator
            if (stepper_az.distanceToGo() == 0 && stepper_el.distanceToGo() == 0) {
                rotator.rotator_status = idle;
            }
        }
    } else {
        // Error handler, stop motors and disable the motor driver
        stepper_az.stop();
        stepper_az.disableOutputs();
        stepper_el.stop();
        stepper_el.disableOutputs();
        if (rotator.rotator_error != homing_error) {
            // Reset error according to error value
            rotator.rotator_error = no_error;
            rotator.rotator_status = idle;
        }
    }
}

/**************************************************************************/
/*!
    @brief    Move both axis with one direction in order to find home position,
              end-stop switches
    @param    seek_az
              Steps to find home position for azimuth axis
    @param    seek_el
              Steps to find home position for elevation axis
    @return   _rotator_error
*/
/**************************************************************************/
enum _rotator_error homing(int32_t seek_az, int32_t seek_el) {
    bool isHome_az = false;
    bool isHome_el = false;

    // Move motors to "seek" position
    stepper_az.moveTo(seek_az);
    stepper_el.moveTo(seek_el);

    // Homing loop
    while (isHome_az == false || isHome_el == false) {
        // Update WDT
       // wdt.watchdog_reset();
        if (switch_az.get_state() == true && !isHome_az) {
            // Find azimuth home
            stepper_az.moveTo(stepper_az.currentPosition());
            isHome_az = true;
        }
        if (switch_el.get_state() == true && !isHome_el) {
            // Find elevation home
            stepper_el.moveTo(stepper_el.currentPosition());
            isHome_el = true;
        }
        // Check if the rotator goes out of limits or something goes wrong (in
        // mechanical)
        if ((stepper_az.distanceToGo() == 0 && !isHome_az) ||
            (stepper_el.distanceToGo() == 0 && !isHome_el)){
            return homing_error;
        }
        // Move motors to "seek" position
        stepper_az.run();
        stepper_el.run();
    }
    // Delay to Deccelerate and homing, to complete the movements
    uint32_t time = millis();
    while (millis() - time < HOME_DELAY) {
       // wdt.watchdog_reset();
        stepper_az.run();
        stepper_el.run();
    }
    // Set the home position and reset all critical control variables
    stepper_az.setCurrentPosition(0);
    stepper_el.setCurrentPosition(0);
    control_az.setpoint = 0;
    control_el.setpoint = 0;

    return no_error;
}

/**************************************************************************/
/*!
    @brief    Convert degrees to steps according to step/revolution, rotator
              gear box ratio and microstep
    @param    deg
              Degrees in float format
    @return   Steps for stepper motor driver, int32_t
*/
/**************************************************************************/
int32_t deg2step(float deg) {
    return (RATIO * SPR * deg / 360);
}

/**************************************************************************/
/*!
    @brief    Convert steps to degrees according to step/revolution, rotator
              gear box ratio and microstep
    @param    step
              Steps in int32_t format
    @return   Degrees in float format
*/
/**************************************************************************/
float step2deg(int32_t step) {
    return (360.00 * step / (SPR * RATIO));
}
