#ifndef PATOMIC_IMPL_TSX_OSMEM_H
#define PATOMIC_IMPL_TSX_OSMEM_H

#include <patomic/patomic_config.h>
#include <patomic/macros/ignore_unused.h>

#include <stddef.h>


/*
 * OSMEM:
 * - alloc_rw
 * - protect_rx
 * - free
 */

#if PATOMIC_HAVE_WIN32_MEMORYAPI_VIRTUAL

#undef NTDDI_VERSION
#undef WINVER
#undef _WIN32_WINNT
#undef WIN32_LEAN_AND_MEAN

#define NTDDI_VERSION 0x05010000
#define WINVER 0x0501
#define _WIN32_WINNT 0x0501
#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <memoryapi.h>

static void *
patomic_tsx_osmem_alloc_rw(
    size_t size
)
{
    return VirtualAlloc(NULL, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

static int
patomic_tsx_osmem_protect_rx(
    void *addr,
    size_t size
)
{
    DWORD old;
    return (int) VirtualProtect(addr, size, PAGE_EXECUTE_READ, &old);
}

static void
patomic_tsx_osmem_free(
    void *addr,
    size_t size
)
{
    PATOMIC_IGNORE_UNUSED(size);
    PATOMIC_IGNORE_UNUSED(VirtualFree(addr, 0, MEM_DECOMMIT | MEM_RELEASE));
}

#elif PATOMIC_HAVE_SYS_MMAN_MMAP_MPROTECT

#include <sys/mman.h>

#ifndef MAP_ANONYMOUS
    #define MAP_ANONYMOUS MAP_ANON
#endif

static void *
patomic_tsx_osmem_alloc_rw(
    size_t size
)
{
    void *addr = mmap(
        NULL, size,
        PROT_READ | PROT_WRITE, MAP_ANONYMOUS,
        -1, 0
    );
    return (addr == MAP_FAILED) ? NULL : addr;
}

static int
patomic_tsx_osmem_protect_rx(
    void *addr,
    size_t size
)
{
    int res = mprotect(addr, size, PROT_READ | PROT_EXEC);
    return (res == 0) ? 1 : 0;
}

static void
patomic_tsx_osmem_free(
    void *addr,
    size_t size
)
{
    PATOMIC_IGNORE_UNUSED(munmap(addr, size));
}

#else

static void *
patomic_tsx_osmem_alloc_rw(
        size_t size
)
{
    PATOMIC_IGNORE_UNUSED(size);
    return NULL;
}

static int
patomic_tsx_osmem_protect_rx(
        void *addr,
        size_t size
)
{
    PATOMIC_IGNORE_UNUSED(addr);
    PATOMIC_IGNORE_UNUSED(size);
    return 0;
}

static void
patomic_tsx_osmem_free(
        void *addr,
        size_t size
)
{
    PATOMIC_IGNORE_UNUSED(addr);
    PATOMIC_IGNORE_UNUSED(size);
}

#endif

#endif  /* !PATOMIC_IMPL_TSX_OSMEM_H */
