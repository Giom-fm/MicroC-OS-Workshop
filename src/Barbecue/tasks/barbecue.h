/*
 * barbecue.h
 *
 * Stellt die Temperatur des Grills ein bei der gegrillt werden soll und
 * grillt die W�rstchen, welche auf dem Grill liegen.
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#ifndef BARBECUE_H_
#define BARBECUE_H_

#include "../types.h"

#define ENVTEMP 			(0) // niedrigste Grilltemperatur (Umweltbedingt)
#define MAXGRILLTEMP 		(500) // h�chste Grilltemperatur
#define BARBECUE_PERIOD 	(OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC * 0.01)
#define BARBECUE_SIZE		(5)

// Struct um den Grill zu repr�sentieren
typedef struct {
	OS_EVENT* mutex;
	OS_MEM *memory;
	OS_STK stack[TASK_STACK_SIZE];
	int index;
	boolean inflamed;
	Sausage* sausages[BARBECUE_SIZE + 1];
} Barbecue;

// Globale Grillvariable zum Zugriff
Barbecue barbecue;
INT16U grillTemperature;

/*
 * Stellt die Temperatur des Grills ein und grillt die W�rstchen
 *
 * @param pdata Zeiger auf einen �bergebenen Speicherbereich
 */
void barbecueTask(void* pdata);

/*
 * Regelt die Temperatur je nach Input
 *
 * @param temperature Wert wie geregelt werden soll
 */
void handleTemperatureCommand(Temperature temperature);

/*
 * Grillt alle W�rstchen die auf dem Grill liegen (periodisch, timergesteuert)
 *
 * @param ptmr, callback_arg Parameter f�r die Callbackfunktion des Timers
 *                           hier ungenutzt
 */
void grillSausages(void *ptmr, void *callback_arg);

#endif /* BARBECUE_H_ */
