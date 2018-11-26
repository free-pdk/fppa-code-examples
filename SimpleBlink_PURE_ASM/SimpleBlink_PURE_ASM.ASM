.CHIP	PMS154C
//{{PADAUK_CODE_OPTION
	.Code_Option	Comparator_Edge	All_Edge
	.Code_Option	LCD2		Disable		// At ICE, LCD always disable, PB0 PA0/3/4 are independent pins
	.Code_Option	Bootup_Time	Slow
	.Code_Option	Drive		Normal
	.Code_Option	LVR		2.5V
	.Code_Option	Security	Disable		// Security Disable
//}}PADAUK_CODE_OPTION

	goto	FPPA0

FPPA0:
	.ADJUST_IC	SYSCLK=IHRC/16	//	insert calibration code to calibrate IHRC to 16 MHz, set SYSCLK=IHRC/16 (=1MHz) 

	BYTE XSLP_COUNTER[2];           //  RAM for counter variable

	SET1 PAC.3                      //  set PA.3 as output

BLINK_LOOP:
	MOV A, 1<<3                     //  PA.3 bit value
	XOR PA, A                       //  toggle PA.3 bit

	MOV A,0x01                      //  delay 500000 => (-2c call, -5c init, -2c ret)/6c loop = 83331 = 0x014583 => store this value in TEMP_COUNTER
	MOV XSLP_COUNTER[1],A
	MOV A,0x45
	MOV XSLP_COUNTER[0],A
	MOV A,0x83

WAIT_LOOP:                          	//  loop and
	SUB  A,1                        //  decrement counter
	SUBC XSLP_COUNTER[0]            //  until counter 
	SUBC XSLP_COUNTER[1]            //  reaches zero
	T1SN CF                         //  then stop
	GOTO WAIT_LOOP                  //  looping

	GOTO BLINK_LOOP                 //  jump to loop start

