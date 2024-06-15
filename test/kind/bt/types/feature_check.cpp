#include <patomic/types/feature_check.h>

#include <gtest/gtest.h>


/// @brief Test fixture.
class BtTypesFeatureCheck : public testing::Test
{};

/*
 * all opcat have 0-1 bits set
 * all opcat are unique
 * no unexpected opcat
 * all opcats consist of known opcat values
 *
 * all opkind have 0-1 bits set
 * all opkind are unique within their group
 * no unexpected opkind (might need per opcat)
 * all opkinds consist of known and expected opkind values
 *
 * invalid bits remain set
 * each bit corresponds properly to the category (might need per opcat)
 *
 * invalid bits remain set
 * each bit corresponds properly to the category (might need per opcat)
 *
 * invalid kind bits remain set
 * invalid opcat bit is fatally asserted
 * multiple opcat bits are fatally asserted
 * each kind bit corresponds properly to the operation (needs per opcat)
 *
 */
