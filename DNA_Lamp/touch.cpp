#include <Arduino.h>
#include <CapacitiveSensor.h>
#include "touch.h"
#include "constants.h"

CapacitiveSensor   cs_1  = CapacitiveSensor(TOUCH_COMMON,TOUCH_ONOFF);        // 10M resistor between pins 2 and sensor
CapacitiveSensor   cs_2  = CapacitiveSensor(TOUCH_COMMON,TOUCH_2);    
CapacitiveSensor   cs_3 = CapacitiveSensor(TOUCH_COMMON,TOUCH_3);    

boolean t1;  // Touch status
boolean t2;
boolean t3;

boolean tr1;  // Trigger
boolean tr2;
boolean tr3;

boolean to1; // old(internal)
boolean to2;
boolean to3;

long sensor1;
long sensor2;
long sensor3;


int patternNumber = 98; // Sunrise
int brightLevel = 1;
int t1count = 0;
int t2count = 0;
int t3count = 0;

void setup_touch(){

   cs_1.set_CS_AutocaL_Millis(0xFFFFFFFF);     // turn off autocalibrate on channel 1 - just as an example
   cs_2.set_CS_AutocaL_Millis(0xFFFFFFFF);
   cs_3.set_CS_AutocaL_Millis(0xFFFFFFFF);

}

void setPatternOn(){
    patternNumber = 98;
    to1 = true;
    tr1 = false;

}
boolean trigger(bool &old){
    if(old){
         return false;
    }
    old = true;
    return true;
}

boolean isTouchOn(){
    long sensorValue =  cs_1.capacitiveSensor(30);
    if(sensorValue / SENSORRED > SENSITIVTIVE * 3 / 5){
        return true;
    }else{
        return false;
    }
}

void read_touch(long vccAdjust){ //VccAcjust is 1000 for USB
   
    long total1;
    long total2;
    long total3;
    total1 =  cs_1.capacitiveSensor(30);
    total2 =  cs_2.capacitiveSensor(30);
    total3 =  cs_3.capacitiveSensor(30);
    sensor1 = total1;
    sensor2 = total2;
    sensor3 = total3;

    total1 = total1 /  SENSORRED;
    total2 = total2 /  SENSORGREEN;
    total3 = total3 /  SENSORBLUE;

    long total = max(total1,max(total2,total3));
    if(total1<total){total1=0;};
    if(total2<total){total2=0;};
    if(total3<total){total3=0;};

    int treshold = ( SENSITIVTIVE * 2 * vccAdjust ) / 3000; //66% of absolute maximum
    t1 = false;
    t2 = false;
    t3 = false;
    tr1 = false;
    tr2 = false;
    tr3 = false;

    if(total1 > treshold){
        tr1 = trigger(to1);
        t1 = true;
        t1count++;
    }else{
        to1=false;
        t1count = 0;
    }
    if(total2 > treshold){
        tr2 = trigger(to2);
        t2 = true;
        if(to2){
            t2count++;
        }else{
            t2count = 0;
        }
    }else{
        to2=false;
        t2count = 0;
    }
    if(total3 > treshold){
        tr3 = trigger(to3);
        t3 = true;
        t3count++;
    }else{
        to3=false;
        t3count = 0;
    }

    #ifdef debug2
    
    Serial.print("\t");
    Serial.print(total1);                  // print sensor output 1
    Serial.print("\t");
    Serial.print(total2);                  // print sensor output 2
    Serial.print("\t");
    Serial.print(total3);                // print sensor output 3
    Serial.print("\n");
    
    #endif
}

int getTouchHold(){
    if(t1count > 12){
        // keep finger on pattern change
        t1count=0;
        return -1;
    }
    if(t2count > 12){
        // keep finger on pattern change
        t2count=0;
        return -2;
    }
    if(t3count > 12){
        // keep finger on brightness change
        t3count=0;
        return -3;
    }
    return 0;
}

int getTouchPattern(){
	// Read the global variables tr<x> to see which touchpad was triggered
    #ifdef debug3
        Serial.println(t2count);
    #endif

    int touchHold = getTouchHold();
    if (touchHold != 0){
        return touchHold;
    }

	if(tr2==true){
    	patternNumber += 1; 
        patternNumber %= 4;
	}
    if(tr1==true){
        if(patternNumber == 99){
            patternNumber = 98;    
        }else{
            patternNumber = 99;
        }
    }
    return patternNumber;
}

int getTouchBrightLevel(){
	if(tr3==true){
    	brightLevel += 1;
        brightLevel %= 3;
	}
    return brightLevel;
}

long getTestmodeSensor(byte which){

    switch (which){
    case 1:
        return sensor1;
        break;
    case 2:
        return sensor2;
        break;
    case 3:
        return sensor3;
        break;
    
    default:
        break;
    }
   return 0;
}

int getTestmodeTriggered(){
    if(tr1==true){
        return 1;
    }
    if(tr2==true){
        return 2;
    }
    if(tr3==true){
        return 3;
    }
   return 0;

}
