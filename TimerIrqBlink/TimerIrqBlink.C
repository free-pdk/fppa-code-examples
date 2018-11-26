void FPPA0 (void)
{
	.ADJUST_IC	SYSCLK=IHRC/16		 // SYSCLK=IHRC/16 = 1MHz

	PAC.3 = 1;                       // set PA.3 as output

	$ T16M SYSCLK, /64, BIT12;       // timer base = SYSCLK/64 = 15625 Hz
                                     // generate INTRQ.T16 when overflow to bit 12 (4096)
	                                 // first interrupt triggered after 4096 cycles (13 bit 0 change to 1)
	                                 // next interrupt after next 8192 cycles (13 bit 1 needs to change to 0 first and then back to 1)

	WORD tmp = 0;                    // store 0 to timer value (first run)
    stt16 tmp;
	
	$ INTEN T16;                     // enable T16 interrupt

	INTRQ = 0;                       // clear any pending interrupt requests
	ENGINT;                          // enable global interrupts

	while (1) { }                    // endless loop (empty)
}


void Interrupt(void)
{
	PUSHAF;                          // save A and FLAGs

	if( INTRQ.T16 )                  // interrupt source = timer T16? 
	{
		PA ^= (1<<3);                // toggle bit of PA.3 output
		INTRQ.T16 = 0;               // clear interrupt request for T16
	}

	POPAF;                           // restore A and FLAGs
}

