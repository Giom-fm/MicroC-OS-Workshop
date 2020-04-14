/*
 * barbecue.h
 *
 * Stellt die Temperatur des Grills ein bei der gegrillt werden soll und
 * grillt die Würstchen, welche auf dem Grill liegen.
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#ifndef BARBECUE_H_
#define BARBECUE_H_

#include "../types.h"

#define ENVTEMP 			(0) // niedrigste Grilltemperatur (Umweltbedingt)
#define MAXGRILLTEMP 		(500) // höchste Grilltemperatur
#define BARBECUE_PERIOD 	(OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC * 0.01)
#define BARBECUE_SIZE		(5)

// Struct um den Grill zu repräsentieren
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
 * Stellt die Temperatur des Grills ein und grillt die Würstchen
 *
 * @param pdata Zeiger auf einen übergebenen Speicherbereich
 */
void barbecueTask(void* pdata);

/*
 * Regelt die Temperatur je nach Input
 *
 * @param temperature Wert wie geregelt werden soll
 */
void handleTemperatureCommand(Temperature temperature);

/*
 * Grillt alle Würstchen die auf dem Grill liegen (periodisch, timergesteuert)
 *
 * @param ptmr, callback_arg Parameter für die Callbackfunktion des Timers
 *                           hier ungenutzt
 */
void grillSausages(void *ptmr, void *callback_arg);

#endif /* BARBECUE_H_ */
