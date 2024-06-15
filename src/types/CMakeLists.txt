# add directory files to target
target_sources(${target_name} PRIVATE
    align.c
    feature_check_opcat.c
    feature_check_opkind.c
    ids.c
    memory_order.c
    transaction.c
)
