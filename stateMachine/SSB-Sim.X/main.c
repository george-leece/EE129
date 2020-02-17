/* 
 * File:   main.c
 * Author: gleece
 *
 * Created on January 24, 2020, 12:37 PM
 */
#define _SUPPRESS_PLIB_WARNING 1

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include "plib.h"
#include <BOARD.h>
#include "TopSM.h"
#include "wmSM.h"
#include "lmrSM.h"
#include "configure.h"

SSB ssb = {};
static int TIMER_TICK = 0; // used to flag the global timer

int main()
{
    BOARD_Init();
    OledInit();
    AdcInit();
    ButtonsInit();


    // initialize timers and timer ISRs:
    // <editor-fold defaultstate="collapsed" desc="TIMER SETUP">

    // Configure Timer 2 using PBCLK as input. We configure it using a 1:16 prescalar, so each timer
    // tick is actually at F_PB / 16 Hz, so setting PR2 to F_PB / 16 / 100 yields a .01s timer.
    OpenTimer2(T2_ON | T2_SOURCE_INT | T2_PS_1_16, BOARD_GetPBClock() / 16 / 100);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T2);
    INTSetVectorPriority(INT_TIMER_2_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_2_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T2, INT_ENABLED);

    // Configure Timer 3 using PBCLK as input. We configure it using a 1:256 prescalar, so each timer
    // tick is actually at F_PB / 256 Hz, so setting PR3 to F_PB / 256 / 5 yields a .2s timer.
    OpenTimer3(T3_ON | T3_SOURCE_INT | T3_PS_1_256, BOARD_GetPBClock() / 256 / 5);

    // Set up the timer interrupt with a medium priority of 4.
    INTClearFlag(INT_T3);
    INTSetVectorPriority(INT_TIMER_3_VECTOR, INT_PRIORITY_LEVEL_4);
    INTSetVectorSubPriority(INT_TIMER_3_VECTOR, INT_SUB_PRIORITY_LEVEL_0);
    INTEnable(INT_T3, INT_ENABLED);

    // </editor-fold>
    printf("Welcome to Smart Slug Bin. Compiled on %s %s\n", __DATE__, __TIME__);
    sprintf(display, "Top Level\nState: IDLE\n");
    OledClear(OLED_COLOR_BLACK); //Removes any lingering characters
    OledDrawString(display);
    OledUpdate(); // Prints the new OLED display
    ssb.topState = TOPINIT;
    while (1) {
        if (ssb.button || TIMER_TICK) {
            ssb = runTopSM(ssb);
            TIMER_TICK = FALSE;
        }
    }
}

/*The 5hz timer is used to update the free-running timer and to generate TIMER_TICK events*/
void __ISR(_TIMER_3_VECTOR, ipl4auto) TimerInterrupt5Hz(void)
{

    // Clear the interrupt flag.
    IFS0CLR = 1 << 12;
    // The global clock used for measuring time and is incremented every .2 seconds
    ssb.globalTime++;
    TIMER_TICK = TRUE;


}

/*The 100hz timer is used to check for button and ADC events*/
void __ISR(_TIMER_2_VECTOR, ipl4auto) TimerInterrupt100Hz(void)
{

    // Clear the interrupt flag.
    IFS0CLR = 1 << 8;
    // ISR to check if a button was pressed or the adc potentiometer changed and checks every .01 seconds
    ssb.button = ButtonsCheckEvents();

    //    ssb.lock = 
    //    ADC_event = AdcChanged();

}
