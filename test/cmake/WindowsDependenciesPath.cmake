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

    set(path "")
    set(glue "")
    foreach(target IN LISTS ARGN)
        get_target_property(type "${target}" TYPE)
        if(type STREQUAL "SHARED_LIBRARY")
            set(path "${path}${glue}$<TARGET_FILE_DIR:${target}>")
            set(glue "\;")  # backslash is important
        endif()
    endforeach()

    if(NOT path STREQUAL "")
        set(${VAR} "${path}" PARENT_SCOPE)
    else()
        set(${VAR} "" PARENT_SCOPE)
    endif()
endfunction()
