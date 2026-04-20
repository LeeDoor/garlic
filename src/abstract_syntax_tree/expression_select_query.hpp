#pragma once
#include "query.hpp"
#include "expression.hpp"

namespace garlic {

class ExpressionSelectQuery : public Query {
public:
    ExpressionSelectQuery();
    ExpressionSelectQuery(sptr<Expression> column_expression);

    void append_column(sptr<Expression> column_expression);

    sptr<QueryResult> resolve(sptr<TableValueGatherer> gatherer) override;

private:
    struct Column { 
	ColumnNameType column_name;
	sptr<Expression> content;
    };
    
    static Column create_column(sptr<Expression> column_content);

    using ColumnsContainer = std::list<Column>;
    ColumnsContainer columns_ {};
};

}
