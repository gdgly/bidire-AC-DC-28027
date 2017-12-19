//###########################################################################
//
// FILE:    F2802x_Adc.c
//
// TITLE:   F2802x ADC Initialization & Support Functions.
//
//###########################################################################
// $TI Release: F2802x Support Library v230 $
// $Release Date: Fri May  8 07:43:05 CDT 2015 $
// $Copyright: Copyright (C) 2008-2015 Texas Instruments Incorporated -
//             http://www.ti.com/ ALL RIGHTS RESERVED $
//###########################################################################

#include "F2802x_Device.h"     // Headerfile Include File
#include "f2802x_examples.h"   // Examples Include File

#include "F2802x_AdcDefines.h"

#define ADC_usDELAY  1000L

//---------------------------------------------------------------------------
// InitAdc:
//---------------------------------------------------------------------------
// This function initializes ADC to a known state.
//
// NOTE: ADC INIT IS DIFFERENT ON 2802x DEVICES COMPARED TO OTHER 28X DEVICES
//
void InitAdc(void)
{
    extern void DSP28x_usDelay(Uint32 Count);

    // *IMPORTANT*
    // The Device_cal function, which copies the ADC calibration values from TI reserved
    // OTP into the ADCREFSEL and ADCOFFTRIM registers, occurs automatically in the
    // Boot ROM. If the boot ROM code is bypassed during the debug process, the
    // following function MUST be called for the ADC to function according
    // to specification. The clocks to the ADC MUST be enabled before calling this
    // function.
    // See the device data manual and/or the ADC Reference
    // Manual for more information.

        EALLOW;
        SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;
        (*Device_cal)();
        EDIS;

    // To powerup the ADC the ADCENCLK bit should be set first to enable
    // clocks, followed by powering up the bandgap, reference circuitry, and ADC core.
    // Before the first conversion is performed a 5ms delay must be observed
    // after power up to give all analog circuits time to power up and settle

    // Please note that for the delay function below to operate correctly the
    // CPU_RATE define statement in the F2802x_Examples.h file must
    // contain the correct CPU clock period in nanoseconds.
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCBGPWD  = 1;      // Power ADC BG
    AdcRegs.ADCCTL1.bit.ADCREFPWD = 1;      // Power reference
    AdcRegs.ADCCTL1.bit.ADCPWDN   = 1;      // Power ADC
    AdcRegs.ADCCTL1.bit.ADCENABLE = 1;      // Enable ADC
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;      // Select internal BG
    EDIS;

    DELAY_US(ADC_usDELAY);         // Delay before converting ADC channels
}

void ConfigureADC()
{
	EALLOW;
	//Disable ADCINT1 trigger firstly, or the TRIGSEL field is ignored.
	AdcRegs.ADCINTSOCSEL1.all = ADC_TRIGSEL_ENABLE;
//	AdcRegs.ADCINTSOCSEL2.all = ADC_TRIGSEL_ENABLE;

	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN0 	= ADC_SINGLE_SAMP;	//Single sample mode
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN2 	= ADC_SINGLE_SAMP;	//Single sample mode
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN4 	= ADC_SINGLE_SAMP;	//Single sample mode
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN6 	= ADC_SINGLE_SAMP;	//Single sample mode
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN8 	= ADC_SINGLE_SAMP;	//Single sample mode
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN10	= ADC_SINGLE_SAMP;	//Single sample mode
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN12	= ADC_SINGLE_SAMP;	//Single sample mode
	AdcRegs.ADCSAMPLEMODE.bit.SIMULEN14	= ADC_SINGLE_SAMP;	//Single sample mode

	AdcRegs.SOCPRICTL.bit.SOCPRIORITY = 0x0;		//SOC priority is handled in round robin mode for all channels.
	AdcRegs.ADCCTL2.bit.ADCNONOVERLAP = ADC_OVERLAP_ENABLE;	// Enable non-overlap mode
	AdcRegs.ADCCTL1.bit.INTPULSEPOS	= 1;	// ADCINT1 trips after AdcResults latch
	AdcRegs.INTSEL1N2.bit.INT1E     = ADC_INT_ENABLE;	// Enabled ADCINT1
	AdcRegs.INTSEL1N2.bit.INT1CONT  = ADC_INT_CONTINUOUS_DISABLE;	// Disable ADCINT1 Continuous mode
	AdcRegs.INTSEL1N2.bit.INT1SEL 	= ADC_INT_TRIG_EOC2;    // setup EOC0 to trigger ADCINT1 to fire

	/*******************************************************
	*
	********************************************************/

	AdcRegs.ADCSOC0CTL.bit.CHSEL 	= ADC_IN_A3;    // CSac-FB
	AdcRegs.ADCSOC1CTL.bit.CHSEL 	= ADC_IN_B1;    //ACL-FB
	AdcRegs.ADCSOC2CTL.bit.CHSEL 	= ADC_IN_B2;    //ACN-FB
	AdcRegs.ADCSOC3CTL.bit.CHSEL 	= ADC_IN_A1;    //H-FB ĸ�ߵ�ѹ����
	AdcRegs.ADCSOC4CTL.bit.CHSEL 	= ADC_IN_A0;    //REF3Vp
//	AdcRegs.ADCSOC5CTL.bit.CHSEL 	= ADC_IN_B2;    //
//	AdcRegs.ADCSOC6CTL.bit.CHSEL 	= ADC_IN_B4;    //
//	AdcRegs.ADCSOC7CTL.bit.CHSEL 	= ADC_IN_B0;    //
//	AdcRegs.ADCSOC8CTL.bit.CHSEL 	= ADC_IN_A5;    //

	AdcRegs.ADCSOC0CTL.bit.TRIGSEL	= ADC_TRIGSEL_EPWM1_ADCSOCA;    // set SOC0 start trigger on EPWM5ADCSOCA, due to round-robin SOC0 converts first then SOC1
	AdcRegs.ADCSOC1CTL.bit.TRIGSEL	= ADC_TRIGSEL_EPWM1_ADCSOCA;    //
	AdcRegs.ADCSOC2CTL.bit.TRIGSEL	= ADC_TRIGSEL_EPWM1_ADCSOCA;    //
	AdcRegs.ADCSOC3CTL.bit.TRIGSEL	= ADC_TRIGSEL_EPWM1_ADCSOCA;    //
	AdcRegs.ADCSOC4CTL.bit.TRIGSEL	= ADC_TRIGSEL_EPWM1_ADCSOCA;    //
//	AdcRegs.ADCSOC5CTL.bit.TRIGSEL	= ADC_TRIGSEL_EPWM5_ADCSOCA;    //
//	AdcRegs.ADCSOC6CTL.bit.TRIGSEL	= ADC_TRIGSEL_EPWM6_ADCSOCA;    //
//	AdcRegs.ADCSOC7CTL.bit.TRIGSEL	= ADC_TRIGSEL_EPWM6_ADCSOCA;    //
//	AdcRegs.ADCSOC8CTL.bit.TRIGSEL	= ADC_TRIGSEL_EPWM1_ADCSOCA;    //

	AdcRegs.ADCSOC0CTL.bit.ACQPS	= ADC_ACQUISITION_7ADCCLK;	//Sample windowis 7 cycles long (6+1 clock cycles)
	AdcRegs.ADCSOC1CTL.bit.ACQPS	= ADC_ACQUISITION_7ADCCLK;	//Sample windowis 7 cycles long (6+1 clock cycles)
	AdcRegs.ADCSOC2CTL.bit.ACQPS	= ADC_ACQUISITION_7ADCCLK;	//Sample windowis 7 cycles long (6+1 clock cycles)
	AdcRegs.ADCSOC3CTL.bit.ACQPS	= ADC_ACQUISITION_7ADCCLK;	//Sample windowis 7 cycles long (6+1 clock cycles)
	AdcRegs.ADCSOC4CTL.bit.ACQPS	= ADC_ACQUISITION_7ADCCLK;	//Sample windowis 7 cycles long (6+1 clock cycles)
//	AdcRegs.ADCSOC5CTL.bit.ACQPS	= ADC_ACQUISITION_7ADCCLK;	//Sample windowis 7 cycles long (6+1 clock cycles)
//	AdcRegs.ADCSOC6CTL.bit.ACQPS	= ADC_ACQUISITION_7ADCCLK;	//Sample windowis 7 cycles long (6+1 clock cycles)
//	AdcRegs.ADCSOC7CTL.bit.ACQPS	= ADC_ACQUISITION_7ADCCLK;	//Sample windowis 7 cycles long (6+1 clock cycles)
//	AdcRegs.ADCSOC8CTL.bit.ACQPS	= ADC_ACQUISITION_7ADCCLK;	//Sample windowis 7 cycles long (6+1 clock cycles)
	EDIS;
}

void InitAdcAio()
{

   EALLOW;

/* Configure ADC pins using AIO regs*/
// This specifies which of the possible AIO pins will be Analog input pins.
// NOTE: AIO1,3,5,7-9,11,13,15 are analog inputs in all AIOMUX1 configurations.
// Comment out other unwanted lines.

    GpioCtrlRegs.AIOMUX1.bit.AIO2 = 2;    // Configure AIO2 for A2 (analog input) operation
    GpioCtrlRegs.AIOMUX1.bit.AIO4 = 2;    // Configure AIO4 for A4 (analog input) operation
    GpioCtrlRegs.AIOMUX1.bit.AIO6 = 2;    // Configure AIO6 for A6 (analog input) operation
    GpioCtrlRegs.AIOMUX1.bit.AIO10 = 2;   // Configure AIO10 for B2 (analog input) operation
    GpioCtrlRegs.AIOMUX1.bit.AIO12 = 2;   // Configure AIO12 for B4 (analog input) operation
    GpioCtrlRegs.AIOMUX1.bit.AIO14 = 2;   // Configure AIO14 for B6 (analog input) operation

    EDIS;
}

/* AdcoffsetSelfCal-
   This function re-calibrates the ADC zero offset error by converting the VREFLO reference with
   the ADC and modifying the ADCOFFTRIM register. VREFLO is sampled by the ADC using an internal
   MUX select which connects VREFLO to A5 without sacrificing an external ADC pin. This
   function calls two other functions:
   - AdcChanSelect(channel) � selects the ADC channel to convert
   - AdcConversion() � initiates several ADC conversions and returns the average
*/
void AdcOffsetSelfCal()
{
    Uint16 AdcConvMean;
    EALLOW;
    AdcRegs.ADCCTL1.bit.ADCREFSEL = 0;                  //Select internal reference mode
    AdcRegs.ADCCTL1.bit.VREFLOCONV = 1;                 //Select VREFLO internal connection on B5
    AdcChanSelect(13);                                  //Select channel B5 for all SOC
    AdcRegs.ADCOFFTRIM.bit.OFFTRIM = 80;                //Apply artificial offset (+80) to account for a negative offset that may reside in the ADC core
    AdcConvMean = AdcConversion();                      //Capture ADC conversion on VREFLO
    AdcRegs.ADCOFFTRIM.bit.OFFTRIM = 80 - AdcConvMean;  //Set offtrim register with new value (i.e remove artical offset (+80) and create a two's compliment of the offset error)
    AdcRegs.ADCCTL1.bit.VREFLOCONV = 0;                 //Select external ADCIN5 input pin on B5
    EDIS;
}

/*  AdcChanSelect-
    This function selects the ADC channel to convert by setting all SOC channel selects to a single channel.

     * IMPORTANT * This function will overwrite previous SOC channel select settings. Recommend saving
           the previous settings.
 */
void AdcChanSelect(Uint16 ch_no)
{
    AdcRegs.ADCSOC0CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC1CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC2CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC3CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC4CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC5CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC6CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC7CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC8CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC9CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC10CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC11CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC12CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC13CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC14CTL.bit.CHSEL= ch_no;
    AdcRegs.ADCSOC15CTL.bit.CHSEL= ch_no;
} //end AdcChanSelect

/* AdcConversion -
   This function initiates several ADC conversions and returns the average. It uses ADCINT1 and ADCINT2
   to "ping-pong" between SOC0-7 and SOC8-15 and is referred to as "ping-pong" sampling.

     * IMPORTANT * This function will overwrite previous ADC settings. Recommend saving previous settings.
*/
Uint16 AdcConversion(void)
{
    Uint16 index, SampleSize, Mean, ACQPS_Value;
    Uint32 Sum;

    index       = 0;            //initialize index to 0
    SampleSize  = 256;          //set sample size to 256 (**NOTE: Sample size must be multiples of 2^x where is an integer >= 4)
    Sum         = 0;            //set sum to 0
    Mean        = 999;          //initialize mean to known value

    //Set the ADC sample window to the desired value (Sample window = ACQPS + 1)
    ACQPS_Value = 6;
    AdcRegs.ADCSOC0CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC1CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC2CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC3CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC4CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC5CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC6CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC7CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC8CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC9CTL.bit.ACQPS  = ACQPS_Value;
    AdcRegs.ADCSOC10CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC11CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC12CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC13CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC14CTL.bit.ACQPS = ACQPS_Value;
    AdcRegs.ADCSOC15CTL.bit.ACQPS = ACQPS_Value;


    //Enable ping-pong sampling

    // Enabled ADCINT1 and ADCINT2
    AdcRegs.INTSEL1N2.bit.INT1E = 1;
    AdcRegs.INTSEL1N2.bit.INT2E = 1;

    // Disable continuous sampling for ADCINT1 and ADCINT2
    AdcRegs.INTSEL1N2.bit.INT1CONT = 0;
    AdcRegs.INTSEL1N2.bit.INT2CONT = 0;

    AdcRegs.ADCCTL1.bit.INTPULSEPOS = 1;    //ADCINTs trigger at end of conversion

    // Setup ADCINT1 and ADCINT2 trigger source
    AdcRegs.INTSEL1N2.bit.INT1SEL = 6;      //EOC6 triggers ADCINT1
    AdcRegs.INTSEL1N2.bit.INT2SEL = 14;     //EOC14 triggers ADCINT2

    // Setup each SOC's ADCINT trigger source
    AdcRegs.ADCINTSOCSEL1.bit.SOC0  = 2;    //ADCINT2 starts SOC0-7
    AdcRegs.ADCINTSOCSEL1.bit.SOC1  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC2  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC3  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC4  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC5  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC6  = 2;
    AdcRegs.ADCINTSOCSEL1.bit.SOC7  = 2;
    AdcRegs.ADCINTSOCSEL2.bit.SOC8  = 1;    //ADCINT1 starts SOC8-15
    AdcRegs.ADCINTSOCSEL2.bit.SOC9  = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC10 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC11 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC12 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC13 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC14 = 1;
    AdcRegs.ADCINTSOCSEL2.bit.SOC15 = 1;

    DELAY_US(ADC_usDELAY);                  // Delay before converting ADC channels


    //ADC Conversion

    AdcRegs.ADCSOCFRC1.all = 0x00FF;  // Force Start SOC0-7 to begin ping-pong sampling

    while( index < SampleSize ){

        //Wait for ADCINT1 to trigger, then add ADCRESULT0-7 registers to sum
        while (AdcRegs.ADCINTFLG.bit.ADCINT1 == 0){}
        AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;   //Must clear ADCINT1 flag since INT1CONT = 0
        Sum += AdcResult.ADCRESULT0;
        Sum += AdcResult.ADCRESULT1;
        Sum += AdcResult.ADCRESULT2;
        Sum += AdcResult.ADCRESULT3;
        Sum += AdcResult.ADCRESULT4;
        Sum += AdcResult.ADCRESULT5;
        Sum += AdcResult.ADCRESULT6;
        // Wait for SOC9 conversion to start, which gives time for SOC7 conversion result
        while( AdcRegs.ADCSOCFLG1.bit.SOC9 == 1 ){}
        Sum += AdcResult.ADCRESULT7;

        //Wait for ADCINT2 to trigger, then add ADCRESULT8-15 registers to sum
        while (AdcRegs.ADCINTFLG.bit.ADCINT2 == 0){}
        AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;   //Must clear ADCINT2 flag since INT2CONT = 0
        Sum += AdcResult.ADCRESULT8;
        Sum += AdcResult.ADCRESULT9;
        Sum += AdcResult.ADCRESULT10;
        Sum += AdcResult.ADCRESULT11;
        Sum += AdcResult.ADCRESULT12;
        Sum += AdcResult.ADCRESULT13;
        Sum += AdcResult.ADCRESULT14;
        // Wait for SOC1 conversion to start, which gives time for SOC15 conversion result
        while( AdcRegs.ADCSOCFLG1.bit.SOC1 == 1 ){}
        Sum += AdcResult.ADCRESULT15;

        index+=16;

    } // end data collection

    //Disable ADCINT1 and ADCINT2 to STOP the ping-pong sampling
    AdcRegs.INTSEL1N2.bit.INT1E = 0;
    AdcRegs.INTSEL1N2.bit.INT2E = 0;

    while(AdcRegs.ADCSOCFLG1.all != 0){} // Wait for any pending SOCs to complete

    // Clear any pending interrupts
    AdcRegs.ADCINTFLGCLR.bit.ADCINT1 = 1;
    AdcRegs.ADCINTFLGCLR.bit.ADCINT2 = 1;
    AdcRegs.ADCINTOVFCLR.bit.ADCINT1 = 1;
    AdcRegs.ADCINTOVFCLR.bit.ADCINT2 = 1;

    //reset RR pointer to 32, so that next SOC is SOC0
    AdcRegs.SOCPRICTL.bit.SOCPRIORITY = 1;
    while( AdcRegs.SOCPRICTL.bit.SOCPRIORITY != 1 );
    AdcRegs.SOCPRICTL.bit.SOCPRIORITY = 0;
    while( AdcRegs.SOCPRICTL.bit.SOCPRIORITY != 0 );

    Mean = Sum / SampleSize;    //Calculate average ADC sample value

    return Mean;                //return the average

}//end AdcConversion

void InitComp ( void )
{
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.ADCENCLK = 1;                 // Enable Clock to the ADC
	AdcRegs.ADCCTL1.bit.ADCBGPWD = 1;                     // Comparator shares the internal BG reference of the ADC, must be powered even if ADC is unused
	DELAY_US ( 1000 );										 // Delay for Power Up
	
	//COMP1 LLC OCP
	SysCtrlRegs.PCLKCR3.bit.COMP1ENCLK = 1;               // Enable clock to the Comparator 1 block 
	Comp1Regs.COMPCTL.bit.COMPDACEN = 1;                  // Power up Comparator 1 locally
	Comp1Regs.COMPCTL.bit.COMPSOURCE = 0;              // Connect the inverting input to the internal DAC
	Comp1Regs.DACVAL.bit.DACVAL = 550;//775;				    // Set DAC output to midpoint

	//COMP2 Boost OCP
	SysCtrlRegs.PCLKCR3.bit.COMP2ENCLK = 1;               // Enable clock to the Comparator 1 block 
	Comp2Regs.COMPCTL.bit.COMPDACEN = 1;                  // Power up Comparator 1 locally
	Comp2Regs.COMPCTL.bit.COMPSOURCE = 0;              // Connect the inverting input to the internal DAC
	Comp2Regs.DACVAL.bit.DACVAL = 900;				    // Set DAC output to midpoint

//	//COMP3 Boost OVP
//	SysCtrlRegs.PCLKCR3.bit.COMP3ENCLK = 1;               // Enable clock to the Comparator 1 block 
//	Comp3Regs.COMPCTL.bit.COMPDACEN = 1;                  // Power up Comparator 1 locally
//	Comp3Regs.COMPCTL.bit.COMPSOURCE = 0;              // Connect the inverting input to the internal DAC
//	Comp3Regs.DACVAL.bit.DACVAL = 700;			//900V	    // Set DAC output to midpoint
	EDIS;
}

//===========================================================================
// End of file.
//===========================================================================