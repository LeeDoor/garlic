#pragma once
#include "cell_float_value.hpp"
#include "cell_int_value.hpp"
#include "cell_string_value.hpp"
#include "expression.hpp"

namespace garlic {

class TableValueExpression : public Expression {
public:
    TableValueExpression(CellType type, ResolverPtr resolver, std::string column_name)
    : Expression( type, resolver )
    , column_name_{ column_name }
    {} 

    CellValuePtr get_value() const override {
        switch(get_type()) {
        case Int:
            return std::make_shared<CellIntValue>(
                    resolver_->get_int_value(column_name_)
                );
        case Float:
            return std::make_shared<CellFloatValue>(
                    resolver_->get_float_value(column_name_)
                );
        case String:
            return std::make_shared<CellStringValue>(
                    resolver_->get_string_value(column_name_)
                );
        }
    }

private:
    std::string column_name_;
};

}
