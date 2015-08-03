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
#ifndef _DCSBIOS_BUFFERLISTENER_H_
#define _DCSBIOS_BUFFERLISTENER_H_

#include <Arduino.h>

class BufferListener {
    private:
        virtual void onBufferReady(uint8_t *buffer) {}
        BufferListener* _nextListener;

    public:
        static BufferListener* firstBufferStreamListener;
        static void handleBufferReady(uint8_t *buffer);

        BufferListener();
};

inline BufferListener::BufferListener() {
    this->_nextListener = firstBufferStreamListener;
    firstBufferStreamListener = this;
}

#endif