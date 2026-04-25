#pragma once
#include "cell_boolean_value.hpp"
#include "condition.hpp"

namespace garlic {

class ConditionMock : public Condition {
public:
    ConditionMock(bool val) : Condition{ Boolean } {
        EXPECT_CALL(*this, resolve_bool(::testing::_))
            .WillRepeatedly(Return(std::make_shared<CellBooleanValue>(val)));
    }

    MOCK_METHOD(ExpectedCellBooleanValue, resolve_bool, (sptr<CellValueGatherer> gatherer), (const, override));
};

}
