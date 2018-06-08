set(
    SRCS
    "${CMAKE_CURRENT_SOURCE_DIR}/../main_prog.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../mm88c.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/../qs10a5.c"
)
add_definitions(-DUSE_QS10A5)
add_executable(qs10a5 ${SRCS})

install(TARGETS qs10a5 RUNTIME DESTINATION ".")
