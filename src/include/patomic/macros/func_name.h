#ifndef PATOMIC_FUNC_NAME

#include <patomic/patomic_config.h>

/* used internally */
#undef PATOMIC_FUNC_NAME_

#if defined(__FUNCSIG__)
    #define PATOMIC_FUNC_NAME_ __FUNCSIG__
#elif PATOMIC_HAS_GNU_PRETTY_FUNCTION
    #define PATOMIC_FUNC_NAME_ __PRETTY_FUNCTION__
#elif PATOMIC_HAS_GNU_PRETTY_FUNCTION_EXTN
    #define PATOMIC_FUNC_NAME_ __extension__ __PRETTY_FUNCTION__
#elif PATOMIC_HAS_GNU_FUNCTION
    #define PATOMIC_FUNC_NAME_ __FUNCTION__
#elif PATOMIC_HAS_GNU_FUNCTION_EXTN
    #define PATOMIC_FUNC_NAME_ __extension__ __FUNCTION__
#elif PATOMIC_HAS_FUNC
    #define PATOMIC_FUNC_NAME_ __func__
#elif PATOMIC_HAS_FUNC_EXTN
    #define PATOMIC_FUNC_NAME_ __extension__ __func__
#else
    #define PATOMIC_FUNC_NAME_ "(unknown function name)"
#endif

/**
 * @addtogroup macros
 *
 * @brief
 *   The identifier of the variable containing the function name in the current
 *   scope.
 */
#define PATOMIC_FUNC_NAME PATOMIC_FUNC_NAME_

#endif  /* PATOMIC_FUNC_NAME */
