#pragma once
#include "cell_boolean_value.hpp"
#include "condition.hpp"

namespace garlic {

class ConditionMock : public Condition {
public:
    ConditionMock(bool val, UsedTables used_tables = {}) 
    : Condition{ Boolean }
    , used_tables_{ std::move(used_tables) } {
        EXPECT_CALL(*this, resolve_bool(::testing::_))
            .WillRepeatedly(Return(std::make_shared<CellBooleanValue>(val)));
        EXPECT_CALL(*this, get_used_tables())
            .WillRepeatedly(Return(used_tables_));
    }

    MOCK_METHOD(ExpectedCellBooleanValue, resolve_bool, (const TablesGathered& gatherers), (const, override));
    MOCK_METHOD(UsedTables, get_used_tables, (), (const, override));

private:
    UsedTables used_tables_;
};

}
