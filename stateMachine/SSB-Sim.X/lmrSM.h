/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Smart Slug Bin

  @File Name
    lmrSM.h

  @Summary
 Contains the linear motion rail level state machine of Smart Slug Bin

  @Description
 
 * 
 */
/* ************************************************************************** */
#include "configure.h"
#ifndef _LMRSM_H    /* Guard against multiple inclusion */
#define _LMRSM_H

    /**
      @Function
        void runlmrSM(void);

      @Summary
        This is the linear motion rail level of the state machine.

      @Description
     The linear motion rail level runs through 3 states: 
        FORWARD | DROP | REVERSE
        <p>
     FORWARD: 
        <p>
     DROP:
        <p>
     REVERSE:
     
    */
    
    SSB runlmrSM(SSB lmr);
    
    void updateOLEDLMR(SSB lmr);
#endif /* _LMRSM_H */

/* *****************************************************************************
 End of File
 */
