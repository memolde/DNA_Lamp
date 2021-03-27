#pragma once
#include <Arduino.h>

boolean trigger(bool &old);

void setup_touch();
void read_touch(long);
int getTouchPattern();
int getTouchHold();
int getTouchBrightLevel();
long getTestmodeSensor(byte);
int getTestmodeTriggered();
boolean isTouchOn();
void setPatternOn();