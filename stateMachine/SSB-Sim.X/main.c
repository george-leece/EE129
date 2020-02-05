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
#include "Adc.h"
#include "BOARD.h"
#include "Oled.h"
#include "timers.h"
#include "Leds.h"
#include <Buttons.h>

#define WAIT_TIMEOUT 50
#define PRESS 5
#define NONE 1
#define WASTE 0
#define ALUMINUM 1
#define GLASS 2
#define PLASTIC 3
#define CLEAN 0
#define DIRTY 1

int i;

typedef enum {
    IDLE,
    WAIT,
    ACTIVE
} topState;

typedef enum {
    WMINIT,
    CLASSIFY,
    TRASH,
    RECYCLE,
    CONTAMINATION
} wmState;

typedef enum {
    FORWARD,
    REVERSE,
    DROP
} lmrState;

typedef struct {
    topState topState;
    wmState wmState;
    lmrState lmrState;
    uint32_t globalTime;
    uint32_t buttonpress;
    uint8_t button;
    uint8_t event;
    uint8_t item;
    uint8_t contamination;
    uint8_t lock;
    uint32_t waitTimeStart;
    //    uint32_t
} SSB;

SSB ssb = {};

static int ADC_event = 0; // to flag a change in the ADC
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

//void runTopSM(void)
//{
//
//    //write your SM logic here.
//    switch(ssb.topState){
//    case IDLE:
//        if(ssb.button == BUTTON_EVENT_4UP){
//            ssb.topState = WAIT;
//            ssb.waitTimeStart = ssb.globalTime; 
//        }
//        updateOLEDTop(ssb);
//        break;
//    case WAIT:
//        if(ssb.globalTime-ssb.waitTimeStart == WAIT_TIMEOUT){
//            ssb.topState = IDLE;
//        } else if(ssb.button == BUTTON_EVENT_4UP){
//            ssb.topState = ACTIVE;
//        }
//        updateOLEDTop(ssb);
//        break;
//    case ACTIVE:
//        if(ssb.event == NONE){
//            ssb.topState = WAIT;
//            ssb.waitTimeStart = ssb.globalTime;
//        }
//        updateOLEDTop(ssb);
//        runwmState();
//        break;
//    }
//}

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

void runwmSm(void)
{
    switch (ssb.wmState) {
    case WMINIT:
//        while(i<20){
//            i++;
//        }
        ssb.wmState=CLASSIFY;
        ssb.button=FALSE;
        updateOLEDWM(ssb);
        break;
    case CLASSIFY:
        if (ssb.button==BUTTON_EVENT_4DOWN) {
            ssb.item = TRASH;
            ssb.wmState = TRASH;
            ssb.button = FALSE;
            updateOLEDWM(ssb);
        } else if (ssb.button==BUTTON_EVENT_3DOWN) {
            ssb.item = ALUMINUM;
            ssb.wmState = CONTAMINATION;
            ssb.button = FALSE;
            updateOLEDWM(ssb);
        } else if (ssb.button==BUTTON_EVENT_2DOWN) {
            ssb.item = GLASS;
            ssb.wmState = CONTAMINATION;
            ssb.button = FALSE;
            updateOLEDWM(ssb);
        } else if (ssb.button==BUTTON_EVENT_1DOWN) {
            ssb.item = PLASTIC;
            ssb.wmState = CONTAMINATION;
            ssb.button = FALSE;
            updateOLEDWM(ssb);
        }
        break;
    case TRASH:
        ssb.wmState=WMINIT;
        ssb.event=NONE;
        ssb.button=FALSE;
//        updateOLEDWM(ssb);
        break;
    case CONTAMINATION:
        if(ssb.button==BUTTON_EVENT_1DOWN){
            ssb.wmState=RECYCLE;
            ssb.contamination=CLEAN;
            ssb.button=FALSE;
            updateOLEDWM(ssb);
        } else if(ssb.button==BUTTON_EVENT_4DOWN){
            ssb.wmState=TRASH;
            ssb.contamination=DIRTY;
            ssb.button=FALSE;
            updateOLEDWM(ssb);
        }
        break;
    case RECYCLE:
        break;

    }
}

void runTopSM(void)
{
    switch (ssb.topState) {
    case IDLE:
        if (ssb.button == BUTTON_EVENT_4UP) {
            ssb.topState = WAIT;
            ssb.waitTimeStart = ssb.globalTime;
            ssb.button = FALSE;
        }
        updateOLEDTop(ssb);
        break;
    case WAIT:
        if (ssb.globalTime - ssb.waitTimeStart == WAIT_TIMEOUT) {
            ssb.topState = IDLE;
        } else if (ssb.button == BUTTON_EVENT_3UP) {
            ssb.topState = ACTIVE;
            ssb.button = FALSE;
        }
        updateOLEDTop(ssb);
        break;
    case ACTIVE:
        if (ssb.event == NONE) {
            updateOLEDTop(ssb);
            ssb.topState = WAIT;
            ssb.event=FALSE;
            ssb.waitTimeStart = ssb.globalTime;
        }
//        ssb.button = FALSE;
//        ssb.wmState=WMINIT;
//        ssb.buttonpress = ssb.globalTime;
        //        else if(ssb.item)
//                updateOLEDTop(ssb);
        if(ssb.button){
            runwmSm();
        }
        
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
