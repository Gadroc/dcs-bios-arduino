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
#ifndef _DCSBIOS_RS485PCPARSER_H_
#define _DCSBIOS_RS485PCPARSER_H_

#include <Arduino.h>

#define PC_COMMAND_TIMEOUT 250

#define PC_COMMAND_STATUS 's'
#define PC_COMMAND_ADD_EXPORT_DATA 'e'

#define DCSBIOS_RS485_PC_BUFFER_COUNT 4
#define DCSBIOS_RS485_PC_BUFFER_SIZE 255

enum PC_STATE : uint8_t {
    PC_WAITING = 0,         // Waiting for command from PC
    PC_LOADINGDATA = 1,     // Waiting for data load
    PC_STATUSREADY = 2,     // Buffers available for PC
    PC_STATUSFULL = 3,      // Buffers full
    PC_DATARECEIVED = 4,    // Data received and validated
    PC_DATAERROR = 5        // Error receiving data (Timeout or Checksum Error)
};

enum PC_LOADINGSTATE {
    PCLOAD_DATASIZE = 0,        // Waiting for data size
    PCLOAD_DATA = 1,            // Waiting for data load
    PCLOAD_DATACHECKSUM = 2     // Waiting for data checksum    
};

class DcsBiosRs485PcParser {
private:
    PC_STATE _pcState;
    unsigned long _pcCommandTimeout;

    bool _pcBufferMarked;
    uint8_t _pcBufferHead;
    uint8_t _pcBufferTail;
    uint8_t _pcBuffersReady;
    PC_LOADINGSTATE _pcBufferLoadState;
    uint8_t _pcBufferLoadIndex;
    uint8_t _pcBufferLoadChecksum;
    uint8_t _pcBuffersSize[DCSBIOS_RS485_PC_BUFFER_COUNT];
    uint8_t _pcBuffers[DCSBIOS_RS485_PC_BUFFER_COUNT][DCSBIOS_RS485_PC_BUFFER_SIZE];

    void setStatusState();
    void parseCommand(uint8_t in);
    void parseDataLoad(uint8_t in);

public:
    DcsBiosRs485PcParser();
    void processByte(int in);

    PC_STATE getState();
    bool bufferReady();
    uint8_t getCurrentBufferSize();
    uint8_t* getCurrentBuffer();
    void markBufferComplete();
};

inline void DcsBiosRs485PcParser::setStatusState() {
    _pcState = _pcBuffersReady < DCSBIOS_RS485_PC_BUFFER_COUNT ? PC_STATUSREADY : PC_STATUSFULL;
}

inline PC_STATE DcsBiosRs485PcParser::getState() {
    return _pcState;
}

inline bool DcsBiosRs485PcParser::bufferReady() {
    return _pcBuffersReady > 0;
}

inline uint8_t DcsBiosRs485PcParser::getCurrentBufferSize() {
    if (_pcBuffersReady == 0) {
        return 0;
    }

    return _pcBuffersSize[_pcBufferHead];
}

inline uint8_t* DcsBiosRs485PcParser::getCurrentBuffer() {
    if (_pcBuffersReady == 0) {
        return 0;
    }

    return _pcBuffers[_pcBufferHead];
}

inline void DcsBiosRs485PcParser::markBufferComplete() {
    if (_pcBuffersReady == 0) {
        return;
    }

    _pcBufferHead++;
    if (_pcBufferHead == DCSBIOS_RS485_PC_BUFFER_COUNT) {
        _pcBufferHead = 0;
    }
    _pcBuffersReady--;

    _pcBufferMarked = true;
}

#endif