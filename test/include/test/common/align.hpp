// Copyright (c) doodspav.
// SPDX-License-Identifier: LGPL-3.0-only WITH LGPL-3.0-linking-exception

#ifndef PATOMIC_TEST_COMMON_ALIGN_HPP
#define PATOMIC_TEST_COMMON_ALIGN_HPP

#include <cstddef>

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
    void *buf_ptr, std::size_t buf_size, std::size_t align, std::size_t size
) noexcept;


/// @brief
///   Returns a pointer to memory inside the buffer aligned at least to "align"
///   where pointer + "size" does not extend outside the buffer, or nullptr if
///   no such pointer exists.
///
/// @note
///   This function supports invalid alignments which are not powers of 2. If
///   "align" is zero, "buf_ptr" is returned directly as long as "size" is not
///   larger than "buf_size" (in which case nullptr is returned).
const void *
aligned_pointer(
    const void *buf_ptr, std::size_t buf_size, std::size_t align,
    std::size_t size
) noexcept;


/// @brief
///   Provides the runtime alignment of the pointed-to object. This will always
///   be a positive power of 2.
std::size_t
runtime_alignof(const void *ptr) noexcept;


}  // namespace test

#endif  // PATOMIC_TEST_COMMON_ALIGN_HPP
