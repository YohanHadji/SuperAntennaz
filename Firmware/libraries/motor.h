/*!
* @file motor.h
*
* It is a driver for DC motor driver like Pololu Dual MC33926 Motor
* Driver Carrier.
*
* Licensed under the GPLv3
*
*/

#ifndef MOTOR_H_
#define MOTOR_H_

/**************************************************************************/
/*!
    @brief    Class that functions for interacting with a Motor Driver Carrier.
    @param    pwm_pin1
              PWM pin, regulate the duty cycle
    @param    pwm_pin2
              PWM pin, regulate the duty cycle
    @param    fb_pin
              Analog pin that measure the analog current-sense feedback
    @param    en_pin
              Digital pin that enable and disable the motor driver
    @param    sf_pin
              Digital pin that read if the motor driver works properly or not
    @param    maxSpeed
              Set the maximum PWM value that the DC motor can handle, e.g.
              max 255 (8 bit timer)
    @param    minSpeed
              Set the minimum PWM value that the DC motor can handle, e.g.
              min 5 (8-bit timer)
*/
/**************************************************************************/
class motor {

public:
    motor(uint8_t pwm_pin1, uint8_t pwm_pin2, uint8_t fb_pin, uint8_t en_pin,
            uint8_t sf_pin, uint16_t maxSpeed, uint16_t minSpeed) {
        _pwm_pin1 = pwm_pin1;
        _pwm_pin2 = pwm_pin2;
        _maxSpeed = maxSpeed;
        _minSpeed = minSpeed;
        _fb_pin = fb_pin;
        _en_pin = en_pin;
        _sf_pin = sf_pin;
        stop();
    }

    /**************************************************************************/
    /*!
        @brief    Initialize pins of DC motor driver
    */
    /**************************************************************************/
    void init_pin() {
        pinMode(_pwm_pin1, OUTPUT);
        pinMode(_pwm_pin2, OUTPUT);
        /* Feedback and sense */
        pinMode(_fb_pin, INPUT);
        pinMode(_sf_pin,INPUT);
        /* Enable Motors */
        pinMode(_en_pin, OUTPUT);
    }

    /**************************************************************************/
    /*!
        @brief    Set timer frequency, for timers 0, 1, 2
        @param    timer
                  PWM frequency for D5 & D6, set timer 0
                  PWM frequency for D9 & D10, set timer 1
                  PWM frequency for D3 & D11, set timer 2
        @param    divisor
                  For timer 0
                  divisor to    1 for PWM frequency of 62500.00 Hz
                  divisor to    8 for PWM frequency of  7812.50 Hz
                  divisor to   64 for PWM frequency of   976.56 Hz (The DEFAULT)
                  divisor to  256 for PWM frequency of   244.14 Hz
                  divisor to 1024 for PWM frequency of    61.04 Hz
                  For timer 1
                  divisor to     1 for PWM frequency of 31372.55 Hz
                  divisor to     8 for PWM frequency of  3921.16 Hz
                  divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
                  divisor to   256 for PWM frequency of   122.55 Hz
                  divisor to  1024 for PWM frequency of    30.64 Hz
                  For timer 2
                  divisor to     1 for PWM frequency of 31372.55 Hz
                  divisor to     8 for PWM frequency of  3921.16 Hz
                  divisor to    32 for PWM frequency of   980.39 Hz
                  divisor to    64 for PWM frequency of   490.20 Hz (The DEFAULT)
                  divisor to   128 for PWM frequency of   245.10 Hz
                  divisor to   256 for PWM frequency of   122.55 Hz
                  divisor to  1024 for PWM frequency of    30.64 Hz
    */
    /**************************************************************************/
    void init_timer(uint8_t timer, uint16_t divisor) {
        if (timer == 0) {
            if (divisor == 1) {
                TCCR0B = (TCCR0B & B11111000) | B00000001;
            } else if (divisor == 8) {
                TCCR0B = (TCCR0B & B11111000) | B00000010;
            } else if (divisor == 64) {
                TCCR0B = (TCCR0B & B11111000) | B00000011;
            } else if (divisor == 256) {
                TCCR0B = (TCCR0B & B11111000) | B00000100;
            } else if (divisor == 1024) {
                TCCR0B = (TCCR0B & B11111000) | B00000101;
            }
        }
        if (timer == 1) {
            if (divisor == 1) {
                TCCR1B = (TCCR1B & B11111000) | B00000001;
            } else if (divisor == 8) {
                TCCR1B = (TCCR1B & B11111000) | B00000010;
            } else if (divisor == 64) {
                TCCR1B = (TCCR1B & B11111000) | B00000011;
            } else if (divisor == 256) {
                TCCR1B = (TCCR1B & B11111000) | B00000100;
            } else if (divisor == 1024) {
                TCCR1B = (TCCR1B & B11111000) | B00000101;
            }
        }
        if (timer == 2) {
            if (divisor == 1) {
                TCCR2B = (TCCR2B & B11111000) | B00000001;
            } else if (divisor == 8) {
                TCCR2B = (TCCR2B & B11111000) | B00000010;
            } else if (divisor == 32){
                TCCR2B = (TCCR2B & B11111000) | B00000011;
            } else if (divisor == 64) {
                TCCR2B = (TCCR2B & B11111000) | B00000100;
            } else if (divisor == 128) {
                TCCR2B = (TCCR2B & B11111000) | B00000101;
            } else if (divisor == 256) {
                TCCR2B = (TCCR2B & B11111000) | B00000110;
            } else if (divisor == 1024) {
                TCCR2B = (TCCR2B & B11111000) | B00000111;
            }
        }
    }

    /**************************************************************************/
    /*!
        @brief    Enable motor driver
    */
    /**************************************************************************/
    void enable() {
        digitalWrite(_en_pin, HIGH);
    }

    /**************************************************************************/
    /*!
        @brief    Disable motor driver
    */
    /**************************************************************************/
    void disenable() {
        digitalWrite(_en_pin, LOW);
    }

    /**************************************************************************/
    /*!
        @brief    Calculate the load of DC motor
        @return   Return an analog read value 0-1023
    */
    /**************************************************************************/
    uint16_t get_load() {
        return analogRead(_fb_pin);
    }

    /**************************************************************************/
    /*!
        @brief    Get the status flag of motor driver
        @return   HIGH the motor driver is working properly
                  LOW an over-current (short circuit) or over-temperature event
    */
    /**************************************************************************/
    uint8_t get_fault()
    {
      return digitalRead(_sf_pin);
    }

    /**************************************************************************/
    /*!
        @brief    Move the DC motor with constant voltage (~speed)
        @param    speed
                  Range minSpeed - maxSpeed, -255 - +255
    */
    /**************************************************************************/
    void move(int16_t speed) {
        if (speed == 0) {
            stop();
            return;
        }
        if (speed >= 0) {
            speed = speed + _minSpeed;
            if (speed > _maxSpeed)
                speed = _maxSpeed;
            analogWrite(_pwm_pin1, 0);
            analogWrite(_pwm_pin2, speed);
        } else {
            speed = -speed;
            speed = speed + _minSpeed;
            if (speed > _maxSpeed)
                speed = _maxSpeed;
            analogWrite(_pwm_pin1, speed);
            analogWrite(_pwm_pin2, 0);
        }
    }

    /**************************************************************************/
    /*!
        @brief    Stop moving the DC motor
    */
    /**************************************************************************/
    void stop() {
        analogWrite(_pwm_pin1, 0);
        analogWrite(_pwm_pin2, 0);
    }

    /**************************************************************************/
    /*!
        @brief    Update the minimum speed of DC motor
        @param    min
                  Minimum speed, e.g. a range of 0 - 40
    */
    /**************************************************************************/
    void set_min(uint16_t min) {
        _minSpeed = min;
    }

    /**************************************************************************/
    /*!
        @brief    Update the maximum speed of DC motor
        @param    max
                  Maximum speed, e.g. a range of 170 - 255
    */
    /**************************************************************************/
    void set_max(uint16_t max) {
        _maxSpeed = max;
    }

private:
    uint8_t _pwm_pin1, _pwm_pin2, _fb_pin, _en_pin, _sf_pin;
    int16_t _maxSpeed, _minSpeed;
};

#endif /* MOTOR_H_ */
