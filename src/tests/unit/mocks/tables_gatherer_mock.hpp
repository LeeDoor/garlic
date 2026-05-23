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
    ExpectedOrStr<std::vector<ColumnInfo>> get_tables_header(const TableNameType&) const {
        if(!type_or_error_)
            return std::unexpected(type_or_error_.error());
        return std::vector<ColumnInfo>{
            ColumnInfo{ *type_or_error_, "column", 0, 0 }
        };
    }

private:
    ExpectedColumnType type_or_error_;
};

static_assert(TablesHeaderGathererImpl<TablesGathererMock>);

}
