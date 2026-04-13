#pragma once
#include "position.hpp"

namespace garlic::sql_parser {

class ParsingLocation {
    using PositionType = Position;
public:
    PositionType& cur() &;
    const PositionType& cur() const&;
    PositionType token_start() const;
    PositionType raw_query_start() const;
    PositionType content_query_start() const;
    PositionType line_start() const;

    void reset_to_query_start();
    void on_raw_query_start();
    void on_content_query_start();
    void on_token_start();
    void on_line_start();

private:
    PositionType current_position_ {}; // position of unread character
    PositionType token_start_position_ {}; // position of the first character in current token
    PositionType query_raw_start_position {}; // position of the first character in current query; INCLUDING LEADING WHITESPACE
    PositionType query_content_start_position {}; // position of the first MEANINGFUL character in current query; excludes leading whitespaces
    PositionType line_start_position_ {}; // position of the first character in current line
};

}
