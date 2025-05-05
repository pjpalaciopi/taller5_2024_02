/**
 ******************************************************************************
 * @file           : main.c
 * @author         : pjpalaciopi@unal.edu.co
 * @brief          : Main program body
 ******************************************************************************
*/
#include <stm32f4xx.h>
#include <stdint.h>
#include <stm32_assert.h>
#include "gpio_driver_hal.h"
#include "timer_driver_hal.h"

// Pin de prueba
GPIO_Handler_t userLed = {0}; // PA5

// Timer
Timer_Handler_t blinkTimer = {0};

// Start main() function
int main(void){
	// configuracion del Pin
	userLed.pGPIOx = 						GPIOA;
	userLed.pinConfig.GPIO_PinNumber = 		PIN_5;
	userLed.pinConfig.GPIO_PinMode = 		GPIO_MODE_OUT;
	userLed.pinConfig.GPIO_PinOutputType = 	GPIO_OTYPE_PUSHPULL;
	userLed.pinConfig.GPIO_PinOutputSpeed = GPIO_OSPEED_MEDIUM;
	userLed.pinConfig.GPIO_PinPuPdControl = GPIO_PUPDR_NOTHING;

	// cargamos la configuracion del Pin
	gpio_Config(&userLed);
	gpio_WritePin(&userLed, SET); // el led empieza prendido

	// configuracion de timer
	blinkTimer.pTIMx = 								TIM2; // uso el timer 2
	blinkTimer.TIMx_Config.TIMx_Prescaler = 		16000; // 16k/16M = 0.001s
	blinkTimer.TIMx_Config.TIMx_Period = 			250; // 0.001*250 = 0.25s
	blinkTimer.TIMx_Config.TIMx_mode = 				TIMER_UP_COUNTER;
	blinkTimer.TIMx_Config.TIMx_InterruptEnable = 	TIMER_INT_ENABLE;

	// cargamos la configuracion del timer
	timer_Config(&blinkTimer);
	timer_SetState(&blinkTimer, TIMER_ON); // tenemos que encender manualmente el timer

    /* Loop forever */
	while(1){

	}
}

// sobreescribimos la funcion del timer
void Timer2_Callback(void){
	gpio_TooglePin(&userLed);
}

void assert_failed(uint8_t* file, uint32_t line){
	while(1){

	}
}


