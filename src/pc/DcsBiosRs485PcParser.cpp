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
#include "DcsBiosRs485PcParser.h"

DcsBiosRs485PcParser::DcsBiosRs485PcParser() {
    _pcBufferHead = 0;
    _pcBufferTail = 0;
    _pcBuffersReady = 0;
    _pcBufferMarked = false;
    _pcState = PC_WAITING;
}

void DcsBiosRs485PcParser::processByte(int in) {
    if (_pcState == PC_LOADINGDATA && (long)(millis() - _pcCommandTimeout) >= 0) {
        _pcState = PC_DATAERROR;            
    } else if (in == -1) {
        switch(_pcState) {
            case PC_WAITING:
                if (_pcBufferMarked) {
                    setStatusState();
                    _pcBufferMarked = false;
                }
                break;

            case PC_STATUSREADY:
            case PC_STATUSFULL:
                _pcState = PC_WAITING;
                break;

            case PC_DATAERROR:
            case PC_DATARECEIVED:
                setStatusState();
                break;
        }
    } else {
        switch(_pcState) {
            case PC_STATUSREADY:
            case PC_STATUSFULL:
            case PC_DATAERROR:
            case PC_DATARECEIVED:
                _pcState = PC_WAITING;
            case PC_WAITING:
                parseCommand(in);
                break;

            case PC_LOADINGDATA:
                parseDataLoad(in);
                break;
        }
    }
}

void DcsBiosRs485PcParser::parseCommand(uint8_t in) {
    if (in == PC_COMMAND_STATUS) {
        setStatusState();
    }

    if (in == PC_COMMAND_ADD_EXPORT_DATA) {
        if (_pcBuffersReady < DCSBIOS_RS485_PC_BUFFER_COUNT) {
            _pcCommandTimeout = millis() + PC_COMMAND_TIMEOUT;
            _pcState = PC_LOADINGDATA;
            _pcBufferLoadState = PCLOAD_DATASIZE;
        } else {
            _pcState = PC_DATAERROR;
        }
    }
}

void DcsBiosRs485PcParser::parseDataLoad(uint8_t in) {
    _pcCommandTimeout = millis() + PC_COMMAND_TIMEOUT;
    switch (_pcBufferLoadState) {
        case PCLOAD_DATASIZE:
            _pcBuffersSize[_pcBufferTail] = in;
            _pcBufferLoadChecksum = in;
            _pcBufferLoadIndex = 0;
            _pcBufferLoadState = PCLOAD_DATA;
            break;

        case PCLOAD_DATA:
            _pcBuffers[_pcBufferTail][_pcBufferLoadIndex++] = in;
            _pcBufferLoadChecksum += in;
            if (_pcBufferLoadIndex == _pcBuffersSize[_pcBufferTail]) {
                _pcBufferLoadState = PCLOAD_DATACHECKSUM;
            }
            break;

        case PCLOAD_DATACHECKSUM:
            if (_pcBufferLoadChecksum == in) {
                _pcBufferTail++;
                if (_pcBufferTail == DCSBIOS_RS485_PC_BUFFER_COUNT) {
                    _pcBufferTail = 0;
                }
                _pcBuffersReady++;
                _pcState = PC_DATARECEIVED;
            } else {
                _pcState = PC_DATAERROR;
            }
            break;
    }
}