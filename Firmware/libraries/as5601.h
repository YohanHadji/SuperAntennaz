/*!
* @file as5601.h
*
* It is a driver for AS5601 a magnetic rotary position
* sensor. It uses I2C protocol. The resolution of encoder
* is 12-bit.
*
* Licensed under the GPLv3
*
*/

#ifndef AS5601_H_
#define AS5601_H_

#include <Wire.h>

#define I2C_FREQ 100000

#define AS5601_ID      0x36
#define RAW_ANG_HIGH   0x0C
#define RAW_ANG_LOW    0x0D
#define STATUS_REG     0x0B
#define AGC            0x1A
#define MAGNITUDE_HIGH 0x1B
#define MAGNITUDE_LOW  0x1C
#define CONF_HIGH      0x07
#define CONF_LOW       0x08

/**************************************************************************/
/*!
    @brief    Class that functions for interacting with AS5601 magnetic
              rotary position sensor.
*/
/**************************************************************************/
class AS5601 {
public:

    /**************************************************************************/
    /*!
        @brief    Initialize the I2C bus
    */
    /**************************************************************************/
    void Begin() {
        Wire.begin();
        Wire.setClock(I2C_FREQ);
    }

    /**************************************************************************/
    /*!
        @brief    Calculate an unwrap the position
        @param    new_pos
                  Calculate the current position of the sensor
        @return   The state of the AS5601:
                  Magnet is too strong
                  Magnet is too weak
                  Magnet was detect
    */
    /**************************************************************************/
    uint8_t get_pos(double *new_pos) {
        uint16_t raw_angle;
        uint8_t status_val;
        float raw_pos = 0;
        float delta_raw_pos = 0;
        float real_pos = 0;

        raw_angle = i2c_word_transaction(AS5601_ID, RAW_ANG_HIGH);
        // Read Status Bits
        status_val = i2c_byte_transaction(AS5601_ID, STATUS_REG);
        // Check the status register
        if ((status_val & 0x20) && !(status_val & 0x10)
                && !(status_val & 0x08)) {
            // Convert raw value to angle in deg
            raw_pos = (float) raw_angle * 0.0879;
            // Unwrap the angle
            delta_raw_pos = _raw_prev_pos - raw_pos;
            if (delta_raw_pos > 180)
                _n++;
            else if (delta_raw_pos < -180)
                _n--;
            // Calculate the real angle
            real_pos = - ((raw_pos + 360 * _n) / _enc_ratio) - _angle_offset;
            _raw_prev_pos = raw_pos;
        }
        *new_pos = (double)real_pos;
        return status_val;
    }

    /**************************************************************************/
    /*!
        @brief    Calculate the automatic gain control (AGC)
        @return   The AGC range is 0-255 counts,
                  the gain value should be in the
                  center of its range
    */
    /**************************************************************************/
    uint8_t get_agc() {
        return i2c_byte_transaction(AS5601_ID, AGC);
    }

    /**************************************************************************/
    /*!
        @brief    Get the magnitude value of the internal CORDIC output
        @return   The magnitude value of the internal CORDIC output
    */
    /**************************************************************************/
    uint16_t get_magnitude() {
        return i2c_word_transaction(AS5601_ID, MAGNITUDE_HIGH);
    }

    /**************************************************************************/
    /*!
        @brief    Get the configuration register
        @return   The Configuration register
    */
    /**************************************************************************/
    uint16_t get_conf() {
        return i2c_word_transaction(AS5601_ID, CONF_HIGH);
    }

    /**************************************************************************/
    /*!
        @brief    Set zero by setting offset angle
        @return   The state of the AS5601:
                  Magnet is too strong
                  Magnet is too weak
                  Magnet was detect
    */
    /**************************************************************************/
    uint8_t set_zero() {
        double current_pos;
        uint8_t status_val = get_pos(&current_pos);
        _angle_offset = current_pos;
        return status_val;
    }

    /**************************************************************************/
    /*!
        @brief    Reset zero position set the offset to zero
    */
    /**************************************************************************/
    void init_zero() {
        _angle_offset = 0.0;
    }

    /**************************************************************************/
    /*!
        @brief    Set the gear ratio between encoder and measure axis
        @param    enc_ratio
                  An uitn8_t, that represents the gear ratio
    */
    /**************************************************************************/
    void set_gear_ratio(uint8_t enc_ratio) {
        _enc_ratio = enc_ratio;
    }

private:
    double _angle_offset = 0;
    int32_t _n = 0;
    float _raw_prev_pos = 0;
    uint8_t _enc_ratio = 0;

    uint8_t i2c_byte_transaction(uint8_t i2c_address, uint8_t i2c_register) {
        Wire.beginTransmission(i2c_address);
        Wire.write(i2c_register);
        Wire.endTransmission();
        Wire.requestFrom(i2c_address, (uint8_t) 1);
        while (Wire.available() == 0)
            ;
        return Wire.read();
    }

    uint16_t i2c_word_transaction(uint8_t i2c_address,uint8_t i2c_register) {
        uint8_t word_high = i2c_byte_transaction(i2c_address, i2c_register);
        uint8_t word_low = i2c_byte_transaction(i2c_address, i2c_register + 1);
        return ((word_high << 8) | word_low);
    }
};

#endif /* AS5601_H_ */

