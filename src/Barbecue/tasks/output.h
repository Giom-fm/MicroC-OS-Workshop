/*
 * output.h
 *
 * Stellt Informationen der anderen Tasks per VGA dar.
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#ifndef OUTPUT_H_
#define OUTPUT_H_

#include "../types.h"
#include "../vga/vga_font.h"
#include "../vga/vga.h"

#define X_KEY_START 82
#define Y_KEY_START 1
#define X_VALUE 22
#define X_TITLE 1
#define Y_FRIDGE 1
#define Y_BUTCHER (Y_FRIDGE + 2)
#define Y_CHEF (Y_BUTCHER + 2)
#define Y_FIREFIGHTER (Y_CHEF + 2)
#define Y_BBQ (Y_FIREFIGHTER + 2)
#define SAUSAGE_SPACE (10)
#define SIDE_WIDTH (22)
#define SIDE_SPACE (6)
#define SAUSAGE_Y DISPLAY_HEIGHT
#define SAUSAGE_WIDTH (3* SIDE_SPACE + 4* SIDE_WIDTH + SAUSAGE_SPACE)
#define NUMBER_POS_Y (SAUSAGE_Y / 8 - 1)
#define TEXT_COLOR VGA_COLOR_WHITE

//Struct um den Output zu repräsentieren
typedef struct {
	OS_STK stack[TASK_STACK_SIZE];
} Output;

//Globale Variable für Zugriff
Output output;

/*
 * Stellt die Zustände der Tasks und der Würstchen per VGA auf dem Bildschirm dar
 *
 * @param pdata Zeiger auf übergebenen Parameterspeicherbereich (ungenutzt)
 */
void outputTask(void *pdata);

/*
 * Gibt die Legende der möglichen Tasten aus
 */
void draw_legend();

/*
 *
 */
void draw_menue();

/*
 * Gibt die Temperatur des Grills und den Zustand der Würstchen aus
 */
void draw_bbq();

/*
 * Gibt den Zustand des Kühlschranks aus
 */
void draw_fridge();

/*
 * Gibt den Zustand des Kochs aus
 */
void draw_chefStatus();

/*
 * Gibt den Zustand des Schlachters aus
 */
void draw_butcherStatus();

/*
 * Gibt den Zustand der Feuerwehr aus
 */
void draw_fireFighterStatus();

/*
 *
 */
void clearValue();

void clearBBQ();

#endif /* OUTPUT_H_ */
