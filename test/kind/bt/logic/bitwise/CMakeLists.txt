# ---- Create Tests ----

create_bt(
    NAME BtLogicBitwiseTest
    SOURCE
        test.cpp
)

create_bt(
    NAME BtLogicBitwiseTestCompl
    SOURCE
        test_compl.cpp
)

create_bt(
    NAME BtLogicBitwiseTestSet
    SOURCE
        test_set.cpp
)

create_bt(
    NAME BtLogicBitwiseTestReset
    SOURCE
       test_reset.cpp
)
