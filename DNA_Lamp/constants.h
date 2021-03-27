#include <Adafruit_NeoPixel.h>


// How many leds in your strip?
#define NUM_LEDS 12
// Data pins of both strips
#define DATA_PIN1 2
#define DATA_PIN2 3

// Pin which triggers the mosfet to disable the led strips
// This is needed to save power since each led draws about 1ma when switched off
#define LED_ENABLE 11

// Pin where all 2 MOhm resistors are connected
#define TOUCH_COMMON 5
// Pins where the touchpads are connected
#define TOUCH_ONOFF 8
#define TOUCH_2 9
#define TOUCH_3 7


#define TOUCH_CYCLES 8

// Uncomment debug to get information on the serial console
//#define debug  x
//#define debug2 x
//#define debug3 x 

// Testmode can be used to calibrate the LEDs and Sensors for real mode comment
//#define TESTMODE 1

// change this value when the lamp does not show   red, green, blue, white   as the first 4 bars
// try to swap 2 color letters which are not shown in the correct sequence
// Red = R, Blue = B, G = Green, W = White
// default is NEO_GRBW 
#define RGBWSEQUENCE NEO_GRBW

#define SENSITIVTIVE 15
// Sensitivity for covererd sensors 10-20
// Sensitivity for uncovered sensors 100-1000

// count the bars showing your desired color without flickering 
// when touching the sensor in a 'normal' way 
// i.e. Not pressing very hard or with more than one finger
// Higher values reduce the sensitivity of the sensor

#define SENSORGREEN 5  // Pattern
#define SENSORBLUE 7  // Brightness 
#define SENSORRED  8   // ON/OFF switch 7
