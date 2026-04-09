#include "parsing_location.hpp"

namespace garlic::sql_parser {

void ParsingLocation::reset() {
    current_location_.initialize();
    token_start_location_.initialize();
    query_start_location_.initialize();
}

ParsingLocation::LocationType& ParsingLocation::cur() & {
    return current_location_;
}
const ParsingLocation::LocationType& ParsingLocation::cur() const& {
    return current_location_;
}
ParsingLocation::LocationType ParsingLocation::token_start() const {
    return token_start_location_;
}
ParsingLocation::LocationType ParsingLocation::query_start() const {
    return query_start_location_;
}
void ParsingLocation::reset_to_query_start() {
    current_location_ = query_start_location_;
}
void ParsingLocation::on_query_start() {
    query_start_location_ = current_location_;
}
void ParsingLocation::on_token_start() {
    token_start_location_ = current_location_;
}

}
