# only add BTs if patomic target is available
if(TARGET patomic::patomic)
    add_subdirectory(bt)
    message(STATUS "Enabled binary tests")
else()
    message(STATUS "Skipping binary tests; patomic target not available")
endif()

# always STs since they don't require patomic in any way
add_subdirectory(st)
message(STATUS "Enabled system tests")

# only add UTs if patomic files are available
# these are currently set by patomic before including this project
if(PATOMIC_BINARY_DIR AND PATOMIC_SOURCE_DIR)
    add_subdirectory(ut)
    message(STATUS "Enabled unit tests")
else()
    message(STATUS "Skipping unit tests; not building as sub-project of patomic")
endif()
