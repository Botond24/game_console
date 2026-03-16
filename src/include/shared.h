#pragma once
#include <Arduino.h>
#include <iostream>
#include "oled.h"
#include "pins.h"

void log(const char* msg, Print& stream = Serial);
void log(const String& msg, Print& stream = Serial);

void exception(const char* msg, Print& stream = Serial);