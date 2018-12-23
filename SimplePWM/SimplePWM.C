void FPPA0(void)
{
    .ADJUST_IC SYSCLK=IHRC/2    // SYSCLK=IHRC/2

    // Y = tm2c[7:4] : frequency of selected clock source 
    // K = tm2b[7:0] : bound register in decimal
    // S1= tm2s[6:5] : pre-scalar (1, 4, 16, 64)
    // S2= tm2s[4:0] : scalar register in decimal (1 ~ 31)

    // 8 bit PWM mode: 
    //   Frequency of Output = Y ÷ [256 × S1 × (S2+1) ]  (8 bit PWM mode)
    //   Duty of Output = [( K + 1 ) ÷ 256] × 100%
    //
    $ TM2S = 8BIT, /1, /1;      // 8-bit PWM, pre-scalar = 1, scalar = 1 (tm2s = 0b0_00_00000)
                                // -> frequency of output = 16MHz ÷ ( 256 × 1 × 1 ) = 62.5 kHz (fastest 8 bit PWM frequency possible)
    TM2B = 127;                 // -> duty of output = [(127+1) ÷ 256] × 100% = 50%


    // 6 bit PWM mode: 
    //   Frequency of Output = Y ÷ [64 × S1 × (S2+1) ]  (6 bit PWM mode)
    //   Duty of Output = [( K + 1 ) ÷ 64] × 100%
    //
//    $ TM2S = 6BIT, /1, /1;      // 6-bit PWM, pre-scalar = 1, scalar = 1 (tm2s = 0b1_00_00000)
//                                // -> frequency of output = 16MHz ÷ ( 64 × 1 × 1 ) = 250 kHz (fastest 6 bit PWM frequency possible)
//    TM2B = 31;                  // -> duty of output = [(31+1) ÷ 64] × 100% = 50%


    TM2CT = 0;                  // reset counter for first period (not required, 0 is reset value)

    $ TM2C = IHRC, PA3, PWM;    // setup timer2 as pwm out: system clock (8MHz), output=PA3, PWM mode (tm2c = 0b0001_10_1_0)

    while(1) {}                 // endless loop, PWM is created internally
}
