#include "zyklus.h"
#include "mcc_generated_files/mcc.h"

static volatile unsigned int zyklen=0;
static volatile unsigned int zyklen1=0;
static volatile unsigned int zyklen2=0;

static volatile unsigned char synchron=1;
static volatile unsigned char blocked=0;

void My_InterruptHandler(void);
void ZYKLUS_Initialize();

void ZYKLUS_Initialize()
{   
   INTERRUPT_GlobalInterruptEnable();
   INTERRUPT_PeripheralInterruptEnable();
   TMR0_SetInterruptHandler(My_InterruptHandler);
   TMR0_StartTimer();
}
/**
 *	Timer3 Interrupt-Routine
 */ 
void My_InterruptHandler(void)
{
   // synchronisiere den Speicherzugriff
	if (synchron)
	{
		synchron=0;		
		zyklen+=(unsigned int)blocked;
		zyklen++;
		zyklen1+=(unsigned int)blocked;
		zyklen1++;
		zyklen2+=(unsigned int)blocked;
		zyklen2++;
		blocked=0;
		synchron=1;
	}
	else
	{
		blocked++;	// falls Zugriff vom Hauptprogramm, merken des Wertes
	}	 
}

/**
 *  Hilfsfunktion für die Synchronisation
 */
void sync()
{
	// forciere Hauptprogramm mit Interruptroutine zu synchronisieren;
	while (blocked > 0);
}

/**
 * gibt an wie viele 25ms seit dem resetZyklus vergangen sind
 */
 unsigned int get_Zyklus()
 {
 	static unsigned int kopie=0;
	sync();
	if (synchron)
	{
		synchron=0;
		kopie=zyklen;
		synchron=1;
	}
	return kopie;
 }

/**
 * setzt den zyklus zurück
 */
void reset_Zyklus()
{
   // synchronisiere den Speicherzugriff
  sync();
	synchron=0;
	zyklen=0;
	synchron=1;		
	
}

/**
 * gibt an wie viele 25ms seit dem resetZyklus1 vergangen sind
 */
 unsigned int get_Zyklus1()
 {
 	static unsigned int kopie=0;
	sync();
	
	if (synchron)
	{
		synchron=0;
		kopie=zyklen1;
		synchron=1;
	}
	return kopie;
 }

/**
 * setzt den zyklus1 zurück
 */
void reset_Zyklus1()
{
   // synchronisiere den Speicherzugriff
  sync();
	synchron=0;
	zyklen1=0;
	synchron=1;		
	
}

/**
 * gibt an wie viele 25ms seit dem resetZyklus2 vergangen sind
 */
 unsigned int get_Zyklus2()
 {
 	static unsigned int kopie=0;
	sync();

	if (synchron)
	{
		synchron=0;
		kopie=zyklen2;
		synchron=1;
	}
	return kopie;
 }

/**
 * setzt den zyklus2 zurück
 */
void reset_Zyklus2()
{
   // synchronisiere den Speicherzugriff
  sync();
	synchron=0;
	zyklen2=0;
	synchron=1;		
	
}
