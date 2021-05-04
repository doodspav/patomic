#ifndef PATOMIC_FAKE_STDATOMIC_H
#define PATOMIC_FAKE_STDATOMIC_H

#define _Atomic(x) x

enum memory_order {
    memory_order_relaxed,
    memory_order_consume,
    memory_order_acquire,
    memory_order_release,
    memory_order_acq_rel,
    memory_order_seq_cst
};

struct atomic_flag { int value; };

#define ATOMIC_BOOL_LOCK_FREE 2
#define ATOMIC_CHAR_LOCK_FREE 2
#define ATOMIC_CHAR16_T_LOCK_FREE 2
#define ATOMIC_CHAR32_T_LOCK_FREE 2
#define ATOMIC_WCHAR_T_LOCK_FREE 2
#define ATOMIC_SHORT_LOCK_FREE 2
#define ATOMIC_INT_LOCK_FREE 2
#define ATOMIC_LONG_LOCK_FREE 2
#define ATOMIC_LLONG_LOCK_FREE 2
#define ATOMIC_POINTER_LOCK_FREE 2

#define ATOMIC_FLAG_INIT 0
#define ATOMIC_VAR_INIT(val) val

#define kill_dependency(y) y

#define atomic_init(a, c) ((void) 0)

#define atomic_flag_test_and_set(f) (0)
#define atomic_flag_clear(f) ((void) 0)
#define atomic_store(a, c) ((void) 0)
#define atomic_load(a) (*(a))
#define atomic_exchange(a, d) (d)
#define atomic_compare_exchange_weak(a, e, d) (0)
#define atomic_compare_exchange_strong(a, e, d) (0)
#define atomic_fetch_add(a, m) (m)
#define atomic_fetch_sub(a, m) (m)
#define atomic_fetch_or(a, m) (m)
#define atomic_fetch_xor(a, m) (m)
#define atomic_fetch_and(a, m) (m)

#define atomic_flag_test_and_set_explicit(f, o) (0)
#define atomic_flag_clear_explicit(f, o) ((void) 0)
#define atomic_store_explicit(a, c, o) ((void) 0)
#define atomic_load_explicit(a, o) (*(a))
#define atomic_exchange_explicit(a, d, o) (d)
#define atomic_compare_exchange_weak_explicit(a, e, d, s, f) (0)
#define atomic_compare_exchange_strong_explicit(a, e, d, s, f) (0)
#define atomic_fetch_add_explicit(a, m, o) (m)
#define atomic_fetch_sub_explicit(a, m, o) (m)
#define atomic_fetch_or_explicit(a, m, o) (m)
#define atomic_fetch_xor_explicit(a, m, o) (m)
#define atomic_fetch_and_explicit(a, m, o) (m)

#define atomic_thread_fence(o) ((void) 0)
#define atomic_signal_fence(o) ((void) 0)

#endif /* !PATOMIC_FAKE_STDATOMIC_H */
