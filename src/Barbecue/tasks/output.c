/*
 * output.c
 *
 * Stellt Informationen der anderen Tasks per VGA dar.
 *
 * Nutzt dafür die vorimplementiere VGA Bibliothek und deren Routinen
 * sowie Mutexe zur Zugriffsregelung globaler Variablen
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#include "output.h"
#include "../util.h"
#include <stdio.h>
#include <stdlib.h>
#include "barbecue.h"
#include "chef.h"
#include "butcher.h"
#include "firefighter.h"
#include <string.h>

FireFighterStatus firefighterStatus = FIREFIGHTER_WAITING;

/*
 * Stellt die Zustände der Tasks und der Würstchen per VGA auf dem Bildschirm dar
 *
 * Nutzt mehrere Mutexe, um auf die Stati der einzelnen Tasks gesichert zuzugreifen
 * - barbecue.mutex
 * - chef.mutex
 * - butcher.mutex
 * - firefighter.mutex
 *
 * @param pdata Zeiger auf übergebenen Parameterspeicherbereich (ungenutzt)
 */
void outputTask(void *pdata) {
	vga_selFont(FONT_6x8); // kleinen Font setzen
	vga_clear(VGA_COLOR_BLACK);

	while (1) {
		draw_legend();
		draw_menue();
		draw_chefStatus();
		draw_butcherStatus();
		draw_fireFighterStatus();
		draw_fridge();
		draw_bbq();
		vga_flush();
		OSTimeDly(WAIT_1);
	}
}

/*
 * Gibt die Legende der möglichen Tasten aus
 */
void draw_legend() {
	INT8U colorKey = TEXT_COLOR;
	int xPosKey = X_KEY_START, yPosKey = Y_KEY_START;

	vga_setCurPos(xPosKey, yPosKey);
	vga_puts("Q: Wurst zu Grill", colorKey);

	yPosKey += 1;
	vga_setCurPos(xPosKey, yPosKey);
	vga_puts("W: Bier zu Chef", colorKey);

	yPosKey += 1;
	vga_setCurPos(xPosKey, yPosKey);
	vga_puts("E: Bier weg von Chef", colorKey);

	yPosKey += 1;
	vga_setCurPos(xPosKey, yPosKey);
	vga_puts("A: Wurst produzieren", colorKey);

	yPosKey += 1;
	vga_setCurPos(xPosKey, yPosKey);
	vga_puts("S: Bier zu Butcher", colorKey);

	yPosKey += 1;
	vga_setCurPos(xPosKey, yPosKey);
	vga_puts("D: Bier weg von Butcher", colorKey);

	yPosKey += 1;
	vga_setCurPos(xPosKey, yPosKey);
	vga_puts("Links: Temperatur - 10", colorKey);

	yPosKey += 1;
	vga_setCurPos(xPosKey, yPosKey);
	vga_puts("Rechts: Temperatur + 10", colorKey);

	yPosKey += 1;
	vga_setCurPos(xPosKey, yPosKey);
	vga_puts("Unten: Temperatur - 50", colorKey);

	yPosKey += 1;
	vga_setCurPos(xPosKey, yPosKey);
	vga_puts("Oben: Temperatur + 50", colorKey);
}

/*
 * Zeichnet das statische Menü der Tasks
 */
void draw_menue() {
	vga_setCurPos(X_TITLE, Y_FRIDGE);
	vga_puts("Kuehlschrank:", TEXT_COLOR);

	vga_setCurPos(X_TITLE, Y_BUTCHER);
	vga_puts("Schlachter:", TEXT_COLOR);

	vga_setCurPos(X_TITLE, Y_CHEF);
	vga_puts("Chefkoch:", TEXT_COLOR);

	vga_setCurPos(X_TITLE, Y_FIREFIGHTER);
	vga_puts("Feuerwehr:", VGA_COLOR_RED);

	vga_setCurPos(X_TITLE, Y_BBQ);
	vga_puts("Temperatur:", TEXT_COLOR);

	vga_setCurPos(X_TITLE, Y_BBQ + 2);
	vga_puts("Grill in Flammen? :", TEXT_COLOR);
}

/*
 * Gibt die Temperatur des Grills und den Zustand der Würstchen aus
 */
void draw_bbq() {
	int xPosBbq = X_VALUE, yPosBbq = Y_BBQ;
	char temperatur[6] = { 0 };
	char sideValue[6] = { 0 };
	INT8U err, colorBbq = TEXT_COLOR;

	OSMutexPend(barbecue.mutex, 0, &err);
	if (err != OS_ERR_NONE) {
		printErrorAndHalt("Output: Failed to get BBQ-Mutex", err);
	}
	clearValue(Y_BBQ);
	clearValue(Y_BBQ + 2);

	sprintf(temperatur, "%d", grillTemperature);
	vga_setCurPos(xPosBbq, yPosBbq);
	vga_puts(temperatur, colorBbq);

	yPosBbq += 2;
	vga_setCurPos(xPosBbq, yPosBbq);
	if (barbecue.inflamed) {
		vga_puts("Ja", VGA_COLOR_RED);
	} else {
		vga_puts("Nein", VGA_COLOR_BLUE);
	}

	clearBBQ();
	int i;
	for (i = 0; i < barbecue.index; i++) {
		Sausage sausage = (*barbecue.sausages[i]);
		int sausage_x = i * (SAUSAGE_WIDTH + SAUSAGE_SPACE) + 13;
		int j;
		for (j = 0; j < 4; j++) {
			int sideValuePercent = (int) sausage.sides[j];
			if (sideValuePercent > 110) {
				sideValuePercent = 110;
			}

			int side_percent = SAUSAGE_Y - sideValuePercent;

			sprintf(sideValue, "%d", sideValuePercent);
			int side_x = sausage_x + j * (SIDE_SPACE + SIDE_WIDTH);
			INT8U numberPosX = side_x / 6 + 1;
			vga_quad(side_x, side_percent, side_x + SIDE_WIDTH, SAUSAGE_Y,
					TRUE, 0x64);
			vga_setCurPos(numberPosX, NUMBER_POS_Y);
			vga_puts(sideValue, VGA_COLOR_WHITE);
		}
	}

	OSMutexPost(barbecue.mutex);
}

/*
 * Gibt den Zustand des Kühlschranks aus
 */
void draw_fridge() {
	char fridgeSausages[15] = { 0 };
	INT8U err;
	OS_Q_DATA fridge_data;
	err = OSQQuery(butcher.fridgeQueue, &fridge_data);

	if (err) {
		printErrorAndHalt("Fridge Query fehlgeschlagen: ", err);
	}

	INT16U sausages = fridge_data.OSNMsgs;
	INT16U size = fridge_data.OSQSize;

	sprintf(fridgeSausages, "%d / %d", sausages, size);

	clearValue(Y_FRIDGE);
	vga_setCurPos(X_VALUE, Y_FRIDGE);
	vga_puts(fridgeSausages, VGA_COLOR_WHITE);
}

/*
 * Gibt den Zustand des Kochs aus
 */
void draw_chefStatus() {
	INT8U err;
	if (OSMutexAccept(chef.mutex, &err)) {
		clearValue(Y_CHEF);
		vga_setCurPos(X_VALUE, Y_CHEF);
		switch (chef.status) {
		case CHEF_BEER:
			vga_puts("Bier", VGA_COLOR_YELLOW);
			break;
		case CHEF_CHECK:
			vga_puts("ist der Checker", VGA_COLOR_WHITE);
			break;
		case CHEF_INSERT:
			vga_puts("legt ne Wurst", VGA_COLOR_WHITE);
			break;
		case CHEF_TURN:
			vga_puts("kriegt nen Turn", VGA_COLOR_WHITE);
			break;
		case CHEF_REMOVE:
			vga_puts("loescht die Wurst aus", VGA_COLOR_WHITE);
			break;
		default:
			break;
		}
		OSMutexPost(chef.mutex);
	}
}

/*
 * Gibt den Zustand des Schlachters aus
 */
void draw_butcherStatus() {
	INT8U err;

	if (OSMutexAccept(butcher.mutex, &err)) {
		clearValue(Y_BUTCHER);
		vga_setCurPos(X_VALUE, Y_BUTCHER);
		switch (butcher.status) {
		case BUTCHER_BEER:
			vga_puts("Bier", VGA_COLOR_YELLOW);
			break;
		case BUTCHER_WAITING:
			vga_puts("doedelt rum", VGA_COLOR_WHITE);
			break;
		case BUTCHER_WORKING:
			vga_puts("produziert Wurst", VGA_COLOR_WHITE);
			break;
		default:
			break;
		}
		OSMutexPost(butcher.mutex);
	}

}

/*
 * Gibt den Zustand der Feuerwehr aus
 */
void draw_fireFighterStatus() {
	INT8U err;

	if (OSMutexAccept(firefighter.mutex, &err)) {
		clearValue(Y_FIREFIGHTER);
		vga_setCurPos(X_VALUE, Y_FIREFIGHTER);

		switch (firefighterStatus) {
		case FIREFIGHTER_WAITING:
			vga_puts("Pole Dance", VGA_COLOR_TEAL);
			break;
		case EXTINGUISHING:
			vga_puts("loeschen", VGA_COLOR_RED);
			break;
		default:
			break;
		}
		OSMutexPost(firefighter.mutex);
	}
}

/*
 * Löscht den Inhalt des Bildschirms vom gegebenen Index in die rechte Richtung
 *
 * @param yIdx y-Index in welcher Zeile gelöscht werden soll
 */
void clearValue(int yIdx) {
	vga_setCurPos(X_VALUE, yIdx);
	vga_puts("Timm ist coooooooooooooooooool", VGA_COLOR_BLACK);
}

/*
 * Löscht die Darstellung des BBQs vom Bildschirm
 */
void clearBBQ() {
	vga_quad(0, 350, DISPLAY_WIDTH, DISPLAY_HEIGHT, TRUE, VGA_COLOR_BLACK);
}
