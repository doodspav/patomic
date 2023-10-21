# ---- Windows Path Prefix ----

# Windows doesn't support rpath, so when linking dynamically the libraries need
# to either be in the same directory or in PATH.
# This function sets a variable to a list of GENERATOR strings which resolve to
# the paths of the linked dependencies.
# See: https://docs.microsoft.com/en-us/windows/win32/dlls/dynamic-link-library-search-order
# Usage: windows_deps_paths(<variable> <link-target>...)
function(windows_deps_paths ARG_VAR)

    # check that we're on Windows
    if(NOT CMAKE_SYSTEM_NAME STREQUAL "Windows")
        set(${ARG_VAR} "" PARENT_SCOPE)
        return()
    endif()

    # create a list of paths to dependency shared library locations
    set(paths "")
    foreach(target IN LISTS ARGN)
        # This will fail if passed a link option that isn't a target.
        # This is intentional; don't do that.
        # Instead, create an IMPORTED library, and set its target properties
        # such as IMPORTED_LOCATION for the library (.a .so etc.) path and
        # set INTERFACE_INCLUDE_DIRECTORIES to the directory containing any
        # necessary header files.
        get_target_property(type "${target}" TYPE)
        if(type STREQUAL "SHARED_LIBRARY")
            list(APPEND paths "$<TARGET_FILE_DIR:${target}>")
        endif()
    endforeach()

    # remove duplicates
    # makes generated file more human readable (if generated)
    list(REMOVE_DUPLICATES paths)

    # concatenate list into string with same format as PATH on Windows
    set(path "")
    set(glue "")
    foreach(p IN LISTS paths)
        set(path "${path}${glue}${p}")
        set(glue "\;")  # backslash is important
    endforeach()

    # "return" string with PATH data
    set(${ARG_VAR} "${path}" PARENT_SCOPE)
endfunction()
