/*
 * MAX6955
 * Based on Philipp Klaus's MAX6955, Copyright 2013, https://github.com/pklaus/MAX6955
 * Mostly rewritten and improved by Dror Gluska
 */

#include "MAX6955.h"

MAX6955::MAX6955(uint8_t id)
{
    _id = (id & 0x0F) | 0x60;
}
void MAX6955::init()
{
    Wire.begin();
    // ↓ 0x00 = No decode for any digit
    //   0xFF = hexadecimal decode for all digits
    writeReg(MAX6955_REG_DecodeMode, 0xFF); // no decode!
    // ↓ 0x00 =  1/16 (min on)  2.5 mA typical segment current
    //   0x0F = 15/16 (max on) 37.5 mA
    setGlobalIntensity(8);
    // ↓ 0x00 display only digit 0
    //   0x07 display all digits 0-7
    setScanLimit(7);
    // ↓ 0x00: Shutdown
    //   0x01: Normal operation
    //   0x20: Blink timing counters reset at I2C ack
    //   0x00: Global intensity
    //   0x40: Individual intensity settings
    powerup(true);
    setConfigurationBit(MAX6955_CONFIG_CLEAR_DIGIT_BIT,1);
    // ↓ 0xFF = digits 0-7 are 14-segment digits
    //   0x00 = digits 0-7 are 16- or 7-segment digits
    writeReg(MAX6955_REG_DigitType, 0b00000000);
    
    // ↓ 0x00 = Display Test Off
    test(false);
}

static char getPrintableChar(char c)
{
    if (c < 24 || c > 126)
    {              // make sure its printable
        return 32; // if not, replace it with "space"
    }
    return c;
}

void MAX6955::write(uint8_t plane, const char *buffer)
{
    char i2cData[8] = {32, 32, 32, 32, 32, 32, 32, 32};
    //i2cData[0] = 0x20; // first digit

    int i = 0;
    int ic = 0;
    int maxLength = strlen(buffer);
    while ((i < 8) && (ic < maxLength))
    {
        if ((i > 0) && (buffer[ic] == '.'))
        {
            i2cData[i - 1] = i2cData[i - 1] | 0b10000000;
        }
        else
        {
            i2cData[i] = getPrintableChar(buffer[ic]);
            i++;
        }
        ic++;
    }

    for (i = 0; i < 8; i++)
    {
        writeReg(plane + i, i2cData[i]);
    }
}

void MAX6955::test(bool enable)
{
    if (enable)
    {
        writeReg(MAX6955_REG_DisplayTest, 0x01);
    }
    else
    {
        writeReg(MAX6955_REG_DisplayTest, 0x00);
    }
}

void MAX6955::writeReg(char command, char data)
{
    Wire.beginTransmission(_id);
    Wire.write(command);
    Wire.write(data);
    Wire.endTransmission();
    delay(1);
}

uint8_t MAX6955::read(uint8_t reg)
{
    Wire.beginTransmission(_id);
    Wire.write(reg);
    Wire.endTransmission(false);

    Wire.requestFrom(_id,(uint8_t) 1);
    if (Wire.available())
    {
        return Wire.read();
    }
    return 0;
}

void MAX6955::clear()
{
    for (int i = MAX6955_REG_Digit_0_PlaneP0; i <= MAX6955_REG_Digit_0a_PlaneP01 + 8; i++)
        writeReg(i, 32);
}

void MAX6955::setScanLimit(uint8_t maxDigits)
{
    if (maxDigits > 0x07)
    {
        return;
    }
    writeReg(MAX6955_REG_ScanLimit, maxDigits);
}

uint8_t MAX6955::getConfiguration()
{
    return read(MAX6955_REG_Configuration);
}

void MAX6955::setConfiguration(uint8_t config)
{
    writeReg(MAX6955_REG_Configuration, config);
}

void MAX6955::setGlobalIntensity(uint8_t intensity)
{
    if (intensity > 0x0F)
    {
        intensity = 0x0F;
    }

    setConfigurationBit(MAX6955_CONFIG_GLOBAL_INTENSITY_BIT,false);

    writeReg(MAX6955_REG_GlobalIntensity, intensity);
}

void MAX6955::setIntensity(uint8_t digit, uint8_t intensity)
{
    if (intensity > 0x0F)
    {
        intensity = 0x0F;
    }

    setConfigurationBit(MAX6955_CONFIG_GLOBAL_INTENSITY_BIT,true);

    uint8_t reg;
    uint8_t value;
    uint8_t shiftleft;

    switch (digit)
    {
    case 0:
        reg = MAX6955_REG_Intensity10;
        shiftleft = 0;
        break;
    case 1:
        reg = MAX6955_REG_Intensity10;
        shiftleft = 4;
        break;
    case 2:
        reg = MAX6955_REG_Intensity32;
        shiftleft = 0;
        break;
    case 3:
        reg = MAX6955_REG_Intensity32;
        shiftleft = 4;
        break;
    case 4:
        reg = MAX6955_REG_Intensity54;
        shiftleft = 0;
        break;
    case 5:
        reg = MAX6955_REG_Intensity54;
        shiftleft = 4;
        break;
    case 6:
        reg = MAX6955_REG_Intensity76;
        shiftleft = 0;
        break;
    case 7:
        reg = MAX6955_REG_Intensity76;
        shiftleft = 4;
        break;
    }

    value = read(reg);

    //zero the bit to set
    if (shiftleft == 0)
    {
        value &= 0b11110000;
    }
    else
    {
        value &= 0b00001111;
    }

    uint8_t adjusted_intensity = intensity << shiftleft;

    //append the intensity to the existing value
    value = value | adjusted_intensity;

    writeReg(reg, value);
}

void MAX6955::pinMode(uint8_t port, uint8_t mode)
{
    uint8_t portConfig = read(MAX6955_REG_PortConfiguration);
    if (mode == INPUT)
    {
        portConfig |= 1 << port;
        writeReg(MAX6955_REG_PortConfiguration, portConfig);
    }
    else if (mode == OUTPUT)
    {
        portConfig &= ~(1 << port);
        writeReg(MAX6955_REG_PortConfiguration, portConfig);
    }
    else
    {
        return;
    }
}

void MAX6955::digitalWrite(uint8_t pin, uint8_t val)
{
    if (val == HIGH)
    {
        all_output_gpio |= 1UL << pin;
    }
    else if (val == LOW)
    {
        all_output_gpio &= ~(1UL << pin);
    }
    writeReg(MAX6955_REG_GPIOData, all_output_gpio);
}

int MAX6955::digitalRead(uint8_t pin)
{
    uint8_t valueAll = read(MAX6955_REG_GPIOData);
    return ((valueAll >> pin) & 1U) != 0;
}

void MAX6955::blink(bool enabled, bool fast)
{
    setConfigurationBit(MAX6955_CONFIG_BLINK_BIT,enabled);
    setConfigurationBit(MAX6955_CONFIG_BLINK_RATE_BIT,fast);
}

void MAX6955::powerup(bool up){
    setConfigurationBit(MAX6955_CONFIG_SHUTDOWN_BIT,(up)? 1: 0);
}