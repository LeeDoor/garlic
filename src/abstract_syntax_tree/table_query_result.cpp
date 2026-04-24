#include "table_query_result.hpp"

namespace garlic {

TableQueryResult::TableQueryResult(Table&& table) 
: table_result_{ form_table_result(std::move(table)) }
{}

StringType TableQueryResult::form_table_result(Table&& table) {
    StringType result_str;
    bool is_first = true;
    for(const Row& row : table) {
	for(const StringType& cell : row) {
	    if(!is_first) {
		result_str += "\t";
	    } is_first = false;
	    result_str += cell;
	}
	result_str += "\n";
	is_first = true;
    }
    result_str += "\n";
    return result_str;
}

StringViewType TableQueryResult::format() const {
    return table_result_;
}

}
