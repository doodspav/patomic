# add all subdirectories
add_subdirectory(helper)
add_subdirectory(macros)
add_subdirectory(stdlib)

# add directory files to target
target_sources(${target_name} PRIVATE
    config.h
)
