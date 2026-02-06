#pragma once
#include "condition.hpp"

namespace garlic {

template<IConditionResolver Resolver>
class EqualsCondition : public Condition<Resolver> {
    using Parent = Condition<Resolver>;
public:
    EqualsCondition(Parent::ExpressionPtr lhs, Parent::ExpressionPtr rhs, Parent::ResolverPtr resolver)
    : expr_left_ { std::move(lhs) }
    , expr_right_{ std::move(rhs) }
    , resolver_  { resolver }
    {}

    bool resolve() override {
        CellType type = expr_left_->get_type();
        switch(type) {
        case String:
            return 0 == std::strncmp(
                    expr_left_->get_string(), 
                    expr_right_->get_string(), 
                    resolver_->get_cell_size());
        case Int:
            return expr_left_->get_int() == expr_right_->get_int();
        case Float:
            return expr_left_->get_float() == expr_right_->get_float();
          break;
        }
    }

private:
    Parent::ConditionResolver resolver_;
    Parent::ExpressionPtr expr_left_;
    Parent::ExpressionPtr expr_right_;
};

}
