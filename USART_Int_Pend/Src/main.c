
#include <stdint.h>
int main(void)
{
	//Setting The pending Register using Interrupt Set Pending Register
	uint32_t *pISPR1 = (uint32_t*) 0XE000E204;

    /* Loop forever */
	for(;;);
}
