// "Hello, world!" for the Padauk PFS154, ported to PADAUK mini-C.
// Written by Philipp Klaus Krause in 2019.
// Ported by free-pdk
// Source code under CC0 1.0.

// Output on PA7 at 1200 baud.


BYTE sendcounter;
WORD senddata;

//void send_bit(void) __interrupt(0)
void Interrupt(void)
{
  pushaf;                   // save registers
  if(intrq.TM2)             // only handle TM2 interrupts
  {
    intrq.TM2 = 0;
    if(sendcounter)         // only send bits if required
	{
      A = (senddata$1) & 2; // test bit 9 of senddata (senddata$0:senddata$1 = 8,7,6,5,4,3,2,1 : 16,15,14,13,12,10,9 => 0x00:0x02 => 0x200)
      if( A )
        PA.7 = 1;           // set PA.7 accordingly
      else
        PA.7 = 0;

      senddata <<= 1;       // shift left senddata to get next bit in position
  
      if(!--sendcounter)    // all bits sent?
        inten.TM2 = 0;      // disable TM2
	}
  }
  popaf;                    // restore registers
}

//int putchar(int c)
#define putchar(c) {A=c;_putchar();}
void _putchar(void)
{
  //while(sending){};
  while(inten.TM2){};

  senddata = (A << 1) | 1;  //construct 1 start bit (0), 8 data bits, 1 stop bit (1)
  sendcounter = 10;
  tm2ct = 0;
  inten.TM2 = 1;
}

//unsigned char _sdcc_external_startup(void)
void _sdcc_external_startup(void)
{
  clkmd = 0x34; // Use IHRC / 2 = 8 Mhz for system clock, enable IHRC, disable ILRC, disable watchdog
}

void main(void)
{
  tm2c = 0x10;   // Use SYSCLK (8 Mhz), Period
  tm2s = 0x60;   // Prescaler 64 , Scaler 1, timer always divedes by 2 ~> 62.5 kHz
  tm2b = 60;     // Divide by 52 ~> 1201 Hz //60 by oscilloscope on PFS154, 118 by oscilloscope on ICE
  inten = 0x00;  // disable all interrupts

  engint;        // global interrupt enable
    
  pac = 0x80;    // enable PA.7 as output

  putchar(0x55); // send initial char for autobaud
  
  while(1)
  {
    putchar('H');
    putchar('e');
    putchar('l');
    putchar('l');
    putchar('o');
    putchar(' ');
    putchar('W');
    putchar('o');
    putchar('r');
    putchar('l');
    putchar('d');
    putchar('!');
    putchar(0x0A);
	.delay 24000000 // Wait approx. 3s.
  }
}

void FPPA0 (void)
{
  .ADJUST_IC DISABLE;
  _sdcc_external_startup();
  main();
  while(1){}
}
