#ifndef PATOMIC_CONFIG_H
#define PATOMIC_CONFIG_H

#include <patomic/_patomic_config.h>


/*
 * Some of the following macros are generated by CMake at build time. CMake
 * generates the <patomic/_patomic_config.h> file which is included below.
 *
 * If you are building this project without CMake, you have three options:
 * 1. provide your own version of <patomic/_patomic_config.h>.
 * 2. modify the defaults for those macros in this file and remove the include.
 * 3. define those macros via compiler flags and remove the include.
 *
 * Some macros may error if predefined as a safety measure, requiring option 2.
 */

#include <patomic/_patomic_config.h>


/*
 * The following macros may be modified and overridden safely, At worst, this
 * may cause a compile-time warning/error or limit runtime functionality
 * exposed by this library.
 *
 * The @note contains expected requirements for the functionality specified by
 * the macro, as a hint to users who may try to manually set their values.
 */



/*
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
 * @addtogroup config.dangerous
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
 * @addtogroup config.dangerous
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
 * @addtogroup config.dangerous
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


/* check exactly one integer representation is supported */
#if PATOMIC_HAS_IR_SIGN_MAGNITUDE && \
    PATOMIC_HAS_IR_ONES_COMPL     && \
    PATOMIC_HAS_IR_TWOS_COMPL
    #error "Multiple integer representations are configured as supported"
#elif !( PATOMIC_HAS_IR_SIGN_MAGNITUDE || \
         PATOMIC_HAS_IR_ONES_COMPL     || \
         PATOMIC_HAS_IR_TWOS_COMPL )
    #error "No integer representation is configured as supported"
#endif


#endif  /* PATOMIC_CONFIG_H */