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
#ifndef _DCSBIOS_STRINGLISTENER_H_
#define _DCSBIOS_STRINGLISTENER_H_

#include <Arduino.h>
#include "ExportStreamListener.h"

template<unsigned int LENGTH> class StringListener : ExportStreamListener {
private:
    unsigned int _address;
    unsigned int _endAddress;
    bool _dirty;
    char _buffer[LENGTH+1];   

protected:
    bool isDirty() {
        return _dirty;
    }
    void clearDirty() {
        _dirty = false;
    }

public:
    StringListener(unsigned int address) {
        _address = address;
        _endAddress = address+LENGTH;
        memset(_buffer, '\0', LENGTH+1);
        _dirty = false;
    }

    virtual void onDcsBiosWrite(unsigned int address, unsigned int value) {
        if ((address >= _address) && (_endAddress > address)) {
            unsigned int index = address - _address;
            _buffer[index] = ((char*)&value)[0];
            index++;
            if (LENGTH > index) {
                _buffer[index] = ((char*)&value)[1];
            }
            // No need to compare existing buffer with current value.  We never get to this
            // point unless the sim has sent a change.
            _dirty = true;
        }    
    }

    char* getString() {
        return _buffer;
    }
};

#endif