#ifndef PATOMIC_TEST_COMMON_TRANSACTION_HPP
#define PATOMIC_TEST_COMMON_TRANSACTION_HPP

#include "generic_int.hpp"
#include "name.hpp"

#include <type_traits>


/// @brief
///   Requires a semicolon be placed after calling this macro.
#ifndef REQUIRE_SEMICOLON
    #define REQUIRE_SEMICOLON() \
        static_assert(true, "")
#endif


/// @brief
///   Check that a status has the expected sub-values.
#define ASSERT_TSX_STATUS_EQ(status, code, info, reason) \
    ASSERT_EQ(                                           \
        code,                                            \
        patomic_transaction_status_exit_code(status)     \
    );                                                   \
    ASSERT_EQ(                                           \
        info,                                            \
        patomic_transaction_status_exit_info(status)     \
    );                                                   \
    ASSERT_EQ(                                           \
        reason,                                          \
        patomic_transaction_status_abort_reason(status)  \
    );                                                   \
    if (code == patomic_TSUCCESS)                        \
    {                                                    \
        ASSERT_EQ(status, 0ul);                          \
    }                                                    \
    else                                                 \
    {                                                    \
        ASSERT_NE(status, 0ul);                          \
    }                                                    \
    REQUIRE_SEMICOLON()


/// @brief
///   Test a transaction operation with zero attempts and zero width.
/// @note
///   The parameter 'op' is any function pointer representing a transaction
///   operation provided by patomic.
#define ASSERT_TSX_ZERO(op)                                     \
    {                                                           \
        patomic_transaction_result_t result {};                 \
        patomic_transaction_config_t config {};                 \
        config.width = 0;                                       \
        config.flag_nullable = nullptr;                         \
                                                                \
        config.attempts = 0;                                    \
        ::test::_detail::call_patomic_op(                       \
            op, nullptr, config, &result                        \
        );                                                      \
                                                                \
        ASSERT_TSX_STATUS_EQ(                                   \
            result.status,                                      \
            patomic_TABORT_EXPLICIT,                            \
            patomic_TINFO_ZERO_ATTEMPTS,                        \
            0                                                   \
        );                                                      \
        ASSERT_EQ(result.attempts_made, 0);                     \
                                                                \
        config.attempts = 5;                                    \
        ::test::_detail::call_patomic_op(                       \
            op, nullptr, config, &result                        \
        );                                                      \
                                                                \
        ASSERT_TSX_STATUS_EQ(                                   \
            result.status,                                      \
            patomic_TSUCCESS,                                   \
            patomic_TINFO_NONE,                                 \
            0                                                   \
        );                                                      \
        ASSERT_EQ(result.attempts_made, 1);                     \
    }                                                           \
    REQUIRE_SEMICOLON()


/// @brief
///   Test a transaction wfb operation with zero attempts, zero fallback
///   attempts, and zero width.
/// @note
///   The parameter 'op' is any function pointer representing a transaction
///   operation provided by patomic.
#define ASSERT_TSX_ZERO_WFB(op)                                          \
    {                                                                    \
        patomic_transaction_result_wfb_t result {};                      \
        patomic_transaction_config_wfb_t config {};                      \
        config.width = 0;                                                \
        config.flag_nullable = nullptr;                                  \
        config.fallback_flag_nullable = nullptr;                         \
                                                                         \
        config.attempts = 0;                                             \
        config.fallback_attempts = 0;                                    \
        ::test::_detail::call_patomic_op(                                \
            op, nullptr, config, &result                                 \
        );                                                               \
                                                                         \
        ASSERT_TSX_STATUS_EQ(                                            \
            result.status,                                               \
            patomic_TABORT_EXPLICIT,                                     \
            patomic_TINFO_ZERO_ATTEMPTS,                                 \
            0                                                            \
        );                                                               \
        ASSERT_EQ(result.attempts_made, 0);                              \
        ASSERT_TSX_STATUS_EQ(                                            \
            result.fallback_status,                                      \
            patomic_TABORT_EXPLICIT,                                     \
            patomic_TINFO_ZERO_ATTEMPTS,                                 \
            0                                                            \
        );                                                               \
        ASSERT_EQ(result.fallback_attempts_made, 0);                     \
                                                                         \
        config.attempts = 0;                                             \
        config.fallback_attempts = 5;                                    \
        ::test::_detail::call_patomic_op(                                \
            op, nullptr, config, &result                                 \
        );                                                               \
                                                                         \
        ASSERT_TSX_STATUS_EQ(                                            \
            result.status,                                               \
            patomic_TABORT_EXPLICIT,                                     \
            patomic_TINFO_ZERO_ATTEMPTS,                                 \
            0                                                            \
        );                                                               \
        ASSERT_EQ(result.attempts_made, 0);                              \
        ASSERT_TSX_STATUS_EQ(                                            \
            result.fallback_status,                                      \
            patomic_TSUCCESS,                                            \
            patomic_TINFO_NONE,                                          \
            0                                                            \
        );                                                               \
        ASSERT_EQ(result.fallback_attempts_made, 1);                     \
                                                                         \
        config.attempts = 5;                                             \
        ::test::_detail::call_patomic_op(                                \
            op, nullptr, config, &result                                 \
        );                                                               \
                                                                         \
        ASSERT_TSX_STATUS_EQ(                                            \
            result.status,                                               \
            patomic_TSUCCESS,                                            \
            patomic_TINFO_NONE,                                          \
            0                                                            \
        );                                                               \
        ASSERT_EQ(result.attempts_made, 1);                              \
    }                                                                    \
    REQUIRE_SEMICOLON()


/// @brief
///   Test a transaction operation with flag set.
/// @note
///   The parameter 'op' is any function pointer representing a transaction
///   operation provided by patomic.
#define ASSERT_TSX_FLAG_SET(op)                              \
    {                                                        \
        unsigned char uc {};                                 \
        const patomic_transaction_flag_t flag = 1;           \
        patomic_transaction_result_t result {};              \
        patomic_transaction_config_t config {};              \
                                                             \
        config.width = 1;                                    \
        config.attempts = 1000u;                             \
        config.flag_nullable = &flag;                        \
                                                             \
        ::test::_detail::call_patomic_op(                    \
            op, &uc, config, &result                         \
        );                                                   \
                                                             \
        ASSERT_TSX_STATUS_EQ(                                \
            result.status,                                   \
            patomic_TABORT_EXPLICIT,                         \
            patomic_TINFO_FLAG_SET,                          \
            0                                                \
        );                                                   \
        ASSERT_GT(result.attempts_made, 0);                  \
    }                                                        \
    REQUIRE_SEMICOLON()


/// @brief
///   Test a transaction wfb operation with flag and fallback flag set.
/// @note
///   The parameter 'op' is any function pointer representing a transaction
///   operation provided by patomic.
#define ASSERT_TSX_FLAG_SET_WFB(op)                          \
    {                                                        \
        unsigned char uc {};                                 \
        const patomic_transaction_flag_t flag = 1;           \
        patomic_transaction_result_wfb_t result {};          \
        patomic_transaction_config_wfb_t config {};          \
                                                             \
        config.width = 1;                                    \
        config.attempts = 1000;                              \
        config.fallback_attempts = 1000;                     \
        config.flag_nullable = &flag;                        \
        config.fallback_flag_nullable = &flag;               \
                                                             \
        ::test::_detail::call_patomic_op(                    \
            op, &uc, config, &result                         \
        );                                                   \
                                                             \
        ASSERT_TSX_STATUS_EQ(                                \
            result.status,                                   \
            patomic_TABORT_EXPLICIT,                         \
            patomic_TINFO_FLAG_SET,                          \
            0                                                \
        );                                                   \
        ASSERT_GT(result.attempts_made, 0);                  \
        ASSERT_TSX_STATUS_EQ(                                \
            result.fallback_status,                          \
            patomic_TABORT_EXPLICIT,                         \
            patomic_TINFO_FLAG_SET,                          \
            0                                                \
        );                                                   \
        ASSERT_GT(result.fallback_attempts_made, 0);         \
    }                                                        \
    REQUIRE_SEMICOLON()


/// @brief
///   Adds a test failure if the status exit code is not success.
#define ADD_FAILURE_TSX_SUCCESS(config, result)                                  \
    if (result.status != 0ul)                                                    \
    {                                                                            \
        using result_t = patomic_transaction_result_t;                           \
        static_assert(std::is_same<decltype(result), result_t>::value, "");      \
        auto code = PATOMIC_TRANSACTION_STATUS_EXIT_CODE(result.status);         \
        auto info = PATOMIC_TRANSACTION_STATUS_EXIT_INFO(result.status);         \
        auto reason = PATOMIC_TRANSACTION_STATUS_ABORT_REASON(result.status);    \
        ADD_FAILURE()                                                            \
            << "Transaction failed:"                                             \
            << "\n\tconfig:"                                                     \
            << "\n\t\t.width:    " << config.width                               \
            << "\n\t\t.attempts: " << config.attempts                            \
            << "\n\tresult:"                                                     \
            << "\n\t\t.status.code:   " << test::name_tsx_exit_code(code)        \
            << "\n\t\t.status.info:   " << test::name_tsx_exit_info(info)        \
            << "\n\t\t.status.reason: " << static_cast<int>(reason)              \
            << "\n\t\t.attempts_made: " << result.attempts_made;                 \
    }                                                                            \
    REQUIRE_SEMICOLON()


/// @brief
///   Returns from the current test function with a failure if the primary and
///   fallback status exit code is not success.
#define ADD_FAILURE_TSX_SUCCESS_WFB(config, result)                                        \
    if (result.status != 0ul && result.fallback_status != 0ul)                             \
    {                                                                                      \
        auto code = PATOMIC_TRANSACTION_STATUS_EXIT_CODE(result.status);                   \
        auto code_wfb = PATOMIC_TRANSACTION_STATUS_EXIT_CODE(result.fallback_status);      \
        auto info = PATOMIC_TRANSACTION_STATUS_EXIT_INFO(result.status);                   \
        auto info_wfb = PATOMIC_TRANSACTION_STATUS_EXIT_INFO(result.fallback_status);      \
        auto reason = PATOMIC_TRANSACTION_STATUS_ABORT_REASON(result.status);              \
        auto reason_wfb = PATOMIC_TRANSACTION_STATUS_ABORT_REASON(result.fallback_status); \
        ADD_FAILURE()                                                                      \
            << "Transaction failed:"                                                       \
            << "\n\tconfig:"                                                               \
            << "\n\t\t.width:    " << config.width                                         \
            << "\n\t\t.attempts: " << config.attempts                                      \
            << "\n\t\t.fallback_attempts: " << config.fallback_attempts                    \
            << "\n\tresult:"                                                               \
            << "\n\t\t.status.code:   " << test::name_tsx_exit_code(code)                  \
            << "\n\t\t.status.info:   " << test::name_tsx_exit_info(info)                  \
            << "\n\t\t.status.reason: " << static_cast<int>(reason)                        \
            << "\n\t\t.attempts_made: " << result.attempts_made                            \
            << "\n\t\t.fallback_status.code:   " << test::name_tsx_exit_code(code_wfb)     \
            << "\n\t\t.fallback_status.info:   " << test::name_tsx_exit_info(info_wfb)     \
            << "\n\t\t.fallback_status.reason: " << static_cast<int>(reason_wfb)           \
            << "\n\t\t.fallback_attempts_made: " << result.fallback_attempts_made;         \
    }                                                                                      \
    REQUIRE_SEMICOLON()

/// @brief
///   Tests that fp_multi_cmpxchg (or another function wrapped to fit that API)
///   works correctly.
/// @details
///   Goes through all the cmpxchg structs, and sets expected equal to object
///   in all but one, and expects the operation to fail. This is done once for
///   every cmpxchg struct.
///   Then, all cmpxchg structs have expected set equal to object and the
///   operation is expected to succeed.
///
/// @param cxs std::vector<test::generic_cmpxchg>
/// @param fp_multi_cmpxchg int(std::vector<test::generic_cmpxchg>&)
#define DO_TEST_TSX_MULTI_CMPXCHG(cxs, fp_multi_cmpxchg)  \
    {                                                     \
        std::vector<test::generic_cmpxchg> cxs_old = cxs; \
                                                          \
        for (std::size_t i = 0; i < cxs.size(); ++i)      \
        {                                                 \
            auto& cx = cxs[i];                            \
            auto& cx_old = cxs_old[i];                    \
                                                          \
            cx.expected = cx.object;                      \
            cx_old.object = cx.object;                    \
            cx_old.desired = cx.desired;                  \
        }                                                 \
                                                          \
        for (std::size_t i = 0; i < cxs.size(); ++i)      \
        {                                                 \
            cxs[i].expected.inv();                        \
            int ok = fp_multi_cmpxchg(cxs);               \
            ASSERT_FALSE(ok);                             \
                                                          \
            for (std::size_t j = 0; j < cxs.size(); ++j)  \
            {                                             \
                auto& cx = cxs[j];                        \
                auto& cx_old = cxs_old[j];                \
                                                          \
                ASSERT_EQ(cx.object, cx_old.object);      \
                ASSERT_EQ(cx.expected, cx.object);        \
                ASSERT_EQ(cx.desired, cx_old.desired);    \
            }                                             \
        }                                                 \
                                                          \
        for (std::size_t i = 0; i < cxs.size(); ++i)      \
        {                                                 \
            cxs_old[i].expected = cxs[i].expected;        \
        }                                                 \
                                                          \
        int ok = fp_multi_cmpxchg(cxs);                   \
        ASSERT_TRUE(ok);                                  \
                                                          \
        for (std::size_t i = 0; i < cxs.size(); ++i)      \
        {                                                 \
            auto& cx = cxs[i];                            \
            auto& cx_old = cxs_old[i];                    \
                                                          \
            ASSERT_EQ(cx.object, cx.desired);             \
            ASSERT_EQ(cx.expected, cx_old.expected);      \
            ASSERT_EQ(cx.desired, cx_old.desired);        \
        }                                                 \
    }                                                     \
    REQUIRE_SEMICOLON()


/// @brief
///   Test a transactional operation wfb with a null flag and unset flag.
#define TEST_TSX(config, name)                          \
    {                                                   \
        SCOPED_TRACE("flag null");                      \
        (config).flag_nullable = nullptr;               \
        test_##name((config).width, 1u, fp_##name);     \
    }                                                   \
    {                                                   \
        SCOPED_TRACE("flag unset");                     \
        const patomic_transaction_flag_t flag {};       \
        (config).flag_nullable = &flag;                 \
        test_##name((config).width, 1u, fp_##name);     \
    }                                                   \
    static_assert(std::is_same<decltype(config),        \
                  patomic_transaction_config_t>::value, \
                  "don't use wfb version of config_t")


/// @brief
///   Test a transactional operation with a null flag and unset flag.
#define TEST_TSX_WFB(config, name)                  \
    {                                               \
        SCOPED_TRACE("flag null");                  \
        (config).flag_nullable = nullptr;           \
        (config).fallback_flag_nullable = nullptr;  \
        test_##name((config).width, 1u, fp_##name); \
    }                                               \
    {                                               \
        SCOPED_TRACE("flag unset");                 \
        const patomic_transaction_flag_t flag {};   \
        (config).flag_nullable = &flag;             \
        (config).fallback_flag_nullable = &flag;    \
        test_##name((config).width, 1u, fp_##name); \
    }                                               \
    REQUIRE_SEMICOLON()


namespace test
{


namespace _detail
{

/// @brief
///   Calls the given op function pointer with all pointer params having the
///   given value. The return value is discarded.
template <class F, class C, class R>
void
call_patomic_op(F f, void *ptr, const C& config, R *result) noexcept;

}  // namespace _detail


/// @brief
///   Helper type for use in testing double and multi cmpxchg ops.
struct generic_cmpxchg
{
    /// @brief
    ///   Constructor to initialize all generic integers the same.
    generic_cmpxchg(std::size_t width, std::size_t align, bool is_signed);

    /// @brief
    ///   Create a patomic_transaction_cmpxchg_t object referencing the owned
    ///   generic_integer objects.
    operator patomic_transaction_cmpxchg_t() noexcept;

    // core data members
    test::generic_integer object;
    test::generic_integer expected;
    test::generic_integer desired;
};


}  // namespace test

#endif  // PATOMIC_TEST_COMMON_TRANSACTION_HPP
