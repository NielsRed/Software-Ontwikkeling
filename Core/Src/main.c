///**
//  ******************************************************************************
//  * @file      main.c
//  * @author    Niels
//  * @version   V1.0
//  * @date      03/05/2012
//  * @brief     STM32F4xx Devices vector table for RIDE7 toolchain.
//  *            This module performs:
//  *                - Set the initial SP
//  *                - Set the initial PC == Reset_Handler,
//  *                - Set the vector table entries with the exceptions ISR address
//  *                - Configure the clock system and the external SRAM mounted on
//  *                  STM324xG-EVAL board to be used as data memory (optional,
//  *                  to be enabled by user)
//  *                - Branches to main in the C library (which eventually
//  *                  calls main()).
//  *            After Reset the Cortex-M4 processor is in Thread mode,
//  *            priority is Privileged, and the Stack is set to Main.
//  ******************************************************************************
//  */

#include "main.h"
#include "UART.h"
#include "stm32_ub_vga_screen.h"
#include <math.h>
#include "API_Draw.h"

int main(void)
{
	SystemInit(); // System speed to 168MHz

	UB_VGA_Screen_Init(); // Init VGA-Screen
	UART2_Init_Interrupt(9600);

  while(1)
  {

  }
}
