find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
set(GEN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/generated)

flex_target(LEXER "lexer.ll" 
    "${GEN_DIR}/lexer.yy.cpp" 
    DEFINES_FILE "${GEN_DIR}/lexer.yy.hpp"
)
bison_target(PARSER "parser.yy" 
    "${GEN_DIR}/parser.tab.cpp" 
    DEFINES_FILE "${GEN_DIR}/parser.tab.hpp" 
    COMPILE_FLAGS "-Wcounterexamples -Werror"
)

add_flex_bison_dependency(LEXER PARSER)
add_library(sql_parser 
    ${GEN_DIR}/lexer.yy.cpp
    ${GEN_DIR}/parser.tab.cpp
    driver.cpp
    query_io.cpp
    parsing_location.cpp
    error_printer.cpp
    sql_ast_executor.cpp
    parsing_context.cpp
)
set(INCLUDE_DIRS 
    ../abstract_syntax_tree 
    ../table 
    ${GENERAL_INCLUDE_DIRECTORIES} 
    ${GEN_DIR} 
    ${CMAKE_CURRENT_SOURCE_DIR}
)
set(LINK_LIBS ${GENERAL_LINK_LIBRARIES} abstract_syntax_tree)

target_include_directories(sql_parser PUBLIC ${INCLUDE_DIRS})
target_link_libraries(sql_parser PUBLIC ${LINK_LIBS})
