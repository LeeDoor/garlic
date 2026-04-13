#pragma once
#include "cell_type.hpp"

namespace garlic::sql_parser {

/// Manages user's input and output. provides an interface to modify the buffer.
class QueryIO {
public:
    QueryIO() : is_{ std::cin } {}

    /// TODO remove
    void reset();
    /// Reads one line to buffer
    void readline();
    /// Shrinks passed amount of characters
    void should_be_shrinked(size_t n);
    /// Clears inner string buffer.
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
