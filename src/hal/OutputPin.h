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
#ifndef _DCSBIOS_OUTPUTPIN_H_
#define _DCSBIOS_OUTPUTPIN_H_

// Interface for all input pins.
class OutputPin {
public:
    virtual bool isValid() = 0;
    virtual void set() = 0;
    virtual void clear() = 0;
    void setState(bool state);
};

inline void OutputPin::setState(bool state) {
    if (state) {
        set();
    } else {
        clear();
    }
}

#endif