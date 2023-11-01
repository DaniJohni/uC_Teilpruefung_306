/* 
 * File:   zyklus.h
 * 
 * Created on 13. November 2020, 09:08
 */

#ifndef ZYKLUS_H
#define	ZYKLUS_H

void ZYKLUS_Initialize();
/**
 * gibt an wie viele  ms seit dem resetZyklus vergangen sind
 */
 unsigned int get_Zyklus();

/**
 * setzt den ms Zyklus zurück
 */
void reset_Zyklus();
/**
 * gibt an wie viele ms seit dem resetZyklus vergangen sind
 */
 unsigned int get_Zyklus1();

/**
 * setzt den ms Zyklus zurück
 */
void reset_Zyklus1();

/**
 * gibt an wie viele ms seit dem resetZyklus vergangen sind
 */
 unsigned int get_Zyklus2();

/**
 * setzt den ms Zyklus zurück
 */
void reset_Zyklus2();

#endif	/* ZYKLUS_H */

