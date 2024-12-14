#include <test/common/transaction.hpp>

namespace test
{


generic_cmpxchg::generic_cmpxchg(
    std::size_t width, std::size_t align, bool is_signed
)
    : object(width, align, is_signed),
      expected(width, align, is_signed),
      desired(width, align, is_signed)
{}


patomic_transaction_cmpxchg_t
generic_cmpxchg::as_patomic() noexcept
{
    patomic_transaction_cmpxchg_t ret;
    ret.obj = object;
    ret.expected = expected;
    ret.desired = desired;
    return ret;
}


}  // namespace test
