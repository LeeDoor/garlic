#pragma once
#include "parsing_session.hpp"

namespace garlic::sql_parser {
    class ParsingResult;

class ParsingContext {
public:
    using ParsingResults = ParsingSession::ParsingResults;
    explicit ParsingContext(bool debug = false);

    /// Driver API
    ParsingResults parse(StringViewType query_string);

private:
    using ContinuationState = ParsingSession::ContinuationState;

    decltype(auto) create_parser(ParsingSession& session, StringViewType query_string);

    void scan_begin(StringViewType query_string);
    void scan_end();

    bool debug_mode_ {};
    std::optional<ContinuationState> continuation_state_ {std::nullopt};
};

}
