/*
 * main.c
 *
 * Hauptfunktion zur Initialisierung der Tasks, der Queues, der Mutexe,
 * des Memorypools, der VGA-Schnittstelle und der PS-2 Schnittstelle
 * sowie das Starten des Multiplexing.
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ps2/ps2.h"
#include "ps2/ps2_keyscan.h"
#include "types.h"
#include "util.h"
#include "tasks/butcher.h"
#include "tasks/chef.h"
#include "tasks/barbecue.h"
#include "tasks/input.h"
#include "tasks/firefighter.h"
#include "tasks/output.h"

/*
 * Methode um den Speicherbereich zu initialisieren aus dem später die Speicherblöcke
 * entnommen werden.
 */
void createMemory() {
	INT8U err = OS_ERR_NONE;
	printf("Alloc Memory... ");

	memoryPool = OSMemCreate(&memoryBuffer[0], MEMORY_BLOCK_NUM,
			MEMORY_BLOCK_SIZE, &err);

	if (err != OS_ERR_NONE) {
		printErrorAndHalt("Error CREATION OF MEMORY POOL FAILED\n", err);
	}
	printf("OK\n");
}

/*
 * Erzeugt den Schlachtertask, der für das Erstellen von Würstchen zuständig ist.
 */
void createButcherTask() {

	butcher.memory = memoryPool;

	INT8U err;
	printf("Creating Butcher Mutex... ");
	butcher.mutex = OSMutexCreate(2, &err);
	if (err) {
		printErrorAndHalt("ERROR: COULD NOT INIT BUTCHER-MUTEX", err);
	}
	printf("OK\n");

	printf("Creating Fridge Queue... ");
	butcher.fridgeQueue = OSQCreate(&butcher.message[0], FRIDGE_QUEUE_LENGTH);
	printf("OK\n");

	printf("Starting Butcher Task... ");
	err = OSTaskCreateExt(butcherTask, NULL,
			(void *) &(butcher.stack[TASK_STACK_SIZE - 1]), PRIORITY_BUTCHER,
			PRIORITY_BUTCHER, butcher.stack, TASK_STACK_SIZE, NULL, 0);
	if (err) {
		printErrorAndHalt("ERROR: COULD NOT START BUTCHER-TASK", err);
	}
	printf("OK\n");
}

/*
 * Erzeugt den Kochtask, der die Würstchen auf dem Grill checkt und auf Kommando Würstchen
 * auf den Grill legt.
 */
void createChefTask() {

	chef.memory = memoryPool;

	INT8U err;
	printf("Creating Chef Mutex... ");
	chef.mutex = OSMutexCreate(3, &err);
	if (err) {
		printErrorAndHalt("ERROR: COULD NOT INIT CHEF-MUTEX", err);
	}
	printf("OK\n");

	printf("Creating Chef Queue... ");
	chef.toBarbecueQueue = OSQCreate(&chef.message[0], BARBECUE_QUEUE_LENGTH);
	printf("OK\n");

	printf("Starting Chef Task... ");
	err = OSTaskCreateExt(chefTask, NULL,
			(void *) &(chef.stack[TASK_STACK_SIZE - 1]), PRIORITY_CHEF,
			PRIORITY_CHEF, chef.stack, TASK_STACK_SIZE, NULL, 0);
	if (err) {
		printErrorAndHalt("ERROR: COULD NOT START CHEF-TASK", err);
	}
	printf("OK\n");
}

/*
 * Erzeugt den Grilltask (Physics), der die Würstchen grillt und entflammen kann.
 */
void createBarbecueTask() {
	INT8U err;
	barbecue.memory = memoryPool;

	//Mutex um den Zugriff auf das Würstchen-Array zu regeln
	barbecue.mutex = OSMutexCreate(1, &err);
	if (err) {
		printErrorAndHalt("Error: COULD NOT CREATE BBQ-MUTEX", err);
	}

	printf("Starting Barbecue Task... ");
	err = OSTaskCreateExt(barbecueTask, NULL,
			(void *) &(barbecue.stack[TASK_STACK_SIZE - 1]), PRIORITY_BARBECUE,
			PRIORITY_BARBECUE, barbecue.stack, TASK_STACK_SIZE, NULL, 0);
	if (err) {
		printErrorAndHalt("ERROR: COULD NOT START BARBECUE-TASK", err);
	}
	printf("OK\n");

}

/*
 * Erzeugt den Inputtask, der die einzelnen MessageQueues zu den Tasks enthält und
 * den Output der PS2 Tastatur empfängt und verarbeitet.
 */
void createInputTask() {
	input.memory = memoryPool;

	printf("Creating Chef CommandQueue... ");
	input.chefCommandQueue = OSQCreate(&input.chefCommandQueueBuffer[0],
			CHEF_CMD_QUEUE_LENGTH);
	printf("OK\n");

	printf("Creating Butcher CommandQueue ... ");
	input.butcherCommandQueue = OSQCreate(&input.butcherCommandQueueBuffer[0],
			BUTCHER_CMD_QUEUE_LENGTH);
	printf("OK\n");

	printf("Creating BBQ CommandQueue ... ");
	input.barbecueCommandQueue = OSQCreate(
			&input.barbecueCommandQueueBuffer[0], BARBECUE_CMD_QUEUE_LENGTH);
	printf("OK\n");

	printf("Creating InputQueue ... ");
	input.inputQueue
			= OSQCreate(&input.inputQueueBuffer[0], INPUT_QUEUE_LENGTH);
	printf("OK\n");

	printf("Starting Input Task... ");
	INT8U err = OSTaskCreateExt(inputTask, NULL,
			(void *) &(input.stack[TASK_STACK_SIZE - 1]), PRIORITY_INPUT,
			PRIORITY_INPUT, input.stack, TASK_STACK_SIZE, NULL, 0);
	if (err) {
		printErrorAndHalt("ERROR: COULD NOT START INPUT-TASK", err);
	}
	printf("OK\n");
}

/*
 * Erzeugt den Outputtask, der die einzelnen Zustände der Tasks abfragt und diese
 * per VGA auf dem Bildschirm anzeigt.
 */
void createOutputTask() {
	printf("Starting Output Task... ");
	INT8U err = OSTaskCreateExt(outputTask, NULL,
			(void *) &(output.stack[TASK_STACK_SIZE - 1]), PRIORITY_OUTPUT,
			PRIORITY_OUTPUT, output.stack, TASK_STACK_SIZE, NULL, 0);
	if (err) {
		printErrorAndHalt("ERROR: COULD NOT START OUTPUT-TASK", err);
	}
	printf("OK\n");

}

/*
 * Methode um die Aufrufe zur Erzeugung der Tasks zu bündeln.
 */
void createTasks() {
	createInputTask();
	createButcherTask();
	createChefTask();
	createBarbecueTask();
	createFirefighterTimer();
	createOutputTask();
}

/*
 * Hauptmethode des Programms die alle benötigten Komponenten initialisiert und
 * das Scheduling startet.
 */
int main(void) {
	printf("Starting...\n");
	ps2_init();
	ps2_keyListener(keyboardCallback);
	vga_dma_init();
	createMemory();
	createTasks();
	OSStart();
	return 0;
}
