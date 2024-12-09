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


}  // namespace test