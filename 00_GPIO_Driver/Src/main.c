/**
 ******************************************************************************
 * @file           : main.c
 * @author         : namontoy@unal.edu.co
 * @brief          : Here starts the magic!!
 ******************************************************************************
 */
#include <stdint.h>
#include <stdio.h>
#include <stm32f4xx.h>

/*
 * Definicion de variables publicas Globales
 * */
uint8_t weeks;						//Usar siempre los tipos de datos definidos en el estandar
int32_t milisecondsPerYear;
//int number;							//NO usar los tipos de datos predefinidos en C
//double decimal;
//char character;

// Definicion de cabeceras de las funcionnes de la clase
uint16_t operacionClase(uint16_t budget, uint16_t cost);

// Definicion de las cabeceras de las funciones del main
extern void configMagic(void);

/*
 * The main function, where everything happens.
 */
int main(void)
{
	/* Configuramos la magia! */
	configMagic();

	/*
	 * Variables locales
	 * */
	uint16_t presupuesto = 1000;
	uint16_t precio;
	uint16_t compras = 500;
	uint16_t operacion;
	uint16_t operacion2;


	// Ciclo principal

		operacion = operacionClase(presupuesto, compras);

		printf("me fui de compras\n");
		printf("mi presupuesto era: %d\n", presupuesto);
		printf("las compras fueron: %d\n", compras);

		operacion = presupuesto - compras;
		printf("(1) he quedado con: %d\n", operacion);

		operacion2 = operacionClase(presupuesto, compras);
		printf("(2) he quedado con: %d\n", operacion2);


//		printf("Hola Mundo!\n");
//		printf("Characters: %c %c\n", 'a', 65);
//		printf("Decimals: %d %ld\n", 1977, 650000L);
//		printf("Preceding with blanks: %10d\n", 1977);
//		printf("Preceding with zeros: %010d\n", 1977);
//		printf("Some different radices: %d %x %o %#x %#o\n", 100, 100, 100, 100,
//				100);
//		printf("floats: %4.2f %+.0e %E\n", 3.1416, 3.1416, 3.1416);
//		printf("Width trick: %*d\n", 5, 10);
//		printf("%s\n", "A string");

	while (1) {
	}

	return 0;
}

uint16_t operacionClase(uint16_t budget, uint16_t cost){
	uint16_t auxOperation;
	auxOperation = budget - cost;
	return auxOperation;
}
