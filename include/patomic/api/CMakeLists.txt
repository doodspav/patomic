# add all subdirectories
add_subdirectory(ops)

# add directory files to target
target_sources(${target_name} PRIVATE
    align.h
    combine.h
    feature_check.h
    ids.h
    memory_order.h
    ops.h
    options.h
    transaction.h
)
