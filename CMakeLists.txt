# https://libcheck.github.io/check/
cmake_minimum_required(VERSION 2.8)

project(TLA-TPE)

# Determine minimum C
set(CMAKE_C_STANDARD 11)

# Enable debugging
set(CMAKE_BUILD_TYPE Debug)

# Show all warning messages
set(CMAKE_C_FLAGS "-Wall -Wextra -g -std=c11 -fsanitize=address -D_POSIX_C_SOURCE=200112L -fno-omit-frame-pointer -Wno-unused-parameter -Wno-unused-variable")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_subdirectory(src)

set(YACC_OUTPUT ${CMAKE_SOURCE_DIR}/y.tab.c)
set(LEX_OUTPUT ${CMAKE_SOURCE_DIR}/lex.yy.c)
set(U3DC_OUTPUT ${CMAKE_SOURCE_DIR}/u3dc)


add_custom_command(OUTPUT ${YACC_OUTPUT} ${CMAKE_SOURCE_DIR}/y.tab.h ${LEX_OUTPUT} ${U3DC_OUTPUT}
        COMMAND make -C ${CMAKE_SOURCE_DIR} build
        COMMENT "Building Parsers")

set_source_files_properties(${YACC_OUTPUT}
        PROPERTIES GENERATED TRUE)
set_source_files_properties(${LEX_OUTPUT}
        PROPERTIES GENERATED TRUE)

add_custom_target(U3DC ${U3DC_OUTPUT})

add_executable(U3D_DUMMY ${YACC_OUTPUT} ${LEX_OUTPUT})
target_link_libraries(U3D_DUMMY U3DC_LIB)