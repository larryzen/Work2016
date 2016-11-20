#pragma once

#if _MSC_VER < 1400 //°æ±¾Ð¡ÓÚ2005
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>


int sprintf_s( char *string, size_t sizeInBytes, const char *format, ... );
int localtime_s(struct tm * _Tm, const time_t * _Time);
int strcpy_s(char * _Dst, int _DstSize, const char * _Src);
int freopen_s( FILE ** _File,  const char * _Filename,  const char * _Mode,  FILE * _OldFile);
int vsprintf_s(char *string, size_t sizeInBytes, const char *format, va_list);



#endif