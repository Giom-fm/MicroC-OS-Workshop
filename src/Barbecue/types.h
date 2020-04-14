/*
 * types.h
 *
 * Definiert einige Konstanten und structs, die von mehreren Tasks benutzt
 * werden (Prioritäten, Queue Länge, Größe der Memoryblöcke, ... /
 * enum für boolean, Struct um Wurstinfos zu enthalten, ...)
 *
 * Autor: tinf102026 (Torge Schuster), tinf101922 (Guillaume Fournier-Mayer
 */
#ifndef TYPES_H
#define TYPES_H
#include "includes.h"

#define WAIT_1						(OS_TICKS_PER_SEC * 1)
#define WAIT_60						(OS_TICKS_PER_SEC * 60)
#define TASK_STACK_SIZE				(2048)
#define MEMORY_BLOCK_SIZE 			(24)
#define MEMORY_BLOCK_NUM 			(200)
#define FRIDGE_QUEUE_LENGTH 		(10)
#define BARBECUE_QUEUE_LENGTH 		(10)
#define CHEF_CMD_QUEUE_LENGTH		(10)
#define BUTCHER_CMD_QUEUE_LENGTH	(10)
#define BARBECUE_CMD_QUEUE_LENGTH	(10)
#define INPUT_QUEUE_LENGTH	        (10)

#define PRIORITY_FIREFIGHTER 	5
#define PRIORITY_BARBECUE		6
#define PRIORITY_CHEF			9
#define PRIORITY_BUTCHER		12
#define PRIORITY_INPUT			15
#define PRIORITY_OUTPUT			18

typedef enum {
	FALSE, TRUE
} boolean;

typedef enum {
	TOP, LEFT, BOTTOM, RIGHT
} SausageSides;

typedef enum {
	WORK, SLEEP
} Command;

typedef enum {
	UP10, DOWN10, UP50, DOWN50
} Temperature;

typedef enum {
	CHEF_BEER, CHEF_INSERT, CHEF_CHECK, CHEF_TURN, CHEF_REMOVE
}ChefStatus;

typedef enum {
	BUTCHER_BEER, BUTCHER_WAITING, BUTCHER_WORKING
}ButcherStatus;

typedef enum {
	FIREFIGHTER_WAITING, EXTINGUISHING
}FireFighterStatus;

typedef struct {
	INT8U id;
	float sides[4];
	SausageSides grilling;
} Sausage;

#endif
