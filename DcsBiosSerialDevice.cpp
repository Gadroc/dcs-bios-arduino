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
#include "DcsBiosSerialDevice.h"

DcsBiosSerialDevice::DcsBiosSerialDevice() {}

void DcsBiosSerialDevice::begin(Stream *serial) {
  _serial = serial;
}

void DcsBiosSerialDevice::process() {
  _parser.processByte(_serial->read());
}

void DcsBiosSerialDevice::sendDcsBiosMessage(const char* msg, const char* arg) {
  _serial->write(msg);
  _serial->write(' ');
  _serial->write(arg);
  _serial->write('\n');
  _serial->flush();
}