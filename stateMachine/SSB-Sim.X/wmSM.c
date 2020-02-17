/* ************************************************************************** */
/** Descriptive File Name

  @Company
 Smart Slug Bin

  @File Name
    wmSM.c

  @Summary
    

  @Description
    
 */

/** 
  @Function
    SSB updateOLEDWM(SSB wm);

  @Summary
    

  @Remarks
    
 */
#include "wmSM.h"
#include "lmrSM.h"
#include "TopSM.h"


SSB runwmSM(SSB wm)
{
    switch (wm.wmState) {
    case WMINIT:
        wm.wmState = CLASSIFY;
        updateOLEDWM(wm);
        break;
    case CLASSIFY:
        if (wm.button == BUTTON_EVENT_4DOWN) {
            wm.wmtime = wm.globalTime;
            wm.item = TRASH;
            wm.wmState = TRASH;
            wm.button = FALSE;
            updateOLEDWM(wm);
        } else if (wm.button == BUTTON_EVENT_3DOWN) {
            wm.item = ALUMINUM;
            wm.wmState = CONTAMINATION;
            wm.button = FALSE;
            updateOLEDWM(wm);
        } else if (wm.button == BUTTON_EVENT_2DOWN) {
            wm.item = GLASS;
            wm.wmState = CONTAMINATION;
            wm.button = FALSE;
            updateOLEDWM(wm);
        } else if (wm.button == BUTTON_EVENT_1DOWN) {
            wm.item = PLASTIC;
            wm.wmState = CONTAMINATION;
            wm.button = FALSE;
            updateOLEDWM(wm);
        }
        break;
    case TRASH:
        if ((wm.globalTime - wm.wmtime > TRASH_TIMEOUT)) {
            wm.wmState = WMINIT;
            wm.event = NO_ITEM;
        }
        break;
    case CONTAMINATION:
        if (wm.button == BUTTON_EVENT_1DOWN) {
            wm.wmState = RECYCLE;
            wm.contamination = CLEAN;
            wm.button = FALSE;
            updateOLEDWM(wm);
        } else if (wm.button == BUTTON_EVENT_4DOWN) {
            wm.wmState = TRASH;
            wm.wmtime = wm.globalTime;
            wm.contamination = DIRTY;
            wm.button = FALSE;
            updateOLEDWM(wm);
        }
        break;
    case RECYCLE:
        if (wm.item > WASTE && wm.contamination > DIRTY) {
            wm = runlmrSM(wm);
        }
        break;
    default:
        break;
    }
    return wm;
}

/** 
  @Function
    SSB updateOLEDWM(SSB wm);

  @Summary
    

  @Remarks
    
 */
void updateOLEDWM(SSB wm)
{
    switch (wm.wmState) {
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
        sprintf(display, "WM Level\nState: CONTAMINATIONItem: %d", wm.item);
        OledClear(OLED_COLOR_BLACK); // Removes any lingering characters
        OledDrawString(display);
        OledUpdate(); // Prints the new OLED display
        break;
    case RECYCLE: // 
        sprintf(display, "WM Level\nState: RECYCLE\nItem %d, %d", wm.item, wm.contamination);
        OledClear(OLED_COLOR_BLACK); //Removes any lingering characters
        OledDrawString(display);
        OledUpdate(); // Prints the new OLED display
        break;
    }
}


/* *****************************************************************************
 End of File
 */
