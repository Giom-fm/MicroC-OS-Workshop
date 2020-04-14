/*
 * input.h
 *
 * Schickt je nach empfangenem PS-2-Signals Kommandos an die Tasks
 *
 * Für spezifischeren Kommentar s. input.c
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#ifndef INPUT_H
#define INPUT_H
#include "../types.h"
#include "../util.h"

//Struct um den Input zu repräsentieren
typedef struct {
	OS_EVENT *butcherCommandQueue;
	void *butcherCommandQueueBuffer[BUTCHER_CMD_QUEUE_LENGTH];
	OS_EVENT *chefCommandQueue;
	void *chefCommandQueueBuffer[CHEF_CMD_QUEUE_LENGTH];
	OS_EVENT *barbecueCommandQueue;
	void *barbecueCommandQueueBuffer[BARBECUE_CMD_QUEUE_LENGTH];
	OS_EVENT *inputQueue;
	void *inputQueueBuffer[BARBECUE_CMD_QUEUE_LENGTH];
	OS_MEM *memory;
	OS_STK stack[TASK_STACK_SIZE];
} Input;

//Globale Variable für Zugriff
Input input;

/*
 * Sendet je nach Input Nachrichten an den Koch, den Schlachter und den Grill
 *
 * @param pdata Zeiger auf übergebenen Parameterspeicherbereich (ungenutzt)
 */
void inputTask(void* pdata);

/*
 * Sendet die Nachricht an den Koch
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 * @param eventType Vorne/Hinten in die Queue schreiben
 */
void sendCommandToChef(Command command, EventType eventType);

/*
 * Sendet die Nachricht an den Schlachter
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 * @param eventType Vorne/Hinten in die Queue schreiben
 */
void sendCommandToButcher(Command command, EventType eventType);

/*
 * Sendet die Nachricht an den Grill
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 * @param eventType Vorne/Hinten in die Queue schreiben
 */
void sendCommandToBarbecue(Temperature temperature, EventType eventType);

/*
 * Callback Funktion von PS2 welche Taste gedrückt wurde, schickt diese
 * weiter an den Inputtask
 *
 * @param key Bytecode der gedrückten Taste
 */
void keyboardCallback(INT8U key);

#endif

