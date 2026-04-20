#pragma once
#include "query.hpp"
#include "expression.hpp"

namespace garlic {

struct SelectColumn { 
    ColumnNameType column_name;
    sptr<Expression> content;

    explicit SelectColumn(sptr<Expression> expression)
    : content{ expression }
    {
	std::stringstream ss; TypeRules::as_str(ss, content->get_type());
	column_name = ss.str();
    }
};


class SelectQuery : public Query {
public:
    using ColumnsContainer = std::list<SelectColumn>;

    SelectQuery();
    SelectQuery(ColumnsContainer columns);

    void append_column(sptr<Expression> column_expression);

    sptr<QueryResult> resolve(sptr<TableValueGatherer> gatherer) override;

private:
    ColumnsContainer columns_ {};
};

}
