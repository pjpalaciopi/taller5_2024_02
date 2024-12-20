/*
 * timer_driver_hal.h
 *
 *  Created on: Dec 6, 2024
 *      Author: pjpalaciopi@unal.edu.co
 */

#ifndef INC_TIMER_DRIVER_HAL_H_
#define INC_TIMER_DRIVER_HAL_H_

#include "stm32f4xx.h"

enum{
	TIMER_INT_DISABLE = 0,
	TIMER_INT_ENABLE
};

enum{
	TIMER_UP_COUNTER = 0,
	TIMER_DOWN_COUNTER
};

enum{
	TIMER_OFF = 0,
	TIMER_ON
};

/*
 * Estructura que contiene la configuracion minima para el manejo del Timer
 */
typedef struct{
	uint8_t		TIMx_mode;				// Up or Down
	uint16_t	TIMx_Prescaler;			// prescaler
	uint32_t	TIMx_Period;			// valor en ms del periodo del Timer
	uint8_t		TIMx_InterruptEnable;	// activa o desactiva el modo iterrupciones
} Timer_BasicConfig;

typedef struct{
	TIM_TypeDef			*pTIMx;
	Timer_BasicConfig	TIMx_Config;
} Timer_Handler_t;

/*
 * For testing assert parameters - checking basic configurations
 */
#define IS_TIMER_INTERRUPT(VALUE)	(((VALUE) == TIMER_INT_DISABLE) || ((VALUE) == TIMER_INT_ENABLE))

#define IS_TIMER_MODE(VALUE)	(((VALUE) == TIMER_UP_COUNTER) || ((VALUE) == TIMER_DOWN_COUNTER))

#define IS_TIMER_STATE(VALUE)	(((VALUE) == TIMER_OFF) || ((VALUE) == TIMER_ON))

#define IS_TIMER_PRESC(VALUE)	(((uint32_t)VALUE) > 1 && ((uint32_t)VALUE) < 0xFFFE)

#define IS_TIMER_PERIOD(PERIOD)	(((uint32_t)PERIOD) > 1)

void timer_Config(Timer_Handler_t *pTimerHandler);
void timer_SetState(Timer_Handler_t *TimerHandler, uint8_t newState);

// esta funcion debe ser sobre-escrita en el main para que el sistema funcione
void Timer2_Callback(void);
void Timer3_Callback(void);
void Timer4_Callback(void);
void Timer5_Callback(void);
void Timer9_Callback(void);
void Timer10_Callback(void);
void Timer11_Callback(void);

#endif /* INC_TIMER_DRIVER_HAL_H_ */
