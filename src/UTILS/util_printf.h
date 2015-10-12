/*
 * printf.h
 *
 *  Created on: 19 fevr. 2013
 *      Author: f.teutsch
 */

#ifndef _PRINTF_H_
#define _PRINTF_H_


#include "BSP.h"
#include <stdarg.h>


int print(char **out, const char *format, va_list args );
int _printf(const char *format, ...);
int _no_printf(const char *format, ...);
int _sprintf(char *out, const char *format, ...);
void _printfFloat(float Value, uint8_t NbChiffreApresLaVirgule);


#endif /* PRINTF_H_ */
