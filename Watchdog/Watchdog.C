void FPPA0 (void)
{
  .ADJUST_IC  SYSCLK=IHRC/16, IHRC=16MHz, VDD=3.3V

  CLKMD.En_ILRC = 1;                 //enable Internal Low speed RC (required for watchdog to run)
                                     // ILRC runs at around 70kHz +/- 10kHz on PMx154 (depending on VDD and ambient temperature)

  $ MISC = WDT_256K;                 //watchdog timeout is 262144 ILRC periods = apx. 3.8 seconds (Slow_WakeUp,LCD_Disabled,LVR_Enabled)
                                     // -> wdreset must be called at least every 3.8 seconds, otherwise reset is triggered
                                     //    NOTE: ICE watchdog timeouts are completely different, check datasheet

  wdreset;                           //reset watchdog
  CLKMD.En_WatchDog = 1;             //enable watchdog


  PAC.3 = 1;                         //PA.3 as output
  PA.3 = 1;                          //PA.3 high (LED on)

  .delay 1000000;                    //wait 1 second

  wdreset;                           //reset watchdog
  PA.3 = 0;                          //PA.3 low (LED off)

  while(1) {}                        //endless loop which does not 'feeds' the watch 'dog' 
                                     // -> reset will be triggered (cpu will restart, LED will go on again for 1 second and so on...)
                                     //    without watchdog beeing triggered the LED would stay off for evers
}


