// TI File $Revision: /main/1 $
// Checkin $Date: December 5, 2008   18:01:09 $
//###########################################################################
//
// FILE:	DSP2803x_Sci.c
//
// TITLE:	DSP2803x SCI Initialization & Support Functions.
//
//###########################################################################
// $TI Release: 2803x C/C++ Header Files V1.21 $
// $Release Date: December 1, 2009 $
//###########################################################################

#include "F2802x_Device.h"     // DSP2803x Headerfile Include File
#include "f2802x_examples.h"   // DSP2803x Examples Include File

//---------------------------------------------------------------------------
// InitSci:
//---------------------------------------------------------------------------
// This function initializes the SCI(s) to a known state.
//
void InitSci ( void )
{
	// Initialize SCI-A:
	InitSciGpio();
	SciaRegs.SCICCR.all = 0x0007;            // 1 stop bit,  No loopback
	// No parity,8 char bits,
	// async mode, idle-line protocol
	SciaRegs.SCICTL1.all = 0x0003;           // enable TX, RX, internal SCICLK,
	// Disable RX ERR, SLEEP, TXWAKE
	SciaRegs.SCIHBAUD = 0x0000;            	 // RATE IS 9600 bit/s
	SciaRegs.SCILBAUD = 0x00C2;				//BRP=LSPCLK/f/8-1 LSPCLK=15MHZ(19200-0x60)
	SciaRegs.SCICTL2.bit.TXINTENA =0;		//��ֹ�ж�
	SciaRegs.SCICTL2.bit.RXBKINTENA =0;
	SciaRegs.SCICTL1.all = 0x0023;     		 // Relinquish SCI from Reset
	memset ( Rxd_buf, 0, sizeof ( Sci_packet_t ) );
}

//---------------------------------------------------------------------------
// Example: InitSciGpio:
//---------------------------------------------------------------------------
// This function initializes GPIO pins to function as SCI pins
//
// Each GPIO pin can be configured as a GPIO pin or up to 3 different
// peripheral functional pins. By default all pins come up as GPIO
// inputs after reset.
//
// Caution:
// Only one GPIO pin should be enabled for SCITXDA/B operation.
// Only one GPIO pin shoudl be enabled for SCIRXDA/B operation.
// Comment out other unwanted lines.

void InitSciGpio()
{
	InitSciaGpio();
}

void InitSciaGpio()
{
	EALLOW;

	/* Enable internal pull-up for the selected pins */
// Pull-ups can be enabled or disabled disabled by the user.
// This will enable the pullups for the specified pins.

	GpioCtrlRegs.GPAPUD.bit.GPIO19 = 0;    // Disable pull-up for GPIO19 (SCIRXDA)
//	GpioCtrlRegs.GPAPUD.bit.GPIO7 = 0;     // Enable pull-up for GPIO7  (SCIRXDA)

	GpioCtrlRegs.GPAPUD.bit.GPIO18 = 0;	   // Enable pull-up for GPIO18 (SCITXDA)
//	GpioCtrlRegs.GPAPUD.bit.GPIO12 = 0;	   // Enable pull-up for GPIO12 (SCITXDA)

	/* Set qualification for selected pins to asynch only */
// Inputs are synchronized to SYSCLKOUT by default.
// This will select asynch (no qualification) for the selected pins.

	GpioCtrlRegs.GPAQSEL2.bit.GPIO19 = 3;  // Asynch input GPIO19(SCIRXDA)
//	GpioCtrlRegs.GPAQSEL1.bit.GPIO7 = 3;   // Asynch input GPIO7 (SCIRXDA)

	/* Configure SCI-A pins using GPIO regs*/
// This specifies which of the possible GPIO pins will be SCI functional pins.

	GpioCtrlRegs.GPAMUX2.bit.GPIO18 = 2;   // Configure GPIO18 for SCIRXDA operation
//	GpioCtrlRegs.GPAMUX1.bit.GPIO7 = 2;    // Configure GPIO7  for SCIRXDA operation

	GpioCtrlRegs.GPAMUX2.bit.GPIO19 = 2;   // Configure GPIO19 for SCITXDA operation
//	GpioCtrlRegs.GPAMUX1.bit.GPIO12 = 2;   // Configure GPIO12 for SCITXDA operation

	EDIS;
}

//===========================================================================
// End of file.
//===========================================================================
