/*!
* @file watchdog.h
*
* It is a driver for watchdog timer.
*
* Licensed under the GPLv3
*
*/

#ifndef WATCHDOG_H_
#define WATCHDOG_H_

#include <avr/wdt.h>
#include "globals.h"
#include "easycomm.h"
#include "rotator_pins.h"

/**************************************************************************/
/*!
    @brief    Class that functions for interacting with a watchdog timer
*/
/**************************************************************************/
class wdt_timer{
public:

    /**************************************************************************/
    /*!
        @brief    Initialize watchdog timer to 2sec time out and to set up
                  interrupt routine
    */
    /**************************************************************************/
    void watchdog_init() {
        cli();
        wdt_reset();
        WDTCSR |= _BV(WDCE) | _BV(WDE);
        WDTCSR = _BV(WDIE) | _BV(WDE) | _BV(WDP3) | _BV(WDP2) | _BV(WDP1);
        sei();
    }

    /**************************************************************************/
    /*!
        @brief    Reset the watchdog timer
    */
    /**************************************************************************/
    void watchdog_reset() {
        wdt_reset();
    }
};

/**************************************************************************/
/*!
    @brief    Watchdog timer interrupt routine that implements a minimal
              easycomm protocol to get the errors and reset the uC
*/
/**************************************************************************/
ISR(WDT_vect) {
    // Disable motors
    digitalWrite(MOTOR_EN, LOW);
    // Set error
    rotator.rotator_error = wdt_error;
    rotator.rotator_status = error;
    // Enable interrupts for serial communication
    sei();

    while (1) {
        // Reset the watchdog timer because the interrupts are enabled
        wdt_reset();
        // Implement a minimal easycomm protocol to get the errors and reset uC
        char buffer[BUFFER_SIZE];
        char incomingByte;
        static uint16_t BufferCnt = 0;
        String str1, str2, str3, str4, str5, str6;
        while (rs485.available() > 0) {
            incomingByte = rs485.read();
            if (incomingByte == '\n' || incomingByte == '\r') {
                buffer[BufferCnt] = 0;
                if (buffer[0] == 'G' && buffer[1] == 'S') {
                    str1 = String("GS");
                    str2 = String(rotator.rotator_status, DEC);
                    str3 = String("\n");
                    rs485.print(str1 + str2 + str3);
                } else if (buffer[0] == 'G' && buffer[1] == 'E') {
                    str1 = String("GE");
                    str2 = String(rotator.rotator_error, DEC);
                    str3 = String("\n");
                    rs485.print(str1 + str2 + str3);
                } else if (buffer[0] == 'R' && buffer[1] == 'B') {
                    while(1);
                }
                BufferCnt = 0;
                rs485.flush();
            } else {
                buffer[BufferCnt] = incomingByte;
                BufferCnt++;
            }
        }
        // Reset the watchdog timer
        wdt_reset();
    }
}

#endif /* WATCHDOG_H_ */
