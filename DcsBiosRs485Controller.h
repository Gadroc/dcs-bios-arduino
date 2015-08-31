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
#ifndef _DCSBIOS_BUSCONTROLLERS485_H_
#define _DCSBIOS_BUSCONTROLLERS485_H_

#include <Arduino.h>
#include "DcsBiosRs485BusParser.h"
#include "DcsBiosRs485PcParser.h"
#include "DirectOutputPin.h"

#define PC_NOTIFICATION_STATUSREADY 'r'
#define PC_NOTIFICATION_STATUSFULL 't'
#define PC_NOTIFICATION_DATARECEIVED 'v'
#define PC_NOTIFICATION_DATAERROR 'x'    
#define PC_NOTIFICATION_MESSAGE 'm'    


#define DCSBIOS_RS485_DEVICE_RESPONSE_TIMEOUT 2

class DcsBiosRs485Controller {
private:
    Stream* _busStream;
    DcsBiosRs485BusParser _bus;
    DirectOutputPin _busTxPin;
    uint8_t _busBufferSize;
    uint8_t _busBuffer[DCSBIOS_RS485_MAX_PACKET_DATA_SIZE];
    bool _busWaitingResponse;
    uint8_t _busPollingAddress;
    unsigned long _busPollingTimeout;

    Stream* _pcStream;
    DcsBiosRs485PcParser _pc;

    void processBusInput();
    void sendPollingPacket();
    void processBusPacket();    

    void processPcInput();

public:
    void begin(Stream* busStream, int txPin, Stream* pcStream);
    void process();
};

#endif