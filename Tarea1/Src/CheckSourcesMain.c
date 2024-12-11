/*
 * CheckSourcesMain.c
 *
 *  Created on: Dec 2, 2024
 *      Author: namontoy
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stm32f4xx.h>

/* Definiciond de variables */
uint32_t rotation;
int16_t temperatureValue;
int dummy;

// Definicion de las cabeceras de las funciones del main
extern void configMagic(void);
int16_t getTemperature(uint8_t getData);
uint32_t checkRotation(void);
uint8_t leaking(uint16_t const range);
uint8_t positiveFunction(uint8_t data);
int dostuff(char *data, int value);
uint8_t weirdFunction(uint8_t data);



/*
 * The main function, where everything happens.
 */
int main(void)
{
	/* Configuramos la magia! */
	configMagic();

	// Ciclo principal
	printf("Hola Mundo!\n");

	/* Probando diferentes funciones */
	checkRotation();
	getTemperature(23);
	leaking(350);
	positiveFunction(2);
	// dostuff("abc", 1024);	// usar comillas simples
	dostuff("abc", 1024);
	weirdFunction(26);

	while (1) {
	}

	return 0;
}

/* Describir cuales son los problemas que hay en esta función y como se pueden corregir */
uint32_t checkRotation(void){

	if(rotation > 25){
//		if(rotation > 45){			// esta condicion es redundante con la anterior
//			if(rotation < 23){		// esta linea nunca se ejecuta porque contradice el if anterior
//				return 0;
//			}
//		}
		return 1;
	}
	else if (rotation < 23){
		return 0;
	}
	else{
		return 1;
	}
}

/* Describir cuales son los problemas que hay en esta función y como se pueden corregir */
int16_t getTemperature(uint8_t getData){
	if(getData == 1){
		for(int i = 0; i < 10; i++){
			temperatureValue = i*35;
			//i--; // i debe incrementar para que acabe el for
			i++;
		}
		return temperatureValue;
	}
	return 0;
}

/* Describir cuales son los problemas que hay en esta función y como se pueden corregir */
uint8_t leaking(uint16_t const range){
	char a[10];

	/* Utilice una linea de codigo del ciclo FOR y luego la otra, ¿que observa en la salida del cppcheck?*/
	// el range es ingresado por el usuario, pero no deberia ser > 10, se podria definir como una variable global uint16_t const range = 10;
		for(uint8_t size = 0; size < range; size++){
//	for(uint8_t size = 0; size < 350; size++){	// size  es un unsigned de 8 bits asi que el maximo es 255, por lo que la condicion siempre es true
		a[size] = size*2;
	}

	/* Utilice una linea de codigo del ciclo FOR y luego la otra, ¿que observa en la salida del cppcheck?*/
	return a[9];
//	return a[10]; // el valor maximo del indice del arreglo es no inclusivo
}

/* Describir cuales son los problemas que hay en esta función y como se pueden corregir */
uint8_t positiveFunction(uint8_t data){
	if(data == 1){
		return 1;
	}else{
//		return 1; // retorna 1 en ambos casos. Podria poner return 0;
		return 0;
	}
}

/* Describir cuales son los problemas que hay en esta función y como se pueden corregir */
uint8_t weirdFunction(uint8_t data){
	uint8_t weird = 10;
//	if((weird = data) == 25){	// se puede hacer la asignacion fuera del if
	weird = data;
	if (weird == 25){
		return 0;
	}else{
		//return 0;	// la condicion retorna lo mismo. Podria poner return 1
		return 1;
	}
}

/* Describir cuales son los problemas que hay en esta función y como se pueden corregir */
int dostuff(char *data, int value){
	//data[125] = 200;	//
	//data[2] = 200;	// cuando llamammos la funcion el arrego solo tiene tres posiciones
	dummy = value + 1;				// como llamamos la funcion con comillas dobles se vuelve un string literal, lo que lo vuelve inmutable.

	// declaro un string con suficiente espacio
	char str[250];
	// copio el contenido del puntero *data con el string literal que es inmutable a un arrego que es mutable
	// para esto tengo que usar la funcion strcpy() incluyendo en las librerias de preproocesamiento <string.h>
	strcpy(str, data);
	str[150] = 200;
	return 0;
}

/*
 * Explicacion de las opciones del cppcheck
 * --template=gcc
Especifica el formato de salida que debe usar Cppcheck para las advertencias y errores.
Con gcc, el formato sera similar al estilo del compilador GCC.

--enable=all
Habilita todas las verificaciones disponibles en Cppcheck, incluyendo las verificaciones adicionales como:
Estilo de codigo (style)
Errores potenciales (performance)
Problemas portables (portability)
Errores de diseño (unusedFunction y más).
Si no se incluye, Cppcheck solo realizara un analisis basico de errores criticos.

--platform=unix64
Especifica la plataforma objtivo del codigo.
En este caso, unix64 indica un entorno basado en Unix de 64 bits.
Esto afecta la detección de problemas específicos de plataforma, como tipos de datos, alineacion de memoria, tamaños de variables, etc.

--std=c11
Indica que el codigo debe analizarse según el estandar C11.

--verbose
Muestra detalles adicionales sobre el proceso de analisis.
Incluye informacion como:
Archivos que se estan procesando.
Verificaciones habilitadas.
Supresiones aplicadas.
Útil para verificar el comportamiento de Cppcheck.

--suppress=unusedFunction
Suprime las advertencias relacionadas con funciones no utilizadas.
Por defecto, Cppcheck reporta todas las funciones estáticas no utilizadas, pero con esta opcion, estas advertencias se ignoran.

--inconclusive
Habilita la deteccion de problemas inconclusos.
Los problemas inconclusos son aquellos que Cppcheck no puede confirmar al 100%, pero podrían indicar errores potenciales.

CheckSourcesMain.c
Especifica el archivo de codigo fuente que se analizará (CheckSourcesMain.c).
Tambien puede ser un directorio, en estcaso Cppcheck analiza recursivamente todos los archivos fuente dentro.

 */
