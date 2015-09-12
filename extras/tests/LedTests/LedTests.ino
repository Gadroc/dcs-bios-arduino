#include <ArduinoUnit.h>
#include <DcsBios.h>

class TestAnalogOutput : public AnalogOutput {
private:
    unsigned int value;

public:
  virtual unsigned int maxValue() {
    return 255;
  }
  virtual void write(unsigned int newValue) {
    value = newValue;
  }
  unsigned int getValue() {
    return value;
  }
};

class TestDigitalOutput : public OutputPin {
 private:
  int value = 0;

public:
  virtual void set() {
    value = 1;
  }

  virtual void clear() {
    value = -1;
  }

  int getValue() {
    return value;
  }
};

test(ledOuptutOn) {
  TestDigitalOutput output;
  Led led(0x1026, 0x1000, 12, output);
  led.onDcsBiosWrite(0x1026, 0x1000);
  led.onDcsBiosFrameSync();
  assertEqual(output.getValue(), 1);
}

test(ledOuptutOff) {
  TestDigitalOutput output;
  Led led(0x1026, 0x0800, 12, output);
  led.onDcsBiosWrite(0x1026, 0x1000);
  led.onDcsBiosFrameSync();
  assertEqual(output.getValue(), -1);
}

test(zeroDimmableOutput) {
  TestAnalogOutput output;  
  DimmableLed led(0x114c, 0xffff, 0, output);
  led.onDcsBiosWrite(0x114c, 0x0000);
  led.onDcsBiosFrameSync();
  assertEqual(output.getValue(), 0);
}

test(midDimmableOutput) {
  TestAnalogOutput output;  
  DimmableLed led(0x114c, 0xffff, 0, output);
  led.onDcsBiosWrite(0x114c, 0x7fff);
  led.onDcsBiosFrameSync();
  assertEqual(output.getValue(), 127); 
}

test(maxDimmableOutput) {
  TestAnalogOutput output;  
  DimmableLed led(0x114c, 0xffff, 0, output);
  led.onDcsBiosWrite(0x114c, 0xffff);
  led.onDcsBiosFrameSync();
  assertEqual(output.getValue(), 255);
}

void setup() {
  Serial.begin(9600);
  while(!Serial); // for the Arduino Leonardo/Micro only
}

void loop() {
  Test::run();
}
