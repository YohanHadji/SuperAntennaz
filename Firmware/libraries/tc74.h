/*!
* @file tc74.h
*
* It is a driver for a TC74 Temperature sensor.
*
* Licensed under the GPLv3
*
*/

#ifndef TC74_H_
#define TC74_H_

#include <Wire.h>

#define TC74_TEMPERATURE_REGISTER 0x00
#define TC74_CONFIGURATION_REGISTER 0x01
#define TC74_STANDBY_COMMAND 0x80
#define TC74_AWAKE_COMMAND 0x00
#define TC74_DATA_READY_FLAG 0x40

/**************************************************************************/
/*!
    @brief    Class that functions for interacting with a TC74 Temperature sensor
    @param    id
              Set the ID of temperature sensor, I2C address
              TC74_A0 0b10010000
              TC74_A1 0b10010010
              TC74_A2 0b10010100
              TC74_A3 0b10010110
              TC74_A4 0b10011000
              TC74_A5 0b10011010
              TC74_A6 0b10011100
              TC74_A7 0b10011110
*/
/**************************************************************************/
class tc74 {
public:

    tc74(uint8_t id) {
        _id = id;
    }

    /**************************************************************************/
    /*!
        @brief    Initialize the I2C bus
    */
    /**************************************************************************/
    void init() {
        Wire.begin();
        Wire.setClock(I2C_FREQ);
    }

    /**************************************************************************/
    /*!
        @brief    Reads the int8_t in temperature measurement register
        @return   The temperature int8_t
    */
    /**************************************************************************/
    int8_t get_temp() {
        Wire.beginTransmission(_id);
        Wire.write(TC74_TEMPERATURE_REGISTER);
        Wire.endTransmission();
        Wire.requestFrom(_id, 1);
        while (Wire.available() == 0)
            ;
        return Wire.read();
    }

    /**************************************************************************/
    /*!
        @brief    Reads the int8_t in status register
        @return   The status register int8_t
    */
    /**************************************************************************/
    int8_t get_status() {
        Wire.beginTransmission(_id);
        Wire.write(TC74_CONFIGURATION_REGISTER);
        Wire.endTransmission();
        Wire.beginTransmission(_id);
        Wire.write(TC74_AWAKE_COMMAND);
        Wire.endTransmission();
        Wire.requestFrom(_id, 1);
        while (Wire.available() == 0)
            ;
        return Wire.read();
    }

    /**************************************************************************/
    /*!
        @brief    Wake up request to the sensor on the specified address
        @return   The status register int8_t
    */
    /**************************************************************************/
    int8_t wake_up() {
        Wire.beginTransmission(_id);
        Wire.write(TC74_CONFIGURATION_REGISTER);
        Wire.endTransmission();
        Wire.beginTransmission(_id);
        Wire.write(TC74_AWAKE_COMMAND);
        Wire.endTransmission();
        return get_status();
    }

    /**************************************************************************/
    /*!
        @brief    Sleep device request to the sensor on the specified address
        @return   The status register int8_t
    */
    /**************************************************************************/
    int8_t sleep() {
        Wire.beginTransmission(_id);
        Wire.write(TC74_CONFIGURATION_REGISTER);
        Wire.endTransmission();
        Wire.beginTransmission(_id);
        Wire.write(TC74_STANDBY_COMMAND);
        Wire.endTransmission();
        return get_status();
    }

private:
    int _id;
};

#endif /* TC74_H_ */
