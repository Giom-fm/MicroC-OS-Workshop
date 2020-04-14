/*
 * butcher.c
 *
 * Produziert W�rstchen, trinkt Bier oder hat nichts zu tun.
 *
 * Nimmt Kommandos des Inputs entgegen
 * (Produziere W�rstchen, trinke Bier, mache weiter)
 * und f�ngt nach 60 Sekunden leerem K�hlschrank selber an, W�rstchen zu
 * produzieren und legt diese in den K�hlschrank (repr�sentiert als Queue)
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#include "butcher.h"
#include <stdio.h>
#include "../util.h"
#include "input.h"
#include "output.h"

// Globales Flag ob der Timer zum selbst�ndigen Arbeiten gesetzt ist
boolean timerSet;
OS_TMR *timerButcher;

/*
 * Erzeugt W�rstchen und legt sie in den K�hlschrank
 *
 * Nutzt den butcher.mutex um seinen Status zu setzen und
 * empf�ngt per Queue Kommandos vom Input und sendet ebenfalls per Queue
 * W�rstchen-Zeiger an den Koch
 *
 * @param pdata Zeiger auf �bergebene Parameter (ungenutzt)
 */
void butcherTask(void* pdata) {
	INT8U err;
	int id = 1;
	timerSet = FALSE;

	timerButcher = OSTmrCreate(BUTCHER_PERIOD, 0, OS_TMR_OPT_ONE_SHOT,
			startWorking, (void*) 0, "BUTCHERTMR", &err);
	if (err) {
		printErrorAndHalt("Butcher: Timer Init Failed", err);
	}
	setButcherStatus(BUTCHER_WAITING);
	while (1) {
		void* message = OSQAccept(input.butcherCommandQueue, &err);
		if (message != (void*) 0) {
			//TODO ERRORHANDLING
			Command command = *((Command*) message);
			returnMemory("Butcher", input.memory, message);
			handleButcherCommand(command);
		} else {
			if (butcher.status == BUTCHER_WORKING) {
				if (isQueueFull(butcher.fridgeQueue)) {
					//printf("Butcher: Stop Making Sausage\n");
					setButcherStatus(BUTCHER_WAITING);
				} else {
					setButcherStatus(BUTCHER_WORKING);
					Sausage* sausageMessage = getMemory("Butcher Wurst",
							butcher.memory);
					*sausageMessage = createSausage(id++);
					sendMessage(QUEUE, "Butcher", butcher.fridgeQueue,
							(void*) sausageMessage);
				}
			} else {
				if (!timerSet && isQueueEmpty(butcher.fridgeQueue)) {
					timerSet = TRUE;
					OSTmrStart(timerButcher, &err);
					//printf("K�hlschrank leer, 60s Timer beginnt\n");
				}

				OSTimeDly(WAIT_1 / 10);
			}
		}
	}
}

/*
 * Setzt den Status des Schlachters
 *
 * @param status Status der gesetzt werden soll
 */
void setButcherStatus(ButcherStatus status) {
	INT8U err;
	OSMutexPend(butcher.mutex, 0, &err);
	if (err) {
		printErrorAndHalt("Butcher: Failed to set status", err);
	}
	if (status == BUTCHER_BEER) {
		butcher.old_status = butcher.status;
	}
	butcher.status = status;
	OSMutexPost(butcher.mutex);
}

/*
 * Erzeugt W�rstchen und legt sie in den K�hlschrank
 *
 * @param id ID zur Identifizierung
 */
Sausage createSausage(int id) {
	OSTimeDly(WAIT_1);
	Sausage sausage;
	sausage.id = id;
	int i;
	for (i = 0; i < 4; i++) {
		sausage.sides[i] = 0.0;
	}
	sausage.grilling = TOP;
	return sausage;
}

/*
 * L�sst den Schlachter arbeiten oder Bier trinken
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 */
void handleButcherCommand(Command command) {
	INT8U err;

	switch (command) {
	case WORK: {
		if (butcher.status != BUTCHER_WORKING && !isQueueFull(
				butcher.fridgeQueue)) {
			//printf("Butcher: Start Making Sausage\n");
			setButcherStatus(BUTCHER_WORKING);
			OSTmrStop(timerButcher, OS_TMR_OPT_NONE, (void*) 0, &err);
			if (err != OS_ERR_NONE && err != OS_ERR_TMR_STOPPED) {
				printErrorAndHalt("Butcher: Failed to stop Timer", err);
			}

		}
		break;
	}
	case SLEEP: {
		//printf("Butcher: Start Drinking Beer - Stop Working\n");
		setButcherStatus(BUTCHER_BEER);
		OSTmrStop(timerButcher, OS_TMR_OPT_NONE, (void*) 0, &err);
		if (err != OS_ERR_NONE && err != OS_ERR_TMR_STOPPED) {
			printErrorAndHalt("Butcher: Failed to stop Timer", err);
		}
		OSTaskSuspend(OS_PRIO_SELF);
		break;
	}
	}
}

/*
 * Startet die Produktion von W�rstchen wegen leerem K�hlschranks
 *
 * @param ptmr, callback_arg Parameter f�r die Callbackfunktion des Timers
 *                           hier ungenutzt
 */
void startWorking(void *ptmr, void *callback_arg) {
	timerSet = FALSE;
	setButcherStatus(BUTCHER_WORKING);
}

