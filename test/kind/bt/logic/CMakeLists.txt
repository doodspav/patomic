# ---- Create Tests ----

create_bt(
    NAME BtLogicStore
    SOURCE
        store.cpp
)

create_bt(
    NAME BtLogicLoad
    SOURCE
        load.cpp
)

# add all subdirectories
add_subdirectory(xchg)
add_subdirectory(bitwise)
add_subdirectory(binary)
add_subdirectory(arithmetic)
add_subdirectory(special)
add_subdirectory(flag)
add_subdirectory(raw)
