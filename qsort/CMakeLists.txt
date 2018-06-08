set(
    SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/../main_prog.cpp"
)
add_definitions(-DUSE_STDLIB_QSORT)
add_executable(qsort ${SRCS})

install(TARGETS qsort RUNTIME DESTINATION ".")
