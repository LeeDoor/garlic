#pragma once
#include "parsing_error.hpp"

namespace garlic { class Query; }

namespace garlic::sql_parser {

/// Defines a result of any Query. May be a correct query, error or just a blank.
class ParsingResult { 
public:
    using ValueType = std::variant<uptr<Query>, ParsingError>;

    ParsingResult(uptr<Query> query) 
    : value_{ std::move(query) }{}

    ParsingResult(ParsingError&& error) 
    : value_{ std::move(error) } {}

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
};

}
