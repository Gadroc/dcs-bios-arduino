#include <ArduinoUnit.h>
#include <DcsBios.h>

test(regularPacket) {
  DcsBiosRs485Parser parser;
  assertEqual(parser.getPacketState(), PACKET_START);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0xbb);
  assertEqual(parser.getPacketState(), PACKET_LEADIN);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x88);
  assertEqual(parser.getPacketState(), PACKET_ADDRTYPE);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x20);
  assertEqual(parser.getPacketState(), PACKET_SIZE);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x02);
  assertEqual(parser.getPacketState(), PACKET_DATA);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x67);
  assertEqual(parser.getPacketState(), PACKET_DATA);
  assertTrue(parser.hasPacketDataByte());
  assertEqual(parser.getPacketDataByte(), 0x67);  
  parser.processByte(0x96);
  assertEqual(parser.getPacketState(), PACKET_COMPLETE);
  assertTrue(parser.hasPacketDataByte());
  assertEqual(parser.getPacketDataByte(), 0x96);
  parser.processByte(-1);  
  assertEqual(parser.getPacketState(), PACKET_START);
  assertFalse(parser.hasPacketDataByte());
}

test(zeroLengthPacket) {
  DcsBiosRs485Parser parser;
  assertEqual(parser.getPacketState(), PACKET_START);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0xbb);
  assertEqual(parser.getPacketState(), PACKET_LEADIN);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x88);
  assertEqual(parser.getPacketState(), PACKET_ADDRTYPE);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x20);
  assertEqual(parser.getPacketState(), PACKET_SIZE);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(-1);
  assertEqual(parser.getPacketState(), PACKET_SIZE);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x00);
  assertEqual(parser.getPacketState(), PACKET_COMPLETE);
  assertFalse(parser.hasPacketDataByte());
}

test(timeoutPacket) {
  DcsBiosRs485Parser parser;
  assertEqual(parser.getPacketState(), PACKET_START);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0xbb);
  assertEqual(parser.getPacketState(), PACKET_LEADIN);
  assertFalse(parser.hasPacketDataByte());
  delay(3000);
  parser.processByte(-1);
  assertEqual(parser.getPacketState(), PACKET_TIMEOUT);  
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(-1);
  assertEqual(parser.getPacketState(), PACKET_START);
  assertFalse(parser.hasPacketDataByte());
}

test(backToBack) {
  DcsBiosRs485Parser parser;
  assertEqual(parser.getPacketState(), PACKET_START);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0xbb);
  assertEqual(parser.getPacketState(), PACKET_LEADIN);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x88);
  assertEqual(parser.getPacketState(), PACKET_ADDRTYPE);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x20);
  assertEqual(parser.getPacketState(), PACKET_SIZE);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x02);
  assertEqual(parser.getPacketState(), PACKET_DATA);
  assertFalse(parser.hasPacketDataByte());
  parser.processByte(0x67);
  assertEqual(parser.getPacketState(), PACKET_DATA);
  assertTrue(parser.hasPacketDataByte());
  assertEqual(parser.getPacketDataByte(), 0x67);  
  parser.processByte(0x96);
  assertEqual(parser.getPacketState(), PACKET_COMPLETE);
  assertTrue(parser.hasPacketDataByte());
  assertEqual(parser.getPacketDataByte(), 0x96);
  parser.processByte(0xbb);
  assertEqual(parser.getPacketState(), PACKET_LEADIN);
  assertFalse(parser.hasPacketDataByte());  
}

void setup() {
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only
}

void loop() {
  Test::run();
}
