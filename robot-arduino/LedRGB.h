/*
  LedRGB.h
*/

#ifndef LedRGB_h
#define LedRGB_h

#define COLOR_RED 255,0,0
#define COLOR_GREEN 0,255,0
#define COLOR_BLUE 0,0,255
#define COLOR_PURPLE 80,0,80
#define COLOR_CYAN 0,255,255
#define COLOR_BLACK 0,0,0

class LedRGB {
	public:
		LedRGB();
		LedRGB(int redPin, int greenPin, int bluePin);

		void setColor(int red, int green, int blue);
		   
	private:
		int redPin, greenPin, bluePin;
};

#endif
