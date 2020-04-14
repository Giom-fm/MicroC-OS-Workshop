/*
 * chef.h
 *
 * Legt W�rstchen aus dem K�hlschrank auf den Grill (5s), checkt ein W�rstchen
 * auf dem Grill (1s), dreht ein W�rstchen (5s), entfernt ein fertiges
 * W�rstchen (10s) oder trinkt Bier.
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#ifndef CHEF_H_
#define CHEF_H_
#include "../types.h"
#include "butcher.h"

// Struct um den Koch zu repr�sentieren
typedef struct {
	ChefStatus status;
	OS_EVENT* mutex;
	OS_EVENT *toBarbecueQueue;
	void *message[BARBECUE_QUEUE_LENGTH];
	OS_MEM *memory;
	OS_STK stack[TASK_STACK_SIZE];
} Chef;

// Globale Variable f�r den Zugriff
Chef chef;

/*
 * �berpr�ft die W�rstchen auf dem Grill, entfernt die Fertigen und legt neue W�rstchen
 * auf Kommando nach
 *
 * @param pdata Zeiger auf einen �bergebenen Speicherbereich
 */
void chefTask(void* pdata);

/*
 * Setzt den Status des Kochs
 *
 * @param status Status der gesetzt werden soll
 */
void setChefStatus(ChefStatus status);

/*
 * L�sst den Koch arbeiten oder Bier trinken
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 */
void handleChefCommand(Command command);

/*
 * �berpr�ft die W�rstchen auf dem Grill und entfernt/dreht diese
 */
void checkSausages();

/*
 * Holt sich ein W�rstchen aus dem K�hlschrank
 *
 * @return Sausage W�rstchen
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
