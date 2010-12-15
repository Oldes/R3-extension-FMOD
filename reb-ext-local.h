/* THIS SHOULD BE IN R3's reb-ext.h */


//------------------
//-    #RXV_xxx
//
// REBOL EXTENSION GET Macros
//
// provide direct RXIARG access macros
// with these macros, the single argument should be an RXIARG *
//
// this is usefull when the RXIARG is NOT being used from an argument frame
// but as a single value, like when we use RL_Get_Field() or RL_Get_Value()
//
// if the argument is dynamically allocated, ex:
//    RXIARG arg = OS_MAKE(sizeof(RXIARG));
// then use the macros like so:
//     RXV_WORD(*(arg));
//------------------
#define RXV_INT64(a)		(a.int64)
#define RXV_INT32(a)		(i32)(a.int64)
#define RXV_INTEGER(a)	(a.int64) // maps to RXT_INTEGER
#define RXV_DEC64(a)		(a.dec64)
#define RXV_DECIMAL(a)	(a.dec64) // maps to RXT_DECIMAL
#define RXV_LOGIC(a)		(a.int32a)
#define RXV_CHAR(a)		(a.int32a)
#define RXV_TIME(a)		(a.int64)
#define RXV_DATE(a)		(a.int32a)
#define RXV_WORD(a)		(a.int32a)
#define RXV_PAIR(a)		(a.pair)
#define RXV_TUPLE(a)		(a.bytes)
#define RXV_SERIES(a)		(a.series)
#define RXV_BLOCK(a)		(a.series)
#define RXV_INDEX(a)		(a.index)
#define RXV_OBJECT(a)	(a.addr)
#define RXV_MODULE(a)	(a.addr)
#define RXV_HANDLE(a)	(a.addr)
#define RXV_IMAGE(a)		(a.image)
#define RXV_GOB(a)		(a.addr)
