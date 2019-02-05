// "Hello, world!" for the Padauk PFS154, ported to PADAUK mini-C.
// Written by Philipp Klaus Krause in 2019.
// Ported and adapted by free-pdk
// Source code under CC0 1.0.

// Output on PA7 at 9600 baud.


BYTE sendcounter;
WORD senddata;

//void send_bit(void) __interrupt(0)
void Interrupt(void)
{
  pushaf;                   // save registers
  if(intrq.TM2)             // only handle TM2 interrupts
  {
	SR senddata$1           // shift right upper part of senddata
	SRC senddata$0          // shift right with carry lower part of senddata 
	SWAPC PA.7              // use carry from shit to set bit on IO
	if( !--sendcounter )    // all bits sent?
	  inten.TM2 = 0;        // disable TM2 interrupt

    intrq.TM2 = 0;          // mark this interrupt event handled
  }
  popaf;                    // restore registers
}

//int putchar(int c)
#define putchar(c) {A=c;_putchar();}
void _putchar(void)
{
  //while(sending){};
  while(inten.TM2){};

  senddata = (A << 1) | 0x200;  // construct 1 start bit (0), 8 data bits, 1 stop bit (1)
  sendcounter = 10;
  tm2ct = 0;                    // reset TM2 counter
  inten = 0x40;                 // enable TM2 interrupt
}

//unsigned char _sdcc_external_startup(void)
void _sdcc_external_startup(void)
{
  clkmd = 0x34; // Use IHRC / 2 = 8 Mhz for system clock, enable IHRC, disable ILRC, disable watchdog
}

void main(void)
{
  pac = 0x80;      // enable PA.7 as output for serial
  tm2c = 0x20;     // enable TM2 (IHRC 16 Mhz, Period)
  tm2s = 0x20;     // pre scaler 4, scaler 1, timer always divides by 2 > 2MHz 
  tm2b = 208;      // divide by 208 > 9615 Hz
  inten = 0;       // no interrupt selected
  engint;          // global interrupt enable
    
  putchar(0x55);   // send initial char for autobaud
  .delay 8000      // wait 1 ms for autobaud to complete
  
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
