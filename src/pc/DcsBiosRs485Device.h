/*
    Copyright 2015 Craig Courtney

    This file is part of DcsBios-Firmware.

    DcsBios-Firmware is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    DcsBios-Firmware is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with DcsBios-Firmware.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _DCSBIOS_BUSDEVICERS485_H_
#define _DCSBIOS_BUSDEVICERS485_H_

#include <Arduino.h>
#include "DcsBiosDevice.h"
#include "DcsBiosRs485BusParser.h"
#include "hal/DirectOutputPin.h"

class DcsBiosRs485Device : public DcsBiosDevice {
private:
    Stream* _busStream;
    DcsBiosRs485BusParser _bus;
    DirectOutputPin _busTxPin;    
    uint8_t _address;

    void sendPollResponse();
    void addPollingResponseString(const char* string);

    static uint8_t pollingResponseBuffer[DCSBIOS_RS485_MAX_PACKET_DATA_SIZE];
    static uint8_t pollingResponseBufferSize;

public:
    DcsBiosRs485Device(Stream *busStream, int txPin, uint8_t address);

    virtual void sendDcsBiosMessage(const char* message, const char* arg);
    virtual void process();
};

#endif