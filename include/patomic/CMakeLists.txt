# add all subdirectories
add_subdirectory(api)

# add directory files to target
target_sources(${target_name} PRIVATE
    patomic.h
)
