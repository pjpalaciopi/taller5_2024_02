/*
 * gpio_driver_hal.c
 *
 *  Created on: Dec 3, 2024
 *      Author: pjpalaciopi@unal.edu.co
 */

#include "gpio_driver_hal.h"
#include "stm32f4xx.h"
#include "stm32_assert.h"

// headers for private functions
static void gpio_enable_clock_peripheral(GPIO_Handler_t * pGPIOHandler);
static void gpio_config_mode(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_output_type(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_output_speed(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_pullup_pulldown(GPIO_Handler_t *pGPIOHandler);
static void gpio_config_alternate_function(GPIO_Handler_t *pGPIOHandler);

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
		RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOAEN);
	}

	// verificamos para GPIOB
	else if (pGPIOHandler->pGPIOx == GPIOB){
		// escribimos SET en la posicion correspondiente al GPIOB
		RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOBEN);
	}

	// verificamos para GPIOC
	else if (pGPIOHandler->pGPIOx == GPIOC){
		// escribimos SET en la posicion correspondiente al GPIOC
		RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOCEN);
	}

	// verificamos para GPIOD
	else if (pGPIOHandler->pGPIOx == GPIOD){
		// escribimos SET en la posicion correspondiente al GPIOD
		RCC->AHB1ENR |= (RCC_AHB1ENR_GPIODEN);
	}

	// verificamos para GPIOE
	else if (pGPIOHandler->pGPIOx == GPIOE){
		// escribimos SET en la posicion correspondiente al GPIOE
		RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOEEN);
	}
	// verificamos para GPIOH
	else if (pGPIOHandler->pGPIOx == GPIOH){
		// escribimos SET en la posicion correspondiente al GPIOH
		RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOHEN);
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
	pGPIOHandler->pGPIOx->MODER |= auxConfig;
}

/*
 * Configuracion del tipo de Output del pin:
 * - push-pull
 * - openDrain
 */
void gpio_config_output_type(GPIO_Handler_t *pGPIOHandler){
	uint32_t auxConfig = 0;

	// verificamos que el tipo de salida es valido
	assert_param(IS_GPIO_OUTPUT_TYPE(pGPIOHandler->pinConfig.GPIO_PinOutputType));

	// tomamos la configuracion del modo del pin y lo movemos a la posicion correspondiente al numero del Pin
	auxConfig |= (pGPIOHandler->pinConfig.GPIO_PinMode << pGPIOHandler->pinConfig.GPIO_PinNumber);

	// limpiamos antes de cargar
	pGPIOHandler->pGPIOx->OTYPER &= ~(SET << pGPIOHandler->pinConfig.GPIO_PinNumber);

	// cargamos la configuracion al registro OTYPER
	pGPIOHandler->pGPIOx->OTYPER |= auxConfig;
}

/*
 * Selects between the different speeds for the PIN
 * - Low
 * - Medium
 * - Fast
 * - High
 */
void gpio_config_output_speed(GPIO_Handler_t *pGPIOHandler){
	uint32_t auxConfig = 0;

	// verificamos que la velocidad deseada sea valida
	assert_param(IS_GPIO_OSPEED(pGPIOHandler->pinConfig.GPIO_PinOutputSpeed));

	// hacemos la configuracion
	auxConfig = (pGPIOHandler->pinConfig.GPIO_PinOutputSpeed << 2 * pGPIOHandler->pinConfig.GPIO_PinNumber);

	// limpiamos el registro
	pGPIOHandler->pGPIOx->OSPEEDR &= ~(0b11 << 2 * pGPIOHandler->pinConfig.GPIO_PinNumber);

	// cargamos la configuracion
	pGPIOHandler->pGPIOx->OSPEEDR |= auxConfig;
}

// prende o apaga la resistencia pull-up, pull-down para el PINx
void gpio_config_pullup_pulldown(GPIO_Handler_t *pGPIOHandler){
	uint32_t auxConfig = 0;

	// verificamos que sea una configuracion valida
	assert_param(IS_GPIO_PUPDR(pGPIOHandler->pinConfig.GPIO_PinPuPdControl));

	// hacemos la configuracion
	auxConfig = (pGPIOHandler->pinConfig.GPIO_PinPuPdControl << 2 * pGPIOHandler->pinConfig.GPIO_PinNumber);

	// limpiamos el registro
	pGPIOHandler->pGPIOx->PUPDR &= ~(0b11 << 2 * pGPIOHandler->pinConfig.GPIO_PinNumber);

	// cargamos la configuracion
	pGPIOHandler->pGPIOx->PUPDR |= auxConfig;
}

// permite configurar otras funciones (mas especializadas) sobre el PIN
void gpio_config_alternate_function(GPIO_Handler_t *pGPIOHandler){
	uint32_t auxPosition = 0;

	if (pGPIOHandler->pinConfig.GPIO_PinMode == GPIO_MODE_ALTFN){
		// Seleccionamos si se va a usar el registro alto (AFRH) o el bajo (AFRL)
		if (pGPIOHandler->pinConfig.GPIO_PinNumber < 8){
			// estamos en el registro AFRL, que controla del PIN_0 al PIN_7
			auxPosition = 4 * pGPIOHandler->pinConfig.GPIO_PinNumber;

			// limpiamos la posicion del registro
			pGPIOHandler->pGPIOx->AFR[0] &= ~(0b1111 << auxPosition);

			// escribimos la configuracion el la posicion deseada
			pGPIOHandler->pGPIOx->AFR[0] |= (pGPIOHandler->pinConfig.GPIO_PinAltFunMode << auxPosition);
		}

		else{
			// estamos en el registro AFRH, que controla del PIN_8 al PIN_15
			auxPosition = 4 * (pGPIOHandler->pinConfig.GPIO_PinNumber);
			// limpiamos la posicion del registro
			pGPIOHandler->pGPIOx->AFR[1] &= ~(0b1111 << auxPosition);

			// escribimos la configuracion el la posicion deseada
			pGPIOHandler->pGPIOx->AFR[1] |= (pGPIOHandler->pinConfig.GPIO_PinAltFunMode << auxPosition);
		}
	}
}

// funcion para cambiar el estado del pin entregado por el handler, en funcion del valor de la variable newState

void gpio_WritePin(GPIO_Handler_t *pGPIOHandler, uint8_t newState){
	// verificamos que la accion es permitida
	assert_param(IS_GPIO_PIN_ACTION(newState));

	if (newState == SET){
		// trabajando con la parte baja del registro
		pGPIOHandler->pGPIOx->BSRR |= (SET << pGPIOHandler->pinConfig.GPIO_PinNumber);
	}

	else{
		// trabajando con la parte alta del registro
		pGPIOHandler->pGPIOx->BSRR |= (SET << (pGPIOHandler->pinConfig.GPIO_PinNumber + 16));
	}
}

uint32_t gpio_ReadPin(GPIO_Handler_t *pGPIOHandler){
	uint32_t pinValue = 0;

	// limpio lo que hay en el registro salvo la posicion de interes y lo guardo a pinValue
	pinValue = (pGPIOHandler->pGPIOx->IDR) & (1 << pGPIOHandler->pinConfig.GPIO_PinNumber);

	// cargamos el valor del registro IDR, dezplazando a la derecha tantas veces como el numero de pin especificado
	pinValue = (pinValue >> pGPIOHandler->pinConfig.GPIO_PinNumber);

	return pinValue;
}

void gpio_TooglePin(GPIO_Handler_t *pGPIOHandler){

	/* ^ es el operador xor, si en ODR hay 1, ambos inputs el xor son 1 y el output = 0,
	 * Si en ODR hay un 0 el xor da 1
	 */
	pGPIOHandler->pGPIOx->ODR ^= (SET << pGPIOHandler->pinConfig.GPIO_PinNumber);
}
