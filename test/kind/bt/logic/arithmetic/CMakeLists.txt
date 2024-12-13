# ---- Create Tests ----

create_bt(
    NAME BtLogicArithmeticAdd
    SOURCE
        add.cpp
)

create_bt(
    NAME BtLogicArithmeticSub
    SOURCE
        sub.cpp
)

create_bt(
    NAME BtLogicArithmeticInc
    SOURCE
        inc.cpp
)

create_bt(
    NAME BtLogicArithmeticDec
    SOURCE
        dec.cpp
)

create_bt(
    NAME BtLogicArithmeticNeg
    SOURCE
        neg.cpp
)
