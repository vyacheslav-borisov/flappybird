#ifndef PEGAS_TYPES_H
#define PEGAS_TYPES_H
#pragma once

#ifdef ANDROID
#include <stdint.h>
#endif

namespace pegas
{

#ifdef _UNICODE
	
	typedef wchar_t tchar;

#define _text(str) L##str

#else
	
	typedef char tchar;

#define _text(str) str

#endif

	typedef int ANIMATIONID;
	typedef int MILLISECONDS;

#ifdef _WIN32
	typedef signed int			int32;
	typedef signed short		int16;
	typedef signed char			int8;
	typedef unsigned int		uint32;
	typedef unsigned short		uint16;
	typedef unsigned char		uint8;
	typedef __int64				int64;
	typedef unsigned __int64	uint64;
#endif

#ifdef ANDROID
	typedef int32_t  	int32;
	typedef int16_t    	int16;
	typedef int8_t     	int8;
	typedef uint32_t   	uint32;
	typedef uint16_t  	uint16;
	typedef uint8_t   	uint8;
	typedef int64_t		int64;
	typedef uint64_t  	uint64;
#endif

	extern const int32	MIN_INT32;
	extern const int32	MAX_INT32;
	extern const int16	MIN_INT16;
	extern const int16	MAX_INT16;
	extern const int8	MIN_INT8;
	extern const int8	MAX_INT8;
	extern const uint32	MIN_UINT32;
	extern const uint32	MAX_UINT32;
	extern const uint16	MIN_UINT16;
	extern const uint16	MAX_UINT16;
	extern const uint8	MIN_UINT8;
	extern const uint8	MAX_UINT8;

	extern const float	MIN_REAL32;
	extern const float	MAX_REAL32;
	extern const float	TINY_REAL32;

	typedef float CURCOORD;
	typedef int32 RESOURCEID;
	typedef uint32 FLAGSET;
	
	typedef int32 FONTSIZE;
	typedef uint32 RGBCOLOR;

	typedef int32 status;

	const status STATUS_OK = 0;
	const status STATUS_KO = -1;
	const status STATUS_EXIT = -2;
}

#endif //PEGAS_TYPES_H
