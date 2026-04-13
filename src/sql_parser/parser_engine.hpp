#pragma once
#include "parsing_session.hpp"

namespace garlic::sql_parser {
    class ParsingResult;

class ParserEngine {
public:
    struct Results { 
	ParsingSession::ParsingResults results;
	size_t characters_parsed;

	Results(ParsingSession::IntermediateResult&& inter)
	: results{ std::move(inter.parsing_results) }
	, characters_parsed{ inter.characters_parsed }
	{}
    };
    explicit ParserEngine(bool debug = false);

    /// Driver API
    Results parse(StringViewType query_string);

private:
    using ContinuationState = ParsingSession::ContinuationState;

    decltype(auto) create_parser(ParsingSession& session, StringViewType query_string);

    void scan_begin(StringViewType query_string);
    void scan_end();

    bool debug_mode_ {};
    std::optional<ContinuationState> continuation_state_ {std::nullopt};
};

}
