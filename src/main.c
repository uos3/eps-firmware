#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	/* stop watchdog timer */

	/*initialise hardware/functions */

	/* need to contain main loop which will run the msp430 in sleep mode and
	 * then exit in case of any of the interrupts and carry out the required process*/
	while(true){
	    switch(SLEEP MODE){
	    case UART Comms:
	        TOBC Comms Process;
	        break;
	    case Timer:
	        Timer:
	        Timer Interrupt Process;
	        break;
	    case OCP Event:
	        OCP Notification Process;
	        break;
	    case TOBC Interrupt:
	        TOBC Interrupt Process;
	        break;
	    }
	}
	
	return 0;
}
