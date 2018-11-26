//Simple blink program

void FPPA0 (void)
{
	.ADJUST_IC	SYSCLK=IHRC/16		//	insert calibration code to calibrate IHRC to 16 MHz, set SYSCLK=IHRC/16 (=1MHz) 

	PAC.3 = 1;                      //  set PA.3 as output

	while( 1 ) 
	{
		PA.3 = 1;                   //  set PA.3 high 
		.delay 500000               //  delay 500000 cycles (macro inserts NOP loop)


		PA.3 = 0;                   //  set PA.3 low
		.delay 500000               //  delay 500000 cycles (macro inserts NOP loop)
	}
}

