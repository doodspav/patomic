# add all subdirectories
add_subdirectory(api)
add_subdirectory(impl)
add_subdirectory(include)
add_subdirectory(stdlib)

# add directory files to target
target_sources(${target_name} PRIVATE
    patomic.c
)
