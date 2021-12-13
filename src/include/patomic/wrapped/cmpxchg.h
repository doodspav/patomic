#include <patomic/types/memory_order.h>

#include <patomic/macros/force_inline.h>
#include <patomic/macros/ignore_unused.h>


#undef HIDE_P
#undef SHOW_P

#define HIDE_P(x, y)
#define SHOW_P(x, y) ,y

#undef HIDE
#undef SHOW

#define HIDE(x)
#define SHOW(x) x


/*
 * MACRO PARAMETERS
 *
 * - bit_width:
 *   - must be an int literal
 *   - must either have the value of `sizeof(type) * CHAR_BIT` or be `0`
 *
 * - byte_width:
 *   - must be an int literal
 *   - must either have the value of `sizeof(type)` or be `0`
 *
 * - do_atomic_cmpxchg_weak_explicit:
 *   - must be callable as `x = (int) fn(obj_p, exp, des, succ, fail, bi, by);`
 *   - `bi` and `by` will be the macro parameters `bit_width` and `byte_width`
 *   - `obj_p` will be an expression of the form `(volatile atype *) ptr`
 *   - `exp`, `des`, `succ`, and `fail` will be the names of local identifiers
 *   - `exp` and `des` will have type (type) and (const type) respectively
 *   - `succ` and `fail` will have type (const int)
 *   - `succ` will be a valid memory order, and `fail` will be a valid load
 *      memory order which will compare `<= succ`
 *
 * - do_assert:
 *   - must be callable as `do_assert(expr);`
 *   - the result of the expression will not be assigned to anything
 *   - `expr` will evaluate to an expression of type (int)
 *   - standard assertion
 *
 * - do_assert_aligned:
 *   - must be callable as `do_assert_aligned(cv_vptr, TYPE);`
 *   - the result of the expressions will not be assigned to anything
 *   - `cv_vptr` will be of type (void*) and may be const/void qualified
 *   - `TYPE` will represent a valid type
 *   - asserts that `cv_vptr` meets the alignment required by `TYPE`
 *
 * - do_memcpy:
 *   - must be callable as `do_memcpy(vptr, c_vptr, len);`
 *   - the result of the expression will not be assigned to anything
 *   - `vptr` will be of type (void*)
 *   - `c_vptr` will be of type (void*) and may be const qualified
 *   - `len` will be of type (size_t)
 *
 * - type:
 *   - the non-qualified version of the type the atomic operation will be
 *     performed on (e.g. `int`)
 *   - must not be a VLA or array of unknown size
 *
 * - atype:
 *   - the type the atomic operation will be performed on (e.g. `_Atomic(int)`)
 *   - sizeof(atype) must equal sizeof(type)
 *   - must not be const qualified
 *   - may, but need not be, volatile qualified (would be redundant)
 *   - must not be a VLA or array of unknown size
 *
 * - fn_name:
 *   - the full name for the function to be defined
 *   - aforementioned function will always have `static` linkage
 *
 * - order:
 *   - if vis_p is SHOW_P: must be the identifier `order`
 *   - if vis_p is HIDE_P: must be a valid memory order compile time constant
 *     convertible to (int)
 *
 * - vis_p:
 *   - must be one of HIDE_P or SHOW_P
 *   - used to hide or show memory order parameters
 *   - use SHOW_P for the explicit version of a function, and HIDE_P otherwise
 *
 * - inv:
 *   - must be one of HIDE or SHOW
 *   - the inverse of vis_p (e.g. if vis_p is HIDE_P then inv would be SHOW)
 *
 *
 * MACRO LOCALS
 *
 * - these identifiers will be local to the function the macro is used in
 * - their value will be undetermined
 *
 * - ok:
 *   - has type (int)
 *
 * - scratch:
 *   - has type (type)
 */
