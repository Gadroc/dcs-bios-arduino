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

#ifndef _DCSBIOS_BUSCONTROLLER_H_
#define _DCSBIOS_BUSCONTROLLER_H_

#include <Arduino.h>
#include "FastPin.h"
#include "DcsBiosBus.h"

#define DCSBIOS_BANK_COUNT 4
#define DCSBIOS_DEVICE_RESPONSE_TIMEOUT 5
#define DCSBIOS_MAX_DEVICE_ADDRESS 31

class BusController : public DcsBiosBus
{
    private:
        FastPin _pcReceivePin;
        Stream* _pc;

        bool _waitingResponse;
        long _timeout;

        bool _bankReady[DCSBIOS_BANK_COUNT];
        uint8_t _bank[DCSBIOS_BANK_COUNT][DCSBIOS_BANK_SIZE];

        uint8_t _pollingAddress;
        uint8_t _pollingBank;
        long _deviceTimeouts;

        void sendPollingPacket();
        void processBusInput();
        void processBusPacket();
        void processPcInput(int command);
        void loadPcData();

    public:
        void begin(Stream* busStream, int txPin, Stream* pcStream);
        void process();
};

extern BusController DcsBiosController;

#endif