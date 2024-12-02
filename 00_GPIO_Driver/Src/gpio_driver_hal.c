/*
 * gpio_driver_hal.c
 *
 *  Created on: Dec 1, 2024
 *      Author: pjpalaciopi@unal.edu.co
 */

#include "gpio_driver_hal.h"
#include "stm32f4xx_hal.h"
#include "stm32_assert.h"

// headers for private functions
void gpio_enable_clock_peripheral(GPIO_Handler_t * pGPIOHandler);
void gpio_config_mode(GPIO_Handler_t *pGPIOHandler);
void gpio_config_output_type(GPIO_Handler_t *pGPIOHandler);
void gpio_config_output_speed(GPIO_Handler_t *pGPIOHandler);
void gpio_config_pullup_pulldown(GPIO_Handler_t *pGPIOHandler);
void gpio_config_alternate_function(GPIO_Handler_t *pGPIOHandler);

// para todos los perifericos lo PRIMERO es activar la señal de reloj (RCC)
void gpio_Config(GPIO_Handler_t *pGPIOHandler){
	//verificamos que el PIN seleccionado es correcto
	assert_param(IS_GPIO_PIN(pGPIOHandler->pinConfig.GPIO_PinNumber));

	// 1) activar la señal de reloj
	gpio_enable_clock_peripheral(pGPIOHandler);

	// ahora, podemos comenzar a configurar

	// 2) configuracion del registro GPIOx_MODER
	gpio_config_mode(pGPIOHandler);

	// 3) configuracion del registro GPIOx_OTYPE
	gpio_config_output_type(pGPIOHandler);

	// 4) configuracion del registro GPIOx_OSPEED
	gpio_config_output_speed(pGPIOHandler);

	// 5) configuracion de la resistencia pull-up, pull-down o flotante
	gpio_config_pullup_pulldown(pGPIOHandler);

	// 6) configuracion de las funciones alternativas
	gpio_config_alternate_function(pGPIOHandler);
} // fin de GPIO_Config

// Enable clock for specific GPIO port
void gpio_enable_clock_peripheral(GPIO_Handler_t *pGPIOHandler){
	// verificamos que sea un puerto permitido
	assert_param(IS_GPIO_ALL_ISNTANCE(pGPIOHandler->pGPIOx));

	// verificamos para GPIOA
	if (pGPIOHandler->pGPIOx == GPIOA){
		// escribimos SET en la posicion correspondiente al GPIOA
		RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOAEN);
	}

	// verificamos para GPIOB
	else if (pGPIOHandler->pGPIOx == GPIOB){
		// escribimos SET en la posicion correspondiente al GPIOB
		RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOBEN);
	}

	// verificamos para GPIOC
	else if (pGPIOHandler->pGPIOx == GPIOC){
		// escribimos SET en la posicion correspondiente al GPIOC
		RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOCEN);
	}

	// verificamos para GPIOD
	else if (pGPIOHandler->pGPIOx == GPIOD){
		// escribimos SET en la posicion correspondiente al GPIOD
		RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIODEN);
	}

	// verificamos para GPIOE
	else if (pGPIOHandler->pGPIOx == GPIOE){
		// escribimos SET en la posicion correspondiente al GPIOE
		RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOEEN);
	}
	// verificamos para GPIOH
	else if (pGPIOHandler->pGPIOx == GPIOH){
		// escribimos SET en la posicion correspondiente al GPIOH
		RCC->AHB1ENR |= (1 << RCC_AHB1ENR_GPIOHEN);
	}
}

/*
 * Configuracion del modo del pin:
 * - Input
 * - Output
 * - Analog
 * - Alternate Function
 */
void gpio_config_mode(GPIO_Handler_t *pGPIOHandler){
	uint32_t auxConfig = 0;
	// verificamos que el modo sea valido
	assert_param(IS_GPIO_MODE(pHandler->pinConfig.GPIO_PinMode));

	// tomamos la configuracion del Pin_Mode y la movemos a la posicion de pin,
	// como cada pin ocupa 2 bit por eso multiplicamos por 2 el PinNumber
	auxConfig = (pGPIOHandler->pinConfig.GPIO_PinMode << 2 * pGPIOHandler->pinConfig.GPIO_PinNumber);

	// antes de cargar el nuevo valor limpiamos lo que hay en esas posiciones
	pGPIOHandler->pGPIOx->MODER &= ~(0b11 << 2 * pGPIOHandler->pinConfig.GPIO_PinNumber);

	// ahora cargamos la configuracion al registro del puerto
	pGPIOHandler->pGPIOx->MODER = auxConfig;
}

/*
 * Configuracion del tipo de Output del pin:
 * - push-pull
 * - openDrain
 */
gpio_config_output_type(GPIO_Handler_t *pGPIOHandler){

}
