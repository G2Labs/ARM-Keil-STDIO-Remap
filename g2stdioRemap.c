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
#include "g2stdioRemap.h"
/*================================================================================================*/
#define HANDLE_START_VALUE 1000
/*================================================================================================*/
struct __FILE 
{ 
	int handle;
	char name[FILE_NAME_MAX_LENGTH];	
	putCharFun put;
	getCharFun get;
	int isOpen;
};
/*================================================================================================*/
typedef struct
{
	FILE *table;
	int cnt;
} stdioType;
/*================================================================================================*/
static void updateTableEntry(const char* name, putCharFun put, getCharFun get);
static void copyTables(FILE *dest, FILE *src);
static FILE* findFileByName(const char *name);
static FILE* checkFileAttributes(FILE *file, const char *attrib);
/*================================================================================================*/
/*================================================================================================*/
static stdioType entries;
FILE __stdout;
FILE __stdin;
FILE __stderr;
/*================================================================================================*/
/*================================================================================================*/
int initializeStdio(void)
{
	if(entries.table != 0) free(entries.table);
	entries.table = 0;
	entries.cnt = 0;
	
	return 0;
}
/*================================================================================================*/
int addStdioEntry(const char* name, putCharFun put, getCharFun get)
{
	FILE *temporaryEntries = malloc((sizeof(FILE) * (entries.cnt + 1)));	
	if(temporaryEntries == 0) return -1;

	if(entries.table != 0)
	{
		if(entries.cnt > 0) copyTables(temporaryEntries, entries.table);		
		free(entries.table);
	}
	
	entries.table = temporaryEntries;	
	updateTableEntry(name, put, get);

	return 0;
}
/*================================================================================================*/
/*================================================================================================*/
/*================================================================================================*/
static void updateTableEntry(const char* name, putCharFun put, getCharFun get)
{
	entries.table[entries.cnt].handle = entries.cnt + HANDLE_START_VALUE;
	strncpy(entries.table[entries.cnt].name, name, FILE_NAME_MAX_LENGTH);
	entries.table[entries.cnt].get = get;
	entries.table[entries.cnt].put = put;
	entries.table[entries.cnt].isOpen = 0;
	entries.cnt++;
}
/*================================================================================================*/
static void copyTables(FILE *dest, FILE *src)
{
	int i;
	char *dest_c = (char*)dest, *src_c = (char *)src;
	
	for(i = 0; i < (sizeof(FILE) * entries.cnt); i++)
		*(dest_c++) = *(src_c++);
}
/*================================================================================================*/
FILE *fopen(const char *name, const char *attrib)
{
	FILE *file = findFileByName(name);	
	return checkFileAttributes(file, attrib);
}
/*================================================================================================*/
static FILE* findFileByName(const char *name)
{
	int i;
	
	for(i = 0; i < entries.cnt; i++)
		if(strcmp(name, entries.table[i].name) == 0)
			return &entries.table[i];
	return 0;
}
/*================================================================================================*/
static FILE* checkFileAttributes(FILE *file, const char *attrib)
{
	if(file == 0) return 0;
	if((strchr(attrib, 'r') > 0) && (file->get == 0)) return 0;
	if((strchr(attrib, 'R') > 0) && (file->get == 0)) return 0;
	if((strchr(attrib, 'w') > 0) && (file->put == 0)) return 0;
	if((strchr(attrib, 'W') > 0) && (file->put == 0)) return 0;
	if(file->isOpen) return 0;
	file->isOpen = 1;
	return file;
}
/*================================================================================================*/
int fclose(FILE *ptr){ ptr->isOpen = 0; return 0; }
/*================================================================================================*/
int fflush(FILE *ptr){ return 0; }
/*================================================================================================*/
int fputc(int c, FILE *f)
{
	if(f == &__stdout) ITM_SendChar(c);
	if(f->isOpen == 0) return c;
	if(f->put != 0)    f->put(c);
	return c;
}
/*================================================================================================*/
int fgetc(FILE *f)
{
	if(f->isOpen == 0) return 0;
	if(f == &__stdin) return 0;
	if(f->get != 0)   return (int)(f->get());
	return 0;
}
/*================================================================================================*/
int ferror(FILE *f){ return EOF; }
/*================================================================================================*/
void _ttywrch(int c){}
/*================================================================================================*/
void _sys_exit(int return_code)
{
	if(return_code != 0) EXIT_TEXT_ERROR;
	else                 EXIT_TEXT_OK;
	
	while(1);
}
/*================================================================================================*/
/*                                             EOF                                                */
/*================================================================================================*/
