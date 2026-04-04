#pragma once
#include "location.hh"
#include "cell_type.hpp"
#include "error_stage.hpp"

namespace garlic::sql_parser {

class QueryIO {
public:
    QueryIO() : is_{ std::cin } {}

    void reset();
    bool more_context_available() const;
    bool query_empty() const;
    void readline();
    void shrink_queries(int executed_queries);
    void print_error(ErrorStage err, const std::string& msg, const yy::location& err_loc) const;
    StringViewType get_query() const;
    
private:
    void print_prompt() const;

    bool more_ctx_available_ { false };

    std::istream& is_;

    std::string query_ {};
    std::string input_line_ {};
};

}
