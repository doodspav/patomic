# ---- Windows Path Prefix ----

# Windows doesn't support rpath, so when linking dynamically the libraries need
# to either be in the same directory or on PATH.
# This function sets a variable to a GENERATOR string that may be prepended to
# path in order to find linked dependencies
# See: https://docs.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order
# Usage: windows_deps_path(<variable> <link-target>...)
function(windows_deps_path VAR)
    if(NOT CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
        set(${VAR} "" PARENT_SCOPE)
        return()
    endif()

    set(paths "")
    foreach(target IN LISTS ARGN)
        # This will fail if passed a link option that isn't a target
        # This is intentional; don't do that
        # Instead, create an IMPORTED library, and set its target properties
        # such as IMPORTED_LOCATION for the library (.a .so etc.) path and
        # set INTERFACE_INCLUDE_DIRECTORIES to the directory containing any
        # necessary header files
        get_target_property(type "${target}" TYPE)
        if(type STREQUAL "SHARED_LIBRARY")
            list(APPEND paths "$<TARGET_FILE_DIR:${target}>")
        endif()
    endforeach()

    list(REMOVE_DUPLICATES paths)

    set(path "")
    set(glue "")
    foreach(p IN LISTS paths)
        set(path "${path}${glue}${p}")
        set(glue "\;")  # backslash is important
    endforeach()

    set(${VAR} "${path}" PARENT_SCOPE)
endfunction()
