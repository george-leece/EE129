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
//#include "TopSM.h"
#include "configure.h"


//typedef enum {
//    IDLE,
//    WAIT,
//    ACTIVE
//} topState;

//typedef enum {
//    WMINIT,
//    CLASSIFY,
//    TRASH,
//    RECYCLE,
//    CONTAMINATION
//} wmState;
//
//typedef enum {
//    LMRINIT,
//    FORWARD,
//    REVERSE,
//    DROP
//} lmrState;

//typedef struct {
//    topState topState;
//    wmState wmState;
//    lmrState lmrState;
//    uint32_t globalTime;
//    uint32_t lmrtime;
//    uint32_t buttonpress;
//    uint8_t button;
//    uint8_t event;
//    uint8_t item;
//    uint8_t contamination;
//    uint8_t lock;
//    uint32_t waitTimeStart;
//    //    uint32_t
//} SSB;

static SSB ssb = {};

//int ADC_event = 0; // to flag a change in the ADC
static int TIMER_TICK = 0; // used to flag the global timer
char display[100]; // used to print on the OLED

void updateOLEDTop(SSB ssb)
{
    switch (ssb.topState) {
    case IDLE:
        sprintf(display, "Top Level\nState: IDLE");
        OledClear(OLED_COLOR_BLACK); // clears the OLED to remove any lingering characters from previous prints
        OledDrawString(display);
        OledUpdate(); // prints the new string
        break;
    case WAIT: //
        sprintf(display, "Top Level\nState: WAITING");
        OledClear(OLED_COLOR_BLACK); // Removes any lingering characters
        OledDrawString(display);
        OledUpdate(); // Prints the new OLED display
        break;
    case ACTIVE: // 
        sprintf(display, "Top Level\nState: ACTIVE");
        OledClear(OLED_COLOR_BLACK); //Removes any lingering characters
        OledDrawString(display);
        OledUpdate(); // Prints the new OLED display
        break;
    }
}

void updateOLEDWM(SSB ssb)
{
    switch (ssb.wmState) {
    case CLASSIFY:
        sprintf(display, "WM Level\nState: CLASSIFY\n");
        OledClear(OLED_COLOR_BLACK); // clears the OLED to remove any lingering characters from previous prints
        OledDrawString(display);
        OledUpdate(); // prints the new string
        break;
    case TRASH: //
        sprintf(display, "WM Level\nState: TRASH\nItem: WASTE");
        OledClear(OLED_COLOR_BLACK); // Removes any lingering characters
        OledDrawString(display);
        OledUpdate(); // Prints the new OLED display
        break;
    case CONTAMINATION:
        sprintf(display, "WM Level\nState: CONTAMINATIONItem: %d", ssb.item);
        OledClear(OLED_COLOR_BLACK); // Removes any lingering characters
        OledDrawString(display);
        OledUpdate(); // Prints the new OLED display
        break;
    case RECYCLE: // 
        sprintf(display, "WM Level\nState: RECYCLE\nItem %d, %d", ssb.item, ssb.contamination);
        OledClear(OLED_COLOR_BLACK); //Removes any lingering characters
        OledDrawString(display);
        OledUpdate(); // Prints the new OLED display
        break;
    }
}

void updateOLEDLMR(SSB ssb)
{
    switch (ssb.lmrState) {
    case FORWARD:
        sprintf(display, "LMR Level\nState: FORWARD\n");
        OledClear(OLED_COLOR_BLACK); // clears the OLED to remove any lingering characters from previous prints
        OledDrawString(display);
        OledUpdate(); // prints the new string
        break;
    case DROP:
        sprintf(display, "LMR Level\nState: DROP");
        OledClear(OLED_COLOR_BLACK); // Removes any lingering characters
        OledDrawString(display);
        OledUpdate(); // Prints the new OLED display
        break;
    case REVERSE: //
        sprintf(display, "LMR Level\nState: REVERSE");
        OledClear(OLED_COLOR_BLACK); // Removes any lingering characters
        OledDrawString(display);
        OledUpdate(); // Prints the new OLED display
        break;
    }
}

void runlmrSM(void)
{
    switch (ssb.lmrState) {
    case LMRINIT:
        ssb.lmrState = FORWARD;
        ssb.lmrtime = ssb.globalTime;
        updateOLEDLMR(ssb);
        break;
    case FORWARD:
        if (ssb.item == ALUMINUM && (ssb.globalTime - ssb.lmrtime > BIN_1)) {
            ssb.lmrState = DROP;
            ssb.lmrtime = ssb.globalTime;
            updateOLEDLMR(ssb);
        } else if (ssb.item == GLASS && (ssb.globalTime - ssb.lmrtime > BIN_2)) {
            ssb.lmrState = DROP;
            ssb.lmrtime = ssb.globalTime;
            updateOLEDLMR(ssb);
        } else if (ssb.item == PLASTIC && (ssb.globalTime - ssb.lmrtime > BIN_3)) {
            ssb.lmrState = DROP;
            ssb.lmrtime = ssb.globalTime;
            updateOLEDLMR(ssb);
        }
        break;
    case DROP:
        if (ssb.globalTime - ssb.lmrtime > DROP_TIMEOUT) {
            ssb.lmrState = REVERSE;
            updateOLEDLMR(ssb);
        }
        break;
    case REVERSE:
        if (ssb.item == ALUMINUM && (ssb.globalTime - ssb.lmrtime > BIN_1)) {
            ssb.lmrState = LMRINIT;
            ssb.wmState = WMINIT;
            ssb.event = NO_ITEM;
            updateOLEDLMR(ssb);
        } else if (ssb.item == GLASS && (ssb.globalTime - ssb.lmrtime > BIN_2)) {
            ssb.lmrState = LMRINIT;
            ssb.wmState = WMINIT;
            ssb.event = NO_ITEM;
            updateOLEDLMR(ssb);
        } else if (ssb.item == PLASTIC && (ssb.globalTime - ssb.lmrtime > BIN_3)) {
            ssb.lmrState = LMRINIT;
            ssb.wmState = WMINIT;
            ssb.event = NO_ITEM;
            updateOLEDLMR(ssb);
        }
        break;
    }
}

void runwmSm(void)
{
    switch (ssb.wmState) {
    case WMINIT:
        ssb.wmState = CLASSIFY;
        updateOLEDWM(ssb);
        break;
    case CLASSIFY:
        if (ssb.button == BUTTON_EVENT_4DOWN) {
            ssb.wmtime = ssb.globalTime;
            ssb.item = TRASH;
            ssb.wmState = TRASH;
            ssb.button = FALSE;
            updateOLEDWM(ssb);
        } else if (ssb.button == BUTTON_EVENT_3DOWN) {
            ssb.item = ALUMINUM;
            ssb.wmState = CONTAMINATION;
            ssb.button = FALSE;
            updateOLEDWM(ssb);
        } else if (ssb.button == BUTTON_EVENT_2DOWN) {
            ssb.item = GLASS;
            ssb.wmState = CONTAMINATION;
            ssb.button = FALSE;
            updateOLEDWM(ssb);
        } else if (ssb.button == BUTTON_EVENT_1DOWN) {
            ssb.item = PLASTIC;
            ssb.wmState = CONTAMINATION;
            ssb.button = FALSE;
            updateOLEDWM(ssb);
        }
        break;
    case TRASH:
        if ((ssb.globalTime - ssb.wmtime > TRASH_TIMEOUT)) {
            ssb.wmState = WMINIT;
            ssb.event = NO_ITEM;
//            ssb.button = FALSE;
//            updateOLEDWM(ssb);
        }
        break;
    case CONTAMINATION:
        if (ssb.button == BUTTON_EVENT_1DOWN) {
            ssb.wmState = RECYCLE;
            ssb.contamination = CLEAN;
            ssb.button = FALSE;
            updateOLEDWM(ssb);
        } else if (ssb.button == BUTTON_EVENT_4DOWN) {
            ssb.wmState = TRASH;
            ssb.wmtime = ssb.globalTime;
            ssb.contamination = DIRTY;
            ssb.button = FALSE;
            updateOLEDWM(ssb);
        }
        break;
    case RECYCLE:
        if (ssb.item > WASTE && ssb.contamination > DIRTY) {
            runlmrSM();
        }
        break;
    }
}

void runTopSM(void)
{
    switch (ssb.topState) {
    case TOPINIT:
        ssb.topState = IDLE;
        updateOLEDTop(ssb);
        break;
    case IDLE:
        if (ssb.button == BUTTON_EVENT_4DOWN) {
            ssb.topState = WAIT;
            ssb.waitTimeStart = ssb.globalTime;
            ssb.button = BUTTON_EVENT_NONE;
        }
        updateOLEDTop(ssb);
        break;
    case WAIT:
        if (ssb.globalTime - ssb.waitTimeStart == WAIT_TIMEOUT) {
            ssb.topState = IDLE;
            updateOLEDTop(ssb);
        } else if (ssb.button == BUTTON_EVENT_3DOWN) {
            ssb.topState = ACTIVE;
            ssb.button = BUTTON_EVENT_NONE;
//            updateOLEDTop(ssb);
        }
        break;
    case ACTIVE:
        runwmSm();
        if (ssb.event == NO_ITEM) {
            ssb.topState = WAIT;
            ssb.event = FALSE;
            ssb.waitTimeStart = ssb.globalTime;
            updateOLEDTop(ssb);
        }
        //        ssb.button = FALSE;
        //        ssb.wmState=WMINIT;
        //        ssb.buttonpress = top.globalTime;
        //        else if(ssb.item)
        //                updateOLEDTop(ssb);
        //        if(ssb.button){
        

        break;
    }
}

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
            runTopSM();
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
