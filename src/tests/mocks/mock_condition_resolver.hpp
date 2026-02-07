#pragma once
#include "condition_resolver.hpp"

namespace garlic {

class MockConditionResolver : public ConditionResolver {
public:
    MOCK_METHOD(IntType, get_int_value, (size_t column), (const, override));
    MOCK_METHOD(FloatType, get_float_value, (size_t column), (const, override));
    MOCK_METHOD(StringViewType, get_string_value, (size_t column), (const, override));
    MOCK_METHOD(void, set_row_id, (size_t new_row_id), (override));
};

}
