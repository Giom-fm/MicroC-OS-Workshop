/*
 * chef.h
 *
 * Legt Würstchen aus dem Kühlschrank auf den Grill (5s), checkt ein Würstchen
 * auf dem Grill (1s), dreht ein Würstchen (5s), entfernt ein fertiges
 * Würstchen (10s) oder trinkt Bier.
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#ifndef CHEF_H_
#define CHEF_H_
#include "../types.h"
#include "butcher.h"

// Struct um den Koch zu repräsentieren
typedef struct {
	ChefStatus status;
	OS_EVENT* mutex;
	OS_EVENT *toBarbecueQueue;
	void *message[BARBECUE_QUEUE_LENGTH];
	OS_MEM *memory;
	OS_STK stack[TASK_STACK_SIZE];
} Chef;

// Globale Variable für den Zugriff
Chef chef;

/*
 * Überprüft die Würstchen auf dem Grill, entfernt die Fertigen und legt neue Würstchen
 * auf Kommando nach
 *
 * @param pdata Zeiger auf einen übergebenen Speicherbereich
 */
void chefTask(void* pdata);

/*
 * Setzt den Status des Kochs
 *
 * @param status Status der gesetzt werden soll
 */
void setChefStatus(ChefStatus status);

/*
 * Lässt den Koch arbeiten oder Bier trinken
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 */
void handleChefCommand(Command command);

/*
 * Überprüft die Würstchen auf dem Grill und entfernt/dreht diese
 */
void checkSausages();

/*
 * Holt sich ein Würstchen aus dem Kühlschrank
 *
 * @return Sausage Würstchen
 */
Sausage* getSausageFromFridge();

/*
 * Legt die Wurst auf den Grill
 *
 * @param sausage Wurst die gebraten werden soll
 */
void insertSausage(Sausage* sausage);

/*
 * Entfernt die Wurst am gegebenen Index vom Grill
 *
 * @param index Index an dem die Wurst entnommen werden soll
 */
void removeSausage(int index);

/*
 * Dreht die Wurst am gegebenen Index
 *
 * @param index Index an dem die Wurst gedreht werden soll
 */
void turnSausage(INT8U index);

#endif /* CHEF_H_ */
