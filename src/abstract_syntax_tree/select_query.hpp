#pragma once
#include "query.hpp"
#include "expression.hpp"

namespace garlic {

struct Selector { 
    ColumnNameType column_name;
    uptr<Expression> content;

    Selector() : column_name{ "" }, content{ nullptr } {}
    explicit Selector(uptr<Expression> expression)
    : content{ std::move(expression) }
    {
	std::stringstream ss; TypeRules::as_str(ss, content->get_type());
	column_name = ss.str();
    }
    Selector(const ColumnNameType& column_name, uptr<Expression> expression)
    : column_name{ column_name }
    , content{ std::move(expression) }
    {}
};


class SelectQuery : public Query {
public:
    using ColumnsContainer = std::list<Selector>;

    SelectQuery();
    SelectQuery(ColumnsContainer columns);

    ExpectedQueryResult resolve(sptr<TableValueGatherer> gatherer) override;

private:
    ColumnsContainer columns_ {};
};

}
