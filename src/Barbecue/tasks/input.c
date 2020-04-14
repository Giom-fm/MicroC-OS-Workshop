/*
 * input.c
 *
 * Unterscheidet die PS/2-Signale und schickt die Kommandos
 * (Temperaturregelung, arbeiten/Bier, ..) an die entsprechenden Tasks
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#include "input.h"
#include "chef.h"
#include <stdio.h>

/*
 * Sendet je nach Input Nachrichten an den Koch, den Schlachter und den Grill
 *
 * Nutzt die inputQueue um Nachrichten vom PS/2-Callback zu buffern und nutzt
 * die jeweiligen Queues (chefCommandQueue, butcherCommandQueue, barbecueCommandQueue)
 * um Kommandos an die Tasks zu schicken.
 *
 * @param pdata Zeiger auf übergebenen Parameterspeicherbereich (ungenutzt)
 */
void inputTask(void* pdata) {
	INT8U err;
	while (1) {

		//printf("Inputtask wartet\n");
		INT8U key;
		void *message = OSQPend(input.inputQueue, 0, &err);
		if (err) {
			printErrorAndHalt("Error: Failed to get Input from ISR", err);
		} else {
			key = *((INT8U*) message);
			returnMemory("Input", input.memory, message);
		}

		//printf("Inputtask got %d from ISR\n", key);

		switch (key) {
		case 'q': {
			sendCommandToChef(WORK, QUEUE);
			break;
		}
		case 'w': {
			sendCommandToChef(SLEEP, QUEUE_FRONT);
			break;
		}
		case 'e': {
			OSTaskResume(PRIORITY_CHEF);
			break;
		}
		case 'a': {
			sendCommandToButcher(WORK, QUEUE);
			break;
		}
		case 's': {
			sendCommandToButcher(SLEEP, QUEUE_FRONT);
			break;
		}
		case 'd': {
			INT8U err;
			OSMutexPend(butcher.mutex, 0, &err);
			if (err) {
				printErrorAndHalt("Input: Failed to get Butcher Mutex", err);
			}
			butcher.status = butcher.old_status;
			OSMutexPost(butcher.mutex);
			OSTaskResume(PRIORITY_BUTCHER);
			break;
		}
		case 213: {// ARROW LEFT
			sendCommandToBarbecue(DOWN10, QUEUE);
			break;
		}

		case 214: {// ARROR RIGHT
			sendCommandToBarbecue(UP10, QUEUE);
			break;
		}

		case 215: {//ARROW DOWN
			sendCommandToBarbecue(DOWN50, QUEUE);
			break;
		}

		case 212: {//ARROW UP
			sendCommandToBarbecue(UP50, QUEUE);
			break;
		}

		default: {
		}
		}

	}
}

/*
 * Sendet die Nachricht an den Koch
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 * @param eventType Vorne/Hinten in die Queue schreiben
 */
void sendCommandToChef(Command command, EventType eventType) {
	Command *message = getMemory("input zu chef", input.memory);
	*message = command;
	sendMessage(eventType, "input chef", input.chefCommandQueue,
			(void*) message);
}

/*
 * Sendet die Nachricht an den Schlachter
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 * @param eventType Vorne/Hinten in die Queue schreiben
 */
void sendCommandToButcher(Command command, EventType eventType) {
	Command *message = getMemory("input zu butcher", input.memory);
	*message = command;
	sendMessage(eventType, "input butcher", input.butcherCommandQueue,
			(void*) message);
}

/*
 * Sendet die Nachricht an den Grill
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 * @param eventType Vorne/Hinten in die Queue schreiben
 */
void sendCommandToBarbecue(Temperature temperature, EventType eventType) {
	Temperature *message = getMemory("input zu bbq", input.memory);
	*message = temperature;
	sendMessage(eventType, "input bbq", input.barbecueCommandQueue,
			(void*) message);
}

/*
 * Callback Funktion von PS2 welche Taste gedrückt wurde, schickt diese
 * weiter an den Inputtask
 *
 * @param key Bytecode der gedrückten Taste
 */
void keyboardCallback(INT8U key) {
	OSIntEnter();

	INT8U *message = getMemory("input callback", input.memory);
	*message = key;
	//printf("ISR got: %d and sent %d\n", key, *message);
	sendMessage(QUEUE, "input callback", input.inputQueue, (void*) message);

	OSIntExit();
}
