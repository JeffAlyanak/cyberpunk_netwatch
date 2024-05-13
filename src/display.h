#include <Arduino.h>
#include "cyber_watch.h"

#ifndef DISPLAY_H
#define DISPLAY_H

#define DIGIT_ENABLE_DELAY 250 // µs delay between enabling each digit

void initDisplay();
void clearAll();
void segmentEnable(int segment);
void lightNumber(int numberToDisplay);
void displayNum(int toDisplay);
void displayLetters(char *str);

#endif // DISPLAY_H
