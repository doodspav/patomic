# add directory files to target
target_sources(${target_name} PRIVATE
    align.c
    ids.c
    memory_order.c
    transaction.c
)
