/*
 * util.c
 *
 * Liefert verschiedene Routinen bzgl. Memorymanagement, Queue Abfragen
 * und senden der Nachrichten, die von mehreren Tasks benutzt werden.
 *
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#include "util.h"
#include <stdio.h>
#include <string.h>
#include "./tasks/input.h"

/*
 * Gibt den übergebenen String plus den Fehlercode aus und hält das Programm an.
 *
 * @param message Nachricht die ausgegeben werden soll
 * @param err Fehlercode
 */
void printErrorAndHalt(char* message, INT8U err) {
	printf("%s: %d\n", message, err);
	while (1)
		;
}

/*
 * Übergibt die Nachricht entweder vorne oder hinten an die gegebene Queue
 *
 * @param type Ob vorne oder hinten übergeben werden soll
 * @param trace Wer die Nachricht senden möchte (wichtig im Fehlerfall)
 * @param event Queue in welche geschrieben werden soll
 * @param message Zeiger auf den Speicherbereich der gesendet werden soll
 */
void sendMessage(EventType type, char *trace, OS_EVENT* event, void* message) {
	INT8U err = OS_ERR_NONE;
	switch (type) {
	case QUEUE: {
		err = OSQPost(event, message);
		if (err == OS_ERR_Q_FULL) {
			err = OS_ERR_NONE;
			returnMemory(trace, input.memory, message);
			//printf("Lost something %s\n", trace);
		}
		break;
	}
	case QUEUE_FRONT: {
		err = OSQPostFront(event, message);
		if (err == OS_ERR_Q_FULL) {
			err = OS_ERR_NONE;
			returnMemory(trace, input.memory, message);
			//printf("Lost something %s\n", trace);
		}
		break;
	}
	}

	if (err) {
		char errorMsg[50] = "SendMessage Error from ";
		strcat(errorMsg, trace);
		printErrorAndHalt(errorMsg, err);
	}
}


/*
 * Holt sich aus dem gegebenen Memorypool einen Speicherblock
 *
 * @param trace wer den Speicherblock holen möchte
 * @param memory Zeiger auf den Memorypool
 * @return void* Zeiger auf den Speicherblock
 */
void* getMemory(char* trace, OS_MEM* memory) {
	INT8U err;
	void *memoryBlock = OSMemGet(memory, &err);
	char errorMsg[50] = "Error: Failed to get Memory from ";

	if (err) {
		strcat(errorMsg, trace);
		printErrorAndHalt(errorMsg, err);
	}

	if (memoryBlock == (void *) 0) {
		strcat(errorMsg, trace);
		strcat(errorMsg, "Memblock 0");
		printErrorAndHalt(errorMsg, err);
	}
	return memoryBlock;
}

/*
 * Gibt den Speicherblock an den Memorypool zurück
 *
 * @param trace wer den Speicherblock zurückgeben möchte
 * @param memory Zeiger auf den Memorypool
 * @param message Zeiger auf den Speicherblock der zurückgegeben werden soll
 */
void returnMemory(char* trace, OS_MEM* memory, void* message) {
	INT8U err = OSMemPut(memory, message);
	if (err) {
		char errorMsg[50] = "ReturnMemory Error from ";
		strcat(errorMsg, trace);
		printErrorAndHalt(errorMsg, err);
	}
}

/*
 * Überprüft, ob die gegebene Queue voll ist
 *
 * @param event Queue die überprüft werden soll
 * @return boolean ob voll oder nicht
 */
boolean isQueueFull(OS_EVENT* event) {
	INT8U err;
	OS_Q_DATA qdata;
	err = OSQQuery(event, &qdata);
	if (err != OS_ERR_NONE) {
		printErrorAndHalt("Error while Quering Queue Full", err);
	} else if (qdata.OSNMsgs == qdata.OSQSize) {
		return TRUE;
	}
	return FALSE;
}

/*
 * Überprüft, ob die gegebene Queue leer ist
 *
 * @param event Queue die überprüft werden soll
 * @return boolean ob voll oder nicht
 */
boolean isQueueEmpty(OS_EVENT* event) {
	INT8U err;
	OS_Q_DATA qdata;
	err = OSQQuery(event, &qdata);
	if (err != OS_ERR_NONE) {
		printErrorAndHalt("Error while Quering Queue Empty", err);
	} else if (qdata.OSNMsgs == 0) {
		return TRUE;
	}
	return FALSE;
}
