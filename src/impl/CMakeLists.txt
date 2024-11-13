# add all subdirectories
add_subdirectory(null)
add_subdirectory(std)

# add directory files to target
target_sources(${target_name} PRIVATE
    register.h
)
