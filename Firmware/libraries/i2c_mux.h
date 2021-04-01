/*!
* @file i2c_mux.h
*
* It is a driver for I2C 1-of-2 multiplexer (like PCA9540B) with 2-Channels.
*
* Licensed under the GPLv3
*
*/

#ifndef I2C_MUX_H_
#define I2C_MUX_H_

#include <Wire.h>

#define I2C_FREQ 100000

/**************************************************************************/
/*!
    @brief    Class that functions for interacting with  I2C 1-of-2 multiplexer.
    @param    id
              I2C ID in HEX
    @param    ch0
              Channel 0 in HEX
    @param    ch1
              Channel 1 in HEX
*/
/**************************************************************************/
class i2c_mux {
public:

    i2c_mux(uint8_t id, uint8_t ch0, uint8_t ch1) {
        _id = id;
        _ch0 = ch0;
        _ch1 = ch1;
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
        @brief    Change the channel
        @param    ch
                  Set the channel that is connected with Master, CH0 or CH1
    */
    /**************************************************************************/
    void set_channel(uint8_t ch) {
        if (ch == _ch0) {
            Wire.beginTransmission(_id);
            Wire.write(_ch0);
            Wire.endTransmission();
        } else if (ch == _ch1) {
            Wire.beginTransmission(_id);
            Wire.write(_ch1);
            Wire.endTransmission();
        }
    }

private:
    uint8_t _id, _ch0, _ch1;
};

#endif /* I2C_MUX_H_ */
