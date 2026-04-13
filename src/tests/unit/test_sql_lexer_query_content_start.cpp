#include "lexer.yy.hpp"
#include "parsing_session.hpp"

namespace garlic::sql_parser {

namespace {

yy::parser::symbol_type next_token(ParsingSession& session) {
    return yylex(session);
}

} // namespace

TEST(test_sql_lexer_query_content_start, leadingWhitespaceThenSelectShouldSetContentStartToSelect) {
    ParsingSession session;

    const std::string input = "\n  SELECT 1;";
    YY_BUFFER_STATE buffer = yy_scan_bytes(input.data(), static_cast<int>(input.size()));
    ASSERT_NE(buffer, nullptr);

    auto token = next_token(session);
    yy_delete_buffer(buffer);

    ASSERT_EQ(token.kind(), yy::parser::symbol_kind::S_SELECT);
    const Position content_start = session.location().content_query_start();
    EXPECT_EQ(content_start.get_line(), 2u);
    EXPECT_EQ(content_start.get_column(), 3u);
}

TEST(test_sql_lexer_query_content_start, secondQueryShouldResetAndSetContentStartAgain) {
    ParsingSession session;

    const std::string input = "  SELECT 1;\n   SELECT 2;";
    YY_BUFFER_STATE buffer = yy_scan_bytes(input.data(), static_cast<int>(input.size()));
    ASSERT_NE(buffer, nullptr);

    int select_count = 0;
    Position second_select_start;

    while (true) {
        auto token = next_token(session);
        if (token.kind() == yy::parser::symbol_kind::S_SELECT) {
            ++select_count;
            if (select_count == 2) {
                second_select_start = session.location().content_query_start();
                break;
            }
        } else if (token.kind() == yy::parser::symbol_kind::S_SEMICOLON) {
            session.blank_parsed();
        } else if (token.kind() == yy::parser::symbol_kind::S_YYEOF) {
            break;
        }
    }

    yy_delete_buffer(buffer);
    ASSERT_EQ(select_count, 2);
    EXPECT_EQ(second_select_start.get_line(), 2u);
    EXPECT_EQ(second_select_start.get_column(), 4u);
}

} // namespace garlic::sql_parser
