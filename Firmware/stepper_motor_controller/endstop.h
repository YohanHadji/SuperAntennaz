/*!
* @file endstop.h
*
* It is a driver for end-stops, mechanical, optical, magnetic etc.
*
* Licensed under the GPLv3
*
*/

#ifndef ENDSTOP_H_
#define ENDSTOP_H_

/**************************************************************************/
/*!
    @brief    Class that functions for interacting with end-stop.
    @param    pin
              Set the arduino pin
    @param    default_state
              Set the default state of end-stop HIGH or LOW
*/
/**************************************************************************/
class endstop {
public:

    endstop(uint8_t pin, bool default_state) {
        _pin = pin;
        _default_state = default_state;
    }

    /**************************************************************************/
    /*!
        @brief    Initialize the Input pin for end-stop
    */
    /**************************************************************************/
    void init() {
        pinMode(_pin, INPUT_PULLUP);
    }

    /**************************************************************************/
    /*!
        @brief    Get the state of end-stop
        @return   True if end-stop is triggered
    */
    /**************************************************************************/
    bool get_state() {
        if (digitalRead(_pin) == _default_state)
            return true;
        else
            return false;
    }

private:
    uint8_t _pin;
    bool _default_state;
};

#endif /* ENDSTOP_H_ */
