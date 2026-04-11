#pragma once
#include "parsing_error.hpp"

namespace garlic { class Query; }

namespace garlic::sql_parser {

class ParsingResult { 
public:
    using ValueType = std::variant<uptr<Query>, ParsingError, std::monostate>;

    explicit ParsingResult(Position end_pos) : value_{ std::monostate{} }, end_pos_{ end_pos } {}

    ParsingResult(uptr<Query> query, Position end_pos) 
    : value_{ std::move(query) }, end_pos_{ end_pos } {}

    ParsingResult(ParsingError&& error, Position end_pos) 
    : value_{ std::move(error) }, end_pos_{ end_pos } {}

    Position get_end_position() const { return end_pos_; }
    void update_end_pos(Position end_pos) { end_pos_ = end_pos; }

    bool is_blank() const { 
	return std::holds_alternative<std::monostate>(value_);
    }
    bool is_error() const {
	return std::holds_alternative<ParsingError>(value_);
    }
    bool is_query() const {
	return std::holds_alternative<uptr<Query>>(value_);
    }
    const ParsingError& as_error() const& {
	const ParsingError* error = std::get_if<ParsingError>(&value_);
	if(!error) throw std::logic_error("ParsingResult is not an error but called as_error()");
	return *error;
    }
    const uptr<Query>& as_query() const& {
	const uptr<Query>* query = std::get_if<uptr<Query>>(&value_);
	if(!query) throw std::logic_error("ParsingResult is not a query but called as_query()");
	return *query;
    }

private:
    ValueType value_;
    Position end_pos_;
};

}
