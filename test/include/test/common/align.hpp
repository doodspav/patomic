#ifndef PATOMIC_TEST_COMMON_ALIGN_HPP
#define PATOMIC_TEST_COMMON_ALIGN_HPP

namespace test
{


/// @brief
///   Returns a pointer to memory inside the buffer aligned at least to "align"
///   where pointer + "size" does not extend outside the buffer, or nullptr if
///   no such pointer exists.
///
/// @note
///   This function supports invalid alignments which are not powers of 2. If
///   "align" is zero, "buf_ptr" is returned directly as long as "size" is not
///   larger than "buf_size" (in which case nullptr is returned).
void *
aligned_pointer(
    void *buf_ptr, size_t buf_size, size_t align, size_t size
) noexcept;


/// @copydoc test::aligned_pointer
const void *
aligned_pointer(
    const void *buf_ptr, size_t buf_size, size_t align, size_t size
) noexcept;


}

#endif  // PATOMIC_TEST_COMMON_ALIGN_HPP
