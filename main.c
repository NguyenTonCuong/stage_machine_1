#include "state.h"

static state_t State;
static	unsigned int	genTimerCount;

void initState(void)
{
	State = S0_OFF;
}
void statemachineUpdate(void)
{
	switch (State)
	{
		case S0_OFF:
			if (sensor() == NHAN)	
			{
				State = S1_OFF;
				genTimerCount = 3;
			}
			break;
			
		case S1_OFF:
			if (sensor() == NHA) 					State = S0_OFF;
			else if (genTimerCount==0) 		State = S2_ON;
		break;
		
		case S2_ON:
			if (sensor()== NHAN) 					State = S2_ON;
			else 													State = S3_ON;
		break;
		
		case S3_ON:
			if (sensor()== NHAN) 
			{
			State = S4_ON;				
			genTimerCount =6;
			}
		break;
		
		case S4_ON:
			if (sensor()== NHA)						State = S3_ON;
		  else if (genTimerCount==0)		State = S5_OFF;
			break;
		case S5_OFF:
			if (sensor() ==NHA) 					State = S0_OFF;
			else 													State = S5_OFF;
		break;
	}	
switch (State)
{
	case S0_OFF:
	case S1_OFF:
	case S5_OFF:
		ledCtrl(LEDOFF) ;
	break;
	case S2_ON:
	case S3_ON:
	case S4_ON:
		ledCtrl(LEDON);
	break;	
}
}
void sensorInit(void)
{
	    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
			GPIODirModeSet(GPIO_PORTF_BASE,SENSOR, GPIO_DIR_MODE_IN);
			GPIOPadConfigSet(GPIO_PORTF_BASE, SENSOR,GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
}
sensor_t sensor(void)
{
	unsigned int i,m;
	i= GPIOPinRead(GPIO_PORTF_BASE, SENSOR);
	if( (i& SENSOR)==0)
	{
		return NHAN;
	}
	else 
   {
		return NHA;
}
void	ledCtrlInit()
{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED);
}
void		ledCtrl(int	state)
{
	if (state == LEDON)	GPIOPinWrite(GPIO_PORTF_BASE, LED, LED);
	else GPIOPinWrite(GPIO_PORTF_BASE, LED, 0);
}
void SysTickIntHandler(void)
{
    //
    // Update the Timer counter.
    //
    if (genTimerCount != 0)
		{
			genTimerCount--;
		}
}
void ConfigSystick(void);
{
		SysCtlClockSet(SYSCTL_SYSDIV_20 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
//config_io
	 SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	 GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3);
	 GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1| GPIO_PIN_2| GPIO_PIN_3,0 );
		SysTickIntRegister(SysTickIntHandler);

	 SysTickPeriodSet(SysCtlClockGet());
// enable interrupt
	 IntMasterEnable();
//enable systick interrupt
	 SysTickIntEnable();
//enable systick
	 SysTickEnable();
}

