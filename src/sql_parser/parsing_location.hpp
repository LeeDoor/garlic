#pragma once
#include "location.hh"

namespace garlic::sql_parser {

class ParsingLocation {
    using LocationType = yy::location;
public:
    void reset();

    LocationType& cur() &;
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
