/*
 * timer_driver_hal.c
 *
 *  Created on: Dec 6, 2024
 *      Author: pjpalaciopi@unal.edu.co
 */

#include "stm32f4xx.h"
#include "stm32_assert.h"

#include "timer_driver_hal.h"

// variable que guarda la referencia del periferico que se esta utilizando
TIM_TypeDef *ptrTimerUsed;

// Headers for private functions
static void timer_enable_clock_peripheral(Timer_Handler_t *pTimerHandler);
static void timer_set_prescaler(Timer_Handler_t *pTimerHandler);
static void timer_set_period(Timer_Handler_t *pTimerHandler);
static void timer_set_mode(Timer_Handler_t *pTimerHandler);
static void timer_config_interrupt(Timer_Handler_t *pTimerHandler);

void timer_Config(Timer_Handler_t *pTimerHandler){
	// guardamos una referencia al periferico que estamos utilizando
	ptrTimerUsed = pTimerHandler->pTIMx;

	//0. Desactivamos las interrupciones globales mientras configuramos el sistema
	__disable_irq();

	//1. Activar la señal de reloj del periferico requerido
	timer_enable_clock_peripheral(pTimerHandler);

	//2. Configuramos el prescaler
	timer_set_prescaler(pTimerHandler);

	//3. Configuramos si UP_COUNT o DOWN_COUNT (mode)
	timer_set_mode(pTimerHandler);

	//4. Configuramos el Auto-reload
	timer_set_period(pTimerHandler);

	//5. Configuramos la interrupcion
	timer_config_interrupt(pTimerHandler);

	//x. Volvemos a activar las interrupciones del sistema
	__enable_irq();

	// El Timer inicia apagado
	timer_SetState(pTimerHandler, TIMER_OFF);

}

void timer_enable_clock_peripheral(Timer_Handler_t *pTimerHandler){
	// verificamos que es un periferico permitido
	assert_param(IS_TIM_INSTANCE(pTimerHandler->pTIMx));

	if (pTimerHandler->pTIMx == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if (pTimerHandler->pTIMx == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if (pTimerHandler->pTIMx == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	else if (pTimerHandler->pTIMx == TIM5){
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	}
	else if (pTimerHandler->pTIMx == TIM9){
		RCC->APB2ENR |= RCC_APB2ENR_TIM9EN;
	}
	else if (pTimerHandler->pTIMx == TIM10){
		RCC->APB2ENR |= RCC_APB2ENR_TIM10EN;
	}
	else if (pTimerHandler->pTIMx == TIM11){
		RCC->APB2ENR |= RCC_APB2ENR_TIM11EN;
	}
	else{
		__NOP();
	}
}

void timer_set_prescaler(Timer_Handler_t *pTimerHandler){
	// verificamos que el valor del prescaler es valido
	assert_param(IS_TIMER_PRESC(pTimerHandler->TIMx_Config.TIMx_Prescaler));

	// configuramos el valor del prescaler
	pTimerHandler->pTIMx->PSC = pTimerHandler->TIMx_Config.TIMx_Prescaler - 1;
}
