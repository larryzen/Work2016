#include "safestring.h"

#if _MSC_VER < 1400 //°æ±¾Ð¡ÓÚ2005


int sprintf_s( char *string, size_t sizeInBytes, const char *format, ... )
{
	va_list args;
	va_start(args, format);
	int n=vsprintf(string, format, args);
	va_end(args);
	return n;
}

int localtime_s( struct tm * _Tm, const time_t * _Time )
{
	*_Tm = *localtime(_Time);
	return 0;
}

int strcpy_s(char * _Dst, int _DstSize, const char * _Src )
{
	strcpy(_Dst, _Src);
	return 0;
}

int freopen_s( FILE ** _File, const char * _Filename, const char * _Mode, FILE * _OldFile )
{
	freopen(_Filename, _Mode, _OldFile);
	return 0;
}

int vsprintf_s( char *string, size_t sizeInBytes, const char *format, va_list valist)
{
	return vsprintf(string, format, valist);
}




#endif
