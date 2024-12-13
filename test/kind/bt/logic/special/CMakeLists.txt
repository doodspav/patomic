# ---- Create Tests ----

create_bt(
    NAME BtLogicSpecialDoubleCmpxchg
    SOURCE
        double_cmpxchg.cpp
)

create_bt(
    NAME BtLogicSpecialMultiCmpxchg
    SOURCE
        multi_cmpxchg.cpp
)

create_bt(
    NAME BtLogicSpecialGeneric
    SOURCE
        generic.cpp
)

create_bt(
    NAME BtLogicSpecialGenericWfb
    SOURCE
        generic_wfb.cpp
)
