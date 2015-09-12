#include <ArduinoUnit.h>
#include <DcsBios.h>

test(status) {
	DcsBiosRs485PcParser parser;
	parser.processByte('s');
	assertEqual(parser.getState(), PC_STATUSREADY);

	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);
	parser.processByte('s');
	assertEqual(parser.getState(), PC_STATUSREADY);

	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);
	parser.processByte('s');
	assertEqual(parser.getState(), PC_STATUSREADY);

	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);
	parser.processByte('s');
	assertEqual(parser.getState(), PC_STATUSREADY);

	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);
	parser.processByte('s');
	assertEqual(parser.getState(), PC_STATUSFULL);

	parser.markBufferComplete();
	parser.processByte('s');	
	assertEqual(parser.getState(), PC_STATUSREADY);
}

test(bufferOverload) {
	DcsBiosRs485PcParser parser;
	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);

	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);

	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);

	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);

	parser.processByte('e');
	assertEqual(parser.getState(), PC_DATAERROR);
	parser.processByte(-1);
	assertEqual(parser.getState(), PC_STATUSFULL);
}

test(markBufferComplete) {
	DcsBiosRs485PcParser parser;
	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);
	assertTrue(parser.bufferReady());
	parser.markBufferComplete();
	assertFalse(parser.bufferReady());
	parser.processByte(-1);
	assertEqual(parser.getState(), PC_STATUSREADY);
	parser.processByte(-1);
	assertEqual(parser.getState(), PC_WAITING);	
}

test(dataLoadPauseTimeout) {
	DcsBiosRs485PcParser parser;
	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	delay(PC_COMMAND_TIMEOUT+10);
	parser.processByte(5);
	assertEqual(parser.getState(), PC_DATAERROR);
	assertFalse(parser.bufferReady());
	parser.processByte(-1);
	assertEqual(parser.getState(), PC_STATUSREADY);
}

test(dataLoadDisconnectTimeout) {
	DcsBiosRs485PcParser parser;
	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	delay(PC_COMMAND_TIMEOUT+10);
	parser.processByte(-1);
	assertEqual(parser.getState(), PC_DATAERROR);
	assertFalse(parser.bufferReady());
	parser.processByte(-1);
	assertEqual(parser.getState(), PC_STATUSREADY);
}

test(dataLoadError) {
	DcsBiosRs485PcParser parser;
	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(6);
	assertEqual(parser.getState(), PC_DATAERROR);
	assertFalse(parser.bufferReady());
	parser.processByte(-1);
	assertEqual(parser.getState(), PC_STATUSREADY);
}

test(backToBackDataLoad) {
	DcsBiosRs485PcParser parser;
	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);
	assertEqual(parser.getState(), PC_DATARECEIVED);
	parser.processByte('e');
	parser.processByte(2);
	parser.processByte(1);
	parser.processByte(2);
	parser.processByte(5);
	assertEqual(parser.getState(), PC_DATARECEIVED);
}

test(dataLoad) {
	DcsBiosRs485PcParser parser;
	parser.processByte('e');
	assertEqual(parser.getState(), PC_LOADINGDATA);
	assertFalse(parser.bufferReady());
	parser.processByte(2);
	assertEqual(parser.getState(), PC_LOADINGDATA);
	assertFalse(parser.bufferReady());
	parser.processByte(1);
	assertEqual(parser.getState(), PC_LOADINGDATA);
	assertFalse(parser.bufferReady());
	parser.processByte(-1);
	assertEqual(parser.getState(), PC_LOADINGDATA);
	assertFalse(parser.bufferReady());
	parser.processByte(2);
	assertEqual(parser.getState(), PC_LOADINGDATA);
	assertFalse(parser.bufferReady());
	parser.processByte(5);
	assertEqual(parser.getState(), PC_DATARECEIVED);
	assertTrue(parser.bufferReady());
	parser.processByte(-1);
	assertEqual(parser.getState(), PC_STATUSREADY);
	parser.processByte(-1);
	assertEqual(parser.getState(), PC_WAITING);
}

test(initialState) {
	DcsBiosRs485PcParser parser;
	assertFalse(parser.bufferReady());
	assertEqual(parser.getState(), PC_WAITING);
}

void setup() {
	Serial.begin(9600);
	while(!Serial); // for the Arduino Leonardo/Micro only
}

void loop() {
	Test::run();
}