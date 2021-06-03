#ifndef PATOMIC_TEST_CURRY_OP_HPP
#define PATOMIC_TEST_CURRY_OP_HPP

#include <functional>
#include <type_traits>


#define PT_MAKE_AUTO_0()
#define PT_MAKE_AUTO_1(a) auto a
#define PT_MAKE_AUTO_2(a, ...) auto a, PT_MAKE_AUTO_1(__VA_ARGS__)
#define PT_MAKE_AUTO_3(a, ...) auto a, PT_MAKE_AUTO_2(__VA_ARGS__)

#define PT_GET(dummy, a, b, c, d, ...) d
#define PT_COUNT(...) PT_GET(dummy, ## __VA_ARGS__, 3, 2, 1, 0)

#define PT_CONCAT1(a, b) a ## b
#define PT_CONCAT(a, b) PT_CONCAT1(a, b)

#define PT_MAKE_AUTO(...) PT_CONCAT(PT_MAKE_AUTO_, PT_COUNT(__VA_ARGS__)) (__VA_ARGS__)


#define CURRY_OP_NO_RET(name, parent, ...)                                \
    using name##_t = std::remove_pointer<patomic_opsig_##name##_t>::type; \
    std::function<name##_t> fp_##name;                                    \
    if (!this->m_is_explicit) { fp_##name = m_i##parent##name; }          \
    else if (m_e##parent##name == nullptr) { fp_##name = nullptr; }       \
    else { fp_##name = [=](PT_MAKE_AUTO(__VA_ARGS__)) {                   \
        return m_e##parent##name(__VA_ARGS__, this->m_order);             \
    };}                                                                   \
    do {} while (0)

#define CURRY_OP_RET(name, parent, ret, ...)                              \
    using name##_t = std::remove_pointer<patomic_opsig_##name##_t>::type; \
    std::function<name##_t> fp_##name;                                    \
    if (!this->m_is_explicit) { fp_##name = m_i##parent##name; }          \
    else if (m_e##parent##name == nullptr) { fp_##name = nullptr; }       \
    else { fp_##name = [=](PT_MAKE_AUTO(__VA_ARGS__), auto ret) {         \
        return m_e##parent##name(__VA_ARGS__, this->m_order, ret);        \
    };}                                                                   \
    do {} while (0)

#define CURRY_OP_CMPXCHG(name, parent, ...)                              \
    using name##_t = std::remove_pointer<patomic_opsig_cmpxchg_t>::type; \
    std::function<name##_t> fp_##name;                                   \
    if (!this->m_is_explicit) { fp_##name = m_i##parent##name; }         \
    else if (m_e##parent##name == nullptr) { fp_##name = nullptr; }      \
    else { fp_##name = [=](PT_MAKE_AUTO(__VA_ARGS__)) {                  \
        auto fail = patomic_cmpxchg_fail_order(this->m_order);           \
        return m_e##parent##name(__VA_ARGS__, this->m_order, fail);      \
    };}                                                                  \
    do {} while (0)

#endif  // !PATOMIC_TEST_CURRY_OP_HPP
