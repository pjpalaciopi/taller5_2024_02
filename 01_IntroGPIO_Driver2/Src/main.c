/**
 ******************************************************************************
 * @file           : main.c
 * @author         : pjpalaciopi@unal.edu.co
 * @brief          : Here starts the magic!!
 ******************************************************************************
 */
#include "main.h"

fsm_states_t fsm_torniquete = {0};
char bufferData[16];

// Definicion de cabeceras del main
void initProgram(void);
e_PossibleStates state_machine_action(uint8_t event);

int main(void)
{

	configMagic();
	initProgram();

	// ciclo principal
	printf("Hola mundo\n");

	while (1) {
		// esta funcion lee la informacion que llega por el ppuerto serial desde el coolterm (un colo caracter)
		read_input(bufferData);

		// vemos si el valor leido es 'c' de 'coin'
		if (bufferData[0] == 'c'){
			printf("Coin inserted\n");
			state_machine_action(bufferData[0]);
		}
		else if (bufferData[0] == 'p' ){
			printf("Push entrance\n");
			state_machine_action(bufferData[0]);
		}
		else{
			printf("Wrong key\n");
		}
	}

	return 0;
}

void initProgram(void){
		fsm_torniquete.state = LOCK;
}

e_PossibleStates state_machine_action(uint8_t event){
	switch (fsm_torniquete.state){
	case LOCK:{
		switch (event){
		case 'c':{
			printf("it was LOCKED -> NOW it is UNLOCKED\n");
			printf("state: UNLOCKED\n");
			fsm_torniquete.state = UNLOCK;
			break;
		} // fin case 'c'

		case 'p':{
			printf("insert coin");
			printf("state: LOCKED\n");
			fsm_torniquete.state = LOCK;
			fsm_torniquete.accessCounter++;
			break;
		} // fin case 'p'
		}
		return fsm_torniquete.state;
	} // fin case LOCK

	case UNLOCK:{
		switch (event){
		case 'c':{
			printf("it was UNLOCKED -> now is UNLOCKED\n");
			printf("state: UNLOCK\n");
			fsm_torniquete.state = UNLOCK;
			fsm_torniquete.accessCounter++;
			break;
		} // fin case 'c'
		case 'p':{
			printf("it was UNLOCKED -> now is LOCKED\n");
			printf("state: LOCKED\n");
			fsm_torniquete.state = LOCK;
			break;
		} // fin case 'p'
		}
		return fsm_torniquete.state;
	} // fin case UNLOCK
	} // fin del switch-case principal
	return fsm_torniquete.state;
} // fin de la funcion
