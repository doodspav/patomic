#include <patomic/patomic_version.h>
#include <patomic/macros/force_inline.h>


PATOMIC_FORCE_INLINE int
patomic_version_major(void)
{
    return PATOMIC_VERSION_MAJOR;
}

PATOMIC_FORCE_INLINE int
patomic_version_minor(void)
{
    return PATOMIC_VERSION_MINOR;
}

PATOMIC_FORCE_INLINE int
patomic_version_patch(void)
{
    return PATOMIC_VERSION_PATCH;
}

PATOMIC_FORCE_INLINE int
patomic_version_compatible_with(
    int major,
    int minor
)
{
    return major == PATOMIC_VERSION_MAJOR
        && minor <= PATOMIC_VERSION_MINOR;
}
