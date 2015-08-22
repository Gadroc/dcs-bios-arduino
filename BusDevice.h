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
#ifndef _DCSBIOS_BUSDEVICE_H_
#define _DCSBIOS_BUSDEVICE_H_

#include <Arduino.h>
#include "DcsBiosBus.h"

class BusDevice : DcsBiosBus
{
    private:
        uint8_t _address;
        uint8_t _bank;

        void processPacket();
        
    public:
        void begin(Stream *busStream, int txPin, uint8_t address, uint8_t bank);

        // Must be called oftern in main processing loop.  This routine
        // will process incoming data, notify outputs and send inputs
        // back when necessary.
        void process();
};

extern BusDevice DcsBiosDevice;

#endif