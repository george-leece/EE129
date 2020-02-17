/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Smart Slug Bin

  @File Name
    configure.h

  @Summary
    Holds the necessary data for Bin

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _CONFIGURE_H   /* Guard against multiple inclusion */
#define _CONFIGURE_H

#include <stdio.h>
#include <stdlib.h>
#include <xc.h>
#include <Adc.h>
#include <Oled.h>
#include <timers.h>
#include <Leds.h>
#include <Buttons.h>

#define WAIT_TIMEOUT 50
#define DROP_TIMEOUT 10
#define TRASH_TIMEOUT 10
#define PRESS 5
#define NO_ITEM 1
#define ITEM 2
#define WASTE 0
#define ALUMINUM 1
#define GLASS 2
#define PLASTIC 3
#define CLEAN 1
#define DIRTY 0
#define BIN_1 20
#define BIN_2 30
#define BIN_3 40

//#include "plib.h"
    /** SSB

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
typedef enum {
    TOPINIT,
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
    LMRINIT,
    FORWARD,
    REVERSE,
    DROP
} lmrState;

typedef struct {
    topState topState;
    wmState wmState;
    lmrState lmrState;
    uint32_t globalTime;
    uint32_t wmtime;
    uint32_t lmrtime;
    uint32_t buttonpress;
    uint8_t button;
    uint8_t event;
    uint8_t item;
    uint8_t contamination;
    uint8_t lock;
    uint32_t waitTimeStart;
    //    uint32_t
} SSB;

char display[100]; // used to print on the OLED

#endif /* _CONFIGURE_H */
/* *****************************************************************************
 End of File
 */
