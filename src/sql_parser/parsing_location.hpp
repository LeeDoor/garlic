#pragma once
#include "location.hh"

namespace garlic::sql_parser {

class Multilocation {
public:
    operator yy::location() {
	return location_2d_;
    }

    const yy::location& location() const& {
	return location_2d_;
    }
    size_t chars_amount() const {
	return location_chars_;
    }

    void initialize() {
	location_chars_ = 0;
	location_2d_.initialize();
    }
    void lines(int n = 1) {
	location_chars_ += n;
	location_2d_.lines(n);
    }
    void columns(int n = 1) {
	location_chars_ += n;
	location_2d_.columns(n);
    }
    void step() {
	location_2d_.step();
    }
private:
    size_t location_chars_ {};
    yy::location location_2d_ {};
};

inline std::ostream& operator<<(std::ostream& os, const Multilocation& ml) {
    os << ml.location();
    return os;
}

class ParsingLocation {
    using LocationType = Multilocation;
public:
    void reset();

    LocationType& cur() &;
    const LocationType& cur() const&;
    LocationType token_start() const;
    LocationType query_start() const;

    void reset_to_query_start();
    void on_query_start();
    void on_token_start();

private:
    LocationType current_location_ {};
    LocationType token_start_location_ {};
    LocationType query_start_location_ {};
};

}
