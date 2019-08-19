/*
 * MAX6955
 * Based on Philipp Klaus's MAX6955, Copyright 2013, https://github.com/pklaus/MAX6955
 * Mostly rewritten and improved by Dror Gluska
 */

#pragma once

#include "Arduino.h"
#include "Wire.h"

#define MAX6955_REG_NOP 0x00
#define MAX6955_REG_DecodeMode 0x01
#define MAX6955_REG_GlobalIntensity 0x02
#define MAX6955_REG_ScanLimit 0x03
#define MAX6955_REG_Configuration 0x04
#define MAX6955_REG_GPIOData 0x05
#define MAX6955_REG_PortConfiguration 0x06
#define MAX6955_REG_DisplayTest 0x07
#define MAX6955_REG_KEY_A_Mask 0x08
#define MAX6955_REG_KEY_A_Debounce 0x08
#define MAX6955_REG_KEY_B_Mask 0x09
#define MAX6955_REG_KEY_B_Debounce 0x09
#define MAX6955_REG_KEY_C_Mask 0x0A
#define MAX6955_REG_KEY_C_Debounce 0x0A
#define MAX6955_REG_KEY_D_Mask 0x0B
#define MAX6955_REG_KEY_D_Debounce 0x0B

#define MAX6955_REG_DigitType 0x0C
#define MAX6955_REG_KEY_A_Pressed 0x0C

#define MAX6955_REG_KEY_B_Pressed 0x0D
#define MAX6955_REG_KEY_C_Pressed 0x0E
#define MAX6955_REG_KEY_D_Pressed 0x0F

#define MAX6955_REG_Intensity10 0x10
#define MAX6955_REG_Intensity32 0x11
#define MAX6955_REG_Intensity54 0x12
#define MAX6955_REG_Intensity76 0x13
#define MAX6955_REG_Intensity10a 0x14
#define MAX6955_REG_Intensity32a 0x15
#define MAX6955_REG_Intensity54a 0x16
#define MAX6955_REG_Intensity76a 0x17

#define MAX6955_REG_Digit_0_PlaneP0 0x20
#define MAX6955_REG_Digit_0a_PlaneP0 0x28
#define MAX6955_REG_Digit_0_PlaneP1 0x40
#define MAX6955_REG_Digit_0a_PlaneP1 0x48
#define MAX6955_REG_Digit_0_PlaneP01 0x60
#define MAX6955_REG_Digit_0a_PlaneP01 0x68

#define PLANE_0 MAX6955_REG_Digit_0_PlaneP0
#define PLANE_1 MAX6955_REG_Digit_0_PlaneP1
#define PLANE_01 MAX6955_REG_Digit_0_PlaneP01

/*
    Shutdown Control D0
    0 Shutdown
    1 Normal
*/
#define MAX6955_CONFIG_SHUTDOWN_BIT 0

/*
    Blink Rate D2
    0 Slow (1s when fOSC=4MHz)
    1 Fast (0.5s when fOSC=4MHz)
*/
#define MAX6955_CONFIG_BLINK_RATE_BIT 2

/*
    Global Blink D3
    0 Disabled
    1 Enabled
*/
#define MAX6955_CONFIG_BLINK_BIT 3

/*
    Global Blink Timing Sync D4
    0 Timing Unaffected
    1 Reset on i2c ack
*/
#define MAX6955_CONFIG_BLINK_SYNC_BIT 4

/*
    Global Clear Digit D5
    0 Unaffected
    1 Clear on i2c ack
*/
#define MAX6955_CONFIG_CLEAR_DIGIT_BIT 5

/*
    Global Intensity D6
    0 Single Setting
    1 Individual on MAX6955_REG_Intensity10 and MAX6955_REG_Intensity76
*/
#define MAX6955_CONFIG_GLOBAL_INTENSITY_BIT 6

/*
    Blink Phase D7
    0 P1 Phase
    1 P0 Phase
*/
#define MAX6955_CONFIG_BLINK_PHASE_BIT 7

class MAX6955
{
public:
    MAX6955(uint8_t id);
    void init();
    void writeReg(char command, char data);
    void clear(void);

    void powerup(bool up);

    void test(bool enable);

    /**
         * @brief This function writes string.
         *
         * @param pointer to buffer
         *
         * @return result
         */
    void write(uint8_t plane, const char *buffer);

    /*
        Read Register Value
    */
    uint8_t read(uint8_t reg);

    /*
        Scan Maximum Display Digits
    */
    void setScanLimit(uint8_t maxDigits);

    uint8_t getConfiguration();

    void setConfiguration(uint8_t config);

    void setConfigurationBit(uint8_t bit, bool set){
         uint8_t config = getConfiguration();
         if (set){
            config |= 1 << bit;
         }else{
            config &= ~(1 << bit);
         }
        setConfiguration(config);
    }

    /*
        Set Global Intensity
        0 1/16th (2.5ma)
        1 2/16 (5ma)
        2 3/16 (7.5ma)
        3 4/16 (10ma)
        4 5/16 (12.5ma)
        5 6/16 (15ma)
        6 7/16 (17.5ma)
        7 8/16 (20ma)
        8 9/16 (22.5ma)
        9 10/16 (25ma)
        10 11/16 (27.5ma)
        11 12/16 (30ma)
        12 13/16 (32.5ma)
        13 14/16 (35ma)
        14 15/16 (37.5ma)
    */
    void setGlobalIntensity(uint8_t intensity);

    /*
        Set Specific Digit Intensity
    */
    void setIntensity(uint8_t digit, uint8_t intensity);

    /*
        Set GPIO Pin Mode (0-4), INPUT / OUTPUT
    */
    void pinMode(uint8_t port, uint8_t mode);

    /*
        Set GPIO HIGH / LOW
    */
    void digitalWrite(uint8_t pin, uint8_t val);

    /*
        Get GPIO Value
    */
    int digitalRead(uint8_t pin);

    void blink(bool enabled, bool fast);

private:
    uint8_t _id;
    uint8_t all_output_gpio = 0;
};
