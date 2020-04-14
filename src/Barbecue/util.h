/*
 * util.h
 *
 * Liefert verschiedene Routinen, die mehrere Tasks benutzen
 *
 * Für spezifischeren Kommentar s. util.c
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#ifndef UTIL_H
#define UTIL_H
#include "types.h"

// Variablen für Speichermanagement
OS_MEM* memoryPool;
INT8U memoryBuffer[MEMORY_BLOCK_SIZE * MEMORY_BLOCK_NUM];

// Enum zum Spefizieren wo eine Nachricht eingefügt werden soll
typedef enum {
	QUEUE, QUEUE_FRONT
} EventType;

/*
 * Gibt den übergebenen String plus den Fehlercode aus und hält das Programm an.
 *
 * @param message Nachricht die ausgegeben werden soll
 * @param err Fehlercode
 */
void printErrorAndHalt(char* message, INT8U err);

/*
 * Übergibt die Nachricht entweder vorne oder hinten an die gegebene Queue
 *
 * @param type Ob vorne oder hinten übergeben werden soll
 * @param trace Wer die Nachricht senden möchte (wichtig im Fehlerfall)
 * @param event Queue in welche geschrieben werden soll
 * @param message Zeiger auf den Speicherbereich der gesendet werden soll
 */
void sendMessage(EventType type, char *trace, OS_EVENT* event, void* message);


/*
 * Holt sich aus dem gegebenen Memorypool einen Speicherblock
 *
 * @param trace wer den Speicherblock holen möchte
 * @param memory Zeiger auf den Memorypool
 * @return void* Zeiger auf den Speicherblock
 */
void* getMemory(char* trace, OS_MEM* memory);

/*
 * Gibt den Speicherblock an den Memorypool zurück
 *
 * @param trace wer den Speicherblock zurückgeben möchte
 * @param memory Zeiger auf den Memorypool
 * @param message Zeiger auf den Speicherblock der zurückgegeben werden soll
 */
void returnMemory(char* trace, OS_MEM* memory, void* message);

/*
 * Überprüft, ob die gegebene Queue voll ist
 *
 * @param event Queue die überprüft werden soll
 * @return boolean ob voll oder nicht
 */
boolean isQueueFull(OS_EVENT* event);

/*
 * Überprüft, ob die gegebene Queue leer ist
 *
 * @param event Queue die überprüft werden soll
 * @return boolean ob voll oder nicht
 */
boolean isQueueEmpty(OS_EVENT* event);

#endif
