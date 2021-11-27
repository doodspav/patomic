#include <patomic/patomic.h>
#include <gtest/gtest.h>


TEST(HelperFunctionsTest, version_major)
{
    ASSERT_EQ(patomic_version_major(), PATOMIC_VERSION_MAJOR);
}

TEST(HelperFunctionsTest, version_minor)
{
    ASSERT_EQ(patomic_version_minor(), PATOMIC_VERSION_MINOR);
}

TEST(HelperFunctionsTest, version_patch)
{
    ASSERT_EQ(patomic_version_patch(), PATOMIC_VERSION_PATCH);
}

TEST(HelperFunctionsTest, version_compatible_with)
{
    int major = PATOMIC_VERSION_MAJOR;
    int minor = PATOMIC_VERSION_MINOR;

    // =major and >=minor are compatible
    ASSERT_TRUE(patomic_version_compatible_with(major, minor));
    ASSERT_TRUE(patomic_version_compatible_with(major, minor - 1));

    // !=major or <minor are incompatible
    ASSERT_FALSE(patomic_version_compatible_with(major - 1, minor));
    ASSERT_FALSE(patomic_version_compatible_with(major + 1, minor));
    ASSERT_FALSE(patomic_version_compatible_with(major, minor + 1));
}

TEST(HelperFunctionsTest, is_valid_order)
{
    ASSERT_TRUE(patomic_is_valid_order(patomic_RELAXED));
    ASSERT_TRUE(patomic_is_valid_order(patomic_CONSUME));
    ASSERT_TRUE(patomic_is_valid_order(patomic_ACQUIRE));
    ASSERT_TRUE(patomic_is_valid_order(patomic_RELEASE));
    ASSERT_TRUE(patomic_is_valid_order(patomic_ACQ_REL));
    ASSERT_TRUE(patomic_is_valid_order(patomic_SEQ_CST));

    ASSERT_FALSE(patomic_is_valid_order(patomic_RELAXED - 1));
    ASSERT_FALSE(patomic_is_valid_order(patomic_SEQ_CST + 1));
}

TEST(HelperFunctionsTest, is_valid_store_order)
{
    ASSERT_TRUE(patomic_is_valid_store_order(patomic_RELAXED));
    ASSERT_TRUE(patomic_is_valid_store_order(patomic_RELEASE));
    ASSERT_TRUE(patomic_is_valid_store_order(patomic_SEQ_CST));

    ASSERT_FALSE(patomic_is_valid_store_order(patomic_CONSUME));
    ASSERT_FALSE(patomic_is_valid_store_order(patomic_ACQUIRE));
    ASSERT_FALSE(patomic_is_valid_store_order(patomic_ACQ_REL));

    ASSERT_FALSE(patomic_is_valid_store_order(patomic_RELAXED - 1));
    ASSERT_FALSE(patomic_is_valid_store_order(patomic_SEQ_CST + 1));
}

TEST(HelperFunctionsTest, is_valid_load_order)
{
    ASSERT_TRUE(patomic_is_valid_load_order(patomic_RELAXED));
    ASSERT_TRUE(patomic_is_valid_load_order(patomic_CONSUME));
    ASSERT_TRUE(patomic_is_valid_load_order(patomic_ACQUIRE));
    ASSERT_TRUE(patomic_is_valid_load_order(patomic_SEQ_CST));

    ASSERT_FALSE(patomic_is_valid_load_order(patomic_RELEASE));
    ASSERT_FALSE(patomic_is_valid_load_order(patomic_ACQ_REL));

    ASSERT_FALSE(patomic_is_valid_load_order(patomic_RELAXED - 1));
    ASSERT_FALSE(patomic_is_valid_load_order(patomic_SEQ_CST + 1));
}

TEST(HelperFunctionsTest, is_valid_fail_order)
{
    // requires fail < succ and fail is load order
    for (int succ = patomic_RELAXED; succ <= patomic_SEQ_CST; ++succ) {
        for (int fail = patomic_RELAXED; fail <= patomic_SEQ_CST; ++fail) {
            if (fail > succ || !patomic_is_valid_load_order(fail)) {
                ASSERT_FALSE(patomic_is_valid_fail_order(succ, fail));
            }
            else {
                ASSERT_TRUE(patomic_is_valid_fail_order(succ, fail));
            }
        }
    }

    // invalid orders always fail
    int inc = patomic_SEQ_CST + 1;
    int dec = patomic_RELAXED - 1;
    ASSERT_FALSE(patomic_is_valid_fail_order(inc, patomic_SEQ_CST));
    ASSERT_FALSE(patomic_is_valid_fail_order(patomic_SEQ_CST, dec));
    ASSERT_FALSE(patomic_is_valid_fail_order(inc, dec));
}

TEST(HelperFunctionsTest, cmpxchg_fail_order)
{
    ASSERT_EQ(patomic_cmpxchg_fail_order(patomic_RELAXED), patomic_RELAXED);
    ASSERT_EQ(patomic_cmpxchg_fail_order(patomic_CONSUME), patomic_CONSUME);
    ASSERT_EQ(patomic_cmpxchg_fail_order(patomic_ACQUIRE), patomic_ACQUIRE);
    ASSERT_EQ(patomic_cmpxchg_fail_order(patomic_RELEASE), patomic_ACQUIRE);
    ASSERT_EQ(patomic_cmpxchg_fail_order(patomic_ACQ_REL), patomic_ACQUIRE);
    ASSERT_EQ(patomic_cmpxchg_fail_order(patomic_SEQ_CST), patomic_SEQ_CST);

    // not is_valid_* function, so no order validation done
    // simply return input if not recognised as valid order
    int inc = patomic_SEQ_CST + 1;
    int dec = patomic_RELAXED - 1;
    ASSERT_EQ(patomic_cmpxchg_fail_order(inc), inc);
    ASSERT_EQ(patomic_cmpxchg_fail_order(dec), dec);
}
