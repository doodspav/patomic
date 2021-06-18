#ifndef PATOMIC_MSVC_GENERIC_UINT_H
#define PATOMIC_MSVC_GENERIC_UINT_H

#if defined(_MSC_VER)

#include <patomic/macros/force_inline.h>

/*
 * TYPES & OPERATIONS
 *
 * - the whole reason for this existing is msvc has no __int128
 * - so we can't just do __int##width in msvc.c
 * - we need macros that work both on __intN and our custom 128bit type
 *
 * - only UNSIGNED types are supported here (that's all we need)
 */

typedef unsigned __int8  patomic_uint8_t;
typedef unsigned __int16 patomic_uint16_t;
typedef unsigned __int32 patomic_uint32_t;
typedef unsigned __int64 patomic_uint64_t;

/*
 * 128 BIT TYPE
 *
 * - unfortunately no __int128
 * - assuming little endian and twos complement
 * - which should be fine for MSVC
 */

typedef struct {
    unsigned __int64 arr[2];
} patomic_uint128_t;

#define PATOMIC_LOW_64(obj) ((obj).arr[0])
#define PATOMIC_HIGH_64(obj) ((obj).arr[1])

static PATOMIC_FORCE_INLINE patomic_uint128_t
patomic_create_128(
    patomic_uint64_t high,
    patomic_uint64_t low
)
{
    patomic_uint128_t ret;
    PATOMIC_HIGH_64(ret) = high;
    PATOMIC_LOW_64(ret) = low;
    return ret;
}

static PATOMIC_FORCE_INLINE patomic_uint128_t
patomic_lshift_128(
    patomic_uint128_t obj,
    int offset
)
{
    // TODO
    (void) offset;
    return obj;
}

static PATOMIC_FORCE_INLINE patomic_uint128_t
patomic_add_128(
    patomic_uint128_t a,
    patomic_uint128_t b
)
{
    // TODO
    (void) b;
    return a;
}

static PATOMIC_FORCE_INLINE patomic_uint128_t
patomic_inc_128(
    patomic_uint128_t obj
)
{
    return patomic_add_128(obj, patomic_create_128(0, 1));
}

static PATOMIC_FORCE_INLINE patomic_uint128_t
patomic_dec_128(
    patomic_uint128_t obj
)
{
    return patomic_add_128(obj, patomic_create_128(-1, -1));
}


/*
 * OPERATIONS
 *
 * - ALL RETURN VALUES ARE TYPE CAST
 *
 * - set_to_one (equiv. to: obj = 1)
 * - neq_zero (equiv. to: obj == 0)
 * - lshift (equiv. to: obj << val)
 * - or (equiv. to: obj | arg)
 * - xor (equiv. to: obj ^ arg)
 * - and (equiv. to: obj & arg)
 * - neg (equiv. to: ++(~obj))
 */

#define PATOMIC_UINT_SET_TO_ONE(width, obj) PATOMIC_UINT_SET_TO_ONE_##width(obj)
#define PATOMIC_UINT_SET_TO_ONE_8(obj)  obj = (patomic_uint8_t) 1
#define PATOMIC_UINT_SET_TO_ONE_16(obj) obj = (patomic_uint16_t) 1
#define PATOMIC_UINT_SET_TO_ONE_32(obj) obj = (patomic_uint32_t) 1
#define PATOMIC_UINT_SET_TO_ONE_64(obj) obj = (patomic_uint64_t) 1
#define PATOMIC_UINT_SET_TO_ONE_128(obj) obj = patomic_create_128(0, 1)

#define PATOMIC_UINT_NEQ_ZERO(width, obj) PATOMIC_UINT_NEQ_ZERO_##width(obj)
#define PATOMIC_UINT_NEQ_ZERO_8(obj)  ((obj) != 0u)
#define PATOMIC_UINT_NEQ_ZERO_16(obj) ((obj) != 0u)
#define PATOMIC_UINT_NEQ_ZERO_32(obj) ((obj) != 0u)
#define PATOMIC_UINT_NEQ_ZERO_64(obj) ((obj) != 0u)
#define PATOMIC_UINT_NEQ_ZERO_128(obj) ((obj).arr[0] != 0u || (obj).arr[1] != 0u)

#define PATOMIC_UINT_LSHIFT(width, obj, val) PATOMIC_UINT_LSHIFT_##width(obj, val)
#define PATOMIC_UINT_LSHIFT_8(obj, val)  \
    ((patomic_uint8_t) ((patomic_uint8_t) (obj) << (val)))
#define PATOMIC_UINT_LSHIFT_16(obj, val) \
    ((patomic_uint16_t) ((patomic_uint16_t) (obj) << (val)))
#define PATOMIC_UINT_LSHIFT_32(obj, val) \
    ((patomic_uint32_t) ((patomic_uint32_t) (obj) << (val)))
#define PATOMIC_UINT_LSHIFT_64(obj, val) \
    ((patomic_uint64_t) ((patomic_uint64_t) (obj) << (val)))
#define PATOMIC_UINT_LSHIFT_128(obj, val) patomic_lshift_128(obj, (int) (val))

#define PATOMIC_UINT_OR(width, obj, arg) PATOMIC_UINT_OR_##width(obj, arg)
#define PATOMIC_UINT_OR_8(obj, arg)  \
    ((patomic_uint8_t) ((patomic_uint8_t) (obj) | (patomic_uint8_t) (arg)))
#define PATOMIC_UINT_OR_16(obj, arg) \
    ((patomic_uint16_t) ((patomic_uint16_t) (obj) | (patomic_uint16_t) (arg)))
#define PATOMIC_UINT_OR_32(obj, arg) \
    ((patomic_uint32_t) ((patomic_uint32_t) (obj) | (patomic_uint32_t) (arg)))
#define PATOMIC_UINT_OR_64(obj, arg) \
   ((patomic_uint64_t) ((patomic_uint64_t) (obj) | (patomic_uint64_t) (arg)))
#define PATOMIC_UINT_OR_128(obj, arg)                \
    patomic_create_128(                              \
        PATOMIC_HIGH_64(obj) | PATOMIC_HIGH_64(arg), \
        PATOMIC_LOW_64(obj) | PATOMIC_LOW_64(arg)    \
    )

#define PATOMIC_UINT_XOR(width, obj, arg) PATOMIC_UINT_XOR_##width(obj, arg)
#define PATOMIC_UINT_XOR_8(obj, arg)  \
    ((patomic_uint8_t) ((patomic_uint8_t) (obj) ^ (patomic_uint8_t) (arg)))
#define PATOMIC_UINT_XOR_16(obj, arg) \
    ((patomic_uint16_t) ((patomic_uint16_t) (obj) ^ (patomic_uint16_t) (arg)))
#define PATOMIC_UINT_XOR_32(obj, arg) \
    ((patomic_uint32_t) ((patomic_uint32_t) (obj) ^ (patomic_uint32_t) (arg)))
#define PATOMIC_UINT_XOR_64(obj, arg) \
    ((patomic_uint64_t) ((patomic_uint64_t) (obj) ^ (patomic_uint64_t) (arg)))
#define PATOMIC_UINT_XOR_128(obj, arg)               \
    patomic_create_128(                              \
        PATOMIC_HIGH_64(obj) ^ PATOMIC_HIGH_64(arg), \
        PATOMIC_LOW_64(obj) ^ PATOMIC_LOW_64(arg)    \
    )

#define PATOMIC_UINT_AND(width, obj, arg) PATOMIC_UINT_AND_##width(obj, arg)
#define PATOMIC_UINT_AND_8(obj, arg)  \
    ((patomic_uint8_t) ((patomic_uint8_t) (obj) & (patomic_uint8_t) (arg)))
#define PATOMIC_UINT_AND_16(obj, arg) \
    ((patomic_uint16_t) ((patomic_uint16_t) (obj) & (patomic_uint16_t) (arg)))
#define PATOMIC_UINT_AND_32(obj, arg) \
    ((patomic_uint32_t) ((patomic_uint32_t) (obj) & (patomic_uint32_t) (arg)))
#define PATOMIC_UINT_AND_64(obj, arg) \
    ((patomic_uint64_t) ((patomic_uint64_t) (obj) & (patomic_uint64_t) (arg)))
#define PATOMIC_UINT_AND_128(obj, arg)               \
    patomic_create_128(                              \
        PATOMIC_HIGH_64(obj) & PATOMIC_HIGH_64(arg), \
        PATOMIC_LOW_64(obj) & PATOMIC_LOW_64(arg)    \
    )

#define PATOMIC_UINT_NEG(width, obj) PATOMIC_UINT_NEG_##width(obj)
#define PATOMIC_UINT_NEG_8(obj)  \
    ((patomic_uint8_t) (((patomic_uint8_t) ~((patomic_uint8_t) (obj))) + 1u))
#define PATOMIC_UINT_NEG_16(obj) \
    ((patomic_uint16_t) (((patomic_uint16_t) ~((patomic_uint16_t) (obj))) + 1u))
#define PATOMIC_UINT_NEG_32(obj) \
    ((patomic_uint32_t) (((patomic_uint32_t) ~((patomic_uint32_t) (obj))) + 1u))
#define PATOMIC_UINT_NEG_64(obj) \
    ((patomic_uint64_t) (((patomic_uint64_t) ~((patomic_uint64_t) (obj))) + 1u))
#define PATOMIC_UINT_NEG_128(obj) \
    patomic_inc_128(              \
        patomic_create_128(       \
            ~PATOMIC_HIGH_64(obj),\
            ~PATOMIC_LOW_64(obj), \
    ))


#endif  /* defined(_MSC_VER) */

#endif  /* !PATOMIC_MSVC_GENERIC_UINT_H */
