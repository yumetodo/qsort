cmake_minimum_required(VERSION 3.3)
enable_language(CXX)
set(CMAKE_CXX_STANDARD 14) # C++14...
set(CMAKE_CXX_STANDARD_REQUIRED ON) #...is required...
set(CMAKE_CXX_EXTENSIONS OFF) #...without compiler extensions like gnu++11

#
# Set our project name
#
project(qsort_benchmark)

#
# Source files
#
set(
    SRCS
    "./main_prog.cpp"
    "./target_data.cpp"
    "./global_variable.c"
    "./mm88c.c"
    "./qs10a5.c"
    "./qs10a5m.c"
    "./qs9e17.c"
    "./die.c"
)

#
# find thread library
#
find_package(Threads REQUIRED)

#
# Update compiler waring flags
#
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
elseif(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  # Update if necessary
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-long-long -pedantic")
endif()

#
# Compile
#
add_executable(qsort_benchmark ${SRCS})

#
# Link
#
target_link_libraries(qsort_benchmark Threads::Threads)

#
# Install
#
install(TARGETS qsort_benchmark RUNTIME DESTINATION ".")
