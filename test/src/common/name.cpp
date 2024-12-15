#include <test/common/name.hpp>

#include <patomic/api/options.h>


namespace test
{


std::string
name_id(patomic_id_t id)
{
    switch (id)
    {
        case patomic_id_NULL:
            return "NULL";
        case patomic_id_STDC:
            return "STDC";
        default:
            return "(unknown)";
    }
}


std::string
name_options(unsigned int options)
{
    switch (options)
    {
        case patomic_option_NONE:
            return "NONE";
        default:
            return "(unknown)";
    }
}


std::string
name_order(patomic_memory_order_t order)
{
    switch (order)
    {
        case patomic_RELAXED:
            return "RELAXED";
        case patomic_CONSUME:
            return "CONSUME";
        case patomic_ACQUIRE:
            return "ACQUIRE";
        case patomic_RELEASE:
            return "RELEASE";
        case patomic_ACQ_REL:
            return "ACQ_REL";
        case patomic_SEQ_CST:
            return "SEQ_CST";
        default:
            return "(unknown)";
    }
}


std::string
name_tsx_exit_code(patomic_transaction_exit_code_t code)
{
    switch (code)
    {
        case patomic_TSUCCESS:
            return "TSUCCESS";
        case patomic_TABORT_UNKNOWN:
            return "TABORT_UNKNOWN";
        case patomic_TABORT_EXPLICIT:
            return "TABORT_EXPLICIT";
        case patomic_TABORT_CONFLICT:
            return "TABORT_CONFLICT";
        case patomic_TABORT_CAPACITY:
            return "TABORT_CAPACITY";
        case patomic_TABORT_DEBUG:
            return "TABORT_DEBUG";
        default:
            return "(unknown)";
    }
}


std::string
name_tsx_exit_info(unsigned int info)
{
    // setup
    std::string ret;

#define CHECK_INFO(name)               \
    if (info & patomic_TINFO_##name)   \
    {                                  \
        ret += "TINFO_" #name " | ";   \
        info &= ~patomic_TINFO_##name; \
    }                                  \
    static_assert(true, "require semicolon")

    // go through all values
    CHECK_INFO(RETRY);
    CHECK_INFO(NESTED);

    // check for unknown values
    if (info != 0)
    {
        ret += "(unknown) | ";
    }

    // cleanup
    if (ret.empty())
    {
        return "(none)";
    }
    else
    {
        // remove trailing " | "
        return ret.substr(0, ret.size() - 3);
    }
}


}  // namespace test
