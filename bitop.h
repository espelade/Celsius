#ifndef BITOP_H
#define BITOP_H
#include "shogi.h"

#if defined(_WIN32)
#  define CONV2              __fastcall
#else
#  define CONV2
#endif
#if defined(_MSC_VER)
#  define Inline              __inline
#else
#  define Inline			  inline			　
#endif

#define BBToU(b)            ( (b).p[0] | (b).p[1] | (b).p[2] )
#define BBToUShift(b)       ( (b).p[0]<<2 | (b).p[1]<<1 | (b).p[2])
#define PopuCount(bb)       popu_count012( bb.p[0], bb.p[1], bb.p[2] )
#define FirstOne(bb)        first_one012( bb.p[0], bb.p[1], bb.p[2] )

#if defined(_MSC_VER)
/* 順序無くbit取り出しの高速化*/
#define foreach_bitboard_one(bb,sq,XXX) \
{\
/**/ if (bb.p[0]|bb.p[1]|bb.p[2]) /**/ \
  {\
		unsigned long _index_; \
		while ( _BitScanForward( &_index_, bb.p[0] ) ) \
		{ \
				bb.p[0] &= bb.p[0] - 1 ; \
				sq = 26 - _index_; \
				XXX; \
		} \
		while ( _BitScanForward( &_index_, bb.p[1] ) ) \
		{ \
				bb.p[1] &= bb.p[1] - 1; \
				sq = 53 - _index_; \
				XXX; \
		} \
		while( _BitScanForward( &_index_, bb.p[2] ) ) \
		{ \
				bb.p[2] &= bb.p[2] - 1; \
				sq = 80 - _index_; \
				XXX; \
		} \
  }\
}

/* 順序無くbit取り出しの高速化　ビットボードのチェックなし*/
#define foreach_bitboard_one_no_check(bb,sq,XXX) \
{\
    { \
		unsigned long _index_; \
		while ( _BitScanForward( &_index_, bb.p[0] ) ) \
		{ \
			bb.p[0] &= bb.p[0] - 1 ; \
			sq = 26 - _index_; \
			XXX; \
		} \
		while ( _BitScanForward( &_index_, bb.p[1] ) ) \
		{ \
				bb.p[1] &= bb.p[1] - 1; \
				sq = 53 - _index_; \
				XXX; \
		} \
		while( _BitScanForward( &_index_, bb.p[2] ) ) \
		{ \
				bb.p[2] &= bb.p[2] - 1; \
				sq = 80 - _index_; \
				XXX; \
		} \
    } \
}
/*firstone高速化*/
#define foreach_bitboard_firstone(bb,sq,XXX) \
{\
	if (bb.p[0] | bb.p[1] | bb.p[2])  \
	{\
		unsigned long _index_; \
		while (_BitScanReverse(&_index_, bb.p[0])) \
		{ \
			bb.p[0] ^= 1 << _index_; \
			sq = 26 - _index_; \
			XXX; \
		} \
		while (_BitScanReverse(&_index_, bb.p[1])) \
		{ \
			bb.p[1] ^= 1 << _index_; \
			sq = 53 - _index_; \
			XXX; \
		} \
		while (_BitScanReverse(&_index_, bb.p[2])) \
		{ \
			bb.p[2] ^= 1 << _index_; \
			sq = 80 - _index_; \
			XXX; \
		}	\
	}\
}

/*firstone高速化　ビットボードチェックなし*/
#define foreach_bitboard_firstone_no_check(bb,sq,XXX) \
{\
		{\
		unsigned long _index_; \
				while (_BitScanReverse(&_index_, bb.p[0])) \
				{ \
					bb.p[0] ^= 1 << _index_; \
					sq = 26 - _index_; \
					XXX; \
				} \
					while (_BitScanReverse(&_index_, bb.p[1])) \
					{ \
						bb.p[1] ^= 1 << _index_; \
						sq = 53 - _index_; \
						XXX; \
					} \
						while (_BitScanReverse(&_index_, bb.p[2])) \
						{ \
							bb.p[2] ^= 1 << _index_; \
							sq = 80 - _index_; \
							XXX; \
						}	\
		}\
}

/*lastone 高速化*/
#define foreach_bitboard_lastone(bb,sq,XXX) \
{\
/**/ if (bb.p[2]|bb.p[1]|bb.p[0]) /**/ \
  {\
		unsigned long _index_; \
				while ( _BitScanForward( &_index_, bb.p[2] ) ) \
				{ \
				bb.p[2] &= bb.p[2] - 1 ; \
				sq = 80 - _index_; \
				XXX; \
				} \
					while ( _BitScanForward( &_index_, bb.p[1] ) ) \
					{ \
					bb.p[1] &= bb.p[1] - 1; \
					sq = 53 - _index_; \
					XXX; \
					} \
						while( _BitScanForward( &_index_, bb.p[0] ) ) \
						{ \
						bb.p[0] &= bb.p[0] - 1; \
						sq = 26 - _index_; \
						XXX; \
						} \
  }\
}

/* lastone 高速化　ビットボードのチェックなし*/
#define foreach_bitboard_lastone_no_check(bb,sq,XXX) \
{\
	    { \
		unsigned long _index_; \
				while ( _BitScanForward( &_index_, bb.p[2] ) ) \
				{ \
					bb.p[2] &= bb.p[2] - 1 ; \
					sq = 80 - _index_; \
					XXX; \
				} \
					while ( _BitScanForward( &_index_, bb.p[1] ) ) \
					{ \
						bb.p[1] &= bb.p[1] - 1; \
						sq = 53 - _index_; \
						XXX; \
					} \
						while( _BitScanForward( &_index_, bb.p[0] ) ) \
						{ \
							bb.p[0] &= bb.p[0] - 1; \
							sq = 26 - _index_; \
							XXX; \
						} \
	    } \
}
#else
/*if (u0) { return __builtin_clz(bb.p[0]) - 5; }
	if (u1) { return __builtin_clz(bb.p[1]) + 22; }
	return __builtin_clz(bb.p[2]) + 49;
	*/

/* 順序無くbit取り出しの高速化*/
#define foreach_bitboard_one(bb,sq,XXX) \
{\
/**/ if (bb.p[0]|bb.p[1]|bb.p[2]) /**/ \
  {\
  		while( bb.p[0])\
			{\
		sq = __builtin_ctz(bb.p[0]) - 5 ;\
		bb.p[0] &= bb.p[0] - 1 ; \
		XXX; \
			}\
		while( bb.p[1])\
	{\
		sq = __builtin_ctz(bb.p[1]) + 22 ;\
		bb.p[1] &= bb.p[1] - 1 ; \
		XXX; \
	}\
		while( bb.p[2])\
	{\
		sq = __builtin_ctz(bb.p[2]) + 49 ;\
		bb.p[2] &= bb.p[2] - 1 ; \
		XXX; \
	}\
}

/* 順序無くbit取り出しの高速化　ビットボードのチェックなし*/
#define foreach_bitboard_one_no_check(bb,sq,XXX) \
{\
		    while( bb.p[0])\
						{\
		sq = __builtin_ctz(bb.p[0]) - 5 ;\
		bb.p[0] &= bb.p[0] - 1 ; \
		XXX; \
						}\
								while( bb.p[1])\
									{\
		sq = __builtin_ctz(bb.p[1]) + 22 ;\
		bb.p[1] &= bb.p[1] - 1 ; \
		XXX; \
									}\
											while( bb.p[2])\
												{\
		sq = __builtin_ctz(bb.p[2]) + 49 ;\
		bb.p[2] &= bb.p[2] - 1 ; \
		XXX; \
												}\
}
/*firstone高速化*/
#define foreach_bitboard_firstone(bb,sq,XXX) \
{\
		while( bb.p[0])\
		{\
	sq = __builtin_clz(bb.p[0]) - 5 ;\
	bb.p[0] ^= 1 << ( 31 - sq ) ; \
	XXX; \
		}\
		while( bb.p[1])\
		{\
	sq = __builtin_clz(bb.p[1]) + 22 ;\
	bb.p[1] = 1 << ( 31 - sq ) ; \
	XXX; \
		}\
		while( bb.p[2])\
		{\
	sq = __builtin_clz(bb.p[2]) + 49 ;\
	bb.p[2] = 1 << ( 31 - sq ) ; \
	XXX; \
		}\
}

/*firstone高速化　ビットボードチェックなし*/
#define foreach_bitboard_firstone_no_check(bb,sq,XXX) \
{\
		while( bb.p[0])\
		{\
	sq = __builtin_clz(bb.p[0]) - 5 ;\
	bb.p[0] ^= 1 << ( 31 - sq ) ; \
	XXX; \
		}\
		while( bb.p[1])\
		{\
	sq = __builtin_clz(bb.p[1]) + 22 ;\
	bb.p[1] = 1 << ( 31 - sq ) ; \
	XXX; \
		}\
		while( bb.p[2])\
		{\
	sq = __builtin_clz(bb.p[2]) + 49 ;\
	bb.p[2] = 1 << ( 31 - sq ) ; \
	XXX; \
		}\
}

/*lastone 高速化*/
#define foreach_bitboard_lastone(bb,sq,XXX) \
{\
	while( bb.p[2])\
	{\
		sq = __builtin_clz(bb.p[2]) + 49 ;\
		bb.p[2] &= bb.p[2] - 1 ; \
		XXX; \
	}\
	while( bb.p[1])\
	{\
		sq = __builtin_clz(bb.p[1]) + 22 ;\
		bb.p[1] &= bb.p[1] - 1 ; \
		XXX; \
	}\
	while( bb.p[0])\
	{\
		sq = __builtin_clz(bb.p[0]) - 5 ;\
		bb.p[0] &= bb.p[0] - 1 ; \
		XXX; \
	}\
}

/* lastone 高速化　ビットボードのチェックなし*/
#define foreach_bitboard_lastone_no_check(bb,sq,XXX) \
{\
			while( bb.p[2])\
				{\
		sq = __builtin_ctz(bb.p[2]) + 49 ;\
		bb.p[2] &= bb.p[2] - 1 ; \
		XXX; \
				}\
					while( bb.p[1])\
						{\
		sq = __builtin_ctz(bb.p[1]) + 22 ;\
		bb.p[1] &= bb.p[1] - 1 ; \
		XXX; \
						}\
							while( bb.p[0])\
								{\
		sq = __builtin_ctz(bb.p[0]) - 5 ;\
		bb.p[0] &= bb.p[0] - 1 ; \
		XXX; \
								}\
}
#endif

#define LastOne(bb)         last_one210( bb.p[2], bb.p[1], bb.p[0] )
#define BBCmp(b1,b2)        ( (b1).p[0] != (b2).p[0]                    \
				|| (b1).p[1] != (b2).p[1]               \
				|| (b1).p[2] != (b2).p[2] )
#define BBContractShift(b1,b2) ( ( (b1).p[0] & (b2).p[0] ) << 2         \
                               | ( (b1).p[1] & (b2).p[1] ) << 1         \
                               | ( (b1).p[2] & (b2).p[2] ) )

#if defined(HAVE_SSE2) || defined(HAVE_SSE4)

#if defined(HAVE_SSE4)

#  include <smmintrin.h>

#  define BBContract(b1,b2) ( ! _mm_testz_si128( (b1).m, (b2).m ) )
#  define BBTest(b)         ( ! _mm_testz_si128( (b).m, _mm_set1_epi8(0xff) ) )


#else /* no SSE4 */

#  include <emmintrin.h>
#  define BBContract(b1,b2) ( ( (b1).p[0] & (b2).p[0] )                 \
                            | ( (b1).p[1] & (b2).p[1] )                 \
                            | ( (b1).p[2] & (b2).p[2] ) )
#  define BBTest(b)         ( (b).p[0] | (b).p[1] | (b).p[2] )

#endif /* HAVE_SSE4 */

#define BBNot(b,b1)         (b).m = _mm_andnot_si128( (b1).m,                 \
						      _mm_set1_epi8(0xff) )
#define BBIni(b)            (b).m = _mm_setzero_si128()
#define BBAnd(b,b1,b2)      (b).m = _mm_and_si128( (b1).m, (b2).m )
#define BBOr(b,b1,b2)       (b).m = _mm_or_si128( (b1).m, (b2).m )
#define BBXor(b,b1,b2)      (b).m = _mm_xor_si128( (b1).m, (b2).m )
#define BBAndOr(b,b1,b2)    (b).m = _mm_or_si128( (b).m,                      \
                                    _mm_and_si128( (b1).m, (b2).m ) )
#define BBNotAnd(b,b1,b2)   (b).m = _mm_andnot_si128( (b2).m, (b1).m )
#define Xor(sq,b)           (b).m = _mm_xor_si128( (b).m, abb_mask[sq].m )
#define XorFile(sq,b)       (b).m = _mm_xor_si128( (b).m, abb_mask_rl90[sq].m )
#define XorDiag1(sq,b)      (b).m = _mm_xor_si128( (b).m, abb_mask_rr45[sq].m )
#define XorDiag2(sq,b)      (b).m = _mm_xor_si128( (b).m, abb_mask_rl45[sq].m )
#define SetClear(b)         (b).m = _mm_xor_si128( (b).m, bb_set_clear.m )
#define SetClearFile(sq1,sq2,b)  (b).m= _mm_xor_si128( (b).m,                 \
                                        _mm_or_si128( abb_mask_rl90[sq1].m,   \
                                                      abb_mask_rl90[sq2].m ) )
#define SetClearDiag1(sq1,sq2,b) (b).m= _mm_xor_si128( (b).m,                 \
                                        _mm_or_si128( abb_mask_rr45[sq1].m,   \
                                                      abb_mask_rr45[sq2].m ) )
#define SetClearDiag2(sq1,sq2,b) (b).m= _mm_xor_si128( (b).m,                 \
                                        _mm_or_si128( abb_mask_rl45[sq1].m,   \
                                                      abb_mask_rl45[sq2].m ) )

typedef union {
	unsigned int p[4];
	__m128i m;
} bitboard_t;

#else /* NO SSE2 */

#define BBTest(b)           ( (b).p[0] | (b).p[1] | (b).p[2] )
#define BBIni(b)            (b).p[0] = (b).p[1] = (b).p[2] = 0
#define BBNot(b,b1)         (b).p[0] = ~(b1).p[0],                      \
                            (b).p[1] = ~(b1).p[1],                      \
                            (b).p[2] = ~(b1).p[2]
#define BBAnd(b,b1,b2)      (b).p[0] = (b1).p[0] & (b2).p[0],           \
                            (b).p[1] = (b1).p[1] & (b2).p[1],           \
                            (b).p[2] = (b1).p[2] & (b2).p[2]
#define BBOr(b,b1,b2)       (b).p[0] = (b1).p[0] | (b2).p[0],           \
                            (b).p[1] = (b1).p[1] | (b2).p[1],           \
                            (b).p[2] = (b1).p[2] | (b2).p[2]
#define BBXor(b,b1,b2)      (b).p[0] = (b1).p[0] ^ (b2).p[0],           \
                            (b).p[1] = (b1).p[1] ^ (b2).p[1],           \
                            (b).p[2] = (b1).p[2] ^ (b2).p[2]
#define BBAndOr(b,b1,b2)    (b).p[0] |= (b1).p[0] & (b2).p[0],          \
                            (b).p[1] |= (b1).p[1] & (b2).p[1],          \
                            (b).p[2] |= (b1).p[2] & (b2).p[2]
#define BBNotAnd(b,b1,b2)   (b).p[0] = (b1).p[0] & ~(b2).p[0],          \
                            (b).p[1] = (b1).p[1] & ~(b2).p[1],          \
                            (b).p[2] = (b1).p[2] & ~(b2).p[2]
#define BBContract(b1,b2)   ( ( (b1).p[0] & (b2).p[0] )                 \
                            | ( (b1).p[1] & (b2).p[1] )                 \
                            | ( (b1).p[2] & (b2).p[2] ) )
#define Xor(sq,b)           (b).p[0] ^= abb_mask[sq].p[0],              \
                            (b).p[1] ^= abb_mask[sq].p[1],              \
                            (b).p[2] ^= abb_mask[sq].p[2]
#define XorFile(sq,b)       (b).p[0] ^= abb_mask_rl90[sq].p[0],         \
                            (b).p[1] ^= abb_mask_rl90[sq].p[1],         \
                            (b).p[2] ^= abb_mask_rl90[sq].p[2]
#define XorDiag1(sq,b)      (b).p[0] ^= abb_mask_rr45[sq].p[0],         \
                            (b).p[1] ^= abb_mask_rr45[sq].p[1],         \
                            (b).p[2] ^= abb_mask_rr45[sq].p[2]
#define XorDiag2(sq,b)      (b).p[0] ^= abb_mask_rl45[sq].p[0],         \
                            (b).p[1] ^= abb_mask_rl45[sq].p[1],         \
                            (b).p[2] ^= abb_mask_rl45[sq].p[2]
#define SetClear(b)         (b).p[0] ^= (bb_set_clear.p[0]),            \
                            (b).p[1] ^= (bb_set_clear.p[1]),            \
                            (b).p[2] ^= (bb_set_clear.p[2])
#define SetClearFile(sq1,sq2,b)                                         \
    (b).p[0] ^= ( abb_mask_rl90[sq1].p[0] | abb_mask_rl90[sq2].p[0] ),  \
    (b).p[1] ^= ( abb_mask_rl90[sq1].p[1] | abb_mask_rl90[sq2].p[1] ),  \
    (b).p[2] ^= ( abb_mask_rl90[sq1].p[2] | abb_mask_rl90[sq2].p[2] )

#define SetClearDiag1(sq1,sq2,b) \
    (b).p[0] ^= ((abb_mask_rr45[sq1].p[0])|(abb_mask_rr45[sq2].p[0])), \
    (b).p[1] ^= ((abb_mask_rr45[sq1].p[1])|(abb_mask_rr45[sq2].p[1])), \
    (b).p[2] ^= ((abb_mask_rr45[sq1].p[2])|(abb_mask_rr45[sq2].p[2]))

#define SetClearDiag2(sq1,sq2,b) \
    (b).p[0] ^= ((abb_mask_rl45[sq1].p[0])|(abb_mask_rl45[sq2].p[0])), \
    (b).p[1] ^= ((abb_mask_rl45[sq1].p[1])|(abb_mask_rl45[sq2].p[1])), \
    (b).p[2] ^= ((abb_mask_rl45[sq1].p[2])|(abb_mask_rl45[sq2].p[2]))



typedef struct { unsigned int p[3]; } bitboard_t;

#endif /* HAVE_SSE2 */

Inline int CONV2
popu_count012(unsigned int u0, unsigned int u1, unsigned int u2)
{
	int counter = 0;
#if defined(HAVE_SSE4) && defined(_MSC_VER)

	counter += __popcnt(u0);
	counter += __popcnt(u1);
	counter += __popcnt(u2);
#else

	while (u0) { counter++;  u0 &= u0 - 1U; }
	while (u1) { counter++;  u1 &= u1 - 1U; }
	while (u2) { counter++;  u2 &= u2 - 1U; }
#endif
	return counter;
}

#if defined(_MSC_VER)

Inline int CONV2
first_one012(unsigned int u0, unsigned int u1, unsigned int u2)
{
	unsigned long index;

	if (_BitScanReverse(&index, u0)) { return 26 - index; }
	if (_BitScanReverse(&index, u1)) { return 53 - index; }
	_BitScanReverse(&index, u2);
	return 80 - index;
}

Inline int CONV2
last_one210(unsigned int u2, unsigned int u1, unsigned int u0)
{
	unsigned long index;

	if (_BitScanForward(&index, u2)) { return 80 - index; }
	if (_BitScanForward(&index, u1)) { return 53 - index; }
	_BitScanForward(&index, u0);
	return 26 - index;
}

Inline int CONV2
first_one01(unsigned int u0, unsigned int u1)
{
	unsigned long index;

	if (_BitScanReverse(&index, u0)) { return 26 - index; }
	_BitScanReverse(&index, u1);
	return 53 - index;
}

Inline int CONV2
first_one12(unsigned int u1, unsigned int u2)
{
	unsigned long index;

	if (_BitScanReverse(&index, u1)) { return 53 - index; }
	_BitScanReverse(&index, u2);
	return 80 - index;
}

Inline int CONV2
last_one01(unsigned int u0, unsigned int u1)
{
	unsigned long index;

	if (_BitScanForward(&index, u1)) { return 53 - index; }
	_BitScanForward(&index, u0);
	return 26 - index;
}

Inline int CONV2
last_one12(unsigned int u1, unsigned u2)
{
	unsigned long index;

	if (_BitScanForward(&index, u2)) { return 80 - index; }
	_BitScanForward(&index, u1);
	return 53 - index;
}

Inline int CONV2
first_one1(unsigned int u1)
{
	unsigned long index;

	_BitScanReverse(&index, u1);
	return 53 - index;
}

Inline int CONV2
first_one2(unsigned int u2)
{
	unsigned long index;

	_BitScanReverse(&index, u2);
	return 80 - index;
}

Inline int CONV2
last_one0(unsigned int u0)
{
	unsigned long index;

	_BitScanForward(&index, u0);
	return 26 - index;
}

Inline int CONV2
last_one1(unsigned int u1)
{
	unsigned long index;

	_BitScanForward(&index, u1);
	return 53 - index;
}

#elif defined(__GNUC__) && ( defined(__i386__) || defined(__x86_64__) )

int
first_one012(unsigned int u0, unsigned int u1, unsigned int u2)
{
	if (u0) { return __builtin_clz(u0) - 5; }
	if (u1) { return __builtin_clz(u1) + 22; }
	return __builtin_clz(u2) + 49;
}


int
last_one210(unsigned int u2, unsigned int u1, unsigned int u0)
{
	if (u2) { return 80 - __builtin_ctz(u2); }
	if (u1) { return 53 - __builtin_ctz(u1); }
	return 26 - __builtin_ctz(u0);
}


int
first_one01(unsigned int u0, unsigned int u1)
{
	if (u0) { return __builtin_clz(u0) - 5; }
	return __builtin_clz(u1) + 22;
}


int
first_one12(unsigned int u1, unsigned int u2)
{
	if (u1) { return __builtin_clz(u1) + 22; }
	return __builtin_clz(u2) + 49;
}


int
last_one01(unsigned int u0, unsigned int u1)
{
	if (u1) { return 53 - __builtin_ctz(u1); }
	return 26 - __builtin_ctz(u0);
}


int
last_one12(unsigned int u1, unsigned int u2)
{
	if (u2) { return 80 - __builtin_ctz(u2); }
	return 53 - __builtin_ctz(u1);
}


int first_one1(unsigned int u1) { return __builtin_clz(u1) + 22; }
int first_one2(unsigned int u2) { return __builtin_clz(u2) + 49; }
int last_one0(unsigned int u0) { return 26 - __builtin_ctz(u0); }
int last_one1(unsigned int u1) { return 53 - __builtin_ctz(u1); }

#else

int
first_one012(unsigned int u0, unsigned int u1, unsigned int u2)
{
	if (u0 & 0x7fc0000) { return aifirst_one[u0 >> 18] + 0; }
	if (u0 & 0x7fffe00) { return aifirst_one[u0 >> 9] + 9; }
	if (u0 & 0x7ffffff) { return aifirst_one[u0] + 18; }

	if (u1 & 0x7fc0000) { return aifirst_one[u1 >> 18] + 27; }
	if (u1 & 0x7fffe00) { return aifirst_one[u1 >> 9] + 36; }
	if (u1 & 0x7ffffff) { return aifirst_one[u1] + 45; }

	if (u2 & 0x7fc0000) { return aifirst_one[u2 >> 18] + 54; }
	if (u2 & 0x7fffe00) { return aifirst_one[u2 >> 9] + 63; }
	return aifirst_one[u2] + 72;
}


int
last_one210(unsigned int u2, unsigned int u1, unsigned int u0)
{
	unsigned int j;

	j = u2 & 0x00001ff;  if (j) { return ailast_one[j] + 72; }
	j = u2 & 0x003ffff;  if (j) { return ailast_one[j >> 9] + 63; }
	if (u2 & 0x7ffffff) { return ailast_one[u2 >> 18] + 54; }

	j = u1 & 0x00001ff;  if (j) { return ailast_one[j] + 45; }
	j = u1 & 0x003ffff;  if (j) { return ailast_one[j >> 9] + 36; }
	if (u1 & 0x7ffffff) { return ailast_one[u1 >> 18] + 27; }

	j = u0 & 0x00001ff;  if (j) { return ailast_one[j] + 18; }
	j = u0 & 0x003ffff;  if (j) { return ailast_one[j >> 9] + 9; }
	return ailast_one[u0 >> 18];
}


int
first_one01(unsigned int u0, unsigned int u1)
{
	if (u0 & 0x7fc0000) { return aifirst_one[u0 >> 18] + 0; }
	if (u0 & 0x7fffe00) { return aifirst_one[u0 >> 9] + 9; }
	if (u0 & 0x7ffffff) { return aifirst_one[u0] + 18; }

	if (u1 & 0x7fc0000) { return aifirst_one[u1 >> 18] + 27; }
	if (u1 & 0x7fffe00) { return aifirst_one[u1 >> 9] + 36; }
	return aifirst_one[u1] + 45;
}


int
first_one12(unsigned int u1, unsigned int u2)
{
	if (u1 & 0x7fc0000) { return aifirst_one[u1 >> 18] + 27; }
	if (u1 & 0x7fffe00) { return aifirst_one[u1 >> 9] + 36; }
	if (u1 & 0x7ffffff) { return aifirst_one[u1] + 45; }

	if (u2 & 0x7fc0000) { return aifirst_one[u2 >> 18] + 54; }
	if (u2 & 0x7fffe00) { return aifirst_one[u2 >> 9] + 63; }
	return aifirst_one[u2] + 72;
}


int
last_one01(unsigned int u0, unsigned int u1)
{
	unsigned int j;

	j = u1 & 0x00001ff;  if (j) { return ailast_one[j] + 45; }
	j = u1 & 0x003ffff;  if (j) { return ailast_one[j >> 9] + 36; }
	if (u1 & 0x7ffffff) { return ailast_one[u1 >> 18] + 27; }

	j = u0 & 0x00001ff;  if (j) { return ailast_one[j] + 18; }
	j = u0 & 0x003ffff;  if (j) { return ailast_one[j >> 9] + 9; }
	return ailast_one[u0 >> 18];
}


int
last_one12(unsigned int u1, unsigned int u2)
{
	unsigned int j;

	j = u2 & 0x00001ff;  if (j) { return ailast_one[j] + 72; }
	j = u2 & 0x003ffff;  if (j) { return ailast_one[j >> 9] + 63; }
	if (u2 & 0x7ffffff) { return ailast_one[u2 >> 18] + 54; }

	j = u1 & 0x00001ff;  if (j) { return ailast_one[j] + 45; }
	j = u1 & 0x003ffff;  if (j) { return ailast_one[j >> 9] + 36; }
	return ailast_one[u1 >> 18] + 27;
}


int
first_one1(unsigned int u1)
{
	if (u1 & 0x7fc0000U) { return aifirst_one[u1 >> 18] + 27; }
	if (u1 & 0x7fffe00U) { return aifirst_one[u1 >> 9] + 36; }
	return aifirst_one[u1] + 45;
}


int
first_one2(unsigned int u2)
{
	if (u2 & 0x7fc0000U) { return aifirst_one[u2 >> 18] + 54; }
	if (u2 & 0x7fffe00U) { return aifirst_one[u2 >> 9] + 63; }
	return aifirst_one[u2] + 72;
}


int
last_one0(unsigned int i)
{
	unsigned int j;

	j = i & 0x00001ffU;  if (j) { return ailast_one[j] + 18; }
	j = i & 0x003ffffU;  if (j) { return ailast_one[j >> 9] + 9; }
	return ailast_one[i >> 18];
}


int
last_one1(unsigned int u1)
{
	unsigned int j;

	j = u1 & 0x00001ffU;  if (j) { return ailast_one[j] + 45; }
	j = u1 & 0x003ffffU;  if (j) { return ailast_one[j >> 9] + 36; }
	return ailast_one[u1 >> 18] + 27;
}

#endif

#endif /* BITOP_H */