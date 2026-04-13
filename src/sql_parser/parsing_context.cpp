#include "parsing_context.hpp"

namespace garlic::sql_parser {

ParsingContext::ParsingContext(bool debug)
: debug_mode_{ debug }
{}

decltype(auto) ParsingContext::create_parser(ParsingSession& session, StringViewType query_string) {
    auto deleter = [this](yy::parser* p) {
	scan_end();
	delete p;
    };
    auto parser = std::unique_ptr<yy::parser, decltype(deleter)>(new yy::parser(session), std::move(deleter));
    parser->set_debug_level(debug_mode_);
    scan_begin(query_string);
    return parser;
}

ParsingContext::ParsingResults ParsingContext::parse(StringViewType query_string) {
    ParsingSession session = [this] {
	if(continuation_state_)
	    return ParsingSession{ *continuation_state_ };
	return ParsingSession{ };
    }();

    auto parser = create_parser(session, query_string);
    (*parser)();
    auto [results, continuation_state] = session.get_parsing_result();
    continuation_state_ = std::move(continuation_state);
    return results;
}

}
