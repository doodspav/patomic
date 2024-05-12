# fine with CML file here because include isn't installed for tests

# add directory files to target
target_sources(
    patomic-test-include PRIVATE
    # .
    magic_enum.hpp
    magic_enum_all.hpp
    magic_enum_containers.hpp
    magic_enum_flags.hpp
    magic_enum_format.hpp
    magic_enum_fuse.hpp
    magic_enum_iostream.hpp
    magic_enum_switch.hpp
    magic_enum_utility.hpp
)
