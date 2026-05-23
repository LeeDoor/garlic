#pragma once
#include "cell_type.hpp"
#include "type_rules.hpp"
#include "expression.hpp"

namespace garlic {

struct Selector { 
    ColumnNameType column_name;
    sptr<Expression> ast;

    Selector() : column_name{ "" }, ast{ nullptr } {}
    explicit Selector(sptr<Expression> expression)
    : ast{ std::move(expression) }
    {
        std::stringstream ss; TypeRules::as_str(ss, ast->get_type());
        column_name = ss.str();
    }
    Selector(const ColumnNameType& column_name, sptr<Expression> expression)
    : column_name{ column_name }
    , ast{ std::move(expression) }
    {}
};

}
