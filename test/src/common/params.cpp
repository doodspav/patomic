#include <test/common/name.hpp>
#include <test/common/params.hpp>
#include <test/common/support.hpp>

#include <algorithm>
#include <cctype>
#include <random>
#include <set>

namespace
{


/// @brief
///   Convert implicit or explicit params to string suitable for use as a test
///   name suffix.
template <class Params>
std::string
as_text_suffix_imp_exp(const Params& p)
{
    // create suffix string
    std::string suffix = "id_" + test::name_id(p.id);
    suffix += "_width_" + std::to_string(p.width);
    suffix += "_order_" + test::name_order(p.order);
    suffix += "_options_" + test::name_options(p.options);

    // convert to lower case
    std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                   [](const unsigned char c) noexcept -> char {
        return static_cast<char>(std::tolower(c));
    });

    // return
    return suffix;
}


/// @brief
///   Create all combinations of implicit or explicit params for tests with the
///   provided orders.
template <class Params>
std::vector<Params>
combinations_imp_exp(const std::vector<patomic_memory_order_t>& orders)
{
    // setup
    std::vector<Params> params;

    // create cartesian product
    for (patomic_id_t id : test::supported_ids())
    {
        for (std::size_t width : test::supported_widths())
        {
            for (patomic_memory_order_t order : orders)
            {
                for (unsigned int options : test::supported_options())
                {
                    params.emplace_back(
                        std::make_tuple(id, width, order, options)
                    );
                }
            }
        }
    }

    // return
    return params;
}


}  // namespace

namespace test
{


ParamsImplicit::ParamsImplicit(TupleT tup) noexcept
    : id(std::get<0>(tup)),
      width(std::get<1>(tup)),
      order(std::get<2>(tup)),
      options(std::get<3>(tup))
{}


std::string
ParamsImplicit::as_test_suffix() const
{
    return as_text_suffix_imp_exp(*this);
}


std::vector<ParamsImplicit>
ParamsImplicit::combinations()
{
    return combinations_imp_exp<ParamsImplicit>(supported_orders());
}


std::vector<ParamsImplicit>
ParamsImplicit::combinations_store()
{
    return combinations_imp_exp<ParamsImplicit>(supported_orders_store());
}


std::vector<ParamsImplicit>
ParamsImplicit::combinations_load()
{
    return combinations_imp_exp<ParamsImplicit>(supported_orders_load());
}


ParamsExplicit::ParamsExplicit(TupleT tup) noexcept
    : id(std::get<0>(tup)),
      width(std::get<1>(tup)),
      order(std::get<2>(tup)),
      options(std::get<3>(tup))
{}


std::string
ParamsExplicit::as_test_suffix() const
{
    return as_text_suffix_imp_exp(*this);
}


std::vector<ParamsExplicit>
ParamsExplicit::combinations()
{
    return combinations_imp_exp<ParamsExplicit>(supported_orders());
}


std::vector<ParamsExplicit>
ParamsExplicit::combinations_store()
{
    return combinations_imp_exp<ParamsExplicit>(supported_orders_store());
}


std::vector<ParamsExplicit>
ParamsExplicit::combinations_load()
{
    return combinations_imp_exp<ParamsExplicit>(supported_orders_load());
}


ParamsTransaction::ParamsTransaction(TupleT tup) noexcept
    : id(std::get<0>(tup)),
      options(std::get<1>(tup))
{}


std::string
ParamsTransaction::as_test_suffix() const
{
    // create suffix string
    std::string suffix = "id_" + name_id(id);
    suffix += "_options_" + name_options(options);

    // convert to lower case
    std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                   [](const unsigned char c) noexcept -> char {
        return static_cast<char>(std::tolower(c));
    });

    // return
    return suffix;
}


std::vector<ParamsTransaction>
ParamsTransaction::combinations()
{
    // setup
    std::vector<ParamsTransaction> params;

    // create cartesian product
    for (patomic_id_t id : supported_ids())
    {
        for (unsigned int options : supported_options())
        {
            params.emplace_back(
                std::make_tuple(id, options)
            );
        }
    }

    // return
    return params;
}


}  // namespace test
