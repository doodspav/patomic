/* Copyright (c) doodspav. */
/* SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception */

#include <patomic/api/version.h>


const char *
patomic_version_string(void)
{
    return PATOMIC_VERSION_STRING;
}


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
    const int major,
    const int minor,
    const int patch
)
{
    return PATOMIC_VERSION_COMPATIBLE_WITH(major, minor, patch);
}
