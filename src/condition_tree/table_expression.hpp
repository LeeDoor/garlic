#pragma once
#include "cell_int_value.hpp"
#include "expression.hpp"

namespace garlic {

class TableExpression : public Expression {
public:
    TableExpression(CellType type, ResolverPtr resolver, std::string column_name)
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
            return resolver_->get_float_value(column_name_);
        case String:
            return resolver_->get_string_value(column_name_);
        }
    }

private:
    std::string column_name_;
};

}
