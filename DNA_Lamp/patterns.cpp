#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "constants.h"
#include "touch.h"
#include "patterns.h"

Adafruit_NeoPixel LEDS;
//Adafruit_NeoPixel LEDS1;
//Adafruit_NeoPixel LEDS2;
static uint16_t hue = 0;
static byte brightness = 0;
static float slowhue = 0;
boolean off;
Adafruit_NeoPixel	LEDS1 = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN1, RGBWSEQUENCE + NEO_KHZ800); //NEO_GRBW is default
Adafruit_NeoPixel	LEDS2 = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN2, RGBWSEQUENCE + NEO_KHZ800);

void ledBrightness(byte brightness){
	LEDS1.setBrightness(brightness);
	LEDS2.setBrightness(brightness);
}

void show(){
	LEDS1.show(); 
	LEDS2.show(); 
}

void setBrightness(int level){
	#ifdef debug
	    Serial.print("\t");
    	Serial.print(level); 
	#endif

	switch (level)
	{
	case 0:
		brightness = 30;
		break;
	case 1:
		brightness = 120;
		break;
	case 2:
		brightness = 255;
		break;
	default:
		break;
	}
	ledBrightness(brightness);
}

void setupPatterns(){
	pinMode(LED_ENABLE, OUTPUT);
	digitalWrite(LED_ENABLE,HIGH);
	LEDS1.begin();
	LEDS2.begin();
	ledBrightness(brightness);
	show();
}

void displayPattern(int patternNumber){
	#ifdef debug3
	    Serial.print("Pattern: ");
    	Serial.print(patternNumber); 
	    Serial.print(", slowhue: ");
	    Serial.println(slowhue);
	#endif

		switch (patternNumber)
		{
		case 0:
			patternCycleTroughFromMiddle();
			break;
		case 1:
			patternCycleTrough();
			break;
		case 2:
			patternBarLeftRight();
			break;
		case 3:
			patternRedWhite();
//			patternDNA1();
			break;
		case 98:
			patternSunrise();
			break;
		case 99:
			patternSunset();
			break;
		
		default:
			break;
		}
}


void setBar(int number, uint32_t color){
    LEDS1.setPixelColor(number, color);	
    LEDS2.setPixelColor(number, color);	
}

void pixelh(int num, uint16_t huein,uint16_t offset){
	LEDS1.setPixelColor(num,LEDS.gamma32(LEDS.ColorHSV(huein+offset*100,255,255)));
	LEDS2.setPixelColor(num,LEDS.gamma32(LEDS.ColorHSV(huein+offset*100,255,255)));
}
void pixelh1(int num, uint16_t huein,uint16_t offset){
	LEDS1.setPixelColor(num,LEDS.gamma32(LEDS.ColorHSV(huein+offset*100,255,255)));
}
void pixelh2(int num, uint16_t huein,uint16_t offset){
	LEDS2.setPixelColor(num,LEDS.gamma32(LEDS.ColorHSV(huein+offset*100,255,255)));
}
void pixelhc(int num, uint16_t huein,uint16_t offset,int white){
	LEDS1.setPixelColor(num,LEDS.gamma32(LEDS.ColorHSV(huein+offset*100,255,255)+(white*16777216)));
	LEDS2.setPixelColor(num,LEDS.gamma32(LEDS.ColorHSV(huein+offset*100,255,255)+(white*16777216)));
}
void pixelc(int num, int red, int green, int blue, int white){
	LEDS1.setPixelColor(num,LEDS.gamma32(LEDS.Color(red,green,blue,white)));
	LEDS1.setPixelColor(num,LEDS.Color(red,green,blue,white));
}

void pixelc1(int num, int red, int green, int blue, int white){
	LEDS1.setPixelColor(num,LEDS.gamma32(LEDS.Color(red,green,blue,white)));
}
void pixelc2(int num, int red, int green, int blue, int white){
	LEDS2.setPixelColor(num,LEDS.gamma32(LEDS.Color(red,green,blue,white)));
}
void pixelcol1(int num, uint32_t color){
	LEDS1.setPixelColor(num,LEDS.gamma32(color));
}
void pixelcol2(int num, uint32_t color){
	LEDS2.setPixelColor(num,LEDS.gamma32(color));
}

uint32_t pseudoRandomDNA(int num,int seed){
	int r = 0;
	int g = 0;
	int b = 0;
	static uint64_t prev;
//	srand(seed*13);
//	int random = rand()%4;// (37*seed * num) % 4;
	uint64_t random = (37*(seed*21+11) * (num*27+7) * (prev+3));
	prev = random;
	if(num==0){
//		Serial.println((long)random);
	}
	switch (random % 4)
	{
	case 0:
		r = 255;
		break;
	case 1:
		g = 255;
		break;
	case 2:
		b = 255;
		break;
	case 3:
		r = 255;
		g = 255;
		break;
	
	default:
		break;
	}
	return LEDS.Color(r,g,b,0);
}

void patternCycleTroughFromMiddle(){
	#define SPREAD 20
	#define RIGHTLEFT 40
	for(int i = 0; i < NUM_LEDS/2; i++) {
		pixelh1(i,hue,i*SPREAD);
		pixelh2(i,hue,i*SPREAD+RIGHTLEFT);
		pixelh1(NUM_LEDS-1-i,hue,i*SPREAD+SPREAD);
		pixelh2(NUM_LEDS-1-i,hue,i*SPREAD+RIGHTLEFT+SPREAD);
	}
	hue += 30;
	show();
}
void patternCycleTrough(){
	int rainbowMult = 65536 / (NUM_LEDS+1) / 100;
	for(int i = 0; i < NUM_LEDS; i++) {
		pixelhc(i,hue,i*rainbowMult,70);
	}
	show();
	hue += 10;
}

void patternDNA1(){
	for(int i = 0; i < NUM_LEDS; i++) {
		pixelcol1(i,pseudoRandomDNA(i,hue/1300+i));
		pixelcol2(i,pseudoRandomDNA(i,hue/1300+i+13));
	}
	show();
	hue += 10;
}
 
void patternBarLeftRight(){
	// A Bar is moving right and left
	int divisor = (NUM_LEDS - 1) * 2;
	int ledm1 = NUM_LEDS - 1;
	int bar = hue / 4;
	for(int i = 0; i < NUM_LEDS; i++) {
		if( ( (bar) %divisor == i) || ( (bar+ledm1) %divisor == (ledm1-i)%NUM_LEDS) ) {
			pixelh(i,hue,200);
		}else{
			setBar(i,LEDS.Color(0,0,0,0));
		}
	}
	show();
	hue += 1;
//	hue = hue % 242; // 12*21
}

void patternRedWhite(){
	// Red and white alternating
	//Red white red white
	int r1;
	int r2;
	int w1;
	int w2;

	float val;
	val = (float)hue*2.0;
	if (val > 65535){
       val = 131071-val;
	}
	val = (float)LEDS.sine8(hue/256)*256.0;
	r1 = ((65535.0-val) * (65535.0-val))/16777216.0L;
//	w1 = (val * val)/16777216.0L;
    r1 = (65535.0-val) / 256.0;
    w1 = val / 256;
	r2 = w1;
	w2 = r1;

	for(int i = 0; i < NUM_LEDS/2; i++) {
		pixelc1(i*2+0,r1,0,0,w1);
		pixelc1(i*2+1,r2,0,0,w2);
		pixelc2(i*2+0,r2,0,0,w2);
		pixelc2(i*2+1,r1,0,0,w1);
	}
	/*
	pixelc(0,r1,0,0,w1);
	pixelc(2,r1,0,0,w1);
	pixelc(4,r1,0,0,w1);
	pixelc(6,r1,0,0,w1);

	pixelc(1,r2,0,0,w2);
	pixelc(3,r2,0,0,w2);
	pixelc(5,r2,0,0,w2);
	pixelc(7,r2,0,0,w2);
	*/
	hue +=120;
	show();
}

void pattern5(){
	/*
	// A Bar is moving through the lamp
//	uint8_t off = -45;
	for(int i = 0; i < NUM_LEDS; i++) {
		if((hue/8)%7==i%7){
			setBar(i,CHSV(hue+150,255,255));
		}else{
			setBar(i,CRGB(0,0,0));
		}
		setBetween(i,CRGB(0,0,0));
	}
	FastLED.show(); 
	hue += 1;
	*/
}

void allOff(){
	for(int i = 0; i < NUM_LEDS; i++) {
		setBar(i,LEDS.Color(0,0,0,0));
	}
	show();
}


void sunriseSingle(int bar,float h){

	h = max(h,0);
	float hsqrt = ( float(sqrt( h )));
	int red   = ( hsqrt * hsqrt );//* float(16.0) );
//	int downred = ( hsqrt * 16.0 );
	int green = ( h * hsqrt ) / 256 * 16.0;
	int blue  = ( h * h ) / 256.0;
	int white = blue;
	if(h>130){
		red -= (h-130);
		green -= (h-130);
		blue -= (h-130);
	}

	setBar(bar,LEDS.Color(red,green,blue,white));
	show();
}

void sunriseAll(){
	float floathue = min(slowhue/6,230);
	int maxfloatisquared = ((NUM_LEDS/2-1)+3)*((NUM_LEDS/2-1)+3);
	for(int i = 0; i < NUM_LEDS/2; i++) {
		float floati = i+3;
		float hueadd = floathue-maxfloatisquared+(floati*floati);
		sunriseSingle(i,hueadd);
		sunriseSingle(NUM_LEDS-1-i,hueadd);
	}
}

void patternSunrise(){
	// Sunrise. Afterwards stay as it is.
	off = false;
	LEDS1.setPin(DATA_PIN1);
	LEDS2.setPin(DATA_PIN2);
	digitalWrite(LED_ENABLE,HIGH);
	sunriseAll();
	if(slowhue < 2000){
		slowhue += 10;
	}
	show();
}
void patternSunset(){
	// Sunset. 
	sunriseAll();
	if(slowhue > 0){
		slowhue -= 20;
	}
	if(slowhue > 1380){ //1150
		slowhue = 1380;
	}
	if(slowhue <= 0){
		off = true;
		slowhue = 0;
		allOff();
		ledBrightness(0);
		show();
		delay(1000);
		LEDS1.setPin(-1);
		LEDS2.setPin(-1);
		digitalWrite(LED_ENABLE,LOW);
		return;
	}
	show();
}

void patternTestRGB(int color){
    if(color == 0){
		ledBrightness(128);
		setBar(0,LEDS.Color(255,0,0,0));
		setBar(1,LEDS.Color(0,255,0,0));
		setBar(2,LEDS.Color(0,0,255,0));
		setBar(3,LEDS.Color(0,0,0,255));
	}else{
		ledBrightness(255);
		testmodeTriggered(color);
	}
	show();
}

void testmodeTouchLevel(long level1, long level2,long level3){

	uint8_t r;
	uint8_t g;
	uint8_t b;


	for(int i = 4; i < NUM_LEDS; i++) {
		r = 0;
		g = 0;
		b = 0;
		#ifdef debug_no
			Serial.print("\t");
			Serial.print(level1);                  // print sensor output 1
			Serial.print("\t");
			Serial.print(level2);                  // print sensor output 2
			Serial.print("\t");
			Serial.print(level3);                // print sensor output 3
			Serial.print("\n");
		#endif

		if(level1>(i-3)*SENSITIVTIVE){
			r=255;
		}
		if(level2>(i-3)*SENSITIVTIVE){
			g=255;
		}
		if(level3>(i-3)*SENSITIVTIVE){
			b=255;
		}
		setBar(i,LEDS.Color(r,g,b,0));
	}
}
void testmodeTriggered(int color){
	for(int i = 0; i < NUM_LEDS-1; i++) {
		switch (color)
		{
		case 1:
			setBar(i,LEDS.Color(255,0,0,0));
			break;
		case 2:
			setBar(i,LEDS.Color(0,255,0,0));
			break;
		case 3:
			setBar(i,LEDS.Color(0,0,255,0));
			break;
		
		default:
			break;
		}
	}
	show();
}
boolean isOff(){
	return off;
}
void powerLevel(long level){
	for(int i = 0; i < NUM_LEDS; i++) {
		if((level-500)*2*NUM_LEDS / 1000 > i){  // 2.5-5 V
			setBar(i,LEDS.Color(0,100,255,0));
		}else{
			setBar(i,LEDS.Color(0,0,0,0));
		}
	}
	show();
}
