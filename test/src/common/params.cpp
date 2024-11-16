#include <test/common/name.hpp>
#include <test/common/params.hpp>
#include <test/common/support.hpp>

#include <algorithm>
#include <cctype>
#include <random>
#include <set>

namespace test
{


ParamsImpExp::ParamsImpExp(TupleT tup) noexcept
    : id(std::get<0>(tup)),
      width(std::get<1>(tup)),
      order(std::get<2>(tup)),
      options(std::get<3>(tup))
{}


std::string
ParamsImpExp::as_test_suffix() const
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


std::vector<ParamsImpExp>
ParamsImpExp::combinations()
{
    // setup
    std::vector<ParamsImpExp> params;

    // create cartesian product
    for (patomic_id_t id : supported_ids())
    {
        for (std::size_t width : supported_widths())
        {
            for (patomic_memory_order_t order : supported_orders())
            {
                for (unsigned int options : supported_options())
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


std::vector<ParamsImpExp>
ParamsImpExp::combinations_store()
{
    // setup
    std::vector<ParamsImpExp> params;

    // create cartesian product
    for (patomic_id_t id : supported_ids())
    {
        for (std::size_t width : supported_widths())
        {
            for (patomic_memory_order_t order : supported_orders_store())
            {
                for (unsigned int options : supported_options())
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


std::vector<ParamsImpExp>
ParamsImpExp::combinations_load()
{
    // setup
    std::vector<ParamsImpExp> params;

    // create cartesian product
    for (patomic_id_t id : supported_ids())
    {
        for (std::size_t width : supported_widths())
        {
            for (patomic_memory_order_t order : supported_orders_load())
            {
                for (unsigned int options : supported_options())
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


ParamsTsx::ParamsTsx(TupleT tup) noexcept
    : id(std::get<0>(tup)),
      options(std::get<1>(tup)),
      widths(std::move(std::get<2>(tup)))
{}


std::string
ParamsTsx::as_test_suffix() const
{
    // create suffix string
    std::string suffix = "id_" + name_id(id);
    suffix += "_options_" + name_options(options);

    // convert to lower case
    std::transform(suffix.begin(), suffix.end(), suffix.begin(),
                   [](unsigned char c) noexcept -> char {
        return static_cast<char>(std::tolower(c));
    });

    // return
    return suffix;
}


std::vector<ParamsTsx>
ParamsTsx::combinations()
{
    // setup
    std::vector<ParamsTsx> params;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<std::size_t> distrib;

    // create a selection of constant and random widths
    std::set<std::size_t> widths_set;
    widths_set.insert(0u);
    for (std::size_t width : supported_widths())
    {
        widths_set.insert(width);
    }
    while (widths_set.size() < 16u)
    {
        widths_set.insert(distrib(gen));
    }

    // convert to vector
    const std::vector<std::size_t> widths {
        widths_set.begin(), widths_set.end()
    };

    // cartesian product
    for (patomic_id_t id : supported_ids())
    {
        for (unsigned int options : supported_options())
        {
            params.emplace_back(
                std::make_tuple(id, options, widths)
            );
        }
    }

    // return
    return params;
}


}  // namespace test
