#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <avr/sleep.h>
#include <avr/power.h>

#include "display.h"
#include "cyber_watch.h"

#define SHOW_TIME 2000
#define SHOW_STRING 80

#define BUTTON_TIMEOUT 20
#define DOUBLE_TAP_TIMEOUT 100
#define SET_TIME_FAST_ADVANCE 500

#define SET_TIME_FLASH_SPEED 60

#define DISPLAY_REFRESH_MS 1500

void setup();
void loop();
void setupPins();
void setupInterrupts();
void powerOptimizations();

void displayIntro();

void updateTime();
int combineTime(int hours, int minutes);
unsigned long elapsedTime(unsigned int time);
void showTime();
void setClock();
void setTimer();

void showString(char *str);

#endif /* MAIN_H */
