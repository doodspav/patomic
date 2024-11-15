#include <test/common/support.hpp>

#include <array>

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


std::set<patomic_id_t>
supported_ids()
{
    return {
        patomic_id_NULL,
        patomic_id_STDC
    };
}


SupportCombination::SupportCombination(TupleT tup) noexcept
    : width(std::get<0>(tup)),
      order(std::get<1>(tup)),
      id(std::get<2>(tup)),
      options(std::get<3>(tup))
{}


}  // namespace test
