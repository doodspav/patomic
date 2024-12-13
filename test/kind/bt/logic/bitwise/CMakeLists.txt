# ---- Create Tests ----

create_bt(
    NAME BtLogicBitTest
    SOURCE
        test.cpp
)

create_bt(
    NAME BtLogicBitTestCompl
    SOURCE
        test_compl.cpp
)

create_bt(
    NAME BtLogicBitTestSet
    SOURCE
        test_set.cpp
)

create_bt(
    NAME BtLogicBitTestReset
    SOURCE
       test_reset.cpp
)
