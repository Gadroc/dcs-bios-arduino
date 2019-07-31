set(MasterCaution_SKETCH MasterCaution/MasterCaution.ino)
generate_arduino_firmware(
        MasterCaution
)

set(MasterCautionRs485_SKETCH MasterCautionRs485/MasterCautionRs485.ino)
generate_arduino_firmware(
        MasterCautionRs485
)

set(Rs485BusController_SKETCH Rs485BusController/Rs485BusController.ino)
generate_arduino_firmware(
        Rs485BusController
        BOARD leonardo
)