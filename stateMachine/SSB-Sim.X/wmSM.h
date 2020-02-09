/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _WMSM_H    /* Guard against multiple inclusion */
#define _WMSM_H

#include "configure.h"


    /**
      @Function
        void runwmSM(void);

      @Summary
        This is the top level of the state machine.

      @Description
     The top level runs through 3 states: IDLE | WAIT | ACTIVE
        <p>
     IDLE: The bin will be operating at minimal power in order to conserve 
           energy. The bin will transition into WAIT if an ssb.userEvent happens
           from the proximity sensor.
        <p>
     WAIT: The bin has been activated by a ssb.userEvent which will cause the
           bin to power up and in preparation for an ssb.itemEvent. The bin will
           return to IDLE if it times out. An ssb.itemEvent will transition the
           the bin into ACTIVE.
        <p>
     ACTIVE: The bin will enter wmSM upon transition from WAIT state. Bin will
           transition back into WAIT after the ssb.item has been consumed by 
           trashing or recycling the item.
     */
    void runTopSM(SSB ssb);

#endif /* _WMSM_H */

/* *****************************************************************************
 End of File
 */
