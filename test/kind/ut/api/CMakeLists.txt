# ---- Create Tests ----

create_ut(
    NAME UtApiAlign
    SOURCE
        align.cpp
        "${PATOMIC_SOURCE_DIR}/src/api/align.c"
)
