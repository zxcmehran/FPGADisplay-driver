//////////////////////////////////////////////////////////////////////////////
// FPGA Display Handler IP Core By Mehran Ahadi (http://mehran.ahadi.me)
// This IP allows you to draw shapes and print texts on VGA screen.
// Copyright (C) 2015-2016  Mehran Ahadi
// This work is released under MIT License.
//
// Version:           1.00.a
// Description:       Contains a diagnostic self-test function for the display_handler driver
// Date:              Mon Aug 08 23:53:13 2016 (by Create and Import Peripheral Wizard)
//////////////////////////////////////////////////////////////////////////////


/***************************** Include Files *******************************/

#include "display_handler.h"

/************************** Constant Definitions ***************************/


/************************** Variable Definitions ****************************/

extern Xuint32 LocalBRAM; /* User logic local memory (BRAM) base address */

/************************** Function Definitions ***************************/

/**
 *
 * Run a self-test on the driver/device. Note this may be a destructive test if
 * resets of the device are performed.
 *
 * If the hardware system is not built correctly, this function may never
 * return to the caller.
 *
 * @param   baseaddr_p is the base address of the DISPLAY_HANDLER instance to be worked on.
 *
 * @return
 *
 *    - XST_SUCCESS   if all self-test code passed
 *    - XST_FAILURE   if any self-test code failed
 *
 * @note    Caching must be turned off for this function to work.
 * @note    Self test may fail if data memory and device are not on the same bus.
 *
 */
XStatus DISPLAY_HANDLER_SelfTest(void * baseaddr_p)
{
  int     Index;
  Xuint32 baseaddr;
  Xuint8  Reg8Value;
  Xuint16 Reg16Value;
  Xuint32 Reg32Value;
  Xuint32 Mem32Value;

  /*
   * Check and get the device address
   */
  XASSERT_NONVOID(baseaddr_p != XNULL);
  baseaddr = (Xuint32) baseaddr_p;

  xil_printf("******************************\n\r");
  xil_printf("* User Peripheral Self Test\n\r");
  xil_printf("******************************\n\n\r");

  /*
   * Write to user logic slave module register(s) and read back
   */
  xil_printf("User logic slave module test...\n\r");
  xil_printf("   - write 1 to slave register 0 word 0\n\r");
  DISPLAY_HANDLER_mWriteSlaveReg0(baseaddr, 0, 1);
  Reg32Value = DISPLAY_HANDLER_mReadSlaveReg0(baseaddr, 0);
  xil_printf("   - read %d from register 0 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 1 )
  {
    xil_printf("   - slave register 0 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - write 2 to slave register 1 word 0\n\r");
  DISPLAY_HANDLER_mWriteSlaveReg1(baseaddr, 0, 2);
  Reg32Value = DISPLAY_HANDLER_mReadSlaveReg1(baseaddr, 0);
  xil_printf("   - read %d from register 1 word 0\n\r", Reg32Value);
  if ( Reg32Value != (Xuint32) 2 )
  {
    xil_printf("   - slave register 1 word 0 write/read failed\n\r");
    return XST_FAILURE;
  }
  xil_printf("   - slave register write/read passed\n\n\r");

  /*
   * Write data to user logic BRAMs and read back
   */
  xil_printf("User logic BRAM test...\n\r");
  xil_printf("   - local BRAM address is 0x%08x\n\r", LocalBRAM);
  xil_printf("   - write pattern to local BRAM and read back\n\r");
  for ( Index = 0; Index < 256; Index++ )
  {
    DISPLAY_HANDLER_mWriteMemory(LocalBRAM+4*Index, 0xDEADBEEF);
    Mem32Value = DISPLAY_HANDLER_mReadMemory(LocalBRAM+4*Index);
    if ( Mem32Value != 0xDEADBEEF )
    {
      xil_printf("   - write/read BRAM failed on address 0x%08x\n\r", LocalBRAM+4*Index);
      return XST_FAILURE;
    }
  }
  xil_printf("   - write/read BRAM passed\n\n\r");

  return XST_SUCCESS;
}
