// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

#include <test/common/name.hpp>
#include <test/common/support.hpp>

#include <algorithm>
#include <array>
#include <set>

namespace test
{


std::vector<std::size_t>
supported_widths()
{
    std::set<std::size_t> widths {

        // null implementation

        // stdc implementation
        sizeof(char),
        sizeof(short),
        sizeof(int),
        sizeof(long),
        sizeof(long long),

        // msvc implementation
        1, 2, 4, 8, 16
    };
    return { widths.begin(), widths.end() };
}


std::vector<unsigned int>
supported_options()
{
    // get options to combine
    const std::array<patomic_option_t, 1> single_bit_options {
         patomic_option_NONE
    };

    // create cartesian product of options
    std::set<unsigned int> combined_options;
    const auto combination_count = (1ull << single_bit_options.size()) - 1ull;
    for (auto i = 1ull; i <= combination_count; ++i)
    {
        unsigned int combined_opt = 0u;
        unsigned int bit = 1u;
        for (patomic_option_t opt : single_bit_options)
        {
            if (i & bit)
            {
                combined_opt |= static_cast<unsigned int>(opt);
            }
            bit <<= 1;
        }
        combined_options.insert(combined_opt);
    }

    // success
    return { combined_options.begin(), combined_options.end() };
}


std::vector<patomic_id_t>
supported_ids()
{
    return {
        patomic_id_NULL,
        patomic_id_STDC,
        patomic_id_MSVC
    };
}


std::vector<patomic_memory_order_t>
supported_orders()
{
    return {
        patomic_RELAXED,
        patomic_CONSUME,
        patomic_ACQUIRE,
        patomic_RELEASE,
        patomic_ACQ_REL,
        patomic_SEQ_CST
    };
}


std::vector<patomic_memory_order_t>
supported_orders_store()
{
    return {
        patomic_RELAXED,
        // patomic_CONSUME; undefined behaviour
        // patomic_ACQUIRE; undefined behaviour
        patomic_RELEASE,
        // patomic_ACQ_REL; undefined behaviour
        patomic_SEQ_CST
    };
}


std::vector<patomic_memory_order_t>
supported_orders_load()
{
    return {
        patomic_RELAXED,
        patomic_CONSUME,
        patomic_ACQUIRE,
        // patomic_RELEASE; undefined behaviour
        // patomic_ACQ_REL; undefined behaviour
        patomic_SEQ_CST
    };
}


}  // namespace test
