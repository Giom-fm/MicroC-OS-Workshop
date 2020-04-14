/*
 * firefighter.c
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#include "firefighter.h"
#include "barbecue.h"
#include "output.h"
#include "../util.h"

// Globales Flag ob der Timer zum selbständigen Arbeiten gesetzt ist
boolean timerSet;

/*
 * Erzeugt den Timer der alle 60s den Grill checkt
 *
 * Nutzt mehrere Mutexe, um auf den eigenen Status und den BBQ gesichert zuzugreifen
 * - barbecue.mutex
 * - firefighter.mutex
 */
void createFirefighterTimer() {
	timerSet = FALSE;
	INT8U err;
	OS_TMR *timer;
	timer = OSTmrCreate(0, FIREFIGHTER_PERIOD, OS_TMR_OPT_PERIODIC,
			checkBarbecue, (void*) 0, "FFTMR", &err);
	if (err) {
		printErrorAndHalt("Firefighter: Timer Init Failed", err);
	}

	OSTmrStart(timer, &err);
	if (err) {
		printErrorAndHalt("Firefighter: Timer Start Failed", err);
	}

	firefighter.mutex = OSMutexCreate(4, &err);
	if (err) {
		printErrorAndHalt("ERROR: COULD NOT INIT FF-MUTEX", err);
	}

}

/*
 * Setzt den Status des Timers
 *
 * @param status Status der gesetzt werden soll
 */
void setFFStatus(FireFighterStatus status) {
	INT8U err;
	OSMutexPend(firefighter.mutex,0, &err);
	if (err) {
		printErrorAndHalt("FF: Failed to set Status", err);
	}

	firefighter.status = status;
	OSMutexPost(firefighter.mutex);
}

/*
 * Checkt ob der Grill in Flammen steht.
 *
 * @param ptmr, callback_arg Parameter für die Callbackfunktion des Timers
 *                           hier ungenutzt
 */
void checkBarbecue(void *ptmr, void *callback_arg) {

	//printf("Firefighter: Check BBQ\n");
	INT8U err;
	OSMutexPend(barbecue.mutex, 0, &err);
	if (err != OS_ERR_NONE) {
		printErrorAndHalt("Firefighter-Check: Failed to get Mutex", err);
	}

	if (barbecue.inflamed) {
		setFFStatus(EXTINGUISHING);

		barbecue.inflamed = FALSE;
		int i;
		for (i = barbecue.index - 1; i >= 0; i--) {
			returnMemory("Firefighters", barbecue.memory, barbecue.sausages[i]);
			barbecue.sausages[i] = 0;
		}
		barbecue.index = 0;
	}

	OSMutexPost(barbecue.mutex);
	setFFStatus(FIREFIGHTER_WAITING);
}
