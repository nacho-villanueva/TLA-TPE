# https://libcheck.github.io/check/
cmake_minimum_required(VERSION 2.8)

project(TLA-TPE)

# Determine minimum C
set(CMAKE_C_STANDARD 11)

# Enable debugging
set(CMAKE_BUILD_TYPE Debug)

# Show all warning messages
set(CMAKE_C_FLAGS "-Wall -Wno-unused-variable -g -lm -std=c11 -fsanitize=address -D_POSIX_C_SOURCE=200809L -Wextra -fno-omit-frame-pointer -Wno-unused-parameter -Wno-unused-variable")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_subdirectory(src)

FIND_PACKAGE(BISON REQUIRED)
SET(BisonOutput ${CMAKE_SOURCE_DIR}/y.tab.c)
IF(BISON_FOUND)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${BisonOutput}
            COMMAND ${BISON_EXECUTABLE}
            --defines=${CMAKE_SOURCE_DIR}/y.tab.h
            --output=${BisonOutput}
            ${CMAKE_SOURCE_DIR}/src/yacc.y
            COMMENT "Generating Yacc"
    )
ENDIF()

FIND_PACKAGE(FLEX REQUIRED)
SET(FlexOutput ${CMAKE_SOURCE_DIR}/lex.yy.c)
IF(FLEX_FOUND)
    ADD_CUSTOM_COMMAND(
            OUTPUT ${FlexOutput}
            COMMAND ${FLEX_EXECUTABLE}
            --outfile=${FlexOutput}
            ${CMAKE_SOURCE_DIR}/src/grammar.l
            COMMENT "Generating Lex"
    )
ENDIF()

set_source_files_properties(${YACC_OUTPUT}
        PROPERTIES GENERATED TRUE)
set_source_files_properties(${LEX_OUTPUT}
        PROPERTIES GENERATED TRUE)

add_executable(u3dc ${BisonOutput} ${FlexOutput})
target_link_libraries(u3dc U3D_Core)