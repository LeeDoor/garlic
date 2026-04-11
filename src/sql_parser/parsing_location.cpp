#include "parsing_location.hpp"

namespace garlic::sql_parser {

void ParsingLocation::reset() {
    current_position_.initialize();
    token_start_position_.initialize();
    query_raw_start_position.initialize();
    query_content_start_position.initialize();
    line_start_position_.initialize();
}

ParsingLocation::PositionType& ParsingLocation::cur() & {
    return current_position_;
}
const ParsingLocation::PositionType& ParsingLocation::cur() const& {
    return current_position_;
}
ParsingLocation::PositionType ParsingLocation::token_start() const {
    return token_start_position_;
}
ParsingLocation::PositionType ParsingLocation::raw_query_start() const {
    return query_raw_start_position;
}
ParsingLocation::PositionType ParsingLocation::content_query_start() const {
    return query_content_start_position;
}
ParsingLocation::PositionType ParsingLocation::line_start() const {
    return line_start_position_;
}

void ParsingLocation::reset_to_query_start() {
    current_position_ = query_raw_start_position;
}
void ParsingLocation::on_raw_query_start() {
    query_raw_start_position = current_position_;
}
void ParsingLocation::on_content_query_start() {
    query_content_start_position = current_position_;
}
void ParsingLocation::on_token_start() {
    token_start_position_ = current_position_;
}
void ParsingLocation::on_line_start() {
    line_start_position_ = current_position_;
}

}
