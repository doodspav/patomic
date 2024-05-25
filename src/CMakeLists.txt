# add all subdirectories
add_subdirectory(impl)
add_subdirectory(include/patomic)
add_subdirectory(types)

# add /include files to target
# unfortunately can't have CML file in /include
target_sources(
    ${target_name} PRIVATE
    # include
    "${PROJECT_SOURCE_DIR}/include/patomic/patomic.h"
    # include/types
    "${PROJECT_SOURCE_DIR}/include/patomic/types/align.h"
    "${PROJECT_SOURCE_DIR}/include/patomic/types/feature_check.h"
    "${PROJECT_SOURCE_DIR}/include/patomic/types/ids.h"
    "${PROJECT_SOURCE_DIR}/include/patomic/types/memory_order.h"
    "${PROJECT_SOURCE_DIR}/include/patomic/types/ops.h"
    "${PROJECT_SOURCE_DIR}/include/patomic/types/options.h"
    "${PROJECT_SOURCE_DIR}/include/patomic/types/transaction.h"
    # include/types/ops
    "${PROJECT_SOURCE_DIR}/include/patomic/types/ops/explicit.h"
    "${PROJECT_SOURCE_DIR}/include/patomic/types/ops/implicit.h"
    "${PROJECT_SOURCE_DIR}/include/patomic/types/ops/transaction.h"
)

# add directory files to target
target_sources(
    ${target_name} PRIVATE
    # .
    patomic.c
)
