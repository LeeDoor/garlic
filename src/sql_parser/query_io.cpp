#include "query_io.hpp"
#include "cell_type.hpp"
#include "manual_io.hpp"

namespace garlic::sql_parser {

void QueryIO::reset() {
    shrinked_characters_ = 0;
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
void QueryIO::should_be_shrinked(size_t n) {
    if(n < shrinked_characters_)
	throw std::logic_error("Shrinking " + std::to_string(n) + " chars is less than shrinked before.");

    n -= shrinked_characters_;
    if(query_.size() < n) 
	throw std::logic_error("Shrinking " + std::to_string(n) + " chars is out of query range.");

    query_.erase(0, n);
    shrinked_characters_ += n;
}
void QueryIO::clear_query() {
    shrinked_characters_ += query_.size();
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
