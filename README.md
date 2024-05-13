# Overview

Basic watch firmware for the `cyberpunk_netwatch`, see original repo for [schematics](https://github.com/bennettwarner/cyberpunk_netwatch/tree/main/hardware) and more.

## Usage

The top botton will toggle the display on and off, the display will also timeout after a few seconds (this can be set with SHOW_TIME definition).

Holding the bottom button and pressing or holding the top button will set the time. Holding the top button for a couple of seconds will start it increasing by 10-minute increments.

## Power optimizations

Some quick and dirty notes on power optimizations, these are mostly copied over from [Bennett Warner's original repo](https://github.com/bennettwarner/cyberpunk_netwatch):

Just like the original firmware, we're disabling a number of unused features with board fuses:
```
board_fuses.lfuse = 0xE2
board_fuses.hfuse = 0xDA
board_fuses.efuse = 0xFF
```

And shutting off other stuff manually during init:
```
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

	//Shut off ADC, TWI, SPI, Timer0, Timer1

	ADCSRA &= ~(1<<ADEN); //Disable ADC
	ACSR = (1<<ACD); //Disable the analog comparator
	DIDR0 = 0x3F; //Disable digital input buffers on all ADC0-ADC5 pins
	DIDR1 = (1<<AIN1D)|(1<<AIN0D); //Disable digital input buffer on AIN1/0

	power_twi_disable();
	power_spi_disable();
	power_timer1_disable();
}

And sleeping for `8` seconds instead of just `1` in the `SIGNAL(TIMER2_OVF_vect)` interrupt.
```