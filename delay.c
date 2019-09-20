#include "p30f6010A.h"

//general purpose delay routine

void delay(unsigned int count)
{
	unsigned int i;
	unsigned int j;
	for(j=0;j<count;j++)
	{
	for(i=0;i<=26670;i++) ClrWdt();
     }

} //end delay



