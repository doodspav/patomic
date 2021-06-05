#ifndef PATOMIC_TEST_CURRY_OP_HPP
#define PATOMIC_TEST_CURRY_OP_HPP

#include <cstddef>
#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include <patomic/types/memory_order.h>
#include <patomic/types/ops.h>


namespace patomic {
    namespace test {

        namespace detail {

            template <typename... Ts, std::size_t... Is>
            static constexpr auto split_args(
                std::tuple<Ts...> tup,
                std::index_sequence<Is...>
            )
            {
                return std::make_pair(
                    std::make_tuple(std::get<Is>(tup)...),
                    std::get<sizeof...(Ts) - 1>(tup)
                );
            }

            template <typename... Ts>
            static constexpr auto split_args(Ts... args)
            {
                return split_args(
                    std::make_tuple(args...),
                    std::make_index_sequence<sizeof...(Ts) - 1>{}
                );
            }

            template<typename F, typename... Ts, std::size_t... Is>
            static constexpr auto apply(
                F f,
                std::tuple<Ts...> tup,
                std::index_sequence<Is...>
            )
            {
                return f(std::get<Is>(tup)...);
            }

            template<typename F, typename... Ts>
            static constexpr auto apply(
                F f,
                std::tuple<Ts...> tup
            )
            {
                return apply(f, tup, std::make_index_sequence<sizeof...(Ts)>{});
            }
        }

        template <typename I, typename E>
        std::function<std::remove_pointer_t<I>>
        static curry_op_no_ret(
            I fp_implicit,
            E fp_explicit,
            bool is_explicit,
            patomic_memory_order_t order
        )
        {
            if (!is_explicit) { return {fp_implicit}; }
            else if (fp_explicit == nullptr) { return {nullptr}; }
            else { return {[=](auto... args){
                return fp_explicit(args..., order);
            }}; }
        }

        template <typename I, typename E>
        std::function<std::remove_pointer_t<I>>
        static curry_op_ret(
            I fp_implicit,
            E fp_explicit,
            bool is_explicit,
            patomic_memory_order_t order
        )
        {
            if (!is_explicit) { return {fp_implicit}; }
            else if (fp_explicit == nullptr) { return {nullptr}; }
            else { return {[=](auto... args){
                auto split = detail::split_args(args...);
                detail::apply([=](auto... fs){
                    return fp_explicit(fs..., order, split.second);
                }, split.first);
            }}; }
        }

        std::function<std::remove_pointer_t<patomic_opsig_cmpxchg_t>>
        static curry_op_cmpxchg(
            patomic_opsig_cmpxchg_t fp_implicit,
            patomic_opsig_explicit_cmpxchg_t fp_explicit,
            bool is_explicit,
            patomic_memory_order_t order
        )
        {
            if (!is_explicit) { return {fp_implicit}; }
            else if (fp_explicit == nullptr) { return {nullptr}; }
            else { return {[=](auto... args){
                auto fail = patomic_cmpxchg_fail_order(order);
                return fp_explicit(args..., order, fail);
            }}; }
        }

}}  // ns patomic::test


#endif  // !PATOMIC_TEST_CURRY_OP_HPP
