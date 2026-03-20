#pragma once
#include "query_result.hpp"

namespace garlic {

class StringQueryResult : public QueryResult {
public:


    StringQueryResult() = default;
    template<IsAnyColumnType T>
    StringQueryResult(const T& res)
    : result_str_{ form_string(res) }
    {}

    StringViewType format() const override;

private:
    template<IsAnyColumnType T>
    static std::string form_string(const T& res) {
	if constexpr(std::is_same_v<T, StringType>)
	    return res;
	else return std::to_string(res);
    }

    std::string result_str_;
};

}
