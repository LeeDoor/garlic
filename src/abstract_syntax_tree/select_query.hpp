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

struct Table {
    TableNameType table_name;
};

class SelectQuery : public Query {
public:
    using ColumnsContainer = std::list<Selector>;
    using TablesContainer = std::list<Table>;
    SelectQuery();
    SelectQuery(ColumnsContainer columns);
    SelectQuery(ColumnsContainer columns, TablesContainer tables);

    ExpectedQueryResult resolve(sptr<CellValueGatherer> gatherer) override;

private:
    ColumnsContainer columns_ {};
    TablesContainer tables_ {};
};

}
