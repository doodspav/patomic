#include <patomic/patomic_version.h>


int
patomic_version_major(void)
{
    return PATOMIC_VERSION_MAJOR;
}

int
patomic_version_minor(void)
{
    return PATOMIC_VERSION_MINOR;
}

int
patomic_version_patch(void)
{
    return PATOMIC_VERSION_PATCH;
}

int
patomic_version_compatible_with(
    int major,
    int minor
)
{
    return major == PATOMIC_VERSION_MAJOR
        && minor <= PATOMIC_VERSION_MINOR;
}
