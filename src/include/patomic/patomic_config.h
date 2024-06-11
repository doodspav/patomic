#ifndef PATOMIC_CONFIG_H
#define PATOMIC_CONFIG_H


/*
 * Some of the following macros are generated by CMake at build time. CMake
 * generates the <patomic/_patomic_config.h> file which is included below.
 *
 * If you are building this project without CMake, you have three options:
 * 1. Provide your own version of <patomic/_patomic_config.h>.
 * 2. Modify the defaults for those macros in this file and remove the include.
 * 3. Define those macros via compiler flags and remove the include.
 *
 * Some macros may error if predefined as a safety measure, requiring option 2.
 */

#include <patomic/_patomic_config.h>


/*
 * SAFE CONSTANTS
 * ==============
 *
 * The following macros may be modified and overridden safely, At worst, this
 * may cause a compile-time warning/error or limit runtime functionality
 * exposed by this library.
 *
 * The @note contains expected requirements for the functionality specified by
 * the macro, as a hint to users who may try to manually set their values.
 */

#ifndef PATOMIC_HAS_EXTN
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   '__extension__' is available as a keyword.
     *
     * @note
     *   Usually required: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_EXTN 0
#endif


#ifndef PATOMIC_HAS_LONG_LONG
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   'long long' is available as a type.
     *
     * @note
     *   Usually requires: C99.
     */
    #define PATOMIC_HAS_LONG_LONG 0
#endif


#ifndef PATOMIC_HAS_LONG_LONG_EXTN
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   '__extension__ long long' is available as a type.
     *
     * @note
     *   Usually requires: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_LONG_LONG_EXTN 0
#endif


#ifndef PATOMIC_HAS_MS_INT128
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   '__int128' is available as a type.
     *
     * @note
     *   Usually requires: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_MS_INT128 0
#endif


#ifndef PATOMIC_HAS_MS_INT128_EXTN
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   '__extension__ __int128' is available as a type.
     *
     * @note
     *   Usually requires: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_MS_INT128_EXTN 0
#endif


#ifndef PATOMIC_HAS_STDINT
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   <stdint.h> header is available.
     *
     * @note
     *   Usually requires: C99.
     */
    #define PATOMIC_HAS_STDINT 0
#endif


#ifndef PATOMIC_HAS_STDINT_INTPTR
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   <stdint.h> header is available and makes 'intptr_t' available as a
     *   type.
     *
     * @note
     *   Usually requires: C99 (but the type is optional even then).
     */
    #define PATOMIC_HAS_STDINT_INTPTR 0
#endif


#ifndef PATOMIC_HAS_STDINT_INTPTR_EXTN
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   <stdint.h> header is available and makes '__extension__ intptr_t'
     *   available as a type.
     *
     * @note
     *   Usually requires: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_STDINT_INTPTR_EXTN 0
#endif


#ifndef PATOMIC_HAS_STDDEF_INTPTR
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   <stddef.h> header is available and makes 'intptr_t' available as a
     *   type.
     *
     * @note
     *   Usually requires: old compilers not supporting <stdint.h> (pre-C99).
     *   Microsoft puts intptr_t here in old versions of msvc.
     */
    #define PATOMIC_HAS_STDDEF_INTPTR 0
#endif


#ifndef PATOMIC_HAS_STDDEF_INTPTR_EXTN
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   <stddef.h> header is available and makes '__extension__ intptr_t'
     *   available as a type.
     *
     * @note
     *   Usually requires: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_STDDEF_INTPTR_EXTN 0
#endif


#ifndef PATOMIC_HAS_FUNC
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   '__func__' is a pre-defined identifier.
     *
     * @note
     *   Usually requires: C99.
     */
    #define PATOMIC_HAS_FUNC 0
#endif


#ifndef PATOMIC_HAS_FUNC_EXTN
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   '__extension__ __func__' is a pre-defined identifier.
     *
     * @note
     *   Usually requires: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_FUNC_EXTN 0
#endif


#ifndef PATOMIC_HAS_GNU_FUNCTION
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   '__FUNCTION__' is a pre-defined identifier.
     *
     * @note
     *   Usually requires: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_GNU_FUNCTION 0
#endif


#ifndef PATOMIC_HAS_GNU_FUNCTION_EXTN
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   '__extension__ __FUNCTION__' is a pre-defined identifier.
     *
     * @note
     *   Usually requires: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_GNU_FUNCTION_EXTN 0
#endif


#ifndef PATOMIC_HAS_GNU_PRETTY_FUNCTION
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   '__PRETTY_FUNCTION__' is a pre-defined identifier.
     *
     * @note
     *   Usually requires: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_GNU_PRETTY_FUNCTION 0
#endif


#ifndef PATOMIC_HAS_GNU_PRETTY_FUNCTION_EXTN
    /**
     * @addtogroup config.safe
     *
     * @brief
     *   '__extension__ __PRETTY_FUNCTION__' is a pre-defined identifier.
     *
     * @note
     *   Usually requires: GNU compatible(-ish) compiler.
     */
    #define PATOMIC_HAS_GNU_PRETTY_FUNCTION_EXTN 0
#endif


/*
 * UNSAFE CONSTANTS
 * ================
 *
 * The following macros should never be modified unless cross-compiling for a
 * target platform which does not define them to have the same value as the host
 * platform (and for some reason the compiler defines them with the host
 * platform's value, likely a compiler bug).
 *
 * Changing these macros incorrectly WILL result in a library which exhibits
 * undefined behaviour and will likely function incorrectly.
 *
 * For this reason, they cannot be predefined; they can only be explicitly
 * modified in this file.
 */

#include <limits.h>


#ifdef PATOMIC_HAS_IR_SIGN_MAGNITUDE
    #undef PATOMIC_HAS_IR_SIGN_MAGNITUDE
    #error "Integer representation macro values MUST be changed in source"
#endif
/**
 * @addtogroup config.unsafe
 *
 * @brief
 *   Integer representation on the target platform is sign-magnitude.
 *
 * @warning
 *   This macro cannot be overridden by predefining it; it must be manually
 *   modified in the source file.
 */
#define PATOMIC_HAS_IR_SIGN_MAGNITUDE \
    ((-INT_MAX == INT_MIN) && ((-1 & 1) != 0))


#ifdef PATOMIC_HAS_IR_ONES_COMPL
    #undef PATOMIC_HAS_IR_ONES_COMPL
    #error "Integer representation macro values MUST be changed in source"
#endif
/**
 * @addtogroup config.unsafe
 *
 * @brief
 *   Integer representation on the target platform is one's complement.
 *
 * @warning
 *   This macro cannot be overridden by predefining it; it must be manually
 *   modified in the source file.
 */
#define PATOMIC_HAS_IR_ONES_COMPL \
    ((-INT_MAX == INT_MIN) && ((-1 & 1) == 0))


#ifdef PATOMIC_HAS_IR_TWOS_COMPL
    #undef PATOMIC_HAS_IR_TWOS_COMPL
    #error "Integer representation macro values MUST be changed in source"
#endif
/**
 * @addtogroup config.unsafe
 *
 * @brief
 *   Integer representation on the target platform is two's complement.
 *
 * @warning
 *   This macro cannot be overridden by predefining it; it must be manually
 *   modified in the source file.
 */
#define PATOMIC_HAS_IR_TWOS_COMPL \
    (-INT_MAX != INT_MIN)


/* check that exactly one integer representation is supported */
#if ( PATOMIC_HAS_IR_SIGN_MAGNITUDE + \
      PATOMIC_HAS_IR_ONES_COMPL     + \
      PATOMIC_HAS_IR_TWOS_COMPL ) > 1
    #error "Multiple integer representations are configured as supported"
#elif ( PATOMIC_HAS_IR_SIGN_MAGNITUDE + \
        PATOMIC_HAS_IR_ONES_COMPL     + \
        PATOMIC_HAS_IR_TWOS_COMPL ) < 1
    #error "No integer representation is configured as supported"
#endif


#endif  /* PATOMIC_CONFIG_H */
