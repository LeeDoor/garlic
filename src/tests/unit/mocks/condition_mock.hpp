#pragma once
#include "condition.hpp"

namespace garlic {

class ConditionMock : public Condition {
public:
    ConditionMock(bool val) : Condition{ Boolean } {
        EXPECT_CALL(*this, resolve(::testing::_))
            .WillRepeatedly(Return(ExpectedBoolean{ val }));
    }

    MOCK_METHOD(ExpectedBoolean, resolve, (sptr<TableValueGatherer> gatherer), (const, override));
    MOCK_METHOD(std::optional<StringType>, validate, (), (const, override));
};

}
