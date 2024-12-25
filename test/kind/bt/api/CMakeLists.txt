# Copyright (c) doodspav.
# SPDX-License-Identifier: LGPL-3.0-or-later WITH LGPL-3.0-linking-exception

# ---- Create Tests ----

create_bt(
    NAME BtApiAlign
    SOURCE
        align.cpp
)

create_bt(
    NAME BtApiCombine
    SOURCE
        combine.cpp
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
    NAME BtApiOptions
    SOURCE
        options.cpp
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
