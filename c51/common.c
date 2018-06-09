#include"common.h"
void delay_ms(unsigned int ms) //delay 1ms;
{
	unsigned int a,b;
	for ( a=ms ; a>0 ; a-- )
		for ( b=114; b>0; b-- );
}
void delay_10us(unsigned int us) //delay 10us;
{
	while(us--);
}
