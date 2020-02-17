/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Smart Slug Bin

  @File Name
    wmSM.h

  @Summary
 Contains the Waste Management level state machine of Smart Slug Bin

  @Description
 
 * 
 */
/* ************************************************************************** */
#include "configure.h"
#ifndef _WMSM_H    /* Guard against multiple inclusion */
#define _WMSM_H

/**
  @Function
    SSB runwmSM(SSB wm);

  @Summary
    This is the Waste Management level of the state machine.

  @Description
 The Waste management level runs through 5 states:
    CLASSIFY | CONTAMINATION | RECYCLE | TRASH
    <p>
 CLASSIFY:
    <p>
 CONTAMINATION:
    <p>
 RECYCLE:
    <p>
 TRASH:
   
 */
SSB runwmSM(SSB wm);

void updateOLEDWM(SSB wm);
#endif /* _WMSM_H */

/* *****************************************************************************
 End of File
 */
