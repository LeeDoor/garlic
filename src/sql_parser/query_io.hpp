#pragma once
#include "cell_type.hpp"

namespace garlic::sql_parser {

class QueryIO {
public:
    QueryIO() : is_{ std::cin } {}

    void reset();
    void readline();
    void should_be_shrinked(size_t n);
    void clear_query();

    StringViewType get_query() const;
    bool is_more_context_available() const;
    bool is_query_empty() const;
    
private:
    void print_prompt() const;

    std::istream& is_;
    bool more_ctx_available_ { false };
    StringType query_ {};
    StringType input_line_ {};
    size_t shrinked_characters_ {};
};

}
