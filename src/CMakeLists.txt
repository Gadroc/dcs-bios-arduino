generate_arduino_library(DcsBios
        BOARD uno
        SRCS pc/DcsBiosRs485BusParser.cpp
        pc/DcsBiosRs485Controller.cpp
        pc/DcsBiosRs485Device.cpp
        pc/DcsBiosRs485PcParser.cpp
        pc/DcsBiosSerialDevice.cpp
        hal/AcceleratedStepperOutput.cpp
        hal/DirectInputPin.cpp
        hal/DirectAnalogOuptut.cpp
        hal/DirectInputPin.cpp
        hal/DirectAnalogInput.cpp
        hal/DirectOutputPin.cpp
        hal/DirectStepperDriver.cpp
        dcs/ExportStreamListener.cpp
        dcs/ExportStreamParser.cpp
        dcs/Input.cpp
        dcs/IntegerListener.cpp
        dcs/PollingInput.cpp
        controls/ActionButton.cpp
        controls/DimmableLed.cpp
        controls/Led.cpp
        controls/PollingSwitch.cpp
        controls/Potentiometer.cpp
        controls/Stepper.cpp
        controls/Switch2Pos.cpp
        controls/Switch3Pos.cpp
        controls/SwitchMultiPos.cpp
        HDRS DcsBios.h
        )