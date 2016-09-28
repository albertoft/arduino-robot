#include "Arduino.h"
#include "LedRGB.h"

LedRGB::LedRGB() 
{
  ;
}

LedRGB::LedRGB(int redPin, int greenPin, int bluePin) 
{
	pinMode(redPin, OUTPUT);
	pinMode(greenPin, OUTPUT);
	pinMode(bluePin, OUTPUT);

	this->redPin = redPin;
	this->greenPin = greenPin;
	this->bluePin = bluePin;
}


void LedRGB::setColor(int red, int green, int blue) 
{
	analogWrite(this->redPin, red);
	analogWrite(this->greenPin, green);
	analogWrite(this->bluePin, blue);
}
