set(
    SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/../main_prog.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../mm88c.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../qs9e17.c"
)
add_definitions(-DUSE_QS9E17)
add_executable(qs9e17 ${SRCS})

install(TARGETS qs9e17 RUNTIME DESTINATION ".")
