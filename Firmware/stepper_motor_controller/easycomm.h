/*!
* @file easycomm.h
*
* It is a driver for easycomm 3 protocol as referred, in Hamlib.
*
* Licensed under the GPLv3
*
*/

#ifndef LIBRARIES_EASYCOMM_H_
#define LIBRARIES_EASYCOMM_H_

#include <Arduino.h>
#include <WString.h>
#include <avr/wdt.h>
//#include "rs485.h"
#include "rotator_pins.h"
#include "globals.h"

#define RS485_TX_TIME 9     ///< Delay "t"ms to write in serial for RS485 implementation
#define BUFFER_SIZE   256   ///< Set the size of serial buffer
#define BAUDRATE      19200 ///< Set the Baudrate of easycomm 3 protocol

//rs485 rs485(RS485_DIR, RS485_TX_TIME);

/**************************************************************************/
/*!
    @brief    Class that functions for easycomm 3 implementation
*/
/**************************************************************************/
class easycomm {
public:

    /**************************************************************************/
    /*!
        @brief    Initialize the RS485 bus
    */
    /**************************************************************************/
    void easycomm_init() {
       // rs485.begin(BAUDRATE);
	    Serial.begin(9600);
    }

    /**************************************************************************/
    /*!
        @brief    Get the commands from RS485 and response to the client
    */
    /**************************************************************************/
    void easycomm_proc() {
        char buffer[BUFFER_SIZE];
        char incomingByte;
        char *Data = buffer;
        char *rawData;
        static uint16_t BufferCnt = 0;
        char data[100];
        String str1, str2, str3, str4, str5, str6;

        // Read from serial
        while (Serial.available() > 0) {
            incomingByte = Serial.read();

            // Read new data, '\n' means new pacakage
            if (incomingByte == '\n' || incomingByte == '\r') {
                buffer[BufferCnt] = 0;
                if (buffer[0] == 'A' && buffer[1] == 'Z') {
                    if (buffer[2] == ' ' && buffer[3] == 'E' &&
                        buffer[4] == 'L') {
                        // Send current absolute position in deg
                        str1 = String("AZ");
                        str2 = String(control_az.input, 1);
                        str3 = String(" EL");
                        str4 = String(control_el.input, 1);
                        str5 = String("\n");
                        Serial.print(str1 + str2 + str3 + str4 + str5);
                    } else {
                        // Get the absolute position in deg for azimuth
                        rotator.control_mode = position;
                        rawData = strtok_r(Data, " ", &Data);
                        strncpy(data, rawData + 2, 10);
                        if (isNumber(data)) {
                            control_az.setpoint = atof(data);
                        }
                        // Get the absolute position in deg for elevation
                        rawData = strtok_r(Data, " ", &Data);
                        if (rawData[0] == 'E' && rawData[1] == 'L') {
                            strncpy(data, rawData + 2, 10);
                            if (isNumber(data)) {
                                control_el.setpoint = atof(data);
                            }
                        }
                    }
                } else if (buffer[0] == 'E' && buffer[1] == 'L') {
                        // Get the absolute position in deg for elevation
                        rotator.control_mode = position;
                        rawData = strtok_r(Data, " ", &Data);
                        if (rawData[0] == 'E' && rawData[1] == 'L') {
                            strncpy(data, rawData + 2, 10);
                            if (isNumber(data)) {
                                control_el.setpoint = atof(data);
                            }
                        }
                } else if (buffer[0] == 'V' && buffer[1] == 'U') {
                    // Elevation increase speed in mdeg/s
                    rotator.control_mode = speed;
                    strncpy(data, Data + 2, 10);
                    if (isNumber(data)) {
                        // Convert to deg/s
                        control_el.setpoint_speed = atof(data) / 1000;
                    }
                } else if (buffer[0] == 'V' && buffer[1] == 'D') {
                    // Elevation decrease speed in mdeg/s
                    rotator.control_mode = speed;
                    strncpy(data, Data + 2, 10);
                    if (isNumber(data)) {
                        // Convert to deg/s
                        control_el.setpoint_speed = - atof(data) / 1000;
                    }
                } else if (buffer[0] == 'V' && buffer[1] == 'L') {
                    // Azimuth increase speed in mdeg/s
                    rotator.control_mode = speed;
                    strncpy(data, Data + 2, 10);
                    if (isNumber(data)) {
                        // Convert to deg/s
                        control_az.setpoint_speed = atof(data) / 1000;
                    }
                } else if (buffer[0] == 'V' && buffer[1] == 'R') {
                    // Azimuth decrease speed in mdeg/s
                    rotator.control_mode = speed;
                    strncpy(data, Data + 2, 10);
                    if (isNumber(data)) {
                        // Convert to deg/s
                        control_az.setpoint_speed = - atof(data) / 1000;
                    }
                } else if (buffer[0] == 'S' && buffer[1] == 'A' &&
                           buffer[2] == ' ' && buffer[3] == 'S' &&
                           buffer[4] == 'E') {
                    // Stop Moving
                    rotator.control_mode = position;
                    str1 = String("AZ");
                    str2 = String(control_az.input, 1);
                    str3 = String(" EL");
                    str4 = String(control_el.input, 1);
                    str5 = String("\n");
                    Serial.print(str1 + str2 + str3 + str4 + str5);
                    control_az.setpoint = control_az.input;
                    control_el.setpoint = control_el.input;
                } else if (buffer[0] == 'R' && buffer[1] == 'E' &&
                           buffer[2] == 'S' && buffer[3] == 'E' &&
                           buffer[4] == 'T') {
                    // Reset the rotator, go to home position
                    str1 = String("AZ");
                    str2 = String(control_az.input, 1);
                    str3 = String(" EL");
                    str4 = String(control_el.input, 1);
                    str5 = String("\n");
                    Serial.print(str1 + str2 + str3 + str4 + str5);
                    rotator.homing_flag = false;
                } else if (buffer[0] == 'P' && buffer[1] == 'A' &&
                           buffer[2] == 'R' && buffer[3] == 'K' ) {
                    // Park the rotator
                    rotator.control_mode = position;
                    str1 = String("AZ");
                    str2 = String(control_az.input, 1);
                    str3 = String(" EL");
                    str4 = String(control_el.input, 1);
                    str5 = String("\n");
                    Serial.print(str1 + str2 + str3 + str4 + str5);
                    control_az.setpoint = rotator.park_az;
                    control_el.setpoint = rotator.park_el;
                } else if (buffer[0] == 'V' && buffer[1] == 'E') {
                    // Get the version if rotator controller
                    str1 = String("VE");
                    str2 = String("SatNOGS-v2.2");
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'I' && buffer[1] == 'P' &&
                           buffer[2] == '0') {
                    // Get the inside temperature
                    str1 = String("IP0,");
                    str2 = String(rotator.inside_temperature, DEC);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'I' && buffer[1] == 'P' &&
                           buffer[2] == '1') {
                    // Get the status of end-stop, azimuth
                    str1 = String("IP1,");
                    str2 = String(rotator.switch_az, DEC);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'I' && buffer[1] == 'P' &&
                           buffer[2] == '2') {
                    // Get the status of end-stop, elevation
                    str1 = String("IP2,");
                    str2 = String(rotator.switch_el, DEC);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'I' && buffer[1] == 'P' &&
                           buffer[2] == '3') {
                    // Get the current position of azimuth in deg
                    str1 = String("IP3,");
                    str2 = String(control_az.input, 2);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'I' && buffer[1] == 'P' &&
                           buffer[2] == '4') {
                    // Get the current position of elevation in deg
                    str1 = String("IP4,");
                    str2 = String(control_el.input, 2);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'I' && buffer[1] == 'P' &&
                           buffer[2] == '5') {
                    // Get the load of azimuth, in range of 0-1023
                    str1 = String("IP5,");
                    str2 = String(control_az.load, DEC);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'I' && buffer[1] == 'P' &&
                           buffer[2] == '6') {
                    // Get the load of elevation, in range of 0-1023
                    str1 = String("IP6,");
                    str2 = String(control_el.load, DEC);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'I' && buffer[1] == 'P' &&
                           buffer[2] == '7') {
                    // Get the speed of azimuth in deg/s
                    str1 = String("IP7,");
                    str2 = String(control_az.speed, 2);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'I' && buffer[1] == 'P' &&
                           buffer[2] == '8') {
                    // Get the speed of elevation in deg/s
                    str1 = String("IP8,");
                    str2 = String(control_el.speed, 2);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'G' && buffer[1] == 'S') {
                    // Get the status of rotator
                    str1 = String("GS");
                    str2 = String(rotator.rotator_status, DEC);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if (buffer[0] == 'G' && buffer[1] == 'E') {
                    // Get the error of rotator
                    str1 = String("GE");
                    str2 = String(rotator.rotator_error, DEC);
                    str3 = String("\n");
                    Serial.print(str1 + str2 + str3);
                } else if(buffer[0] == 'C' && buffer[1] == 'R') {
                    // Get Configuration of rotator
                    if (buffer[3] == '1') {
                        // Get Kp Azimuth gain
                        str1 = String("1,");
                        str2 = String(control_az.p, 2);
                        str3 = String("\n");
                        Serial.print(str1 + str2 + str3);
                    } else if (buffer[3] == '2') {
                        // Get Ki Azimuth gain
                        str1 = String("2,");
                         str2 = String(control_az.i, 2);
                         str3 = String("\n");
                         Serial.print(str1 + str2 + str3);
                    } else if (buffer[3] == '3') {
                        // Get Kd Azimuth gain
                        str1 = String("3,");
                        str2 = String(control_az.d, 2);
                        str3 = String("\n");
                        Serial.print(str1 + str2 + str3);
                    } else if (buffer[3] == '4') {
                        // Get Kp Elevation gain
                        str1 = String("4,");
                         str2 = String(control_el.p, 2);
                         str3 = String("\n");
                         Serial.print(str1 + str2 + str3);
                    } else if (buffer[3] == '5') {
                        // Get Ki Elevation gain
                        str1 = String("5,");
                        str2 = String(control_el.i, 2);
                        str3 = String("\n");
                        Serial.print(str1 + str2 + str3);
                    } else if (buffer[3] == '6') {
                        // Get Kd Elevation gain
                        str1 = String("6,");
                        str2 = String(control_el.d, 2);
                        str3 = String("\n");
                        Serial.print(str1 + str2 + str3);
                    } else if (buffer[3] == '7') {
                        // Get Azimuth park position
                        str1 = String("7,");
                        str2 = String(rotator.park_az, 2);
                        str3 = String("\n");
                        Serial.print(str1 + str2 + str3);
                    } else if (buffer[3] == '8') {
                        // Get Elevation park position
                        str1 = String("8,");
                        str2 = String(rotator.park_el, 2);
                        str3 = String("\n");
                        Serial.print(str1 + str2 + str3);
                    } else if (buffer[3] == '9') {
                        // Get control mode
                        str1 = String("9,");
                        str2 = String(rotator.control_mode);
                        str3 = String("\n");
                        Serial.print(str1 + str2 + str3);
                    }
                } else if (buffer[0] == 'C' && buffer[1] == 'W') {
                    // Set Config
                    if (buffer[2] == '1') {
                        // Set Kp Azimuth gain
                        rawData = strtok_r(Data, ",", &Data);
                        strncpy(data, rawData + 4, 10);
                        if (isNumber(data)) {
                            control_az.p = atof(data);
                        }
                    } else if (buffer[2] == '2') {
                        // Set Ki Azimuth gain
                        rawData = strtok_r(Data, ",", &Data);
                        strncpy(data, rawData + 4, 10);
                        if (isNumber(data)) {
                            control_az.i = atof(data);
                        }
                    } else if (buffer[2] == '3') {
                        // Set Kd Azimuth gain
                        rawData = strtok_r(Data, ",", &Data);
                        strncpy(data, rawData + 4, 10);
                        if (isNumber(data)) {
                            control_az.d = atof(data);
                        }
                    } else if (buffer[2] == '4') {
                        // Set Kp Elevation gain
                        rawData = strtok_r(Data, ",", &Data);
                        strncpy(data, rawData + 4, 10);
                        if (isNumber(data)) {
                            control_el.p = atof(data);
                        }
                    } else if (buffer[2] == '5') {
                        // Set Ki Elevation gain
                        rawData = strtok_r(Data, ",", &Data);
                        strncpy(data, rawData + 4, 10);
                        if (isNumber(data)) {
                            control_el.i = atof(data);
                        }
                    } else if (buffer[2] == '6') {
                        // Set Kd Elevation gain
                        rawData = strtok_r(Data, ",", &Data);
                        strncpy(data, rawData + 4, 10);
                        if (isNumber(data)) {
                            control_el.d = atof(data);
                        }
                    }  else if (buffer[2] == '7') {
                        // Set the Azimuth park position
                        rawData = strtok_r(Data, ",", &Data);
                        strncpy(data, rawData + 4, 10);
                        if (isNumber(data)) {
                            rotator.park_az = atof(data);
                        }
                    } else if (buffer[2] == '8') {
                        // Set the Elevation park position
                        rawData = strtok_r(Data, ",", &Data);
                        strncpy(data, rawData + 4, 10);
                        if (isNumber(data)) {
                            rotator.park_el = atof(data);
                        }
                    }
                } else if (buffer[0] == 'R' && buffer[1] == 'S'
                        && buffer[2] == 'T') {
                    // Custom command to test the watchdog timer routine
                    while(1)
                        ;
                } else if (buffer[0] == 'R' && buffer[1] == 'B') {
                    // Custom command to reboot the uC
                    wdt_enable(WDTO_2S);
                    while(1);
                }
                // Reset the buffer an clean the serial buffer
                BufferCnt = 0;
                Serial.flush();
            } else {
                // Fill the buffer with incoming data
                buffer[BufferCnt] = incomingByte;
                BufferCnt++;
            }
        }
    }

private:
    bool isNumber(char *input) {
        for (uint16_t i = 0; input[i] != '\0'; i++) {
            if (isalpha(input[i]))
                return false;
        }
        return true;
    }
};

#endif /* LIBRARIES_EASYCOMM_H_ */
