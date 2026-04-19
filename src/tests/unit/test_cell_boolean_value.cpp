#include "cell_boolean_value.hpp"
#include "cell_type.hpp"

namespace garlic {

TEST(test_cell_boolean_value, initializationAndAccessors) {
    CellBooleanValue t(true);
    CellBooleanValue f(false);

    EXPECT_EQ(t.get_type(), CellType::Boolean);
    EXPECT_EQ(f.get_type(), CellType::Boolean);
    EXPECT_TRUE(t.get_bool());
    EXPECT_FALSE(f.get_bool());
    EXPECT_TRUE(static_cast<bool>(t));
    EXPECT_FALSE(static_cast<bool>(f));
}

TEST(test_cell_boolean_value, conjunctionTruthTable) {
    auto t = std::make_shared<CellBooleanValue>(true);
    auto f = std::make_shared<CellBooleanValue>(false);

    EXPECT_FALSE(f->conjunction(f));
    EXPECT_FALSE(f->conjunction(t));
    EXPECT_FALSE(t->conjunction(f));
    EXPECT_TRUE(t->conjunction(t));
}

TEST(test_cell_boolean_value, disjunctionTruthTable) {
    auto t = std::make_shared<CellBooleanValue>(true);
    auto f = std::make_shared<CellBooleanValue>(false);

    EXPECT_FALSE(f->disjunction(f));
    EXPECT_TRUE(f->disjunction(t));
    EXPECT_TRUE(t->disjunction(f));
    EXPECT_TRUE(t->disjunction(t));
}

TEST(test_cell_boolean_value, equivalenceTruthTable) {
    auto t = std::make_shared<CellBooleanValue>(true);
    auto f = std::make_shared<CellBooleanValue>(false);

    EXPECT_TRUE(f->equivalence(f));
    EXPECT_FALSE(f->equivalence(t));
    EXPECT_FALSE(t->equivalence(f));
    EXPECT_TRUE(t->equivalence(t));
}

TEST(test_cell_boolean_value, implicationTruthTable) {
    auto t = std::make_shared<CellBooleanValue>(true);
    auto f = std::make_shared<CellBooleanValue>(false);

    EXPECT_TRUE(f->implication(f));
    EXPECT_TRUE(f->implication(t));
    EXPECT_FALSE(t->implication(f));
    EXPECT_TRUE(t->implication(t));
}

TEST(test_cell_boolean_value, exclusiveOrTruthTable) {
    auto t = std::make_shared<CellBooleanValue>(true);
    auto f = std::make_shared<CellBooleanValue>(false);

    EXPECT_FALSE(f->exclusiveor(f));
    EXPECT_TRUE(f->exclusiveor(t));
    EXPECT_TRUE(t->exclusiveor(f));
    EXPECT_FALSE(t->exclusiveor(t));
}

}
