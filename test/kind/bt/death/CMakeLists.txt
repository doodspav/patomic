# ---- Create Tests ----

create_bt(
    NAME BtDeathInvalidOffset
    SOURCE
        invalid_offset.cpp
)

create_bt(
    NAME BtDeathInvalidOrder
    SOURCE
        invalid_order.cpp
)

create_bt(
    NAME BtDeathNullParams
    SOURCE
        null_params.cpp
)

create_bt(
    NAME BtDeathUnalignedObject
    SOURCE
        unaligned_object.cpp
)
