void DeepSleep_2s(void)
{
  WORD sleep = 0;
  STT16 sleep

  INTRQ.T16 = 0;

  $ T16M = ILRC, /64, BIT11;                         // T16 clock source = ILRC (typ. 62 kHz on PMC150), / 64 => approx 1 kHz timer, 11BIT = 2048 => approx 2 seconds

  while( !INTRQ.T16 )                                // STOPEXE could be interrupted by multiple sources, we wait for T16 overflow here
    STOPEXE;

  $ T16M = STOP;
}

void FPPA0(void) : stack=2
{
  .ADJUST_IC  SYSCLK=ILRC (IHRC/16), IHRC=16MHz, VDD=3V, Bandgap=Off

#if _SYS (AT_ICE)
  $ CLKMD = ILRC/4, En_ILRC;                         // ICE does not support ILRC/16, so we use /4 instead
#else
  $ CLKMD = ILRC/16, En_ILRC;                        // use ILRC/16 as system clock (3.875kHz), turn off IHRC to save more power
#endif

  $ TM2S = 8BIT, /1, /1;                             // 8-bit PWM, pre-scalar = 1, scalar = 1 (tm2s = 0b0_00_00000)

  while (1) 
  {
    TM2B = 0;
    $ TM2C = ILRC, PA3, PWM;                         // setup timer2 as pwm out: clock ilrc (62kHz), output=PA3, PWM mode
    BYTE i = 1;
    while( i<64 )
    {
      TM2B = i++;
      .delay 50;
#if _SYS (AT_ICE)
      .delay 150;                                    // wait extra time since ICE clock is faster
#endif
    }
    $ TM2C = STOP;
    DeepSleep_2s();
  }
}

