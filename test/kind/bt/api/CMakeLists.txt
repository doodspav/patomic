# create tests

create_bt(
    NAME BtApiAlign
    SOURCE
        align.cpp
)

create_bt(
    NAME BtApiFeatureCheck
    SOURCE
        feature_check_any_all.cpp
        feature_check_leaf.cpp
)

create_bt(
    NAME BtApiIds
    SOURCE
        ids.cpp
)

create_bt(
    NAME BtApiMemoryOrder
    SOURCE
        memory_order.cpp
)

create_bt(
    NAME BtApiTransaction
    SOURCE
        transaction.cpp
)

create_bt(
    NAME BtApiVersion
    SOURCE
        version.cpp
)
