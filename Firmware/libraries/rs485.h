/*!
* @file rs485.h
*
* It is a driver for half-duplex RS485 transceiver.
*
* Licensed under the GPLv3
*
*/

#ifndef RS485_H_
#define RS485_H_

/**************************************************************************/
/*!
    @brief    Class that functions for interacting with a RS485 transceiver
    @param    pin_dir
              Digital pin, to set read or write to RS485 bus
    @param    tx_time
              Time delay in ms in order to transmit and change the status of
              direction pin
*/
/**************************************************************************/
class rs485 {
public:

    rs485(uint8_t pin_dir, uint16_t tx_time) {
        _pin_dir = pin_dir;
        _tx_time = tx_time;
    }

    /**************************************************************************/
    /*!
        @brief    Initialize the RS485 transceiver
        @param    baudrate
                  Set the baudrate of RS485 bus
    */
    /**************************************************************************/
    void begin(uint16_t baudrate) {
        pinMode(_pin_dir, OUTPUT);
        Serial.begin(baudrate);
    }

    /**************************************************************************/
    /*!
        @brief    Print a string to RS485 bus
        @param    str
                  A String to transmit
    */
    /**************************************************************************/
    void print(String str) {
        digitalWrite(_pin_dir, HIGH);
        Serial.print(str);
        delay(_tx_time);
        digitalWrite(_pin_dir, LOW);
    }

    /**************************************************************************/
    /*!
        @brief    Read a char/uint8_t from RS485 bus
        @return   A char/uint8_t
    */
    /**************************************************************************/
    uint8_t read() {
        return Serial.read();
    }

    /**************************************************************************/
    /*!
        @brief    The number of chars/uint8_t that are available in RS485 buffer
        @return   A char/uint8_t
    */
    /**************************************************************************/
    uint8_t available(void) {
        return Serial.available();
    }

    /**************************************************************************/
    /*!
        @brief    Waits for the transmission of outgoing serial data to complete
    */
    /**************************************************************************/
    void flush() {
        Serial.flush();
    }

    /**************************************************************************/
    /*!
        @brief    Disables RS485 communication
    */
    /**************************************************************************/
    void end() {
        Serial.end();
    }

private:
    uint8_t _pin_dir;
    uint16_t _tx_time;
};

#endif /* RS485_H_ */
