#include <Arduino.h>
#include "constants.h"
#include <Adafruit_SleepyDog.h>

int touchdelay;
boolean testmodeOn = false;
boolean powerLevelOn = false;
long vcc = 950;
int pattern = 0;
#include "touch.h"
#include "patterns.h"
#include "voltage.h"


void setup() { 
	#ifdef debug	
		Serial.begin(57600);
		Serial.println("Resetting...");
	#endif
	setupPatterns();
	setup_touch();
}

void testmode(){
		long sensor1 = getTestmodeSensor(1);
		long sensor2 = getTestmodeSensor(2);
		long sensor3 = getTestmodeSensor(3); 
		#ifdef debug_no
			Serial.print("\t");
			Serial.print(sensor1);                  // print sensor output 1
			Serial.print("\t");
			Serial.print(sensor2);                  // print sensor output 2
			Serial.print("\t");
			Serial.print(sensor3);                // print sensor output 3
			Serial.print("\n");
		#endif
		testmodeTouchLevel(sensor1,sensor2,sensor3);
		int triggered = getTestmodeTriggered();
//        testmodeTriggered(triggered);
		patternTestRGB(triggered);
//		setTouchLevel(sensor1,sensor2,sensor3);
	delay(400);
	return;
}

void goToSleep(){

	int sleepMS = Watchdog.sleep(250);
   	#ifdef debug	
		Serial.print("2: ");
		Serial.println(sleepMS);
		delay(100);
	#endif
  sleepMS++;
}

void loop() { 

	#ifdef debug3
	  Serial.print("x");
	#endif
	vcc = (readVcc() * 1000/4600 + vcc * 29 )/30;

	if(powerLevelOn){
		read_touch(vcc);
		int touchHold = getTouchHold();
		if(touchHold==-1){
			powerLevelOn = false;
		}
		powerLevel(vcc);
		delay(50);		
		return;
	}


	if(testmodeOn){
		read_touch(vcc);
		int touchHold = getTouchHold();
		if(touchHold==-1){
			testmodeOn = false;
		}
		testmode();
		return;
	}
	#ifdef TESTMODE
		read_touch(vcc);
		testmode();
	#else
		if(isOff()){
			if(isTouchOn()){
				// Wake up...
				setPatternOn();
				displayPattern(98);
				touchdelay = 9999;
			}else{
				goToSleep();
			}
		}else{
			// Normal running

			if(touchdelay >=TOUCH_CYCLES){ // 1 equals approximately to 50ms
				#ifdef debug
	  				Serial.print("vcc: ");
	  				Serial.print(vcc);
				#endif

				read_touch(vcc);
				pattern = getTouchPattern();
	  				Serial.print(pattern);
				if(pattern == -1){
					testmodeOn = false;
					powerLevelOn = false;
					return;
				}
				if(pattern == -2){
					testmodeOn = true;
					return;
				}
				if(pattern == -3){
					powerLevelOn = true;
					return;
				}
				touchdelay = 0;
				setBrightness(getTouchBrightLevel());
			}else{
				touchdelay += 1;
			}

			displayPattern(pattern);
			delay(20);
		}
	#endif

}
