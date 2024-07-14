# ---- Create Tests ----

create_ut(
    NAME UtInternalAlign
    SOURCE
        align.cpp
        "${PATOMIC_SOURCE_DIR}/src/api/align.c"
)
