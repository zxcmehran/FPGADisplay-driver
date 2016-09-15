##############################################################################
## FPGA Display Handler IP Core By Mehran Ahadi (http://mehran.ahadi.me)
## This IP allows you to draw shapes and print texts on VGA screen.
## Copyright (C) 2015-2016  Mehran Ahadi
## This work is released under MIT License.
##
## Description:       Microprocess Driver Command (tcl)
## Date:              Mon Aug 08 23:53:13 2016 (by Create and Import Peripheral Wizard)
##############################################################################

#uses "xillib.tcl"

proc generate {drv_handle} {
  xdefine_include_file $drv_handle "xparameters.h" "display_handler" "NUM_INSTANCES" "DEVICE_ID" "C_BASEADDR" "C_HIGHADDR" "C_MEM0_BASEADDR" "C_MEM0_HIGHADDR" 
}
