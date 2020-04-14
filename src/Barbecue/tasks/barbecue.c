/*
 * barbecue.c
 *
 * Stellt die Temperatur des Grills ein bei der gegrillt werden soll (per Kommando
 * vom Input) und grillt die Würstchen, welche auf dem Grill liegen, periodisch.
 *
 * Ist ein Würstchen zu stark gebraten/verbrannt, steht der Grill in Flammen und
 * kann nur durch die Feuerwehr gelöscht werden.
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#include "barbecue.h"
#include <stdio.h>
#include "chef.h"
#include "input.h"

/*
 * Stellt die Temperatur des Grills ein und grillt die Würstchen
 *
 * Nutzt seinen barbecue.mutex um den Zugriff auf die Würstchen und die Stati zu sichern
 *
 * @param pdata Zeiger auf einen übergebenen Speicherbereich
 */
void barbecueTask(void* pdata) {
	grillTemperature = ENVTEMP;
	barbecue.sausages[0] = NULL;
	barbecue.index = 0;
	barbecue.inflamed = FALSE;

	INT8U err;
	OS_TMR *timer;
	timer = OSTmrCreate(0, BARBECUE_PERIOD, OS_TMR_OPT_PERIODIC, grillSausages,
			(void*) 0, "BBQTMR", &err);
	if (err) {
		printErrorAndHalt("barbecue: Timer Init Failed", err);
	}
	OSTmrStart(timer, &err);
	if (err) {
		printErrorAndHalt("barbecue: Timer Start Failed", err);
	}

	while (1) {
		INT8U err;

		// TODO Error handing
		void* temperatureMessage = OSQAccept(input.barbecueCommandQueue, &err);
		if (temperatureMessage != (void*) 0) {
			Temperature temperature = *((Temperature*) temperatureMessage);
			returnMemory("Barbecue", input.memory, temperatureMessage);
			handleTemperatureCommand(temperature);
		}
		OSTimeDly(WAIT_1);
	}
}

/*
 * Regelt die Temperatur je nach Input
 *
 * @param temperature Wert wie geregelt werden soll
 */
void handleTemperatureCommand(Temperature temperature) {

	switch (temperature) {

	case UP10: {
		if (grillTemperature < MAXGRILLTEMP) {
			grillTemperature += 10;
		}
		break;
	}
	case DOWN10: {
		if (grillTemperature > ENVTEMP) {
			grillTemperature -= 10;
		}
		break;
	}
	case UP50: {
		if (grillTemperature + 50 < MAXGRILLTEMP) {
			grillTemperature += 50;
		} else {
			grillTemperature = MAXGRILLTEMP;
		}
		break;
	}
	case DOWN50: {
		if (grillTemperature - 50 > ENVTEMP) {
			grillTemperature -= 50;
		} else {
			grillTemperature = ENVTEMP;
		}
		break;
	}
	}
}

//DOKU OS_TASK_TMR_PRIO
/*
 * Grillt alle Würstchen die auf dem Grill liegen (periodisch, timergesteuert)
 *
 * @param ptmr, callback_arg Parameter für die Callbackfunktion des Timers
 *                           hier ungenutzt
 */
void grillSausages(void *ptmr, void *callback_arg) {

	INT8U err = OS_ERR_NONE;
	OSMutexPend(barbecue.mutex, 0, &err);
	if (err != OS_ERR_NONE) {
		printErrorAndHalt("BBQ-Grill: Failed to get Mutex", err);
	}
	int i = 0;
	while (barbecue.sausages[i] != (void*) 0) {
		Sausage* sausage = barbecue.sausages[i];
		SausageSides side = (*sausage).grilling;
		(*sausage).sides[side] += grillTemperature * 0.001;

		if (!barbecue.inflamed) {
			barbecue.inflamed = (*sausage).sides[side] > 100;
		}

		Sausage def_sausage = (*sausage);
		//printf("BBQ: Sausage %d side: %d %%:%f\n", def_sausage.id, side,	def_sausage.sides[side]);
		i++;
	}
	OSMutexPost(barbecue.mutex);
}
