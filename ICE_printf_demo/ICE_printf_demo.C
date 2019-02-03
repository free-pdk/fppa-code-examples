void FPPA0(void)
{
    .ADJUST_IC SYSCLK=IHRC/2    // SYSCLK=IHRC/2

    .printf("Hello World\n");

	BYTE loop=10;
	while( loop>0 )
	{
	  .printf("loop# %d", A);
	  loop--;
	}

    while (1) { }
}
