#include "Blink.h"
#include "Arduino.h"

Blink::Blink(unsigned char _pin) {
	pinMode(_pin, OUTPUT);
	pin = _pin;
}

void Blink::on(int msec) {
	digitalWrite(pin, HIGH);
	delay(msec);
}

void Blink::off(int msec) {
	digitalWrite(pin, LOW);
	delay(msec);
}
