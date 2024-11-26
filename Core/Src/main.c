//--------------------------------------------------------------
// File     : main.c
// Datum    : 30.03.2016
// Version  : 1.0
// Autor    : UB
// mods by	: J.F. van der Bent
// CPU      : STM32F4
// IDE      : CooCox CoIDE 1.7.x
// Module   : CMSIS_BOOT, M4_CMSIS_CORE
// Function : VGA_core DMA LIB 320x240, 8bit color
//--------------------------------------------------------------

#include "main.h"
#include "UART.h"
#include "stm32_ub_vga_screen.h"
#include <math.h>


int main(void)
{
	SystemInit(); // System speed to 168MHz

	UB_VGA_Screen_Init(); // Init VGA-Screen
	UART2_Init_Interrupt(9600);
	uint16_t xp,yp;

	    for(yp = 0; yp < VGA_DISPLAY_Y; yp++) {
	      for(xp = 0; xp < VGA_DISPLAY_X; xp++) {
	    	  if(xp < 200 && xp > 198)
	    	  {
	    		  UB_VGA_SetPixel(xp, yp, VGA_COL_MAGENTA);
	    	  }
	    	  else
	    	  {
	    	       UB_VGA_SetPixel(xp, yp, VGA_COL_CYAN);
	    	  }
	      }
	    }
  while(1)
  {


  }
}

void delay_us(uint32_t us) {
    uint32_t count = us * (SystemCoreClock / 1000000) / 5; // Adjust division based on compiler optimization
    while (count--) {
        __NOP(); // No operation (assembly instruction)
    }
}

