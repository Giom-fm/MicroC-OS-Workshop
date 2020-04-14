/*
 * firefighter.h
 *
 * Periodischer 60-Sekunden Timer um den Grill zu löschen und die Würstchen
 * herunterzunehmen.
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#ifndef FIREFIGHTER_H_
#define FIREFIGHTER_H_

#define FIREFIGHTER_PERIOD 		(OS_TICKS_PER_SEC / OS_TMR_CFG_TICKS_PER_SEC * 6) // 60s
# include "../types.h"

typedef struct {
	FireFighterStatus status;
	OS_EVENT* mutex;
} Firefighter;

Firefighter firefighter;


/*
 * Erzeugt den Timer der alle 60s den Grill checkt
 */
void createFirefighterTimer();

/*
 * Setzt den Status des Feuerwehr-Timers
 *
 * @param status Status der gesetzt werden soll
 */
void setFFStatus(FireFighterStatus status);

/*
 * Checkt ob der Grill in Flammen steht.
 *
 * @param ptmr, callback_arg Parameter für die Callbackfunktion des Timers
 *                           hier ungenutzt
 */
void checkBarbecue(void *ptmr, void *callback_arg);

#endif /* FIREFIGHTER_H_ */
