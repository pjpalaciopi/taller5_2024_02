/**
 ******************************************************************************
 * @file           : main.c
 * @author         : pjpalaciopi@unal.edu.co
 * @brief          : Here starts the magic!!
 ******************************************************************************
 */
#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>


int main(void)
{

	while (1) {
	}

	return 0;
}

uint16_t operacionClase(uint16_t budget, uint16_t cost){
	uint16_t auxOperation;
	auxOperation = budget - cost;
	return auxOperation;
}
