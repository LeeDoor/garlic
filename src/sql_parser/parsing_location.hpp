#pragma once
#include "position.hpp"

namespace garlic::sql_parser {

class ParsingLocation {
    using PositionType = Position;
public:
    void reset();

    PositionType& cur() &;
    const PositionType& cur() const&;
    PositionType token_start() const;
    PositionType query_start() const;

    void reset_to_query_start();
    void on_query_start();
    void on_token_start();

private:
    PositionType current_location_ {};
    PositionType token_start_location_ {};
    PositionType query_start_location_ {};
};

}
