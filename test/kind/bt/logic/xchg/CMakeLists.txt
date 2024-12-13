# ---- Create Tests ----

create_bt(
    NAME BtLogicXchgExchange
    SOURCE
        exchange.cpp
)

create_bt(
    NAME BtLogicXchgCmpxchgWeak
    SOURCE
        cmpxchg_weak.cpp
)

create_bt(
    NAME BtLogicXchgCmpxchgStrong
    SOURCE
        cmpxchg_strong.cpp
)
