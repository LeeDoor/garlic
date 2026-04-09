#pragma once
#include "parsing_error.hpp"

namespace garlic { class Query; }

namespace garlic::sql_parser {

class ParsingResult { 
public:
    using ValueType = std::variant<uptr<Query>, ParsingError, std::monostate>;
    explicit ParsingResult(size_t end_idx) : value_{ }, end_idx_{ end_idx } {}
    ParsingResult(uptr<Query> query, size_t end_idx) 
    : value_{ std::move(query) }, end_idx_{ end_idx } {}

    ParsingResult(ParsingError&& error, size_t end_idx) 
    : value_{ std::move(error) }, end_idx_{ end_idx } {}

    size_t get_end_idx() const { return end_idx_; }
    void update_end_idx(size_t end_idx) { end_idx_ = end_idx; }

    bool is_blank() const { 
	return std::holds_alternative<std::monostate>(value_);
    }
    bool is_error() const {
	const ParsingError* error = std::get_if<ParsingError>(&value_);
	return error != nullptr;
    }
    bool is_query() const {
	const uptr<Query>* query = std::get_if<uptr<Query>>(&value_);
	return query != nullptr;
    }
    const ParsingError& as_error() const& {
	const ParsingError* error = std::get_if<ParsingError>(&value_);
	if(!error) throw std::logic_error("ParsingResult is not an error but called as_error()");
	return *error;
    }
    const uptr<Query>& as_query() const& {
	const uptr<Query>* query = std::get_if<uptr<Query>>(&value_);
	if(!query) throw std::logic_error("ParsingResult is not an query but called as_query()");
	return *query;
    }

private:
    ValueType value_;
    size_t end_idx_;
};

}
