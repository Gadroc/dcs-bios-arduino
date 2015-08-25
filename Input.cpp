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
#include "Input.h"

Input::Input(const char* message) {
    _message = message;
}

uint8_t Input::CommandBuffer[DCSBIOS_BUS_BUFFER_SIZE];
uint8_t Input::CommandBufferSize = 0;

uint8_t Input::getStringLength(const char* message) {
    uint8_t i = 0;
    while (message[i] > 0) {
        i++;
    }
    return i;
}

void Input::addString(const char* message) {
    uint8_t i = 0;
    uint8_t data = message[i++];
    while(data > 0) {
        CommandBuffer[CommandBufferSize++] = data;
        data = message[i++];
    }
}

void Input::sendMessage(int attribute) {
    char buf[7];
    utoa(attribute, buf, 10);
    sendMessage(buf);
}

void Input::sendMessage(const char* arg) {
    uint8_t totalSize = getStringLength(_message) + getStringLength(arg) + 2;
    if (CommandBufferSize + totalSize < DCSBIOS_BUS_BUFFER_SIZE) {
        addString(_message);
        addString(" ");
        addString(arg);
        addString("\n");
    }   
}