#include "query_io.hpp"
#include "cell_type.hpp"
#include "manual_io.hpp"

namespace garlic {

void QueryIO::reset() {
    query_.clear();
    input_line_.clear();
    more_ctx_available_ = true;
}
bool QueryIO::more_context_available() const {
    return more_ctx_available_;
}
bool QueryIO::query_empty() const {
    return query_.empty();
}

void QueryIO::readline() {
    if(!more_context_available()) return;
    print_prompt();
    std::getline(is_, input_line_);
    more_ctx_available_ = !is_.eof(); 
    query_ += input_line_;
    query_ += "\n";
}
void QueryIO::shrink_queries(int n) {
    StringType::size_type remove_until = 0;
    for(int i = 0; i < n; ++i) {
	remove_until = query_.find(';', remove_until);
	if(remove_until == StringType::npos)
	    return query_.clear();
	++remove_until;
    }
    query_.erase(0, remove_until);
}
std::tuple<const char*, int> QueryIO::get_query() const {
    return { query_.data(), query_.size() };
}
void QueryIO::print_error(ErrorStage err, const std::string& msg, const yy::location& error_location) const {
    static const std::unordered_map<ErrorStage, std::string> stage_str {
	{ Lexing, "LEXICAL_ERROR" },
	{ Parsing, "SYNTAX_ERROR" },
	{ SemanticAnalysis, "SEMANTIC_ERROR" },
    };
    os_	<< "[" << stage_str.at(err) << "] "
	<< "at [" << error_location << "] "
	<< msg << std::endl;
}
void QueryIO::print_prompt() const {
    if(is_manual_IO())
	std::cout << "#> " << std::flush;
}

}
