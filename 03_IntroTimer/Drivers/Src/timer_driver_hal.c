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
	pTimerHandler->pTIMx->PSC = pTimerHandler->TIMx_Config.TIMx_Prescaler - 1;	//160000000
}

/*
 * Esta funcion configura el limite hasta donde cuenta el Timer para generar
 * un evento "update" (cuando esta contando de forma ascendente,) o configura
 * el valor desde donde se comienza a contar cuando el sistema funciona de
 * forma descendente
 */
void timer_set_period(Timer_Handler_t *pTimerHandler){
	// verificamos que el valor que genera el periodo es valido
	assert_param(IS_TIMER_PERIOD(pTimerHandler->TIMx_Config.TIMx_Period));

	//FALTA

	// configuramos el valor del auto-reload
	pTimerHandler->pTIMx->ARR = pTimerHandler->TIMx_Config.TIMx_Period - 1;
}

/*
 * Upcounter or Downcounter
 */
void timer_set_mode(Timer_Handler_t *pTimerHandler){
	// verificamos que el modo de funcionamiento es correcto
	assert_param(IS_TIMER_MODE(pTimerHandler->TIMx_Config.TIMx_mode));

	// verificamos cual es el modo que se desea configurar
	if (pTimerHandler->TIMx_Config.TIMx_mode == TIMER_UP_COUNTER){
		// ponemos modo Upcounter = 0
		pTimerHandler->pTIMx->CR1 &= ~TIM_CR1_DIR;
	}
	else{
		// ponemos modo Downcounter = 1
		pTimerHandler->pTIMx->CR1 |= TIM_CR1_DIR;
	}
}

void timer_config_interrupt(Timer_Handler_t *pTimerHandler){
	// verificamos el valor
	assert_param(IS_TIMER_INTERRUPT(pTimerHandler->TIMx_Config.TIMx_InterruptEnable));

	if (pTimerHandler->TIMx_Config.TIMx_InterruptEnable == TIMER_INT_ENABLE){
		// activamos la interrupcion debido al Timerx utilizado
		pTimerHandler->pTIMx->DIER |= TIM_DIER_UIE;

		// activamos el canal del sistema NVIC para que lea la interrupcion
		if (pTimerHandler->pTIMx == TIM2){
			NVIC_EnableIRQ(TIM2_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM3){
			NVIC_EnableIRQ(TIM3_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM4){
			NVIC_EnableIRQ(TIM4_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM5){
			NVIC_EnableIRQ(TIM5_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM9){
			NVIC_EnableIRQ(TIM1_BRK_TIM9_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM10){
			NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM11){
			NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
		}
		else{
			__NOP();
		}
	}
	else{
		// desactivamos la interrupcion devida al Timerx  utilizando
		pTimerHandler->pTIMx->DIER &= ~TIM_DIER_UIE;

		// desactivamos el canal del sistema NVIC para que lea la interrupcion
		if (pTimerHandler->pTIMx == TIM2){
			NVIC_DisableIRQ(TIM2_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM3){
			NVIC_DisableIRQ(TIM3_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM4){
			NVIC_DisableIRQ(TIM4_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM5){
			NVIC_DisableIRQ(TIM5_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM9){
			NVIC_DisableIRQ(TIM1_BRK_TIM9_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM10){
			NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
		}
		else if (pTimerHandler->pTIMx == TIM11){
			NVIC_DisableIRQ(TIM1_TRG_COM_TIM11_IRQn);
		}
		else{
			__NOP();
		}
	}
}

void timer_SetState(Timer_Handler_t *pTimerHandler, uint8_t newState){
	// verificamos que el parametro ingresado sea valido
	assert_param(IS_TIMER_STATE(newState));

	// 4. reiniciamos el registro counter
	pTimerHandler->pTIMx->CNT = 0;

	if (newState == TIMER_ON){
		//5.a Activamos el Timer (el CNT debe comentar a contar)
		pTimerHandler->pTIMx->CNT |= TIM_CR1_CEN;
	}
	else{
		//5.b desactivamos el Timer (el CTN debe detenerse)
		pTimerHandler->pTIMx->CNT &= ~TIM_CR1_CEN;
	}
}


__attribute__((weak)) void Timer2_Callback(void){
	__NOP();
}

__attribute__((weak)) void Timer3_Callback(void){
	__NOP();
}

__attribute__((weak)) void Timer4_Callback(void){
	__NOP();
}

__attribute__((weak)) void Timer5_Callback(void){
	__NOP();
}

__attribute__((weak)) void Timer9_Callback(void){
	__NOP();
}

__attribute__((weak)) void Timer10_Callback(void){
	__NOP();
}

__attribute__((weak)) void Timer11_Callback(void){
	__NOP();
}

void TIM2_IRQHandler(void){
	// limpiamos la bandera
	TIM2->SR &= ~TIM_SR_UIF;

	//llamamos a la funcion que se debe encargar de hacer algo con esta interrupcion
	Timer2_Callback();
}

void TIM3_IRQHandler(void){
	// limpiamos la bandera
	TIM3->SR &= ~TIM_SR_UIF;

	//llamamos a la funcion que se debe encargar de hacer algo con esta interrupcion
	Timer3_Callback();
}

void TIM4_IRQHandler(void){
	// limpiamos la bandera
	TIM4->SR &= ~TIM_SR_UIF;

	//llamamos a la funcion que se debe encargar de hacer algo con esta interrupcion
	Timer4_Callback();
}

void TIM5_IRQHandler(void){
	// limpiamos la bandera
	TIM5->SR &= ~TIM_SR_UIF;

	//llamamos a la funcion que se debe encargar de hacer algo con esta interrupcion
	Timer5_Callback();
}

void TIM9_IRQHandler(void){
	// limpiamos la bandera
	TIM9->SR &= ~TIM_SR_UIF;

	//llamamos a la funcion que se debe encargar de hacer algo con esta interrupcion
	Timer9_Callback();
}

void TIM10_IRQHandler(void){
	// limpiamos la bandera
	TIM10->SR &= ~TIM_SR_UIF;

	//llamamos a la funcion que se debe encargar de hacer algo con esta interrupcion
	Timer10_Callback();
}

void TIM11_IRQHandler(void){
	// limpiamos la bandera
	TIM4->SR &= ~TIM_SR_UIF;

	//llamamos a la funcion que se debe encargar de hacer algo con esta interrupcion
	Timer11_Callback();
}









