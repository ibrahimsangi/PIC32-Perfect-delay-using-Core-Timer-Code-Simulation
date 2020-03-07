/* 
 * File:   main.c
 * Author: IBRAHIM LABS
 *
 * Website: http://ibrahimlabs.blogspot.com/
 *
 * Created on September 7, 2013, 8:42 AM
 */

#include <xc.h>
#include <plib.h>

#ifndef OVERRIDE_CONFIG_BITS

    
    #pragma config FPLLIDIV = DIV_2         // PLL Input Divider
    #pragma config FPLLODIV = DIV_1         // PLL Output Divider
    #pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
    #pragma config FWDTEN   = OFF           // Watchdog Timer
    #pragma config POSCMOD  = HS            // Primary Oscillator
    #pragma config FNOSC    = PRIPLL        // Oscillator Selection
    #pragma config FPLLMUL  = MUL_20        // PLL Multipler

    /*  System clock = 80MHz (8MHz Crystal/ FPLLIDIV * FPLLMUL / FPLLODIV) */

#endif // OVERRIDE_CONFIG_BITS

/*
 *
 */
// PreProcessors
#define SYS_FREQ            (80000000L)                     // 80MHz system clock
#define TOGGLES_PER_SEC     2                               // No. of Toggles per second
#define CORE_TICK_RATE	    (SYS_FREQ/2/TOGGLES_PER_SEC)

int main() {

    /*  Configure device for max performance
     *  Warning!!!
     *          Do not change PBDIV configuration bit.
     *
     * This function changes ROM, RAM wait states and enable Prefetch
     * cache
     */
    
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    /*  Configure Core Timer for RATE defined above 10 times a second */
    OpenCoreTimer(CORE_TICK_RATE);

    /*  Making RB0 and RB4 as digital output
     *  Same as "TRISBbits.TRISB0 = TRISBbits.TRISB4 = 0;"
     */
    mPORTBSetPinsDigitalOut(BIT_0 | BIT_4);
    /*  just like "LATBbits.LATB0 = LATBbits.LATB4 = 1;"    */
    mPORTBSetBits(BIT_0 | BIT_4);


    while(1)
    {
        while (!mCTGetIntFlag());           // wait for flag to set
        mCTClearIntFlag();                  // clear CoreTimer flag
        UpdateCoreTimer(CORE_TICK_RATE);    // updating core timer.
        mPORTBToggleBits(BIT_0 | BIT_4);    // Toggling RB0 and RB4
    }

    return 0;
}

