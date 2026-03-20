#include "unary_logical_condition.hpp"
#include "condition_mock.hpp"
#include "table_value_gatherer_mock.hpp"

namespace garlic {
std::ostream& operator<<(std::ostream& os, UnaryLogicalOperator op) {
    switch(op) {
        case IsTrue:
            os << "Is true";
            break;
        case IsFalse:
            os << "Is false";
            break;
    };
    return os;
}

class TestUnaryLogicalCondition : public ::testing::Test {
protected:
    static const int TABLE_SIZE = 2;
    sptr<testing::StrictMock<TableValueGathererMock>> gatherer
        = std::make_shared<testing::StrictMock<TableValueGathererMock>>();

    uptr<UnaryLogicalCondition> create(bool condition, UnaryLogicalOperator op) {
        return std::make_unique<UnaryLogicalCondition>(
            std::make_unique<ConditionMock>(condition),
            op
        );
    }

    void matches_truth_table(std::array<bool, TABLE_SIZE> table_expected, UnaryLogicalOperator op) {
        constexpr std::array<bool, TABLE_SIZE> table_inp {
             0, 1,
        };
        std::array<bool, TABLE_SIZE> actual{0};
        bool failed = false;
        for(int i = 0; i < TABLE_SIZE; ++i) {
            auto& inp = table_inp[i];
            auto cond = create(inp, op);
            actual[i] = cond->resolve(gatherer);
            failed |= actual[i] != table_expected[i];
        }
        if(failed) {
            std::cerr 
                << "Failed to match truth tables for operator " << op << std::endl
                << "x E A" << std::endl;
            for(int i = 0; i < TABLE_SIZE; ++i) {
                std::cerr << std::noboolalpha 
                    << table_inp[i] << ' '
                    << table_expected[i] << ' ' << actual[i] << std::endl;
            }
            FAIL();
        }
    }
};

TEST_F(TestUnaryLogicalCondition, init) {
    auto cond = create(true, IsTrue);
}

TEST_F(TestUnaryLogicalCondition, Operators) {
    matches_truth_table({0, 1}, IsTrue);
    matches_truth_table({1, 0}, IsFalse);
}

}
