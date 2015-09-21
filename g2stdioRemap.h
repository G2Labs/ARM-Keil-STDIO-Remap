/*==================================================================================================
    Simple STDIO remapper for ARM Keil compilator
    Copyright (C) 2015 Grzegorz Grzeda [PL]

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
==================================================================================================*/
#ifndef G2STDIO_REMAP_H__
#define G2STDIO_REMAP_H__
/*================================================================================================*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <rt_misc.h>

#include "stm32f4xx.h"
/*================================================================================================*/
#pragma import(__use_no_semihosting_swi)
/*================================================================================================*/
/*================================================================================================*/
#define EXIT_TEXT_OK printf("Koniec programu...\n")
#define EXIT_TEXT_ERROR printf("Program zakonczyl prace z kodem: %d\n", return_code)
/*================================================================================================*/
#define FILE_NAME_MAX_LENGTH 20
/*================================================================================================*/
typedef void (*putCharFun)(char c);
typedef char (*getCharFun)(void);
/*================================================================================================*/
int initializeStdio(void);
int addStdioEntry(const char* name, putCharFun, getCharFun);
/*================================================================================================*/
#endif
/*================================================================================================*/
/*                                             EOF                                                */
/*================================================================================================*/
