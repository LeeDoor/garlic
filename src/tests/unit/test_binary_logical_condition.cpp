#include "binary_logical_condition.hpp"
#include "cell_boolean_value.hpp"
#include "condition_mock.hpp"
#include "table_value_gatherer_mock.hpp"

namespace garlic {

static bool unwrap_bool(Condition::ExpectedCellBooleanValue result) {
    EXPECT_TRUE(result.has_value()) << result.error();
    if(!result) return false;
    return (*result)->get_bool();
}

std::ostream& operator<<(std::ostream& os, BinaryLogicalOperator op) {
    switch(op) {
        case And:
            os << "&&";
            break;
        case Or:
            os << "||";
            break;
        case Xor:
            os << "^";
            break;
        case Iff:
            os << "<=>";
            break;
        case Implication:
            os << "->";
            break;
    };
    return os;
}

class TestBinaryLogicalCondition : public ::testing::Test {
protected:
public:
protected:
    static const int TABLE_SIZE = 4;
    sptr<testing::StrictMock<TableValueGathererMock>> gatherer
        = std::make_shared<testing::StrictMock<TableValueGathererMock>>();
    uptr<BinaryLogicalCondition> create(bool lhs, bool rhs, BinaryLogicalOperator op) {
        return std::make_unique<BinaryLogicalCondition>(
            std::make_unique<ConditionMock>(lhs),
            std::make_unique<ConditionMock>(rhs),
            op
        );
    }

    void matches_truth_table(std::array<bool, TABLE_SIZE> table_expected, BinaryLogicalOperator op) {
        constexpr std::array<std::pair<bool, bool>, TABLE_SIZE> table_inp {
            std::pair<bool, bool>{ 0, 0 },
                std::pair<bool, bool>{ 0, 1 },
                std::pair<bool, bool>{ 1, 0 },
                std::pair<bool, bool>{ 1, 1 },
        };
        std::array<bool, TABLE_SIZE> actual{0};
        bool failed = false;
        for(int i = 0; i < TABLE_SIZE; ++i) {
            auto& inp = table_inp[i];
            auto cond = create(inp.first, inp.second, op);
            actual[i] = unwrap_bool(cond->resolve_bool(gatherer));
            failed |= actual[i] != table_expected[i];
        }
        if(failed) {
            std::cerr 
                << "Failed to match truth tables for operator " << op << std::endl
                << "x y E A" << std::endl;
            for(int i = 0; i < TABLE_SIZE; ++i) {
                std::cerr << std::noboolalpha 
                    << table_inp[i].first << ' ' << table_inp[i].second << ' '
                    << table_expected[i] << ' ' << actual[i] << std::endl;
            }
            FAIL();
        }
    }
};

TEST_F(TestBinaryLogicalCondition, init) {
    auto cond = create(true, false, Xor);
}

TEST_F(TestBinaryLogicalCondition, Operators) {
    matches_truth_table({0, 0, 0, 1}, And);
    matches_truth_table({0, 1, 1, 1}, Or);
    matches_truth_table({0, 1, 1, 0}, Xor);
    matches_truth_table({1, 0, 0, 1}, Iff);
    matches_truth_table({1, 1, 0, 1}, Implication);
}

}
