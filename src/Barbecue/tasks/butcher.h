/*
 * butcher.h
 *
 * Produziert W�rstchen, trinkt Bier oder hat nichts zu tun.
 *
 * F�r spezifischeren Kommentar s. butcher.c
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#ifndef BUTCHER_H_
#define BUTCHER_H_
#include "../types.h"

#define BUTCHER_PERIOD 		(OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC * 6) // 60s
// Struct um den Schlachter zu repr�sentieren
typedef struct {
	OS_EVENT *fridgeQueue;
	OS_EVENT *mutex;
	ButcherStatus status;
	ButcherStatus old_status;
	void *message[FRIDGE_QUEUE_LENGTH];
	OS_MEM *memory;
	OS_STK stack[TASK_STACK_SIZE];
} Butcher;


// Globale Schlachtervariable zum Zugriff
Butcher butcher;

/*
 * Erzeugt W�rstchen und legt sie in den K�hlschrank
 *
 * @param pdata Zeiger auf �bergebene Parameter (ungenutzt)
 */
void butcherTask(void* pdata);

/*
 * Setzt den Status des Schlachters
 *
 * @param status Status der gesetzt werden soll
 */
void setButcherStatus(ButcherStatus status);

/*
 * Erzeugt W�rstchen und legt sie in den K�hlschrank
 *
 * @param id ID zur Identifizierung
 */
Sausage createSausage(int id);

/*
 * L�sst den Schlachter arbeiten oder Bier trinken
 *
 * @param command Arbeiten/Pause machen (WORK, SLEEP)
 */
void handleButcherCommand(Command command);

/*
 * Startet die Produktion von W�rstchen wegen leerem K�hlschranks
 *
 * @param ptmr, callback_arg Parameter f�r die Callbackfunktion des Timers
 *                           hier ungenutzt
 */
void startWorking(void *ptmr, void *callback_arg);

#endif /* BUTCHER_H_ */
