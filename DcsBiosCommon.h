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
#ifndef _DCSBIOS_COMMON_H_
#define _DCSBIOS_COMMON_H_

#define DCSBIOS_BANK_SIZE 31
#define DCSBIOS_BUS_BUFFER_SIZE 64
#define DCSBIOS_MAX_ADDRESS 31

#define DCSBIOS_PACKETYPE_POLLING_UPDATE_BANK0 0
#define DCSBIOS_PACKETYPE_POLLING_UPDATE_BANK1 1
#define DCSBIOS_PACKETYPE_POLLING_UPDATE_BANK2 2
#define DCSBIOS_PACKETYPE_POLLING_UPDATE_BANK3 3
#define DCSBIOS_PACKETYPE_SIMULATION_DATA 4
#define DCSBIOS_PACKETYPE_DEVICE_COMMAND 5
#define DCSBIOS_PACKETYPE_CONFIG_REQUEST 6
#define DCSBIOS_PACKETYPE_CONFIG_RESPONSE 7

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#endif