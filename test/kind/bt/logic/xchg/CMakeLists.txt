# ---- Create Tests ----

create_bt(
    NAME BtLogicExchange
    SOURCE
        exchange.cpp
)

create_bt(
    NAME BtLogicCmpxchgWeak
    SOURCE
        cmpxchg_weak.cpp
)

create_bt(
    NAME BtLogicCmpxchgStrong
    SOURCE
        cmpxchg_strong.cpp
)
