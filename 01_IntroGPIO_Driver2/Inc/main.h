/*
 * main.h
 *
 *  Created on: Dec 5, 2024
 *      Author: ingfisica
 */

#ifndef MAIN_H_
#define MAIN_H_

#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>


typedef enum{
	LOCK,
	UNLOCK
} e_PossibleStates;

typedef struct{
	e_PossibleStates state;
	uint8_t accessCounter;
} fsm_states_t;

// definiciones de las cabeceras de las funciones del main
extern void configMagic(void);
extern void clear_Scanf(void);
extern void read_input(char *data);

#endif /* MAIN_H_ */
