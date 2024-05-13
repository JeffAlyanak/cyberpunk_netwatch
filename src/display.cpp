#include "display.h"
#include <Arduino.h>
\
void initDisplay()
{
	pinMode(SEGMENT_A, OUTPUT);
	pinMode(SEGMENT_B, OUTPUT);
	pinMode(SEGMENT_C, OUTPUT);
	pinMode(SEGMENT_D, OUTPUT);
	pinMode(SEGMENT_E, OUTPUT);
	pinMode(SEGMENT_F, OUTPUT);
	pinMode(SEGMENT_G, OUTPUT);

	pinMode(DIGIT_1_PIN, OUTPUT);
	pinMode(DIGIT_2_PIN, OUTPUT);
	pinMode(DIGIT_3_PIN, OUTPUT);
	pinMode(DIGIT_4_PIN, OUTPUT);
}

void clearAll()
{
	digitalWrite(SEGMENT_A, HIGH);
	digitalWrite(SEGMENT_B, HIGH);
	digitalWrite(SEGMENT_C, HIGH);
	digitalWrite(SEGMENT_D, HIGH);
	digitalWrite(SEGMENT_E, HIGH);
	digitalWrite(SEGMENT_F, HIGH);
	digitalWrite(SEGMENT_G, HIGH);
}

void segmentEnable(int segment)
{
	digitalWrite(segment, LOW);
}

void lightNumber(int numberToDisplay)
{

	unsigned int bitmask = 0b00000000;

	switch (numberToDisplay)
	{
		case 0:
			bitmask = 0b11111100;
			break;
		case 1:
			bitmask = 0b01100000;
			break;
		case 2:
			bitmask = 0b11011010;
			break;
		case 3:
			bitmask = 0b11110010;
			break;
		case 4:
			bitmask = 0b01100110;
			break;
		case 5:
			bitmask = 0b10110110;
			break;
		case 6:
			bitmask = 0b10111110;
			break;
		case 7:
			bitmask = 0b11100100;
			break;
		case 8:
			bitmask = 0b11111110;
			break;
		case 9:
			bitmask = 0b11110110;
			break;
		case 'A':
		case 'a':
			bitmask = 0b11101110;
			break;
		case 'B':
		case 'b':
			bitmask = 0b11111110;
			break;
		case 'C':
		case 'c':
			bitmask = 0b10011100;
			break;
		case 'D':
		case 'd':
			bitmask = 0b01111010;
			break;
		case 'E':
		case 'e':
			bitmask = 0b10011110;
			break;
		case 'F':
		case 'f':
			bitmask = 0b10001110;
			break;
		case 'G':
		case 'g':
			bitmask = 0b11110110;
			break;
		case 'H':
		case 'h':
			bitmask = 0b01101110;
			break;
		case 'I':
		case 'i':
			bitmask = 0b00001100;
			break;
		case 'J':
		case 'j':
			bitmask = 0b01110000;
			break;
		case 'K':
		case 'k':
			bitmask = 0b10101110;
			break;
		case 'L':
		case 'l':
			bitmask = 0b00011100;
			break;
		case 'M':
		case 'm':
			bitmask = 0b11010100;
			break;
		case 'N':
		case 'n':
			bitmask = 0b11101100;
			break;
		case 'O':
		case 'o':
			bitmask = 0b11111100;
			break;
		case 'P':
		case 'p':
			bitmask = 0b11001110;
			break;
		case 'Q':
		case 'q':
			bitmask = 0b11100110;
			break;
		case 'R':
		case 'r':
			bitmask = 0b00001010;
			break;
		case 'S':
		case 's':
			bitmask = 0b10110110;
			break;
		case 'T':
		case 't':
			bitmask = 0b00011110;
			break;
		case 'U':
		case 'u':
			bitmask = 0b01111100;
			break;
		case 'V':
		case 'v':
			bitmask = 0b00110000;
			break;
		case 'W':
		case 'w':
			bitmask = 0b01010100;
			break;
		case 'X':
		case 'x':
			bitmask = 0b10010010;
			break;
		case 'Y':
		case 'y':
			bitmask = 0b01110110;
			break;
		case 'Z':
		case 'z':
			bitmask = 0b11011010;
			break;
		case 10:
		case ' ':
			bitmask = 0b00000000;
			break;
	}

	clearAll();
	if (bitmask & (1 << 7)) segmentEnable(SEGMENT_A);
	if (bitmask & (1 << 6)) segmentEnable(SEGMENT_B);
	if (bitmask & (1 << 5)) segmentEnable(SEGMENT_C);
	if (bitmask & (1 << 4)) segmentEnable(SEGMENT_D);
	if (bitmask & (1 << 3)) segmentEnable(SEGMENT_E);
	if (bitmask & (1 << 2)) segmentEnable(SEGMENT_F);
	if (bitmask & (1 << 1)) segmentEnable(SEGMENT_G);
}

void displayNum(int toDisplay)
{
	for(int i = 4 ; i > 0 ; i--)
	{
		switch(i) {
		case 1:
			digitalWrite(DIGIT_1_PIN, HIGH);
			break;
		case 2:
			digitalWrite(DIGIT_2_PIN, HIGH);
			break;
		case 3:
			digitalWrite(DIGIT_3_PIN, HIGH);
			break;
		case 4:
			digitalWrite(DIGIT_4_PIN, HIGH);
			break;
		}

		if(i > 1)
		{
			lightNumber(toDisplay % 10);
		}
		else if(i == 1) //Special case on first digit, don't display 02:11, display 2:11
		{
			if( (toDisplay % 10) != 0) lightNumber(toDisplay % 10);
		}

		toDisplay /= 10;

		delayMicroseconds(DIGIT_ENABLE_DELAY);

		//Turn off all segments
		lightNumber(10);

		//Turn off all digits
		digitalWrite(DIGIT_1_PIN, LOW);
		digitalWrite(DIGIT_2_PIN, LOW);
		digitalWrite(DIGIT_3_PIN, LOW);
		digitalWrite(DIGIT_4_PIN, LOW);
	}

}

//Takes a string like "gren" and displays it, left justified
void displayLetters(char *str)
{
	for(int i = 0 ; i < 4 ; i++)
	{
		//Turn on a digit for a short amount of time
		switch(i)
		{
			case 0:
				digitalWrite(DIGIT_1_PIN, HIGH);
				break;
			case 1:
				digitalWrite(DIGIT_2_PIN, HIGH);
				break;
			case 2:
				digitalWrite(DIGIT_3_PIN, HIGH);
				break;
			case 3:
				digitalWrite(DIGIT_4_PIN, HIGH);
				break;
		}

		//Now display this letter
		lightNumber(str[i]); //Turn on the right segments for this letter

		delayMicroseconds(DIGIT_ENABLE_DELAY);

		//Turn off all segments
		lightNumber(10);
		digitalWrite(DIGIT_1_PIN, LOW);
		digitalWrite(DIGIT_2_PIN, LOW);
		digitalWrite(DIGIT_3_PIN, LOW);
		digitalWrite(DIGIT_4_PIN, LOW);
	}
}