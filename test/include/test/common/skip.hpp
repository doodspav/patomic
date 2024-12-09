#ifndef PATOMIC_TEST_COMMON_SKIP_HPP
#define PATOMIC_TEST_COMMON_SKIP_HPP

#include "name.hpp"


/// @brief
///   Requires a semicolon be placed after calling this macro.
#ifndef REQUIRE_SEMICOLON
    #define REQUIRE_SEMICOLON() \
        static_assert(true, "")
#endif


/// @brief
///   Returns from the current test function with a skip message if the given
///   order is not a valid store order.
#define SKIP_NON_STORE_ORDER(order)                                      \
    if (!PATOMIC_IS_VALID_STORE_ORDER(order))                            \
    {                                                                    \
        GTEST_SKIP() << "Skipping; order '" << ::test::name_order(order) \
                     << "' (" << static_cast<int>(order)                 \
                     << ") is not valid for a store operation";          \
    }                                                                    \
    REQUIRE_SEMICOLON()


/// @brief
///   Returns from the current test function with a skip message if the given
///   order is not a valid load order.
#define SKIP_NON_LOAD_ORDER(order)                                       \
    if (!PATOMIC_IS_VALID_LOAD_ORDER(order))                             \
    {                                                                    \
        GTEST_SKIP() << "Skipping; order '" << ::test::name_order(order) \
                     << "' (" << static_cast<int>(order)                 \
                     << ") is not valid for a load operation";           \
    }                                                                    \
    REQUIRE_SEMICOLON()


/// @brief
///   Returns from the current test function with a skip message if the given
///   op function pointer is NULL.
#define SKIP_NULL_OP_FP(fp, name)                             \
    if ((fp) == nullptr)                                      \
    {                                                         \
        GTEST_SKIP() << "Skipping; op '" name "' is nullptr"; \
    }                                                         \
    REQUIRE_SEMICOLON()


#endif  // PATOMIC_TEST_COMMON_SKIP_HPP
