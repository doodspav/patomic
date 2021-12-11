#ifndef PATOMIC_FUNC_NAME

#include <patomic/patomic_config.h>

#if defined(__FUNCSIG__)
    #define PATOMIC_FUNC_NAME __FUNCSIG__
#elif PATOMIC_HAVE_GNU_PRETTY_FUNCTION
    #define PATOMIC_FUNC_NAME __PRETTY_FUNCTION__
#elif PATOMIC_HAVE_GNU_PRETTY_FUNCTION_EXTN
    #define PATOMIC_FUNC_NAME (__extension__ __PRETTY_FUNCTION__)
#elif PATOMIC_HAVE_GNU_FUNCTION
    #define PATOMIC_FUNC_NAME __FUNCTION__
#elif PATOMIC_HAVE_GNU_FUNCTION_EXTN
    #define PATOMIC_FUNC_NAME (__extension__ __FUNCTION__)
#elif PATOMIC_HAVE_FUNC
    #define PATOMIC_FUNC_NAME __func__
#else
    #define PATOMIC_FUNC_NAME "(unknown)"
#endif

#endif  /* !PATOMIC_FUNC_NAME */
