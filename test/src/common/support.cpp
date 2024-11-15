#include <test/common/name.hpp>
#include <test/common/support.hpp>

#include <algorithm>
#include <array>
#include <cctype>

namespace test
{


std::set<std::size_t>
supported_widths()
{
    return {

        // null implementation

        // stdc implementation
        sizeof(char),
        sizeof(short),
        sizeof(int),
        sizeof(long),
        sizeof(long long)
    };
}


std::set<unsigned int>
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
    return combined_options;
}


std::set<patomic_memory_order_t>
supported_orders_store()
{
    return {
        patomic_RELAXED,
        patomic_RELEASE,
        patomic_SEQ_CST
    };
}


std::set<patomic_memory_order_t>
supported_orders_load()
{
    return {
        patomic_RELAXED,
        patomic_CONSUME,
        patomic_ACQUIRE,
        patomic_SEQ_CST
    };
}


std::set<patomic_memory_order_t>
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


std::set<patomic_id_t>
supported_ids()
{
    return {
        patomic_id_NULL,
        patomic_id_STDC
    };
}


SupportCombination::SupportCombination(TupleT tup) noexcept
    : id(std::get<0>(tup)),
      width(std::get<1>(tup)),
      order(std::get<2>(tup)),
      options(std::get<3>(tup))
{}


std::string
SupportCombination::as_test_suffix() const
{
    // create suffix string
    std::string suffix = "id_" + name_id(id);
    suffix += "_width_" + std::to_string(width);
    suffix += "_order_" + name_order(order);
    suffix += "_options_" + name_options(options);

    // convert to lower case
    std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                   [](unsigned char c) noexcept -> char {
        return static_cast<char>(std::tolower(c));
    });

    // return
    return suffix;
}


}  // namespace test
