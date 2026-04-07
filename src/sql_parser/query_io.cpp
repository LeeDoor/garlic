#include "query_io.hpp"
#include "cell_type.hpp"
#include "manual_io.hpp"

namespace garlic::sql_parser {

void QueryIO::reset() {
    query_.clear();
    input_line_.clear();
    more_ctx_available_ = true;
}
bool QueryIO::is_more_context_available() const {
    return more_ctx_available_;
}
bool QueryIO::is_query_empty() const {
    return query_.empty();
}

void QueryIO::readline() {
    if(!is_more_context_available()) return;
    print_prompt();
    std::getline(is_, input_line_);
    more_ctx_available_ = !is_.eof(); 
    query_ += input_line_;
    query_ += "\n";
}
void QueryIO::shrink_to_last_query() {
    auto rightmost_semicolon = query_.rfind(';');
    if(rightmost_semicolon == StringType::npos)
	rightmost_semicolon = 0;
    query_.erase(0, rightmost_semicolon);
}
void QueryIO::clear_query() {
    query_.clear();
}
StringViewType QueryIO::get_query() const {
    return query_;
}
void QueryIO::print_prompt() const {
    if(is_manual_IO())
	std::cout << "#> " << std::flush;
}

}
