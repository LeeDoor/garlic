#pragma once
#include "tables_header_gatherer.hpp"

namespace garlic {

class TablesGathererMock {
public:
    explicit TablesGathererMock(ExpectedColumnType type_or_error)
    : type_or_error_{ std::move(type_or_error) }
    {}

    TablesGathererMock(CellType type)
    : type_or_error_{ type }
    {}

    TablesGathererMock(StringType error)
    : type_or_error_{ std::unexpected(std::move(error)) }
    {}

    ExpectedColumnType get_tables_column_type(TableNameType, ColumnNameType) const {
        return type_or_error_;
    }

private:
    ExpectedColumnType type_or_error_;
};

static_assert(TablesHeaderGatherer<TablesGathererMock>);

}
