#include "parser.tab.hpp"
#include "position.hpp"
#include "literals_converter.hpp"

namespace garlic::sql_parser {

static std::string extract_string_token_value(yy::parser::symbol_type& symbol) {
    EXPECT_EQ(symbol.kind(), yy::parser::symbol_kind::S_STRING);
    return symbol.value.template as<StringType>();
}

TEST(test_sql_lexer_make_string, singleQuotedLiteralShouldUnquoteAndClearSourceBuffer) {
    std::string raw = "'hello_world'";
    Position pos;

    auto symbol = make_STRING(raw, pos);
    EXPECT_EQ(extract_string_token_value(symbol), "hello_world");
    EXPECT_EQ(raw, "\'hello_world\'");
}

TEST(test_sql_lexer_make_string, doubleQuotedLiteralShouldUnquoteAndKeepInnerSingleQuotes) {
    std::string raw = "\"ab'cd\"";
    Position pos;

    auto symbol = make_STRING(raw, pos);
    EXPECT_EQ(extract_string_token_value(symbol), "ab'cd");
}

TEST(test_sql_lexer_make_string, knownEscapesShouldBeDecoded) {
    std::string raw = "'line1\\nline2\\t\\\\\\'\\\"'";
    Position pos;

    auto symbol = make_STRING(raw, pos);
    EXPECT_EQ(extract_string_token_value(symbol), "line1\nline2\t\\'\"");
}

TEST(test_sql_lexer_make_string, unknownEscapesShouldStayVerbatim) {
    std::string raw = "'a\\0b\\rc'";
    Position pos;

    auto symbol = make_STRING(raw, pos);
    EXPECT_EQ(extract_string_token_value(symbol), "a\\0b\\rc");
}

TEST(test_sql_lexer_make_string, emptyQuotedStringShouldProduceEmptyValue) {
    std::string raw = "''";
    Position pos;

    auto symbol = make_STRING(raw, pos);
    EXPECT_EQ(extract_string_token_value(symbol), "");
}

} // namespace garlic::sql_parser
