#include <avr/io.h>
#include <util/delay.h>

/*
 ATtiny13A 8PDIP
/RST | 1    8 | VCC
 PB3 | 2    7 | PB2 LED
 PB4 | 3    6 | PB1
 GND | 4    5 | PB0
*/

// LED PB2 LED
#define LED      ( 1<<PB2 )
#define LED_INIT DDRB  |=  LED
#define LED_H    PORTB |=  LED
#define LED_L    PORTB &=~ LED
#define LED_I    PORTB ^=  LED

// LED PB4 Mouse Click左
#define CLKL      ( 1<<PB4 )
#define CLKL_INIT DDRB  |=  CLKL 
#define CLKL_H    PORTB |=  CLKL 
#define CLKL_L    PORTB &=~ CLKL 
#define CLKL_I    PORTB ^=  CLKL 

// SW PB1 傾斜スイッチ
// ActiveLow, PullUp
#define SW      ( 1<<PB1 )
#define SW_INIT { DDRB  &=~ SW; PORTB |= SW; }
#define SW_ON   ! ( PINB & SW )

int main(void) {
	LED_INIT;
	SW_INIT;
	CLKL_INIT;
	CLKL_L;

	uint16_t ct1  = 0;
	uint8_t  ct2  = 0;
	uint8_t  prev = 0;

	LED_L;
	for(uint8_t i=0;i<10;i++) {
		LED_I;
		_delay_ms(50);
	}
	LED_H;
	_delay_ms(1000);
	LED_L;

    for(;;) {
		switch(SW_ON) {
			case 1:
				switch(prev) {
				case 1:
					if(ct2 > 50) {
						CLKL_H;
						LED_H;
					} else {
						ct2++;
					}
					break;
				case 0:
					prev = 1;
					ct2 = 0;
					break;
				}
				break;
			case 0:
				switch(prev) {
				case 0:
					if(ct2 > 50) {
						CLKL_L;
						LED_L;
					} else {
						ct2++;
					}
					break;
				case 1:
					prev = 0;
					ct2 = 0;
					break;
				}
				break;
		}


		if((ct1 > 5000) && (ct1 < 5050)) {
			LED_H;
		} if (ct1 >= 5050) {
			LED_L;
			ct1=0;
		}
		_delay_ms(1);
		ct1++;

    }
    return 0;
}
