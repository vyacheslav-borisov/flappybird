#include "../common.h"
#include "types.h"

using namespace pegas;

const int32	    pegas::MIN_INT32   =   (int32)0xffffffff; // (-2147483646);
const int32	    pegas::MAX_INT32   =   (int32)0x7fffffff; // ( 2147483647);
const int16	    pegas::MIN_INT16   =   (int16)0xffff; // (-32768);
const int16	    pegas::MAX_INT16   =   (int16)0x7fff; // ( 32767);
const int8	    pegas::MIN_INT8    =   (int8)0xff; // (-128);
const int8	    pegas::MAX_INT8    =   (int8)0x7f; // ( 127);
const uint32    pegas::MIN_UINT32  =   (uint32)0;
const uint32    pegas::MAX_UINT32  =   (uint32)0xffffffff;
const uint16    pegas::MIN_UINT16  =   (uint16)0;
const uint16    pegas::MAX_UINT16  =   (uint16)0xffff;
const uint8	    pegas::MIN_UINT8   =   (uint8)0;
const uint8	    pegas::MAX_UINT8   =   (uint8)0xff;

const float	pegas::MAX_REAL32  =   static_cast<float>(3.4E+38);
const float	pegas::MIN_REAL32  =   static_cast<float>(-3.4E+38);
const float	pegas::TINY_REAL32  =   static_cast<float>(3.4E-38);
