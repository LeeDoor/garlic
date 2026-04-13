#include "query_input.hpp"
#include "cell_type.hpp"
#include "manual_io.hpp"

namespace garlic::sql_parser {

bool QueryInput::is_more_context_available() const {
    return more_ctx_available_;
}
bool QueryInput::is_query_empty() const {
    return query_.empty();
}

void QueryInput::readline() {
    if(!is_more_context_available()) return;
    print_prompt();
    std::getline(is_, input_line_);
    more_ctx_available_ = !is_.eof(); 
    query_ += input_line_;
    query_ += "\n";
}
void QueryInput::shrink_n_characters(size_t n) {
    if(query_.size() < n) 
	throw std::logic_error("Shrinking " + std::to_string(n) + " chars is out of query range.");

    query_.erase(0, n);
}
void QueryInput::clear_query() {
    query_.clear();
}
StringViewType QueryInput::get_query() const {
    return query_;
}
void QueryInput::print_prompt() const {
    if(is_manual_IO())
	std::cout << "#> " << std::flush;
}

}
