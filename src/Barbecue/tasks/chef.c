/*
 * chef.c
 *
 * Empfängt Kommandos vom Input ob der Koch eine Wurst auf den Grill packen soll,
 * Bier trinkt oder nichts konkretes zu tun (woraufhin er ein zufälliges Würstchen
 * checkt und wenn nötig dreht oder entfernt)
 *
 * Legt Würstchen aus dem Kühlschrank auf den Grill (5s), checkt ein Würstchen
 * auf dem Grill (1s), dreht ein Würstchen (5s), entfernt ein fertiges
 * Würstchen (10s) oder trinkt Bier.
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#include "chef.h"
#include <stdio.h>
#include "../util.h"
#include "input.h"
#include "output.h"
#include "barbecue.h"

// Zähler für Zufall
INT8U counter;

/*
 * Überprüft die Würstchen auf dem Grill, entfernt die Fertigen und legt neue Würstchen
 * auf Kommando nach
 *
 * Empfängt Kommandos vom Input per Queue und nutzt den chef.mutex um seinen Status zu
 * setzen und den barbecue.mutex um die Würstchen zu bearbeiten
 *
 * @param pdata Zeiger auf einen übergebenen Speicherbereich
 */
void chefTask(void* pdata) {

	setChefStatus(CHEF_CHECK);

	while (1) {
		INT8U err;

		void* message = OSQAccept(input.chefCommandQueue, &err);
		if (message != (void *) 0) {
			Command command = *((Command*) message);
			returnMemory("Chef", input.memory, message);
			handleChefCommand(command);
		} else {
			checkSausages();
		}

		if (counter == 255) {
			counter = 0;
		} else {
			counter++;
		}
	}
}

/*
 * Setzt den Status des Kochs
 *
 * @param status Status der gesetzt werden soll
 */
void setChefStatus(ChefStatus status) {
	INT8U err;
	OSMutexPend(chef.mutex, 0, &err);
	if (err) {
		printErrorAndHalt("Chef: Failed to set Status", err);
	}
	chef.status = status;
	OSMutexPost(chef.mutex);
}

/*
 * Lässt den Koch arbeiten oder Bier trinken
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 */
void handleChefCommand(Command command) {

	switch (command) {
	case WORK: {
		INT8U err;
		void* message = OSQAccept(butcher.fridgeQueue, &err);
		if (message != (void *) 0) {
			Sausage* sausage = (Sausage*) message;
			//printf("Chef: Get Sausage: %d\n", (*sausage).id);
			insertSausage(sausage);
		}
		break;
	}
	case SLEEP: {
		setChefStatus(CHEF_BEER);
		OSTaskSuspend(OS_PRIO_SELF);
		break;
	}
	}

}

/*
 * Überprüft die Würstchen auf dem Grill und entfernt/dreht diese
 */
void checkSausages() {
	INT8U err;
	INT8U index;
	Sausage sausage;
	SausageSides grilling;
	float side = 0.0;

	OSMutexPend(barbecue.mutex, 0, &err);
	if (barbecue.index > 0 && !barbecue.inflamed) {
		index = counter % barbecue.index;
		sausage = *barbecue.sausages[index];
		grilling = sausage.grilling;
		side = sausage.sides[grilling];
	}
	OSMutexPost(barbecue.mutex);

	setChefStatus(CHEF_CHECK);

	OSTimeDly(WAIT_1);

	if (side > 80.0) {
		if (grilling == RIGHT) {
			removeSausage(index);
		} else {
			turnSausage(index);
		}
	}
}

/*
 * Legt die Wurst auf den Grill
 *
 * @param sausage Wurst die gebraten werden soll
 */
void insertSausage(Sausage* sausage) {
	INT8U err;
	setChefStatus(CHEF_INSERT);
	OSTimeDly(WAIT_1 * 5);
	OSMutexPend(barbecue.mutex, 0, &err);
	if (err) {
		printErrorAndHalt("Chef-Insert: Failed to get Mutex", err);
	}

	if (barbecue.index < BARBECUE_SIZE) {
		//printf("Sausage %d auf dem Grill\n", (*sausage).id);
		barbecue.sausages[barbecue.index] = sausage;
		barbecue.sausages[barbecue.index + 1] = NULL;
		barbecue.index++;
	} else {
		returnMemory("Chef", chef.memory, sausage);
	}

	OSMutexPost(barbecue.mutex);
}

/*
 * Entfernt die Wurst am gegebenen Index vom Grill
 *
 * @param index Index an dem die Wurst entnommen werden soll
 */
void removeSausage(int index) {
	INT8U err;

	setChefStatus(CHEF_REMOVE);
	OSTimeDly(WAIT_1 * 10);

	OSMutexPend(barbecue.mutex, 0, &err);
	if (err) {
		printErrorAndHalt("Chef-Remove: Failed to get Mutex", err);
	}

	if (barbecue.index > 0) {
		returnMemory("Chef", chef.memory, barbecue.sausages[index]);
		barbecue.index--;
		barbecue.sausages[index] = barbecue.sausages[barbecue.index];
		barbecue.sausages[barbecue.index] = NULL;
	}

	OSMutexPost(barbecue.mutex);
}

/*
 * Dreht die Wurst am gegebenen Index
 *
 * @param index Index an dem die Wurst gedreht werden soll
 */
void turnSausage(INT8U index) {
	INT8U err;
	Sausage *sausage;

	setChefStatus(CHEF_TURN);
	OSTimeDly(WAIT_1 * 5);

	OSMutexPend(barbecue.mutex, 0, &err);
	if (err) {
		printErrorAndHalt("Chef-Turn: Failed to get Mutex", err);
	}

	sausage = barbecue.sausages[index];

	switch ((*sausage).grilling) {
	case TOP:
		(*sausage).grilling = LEFT;
		break;
	case LEFT:
		(*sausage).grilling = BOTTOM;
		break;
	case BOTTOM:
		(*sausage).grilling = RIGHT;
		break;
	case RIGHT:
		printErrorAndHalt("Chef-Turn: Aufgerufen bei der letzten Seite", err);
	}

	OSMutexPost(barbecue.mutex);
}
