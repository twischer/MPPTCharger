/*
    TwoWire.cpp - TWI/I2C library for Arduino & Wiring
    Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

    Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
    Modified December 2014 by Ivan Grokhotkov (ivan@esp8266.com) - esp8266 support
    Modified April 2015 by Hrsto Gochkov (ficeto@ficeto.com) - alternative esp8266 support
    Modified January 2017 by Bjorn Hammarberg (bjoham@esp8266.com) - i2c slave support
*/

#include <cstddef>
extern "C" {
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <cassert>
}

#include <Arduino.h>
#include <wiringPiI2C.h>
#include "Wire.h"

//Some boards don't have these pins available, and hence don't support Wire.
//Check here for compile-time error.
#if !defined(PIN_WIRE_SDA) || !defined(PIN_WIRE_SCL)
#error Wire library is not supported on this board
#endif

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::fdAddress = 0;
int TwoWire::fd = -1;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(size_t);

static int default_sda_pin = SDA;
static int default_scl_pin = SCL;

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire() {}

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(int sda, int scl)
{
    default_sda_pin = sda;
    default_scl_pin = scl;
    flush();
}

void TwoWire::begin(int sda, int scl, uint8_t address)
{
    default_sda_pin = sda;
    default_scl_pin = scl;
    // TODO twi_setAddress(address);
    //twi_attachSlaveTxEvent(onRequestService);
    //twi_attachSlaveRxEvent(onReceiveService);
    flush();
}

void TwoWire::pins(int sda, int scl)
{
    default_sda_pin = sda;
    default_scl_pin = scl;
}

void TwoWire::begin(void)
{
    begin(default_sda_pin, default_scl_pin);
}

void TwoWire::openDevice(uint8_t address)
{
	if (fdAddress != address) {
		close(fd);
		fd = wiringPiI2CSetup(address);
		fdAddress = address;
	}
}

void TwoWire::begin(uint8_t address)
{
    openDevice(address);
    //twi_attachSlaveTxEvent(onRequestService);
    //twi_attachSlaveRxEvent(onReceiveService);
    begin();
}

uint8_t TwoWire::status()
{
    return 1; // TODO twi_status();
}

void TwoWire::begin(int address)
{
    begin((uint8_t)address);
}

void TwoWire::setClock(uint32_t frequency)
{
    // TODO twi_setClock(frequency);
}

void TwoWire::setClockStretchLimit(uint32_t limit)
{
    // TODO twi_setClockStretchLimit(limit);
}

size_t TwoWire::requestFrom(uint8_t address, size_t size, bool sendStop)
{
    if (size > BUFFER_LENGTH)
    {
        size = BUFFER_LENGTH;
    }

    // TODO check how it is implemented for other i2c sensors supported by wiringPi
    // TODO double check that only i2c address is sent and 2 bytes are read
    // No register address has to be sent
    // https://github.com/allwinner-zh/linux-3.4-sunxi/blob/master/drivers/hwmon/ina219.c
    // i2c_smbus_read_word_swapped()
    // i2c_smbus_read_word_data()
    // i2c_smbus_xfer(client->adapter, client->addr, client->flags,
	//			I2C_SMBUS_READ, command,
	//			I2C_SMBUS_WORD_DATA, &data);
    // https://elixir.bootlin.com/linux/v4.1/source/drivers/i2c/i2c-core.c#L2541
    // See also https://github.com/adafruit/Adafruit_Python_PureIO/blob/master/Adafruit_PureIO/smbus.py#L212
    int ret = -1;
    switch (size) {
        case 1:
            ret = wiringPiI2CReadReg8(fd, 0);
            break;
        case 2:
            ret = wiringPiI2CReadReg16(fd, 0);
            break;
        default:
            assert("NOT SUPPORTED" && 0);
    }

    rxBufferIndex = 0;
    rxBufferLength = 0;
    if (ret >= 0)
    {
        for (size_t i=0; i<size; i++) {
            rxBuffer[0] = ret & 0xFF;
            ret >>= 8;
        }
        rxBufferLength = size;

        return size;
    }
    else
    {
        return 0;
    }
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop)
{
    return requestFrom(address, static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
    return requestFrom(address, static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
    return requestFrom(static_cast<uint8_t>(address), static_cast<size_t>(quantity), true);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop)
{
    return requestFrom(static_cast<uint8_t>(address), static_cast<size_t>(quantity), static_cast<bool>(sendStop));
}

void TwoWire::beginTransmission(uint8_t address)
{
    openDevice(address);
    transmitting = 1;
    txAddress = address;
    txBufferIndex = 0;
    txBufferLength = 0;
}

void TwoWire::beginTransmission(int address)
{
    beginTransmission((uint8_t)address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop)
{
    int ret = -1;
    switch (txBufferLength) {
	    case 1:
		    ret = wiringPiI2CWrite(fd, txBuffer[0]);
		    break;
	    case 2:
		    ret = wiringPiI2CWriteReg8(fd, txBuffer[0], txBuffer[1]);
		    break;
	    case 3:
		    ret = wiringPiI2CWriteReg16(fd, txBuffer[0], txBuffer[1] | (txBuffer[2] << 8));
		    break;
	    default:
		    assert("NOT SUPPORTED" && 0);

    }
    txBufferIndex = 0;
    txBufferLength = 0;
    transmitting = 0;
    return ret;
}

uint8_t TwoWire::endTransmission(void)
{
    return endTransmission(true);
}

size_t TwoWire::write(uint8_t data)
{
    if (transmitting)
    {
        if (txBufferLength >= BUFFER_LENGTH)
        {
            setWriteError();
            return 0;
        }
        txBuffer[txBufferIndex] = data;
        ++txBufferIndex;
        txBufferLength = txBufferIndex;
    }
    else
    {
	printf("%s ERROR: beginTransmission() not called\n", __func__);
	return 0;
    }
    return 1;
}

size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
    for (size_t i = 0; i < quantity; ++i)
    {
        if (!write(data[i]))
        {
    	return i;
        }
    }
    return quantity;
}

int TwoWire::available(void)
{
    int result = rxBufferLength - rxBufferIndex;
    return result;
}

int TwoWire::read(void)
{
    int value = -1;
    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
        ++rxBufferIndex;
    }
    return value;
}

int TwoWire::peek(void)
{
    int value = -1;
    if (rxBufferIndex < rxBufferLength)
    {
        value = rxBuffer[rxBufferIndex];
    }
    return value;
}

void TwoWire::flush(void)
{
    rxBufferIndex = 0;
    rxBufferLength = 0;
    txBufferIndex = 0;
    txBufferLength = 0;
}

void TwoWire::onReceiveService(uint8_t* inBytes, size_t numBytes)
{
    // don't bother if user hasn't registered a callback
    if (!user_onReceive)
    {
        return;
    }
    // // don't bother if rx buffer is in use by a master requestFrom() op
    // // i know this drops data, but it allows for slight stupidity
    // // meaning, they may not have read all the master requestFrom() data yet
    // if(rxBufferIndex < rxBufferLength){
    //   return;
    // }

    // copy twi rx buffer into local read buffer
    // this enables new reads to happen in parallel
    for (uint8_t i = 0; i < numBytes; ++i)
    {
        rxBuffer[i] = inBytes[i];
    }

    // set rx iterator vars
    rxBufferIndex = 0;
    rxBufferLength = numBytes;

    // alert user program
    user_onReceive(numBytes);
}

void TwoWire::onRequestService(void)
{
    // don't bother if user hasn't registered a callback
    if (!user_onRequest)
    {
        return;
    }

    // reset tx buffer iterator vars
    // !!! this will kill any pending pre-master sendTo() activity
    txBufferIndex = 0;
    txBufferLength = 0;

    // alert user program
    user_onRequest();
}

void TwoWire::onReceive(void (*function)(int))
{
    // arduino api compatibility fixer:
    // really hope size parameter will not exceed 2^31 :)
    static_assert(sizeof(int) == sizeof(size_t), "something is wrong in Arduino kingdom");
    user_onReceive = reinterpret_cast<void(*)(size_t)>(function);
}

void TwoWire::onReceive(void (*function)(size_t))
{
    user_onReceive = function;
    assert("NOT SUPPORTED" && 0);
    //twi_enableSlaveMode();
}

void TwoWire::onRequest(void (*function)(void))
{
    user_onRequest = function;
    assert("NOT SUPPORTED" && 0);
    //twi_enableSlaveMode();
}

// Preinstantiate Objects //////////////////////////////////////////////////////

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_TWOWIRE)
TwoWire Wire;
#endif
