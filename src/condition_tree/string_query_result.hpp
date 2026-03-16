#pragma once
#include "query_result.hpp"

namespace garlic {

class StringQueryResult : public QueryResult {
public:
    using Ptr = std::shared_ptr<StringQueryResult>;
    virtual ~StringQueryResult() = default;

    template<IsAnyColumnType T>
    StringQueryResult(const T& res)
    : result_str_{ form_string(res) }
    {}
    StringQueryResult(){}

    StringType format() const override {
        return result_str_;   
    }
private:
    template<IsAnyColumnType T>
    static std::string form_string(const T& res) {
	std::stringstream ss;
	ss << res;
	return ss.str();
    }

    std::string result_str_;
};

}
