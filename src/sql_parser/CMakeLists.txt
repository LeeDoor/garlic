find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
set(GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR})

flex_target(LEXER "lexer.l" "${GEN_DIR}/lexer.yy.c" DEFINES_FILE "${GEN_DIR}/lexer.yy.h")
bison_target(PARSER "parser.y" "${GEN_DIR}/parser.tab.c" DEFINES_FILE "${GEN_DIR}/parser.tab.h")

add_flex_bison_dependency(LEXER PARSER)
add_library(sql_parser 
    ${GEN_DIR}/lexer.yy.c
    ${GEN_DIR}/parser.tab.c
)

add_executable(test_sql_parser ${GEN_DIR}/lexer.yy.c ${GEN_DIR}/parser.tab.c test_main.c)
