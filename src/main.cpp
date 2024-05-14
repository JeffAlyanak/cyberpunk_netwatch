#include "main.h"

int show_the_time = false;
int set_the_time = false;

long seconds = 1;
int minutes = 0;
int hours = 12;

void setup()
{
	powerOptimizations();
	setupPins();
	setupInterrupts();

	initDisplay();
	displayIntro();

	sei(); //Enable global interrupts
}

void loop()
{
	sleep_mode(); //wake on INT0 (button press) or TIMER2 (8 seconds)

	if(show_the_time == true)
	{
		if(set_the_time == true) setTime();
		while(digitalRead(TOP_BUTTON_PIN) == LOW);
		digitalWrite(LED_PIN, HIGH);
		showTime();
		digitalWrite(LED_PIN, LOW);

		show_the_time = false;
	}
}

void displayIntro()
{
	digitalWrite(LED_PIN, HIGH);
	showString(const_cast<char *>("   A"));
	showString(const_cast<char *>("  AR"));
	showString(const_cast<char *>(" ARA"));
	showString(const_cast<char *>("ARAS"));
	showString(const_cast<char *>("RASA"));
	showString(const_cast<char *>("ASAK"));
	showString(const_cast<char *>("SAKA"));
	showString(const_cast<char *>("AKA "));
	showString(const_cast<char *>("KA  "));
	showString(const_cast<char *>("A   "));
	showString(const_cast<char *>("    "));
	showTime();
	digitalWrite(LED_PIN, LOW);
}

void setupPins()
{
	pinMode(TOP_BUTTON_PIN, INPUT);
	pinMode(BOTTOM_BUTTON_PIN, INPUT);
	digitalWrite(TOP_BUTTON_PIN, HIGH);    // pull down
	digitalWrite(BOTTOM_BUTTON_PIN, HIGH); // pull down
	pinMode(LED_PIN, OUTPUT);
}

void setupInterrupts()
{
	//Setup TIMER2
	TCCR2A = 0x00;
	TCCR2B = (1<<CS22)|(1<<CS21)|(1<<CS20); //Set CLK/1024 or overflow interrupt every 8s
	ASSR = (1<<AS2); //Enable asynchronous operation
	TIMSK2 = (1<<TOIE2); //Enable the timer 2 interrupt

	//Setup external INT0 interrupt
	EICRA = (1<<ISC01); //Interrupt on falling edge
	EIMSK = (1<<INT0); //Enable INT0 interrupt
	PCMSK1 |= B00000100;
	PCICR |= B00000010;
}

void powerOptimizations()
{
	//To reduce power, setup all pins as inputs with no pullups
	for(int x = 1 ; x < 18 ; x++)
	{
		pinMode(x, INPUT);
		digitalWrite(x, LOW);
	}

	//Power down various bits of hardware to lower power usage  
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	sleep_enable();
	ADCSRA &= ~(1<<ADEN); //Disable ADC
	ACSR = (1<<ACD); //Disable the analog comparator
	DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
	DIDR1 = (1<<AIN1D)|(1<<AIN0D); //Disable digital input buffer on AIN1/0

	power_twi_disable();
	power_spi_disable();
	power_timer1_disable();
}

// Timer interrupt
SIGNAL(TIMER2_OVF_vect)
{
	seconds += 8;
	updateTime();
}

// Top button interrupt
SIGNAL(INT0_vect)
{
	if (digitalRead(TOP_BUTTON_PIN) == LOW)
	{
		show_the_time = true;
	}
	else
	{
		show_the_time = false;
	}
}
// Bottom button interrupt
ISR(PCINT1_vect) 
{
	if (digitalRead(BOTTOM_BUTTON_PIN) == LOW)
	{
		set_the_time = true;
	}
	else
	{
		set_the_time = false;
	}
}

void updateTime()
{
	minutes += seconds / 60;
	seconds %= 60;
	hours += minutes / 60;
	minutes %= 60;
	while(hours > 23) hours -= 24;
}

int combineTime(int hours, int minutes)
{
	return (hours * 100) + minutes;
}

unsigned long elapsedTime(unsigned int time)
{
	return millis() - time;
}

void showTime()
{
	boolean buttonPreviouslyHit = false;
	
	unsigned long startTime = millis();
	while( (millis() - startTime) < SHOW_TIME)
	{
		displayNum(combineTime(hours, minutes));

		if(digitalRead(TOP_BUTTON_PIN) == LOW)
		{
			buttonPreviouslyHit = true;
		}
		else if( (buttonPreviouslyHit == true) && (digitalRead(TOP_BUTTON_PIN) == HIGH) )
		{
			return;
		}
	}
}

void setTime()
{
	unsigned long buttonHoldDuration    = millis();
	unsigned long flashDuration    	    = buttonHoldDuration;
	unsigned long buttonTimeout         = BUTTON_TIMEOUT;

	// will continue as long as bottom button is held
	while(set_the_time)
	{
		// flash digits and LED while in set time mode
		if (elapsedTime(flashDuration) < SET_TIME_FLASH_SPEED)
		{
			displayNum(combineTime(hours, minutes));
			digitalWrite(LED_PIN, HIGH);
		}
		else if (elapsedTime(flashDuration) < (SET_TIME_FLASH_SPEED + SET_TIME_FLASH_SPEED/3 ))
		{
			displayLetters(const_cast<char *>("    "));
			digitalWrite(LED_PIN, LOW);
		} else {
			flashDuration = millis();
		}

		if(digitalRead(TOP_BUTTON_PIN) == LOW)
		{
			// BUTTON_TIMEOUT determines how fast another input is registered when holding button down
			if (elapsedTime(buttonTimeout) > BUTTON_TIMEOUT)
			{
				// if the button is held down longer than SET_TIME_FAST_ADVANCE, we switch to adding 10-minute increments
				if( elapsedTime(buttonHoldDuration) < SET_TIME_FAST_ADVANCE)
				{
					minutes++;
				}
				else
				{
					minutes /= 10;
					minutes *= 10;
					minutes += 10;
				}
				buttonTimeout = millis();
			}
		}
		else
		{
			buttonHoldDuration = millis();
		}

		// disable interrupts temporarily while we update the time 
		cli();
		updateTime();
		sei();
	}
	show_the_time = true;
}

void showString(char *str)
{
	long startTime = millis();
	while( (millis() - startTime) < SHOW_STRING)
	{
		displayLetters(str);
	}
}

