#pragma once
#include "typed_table.hpp"
#include "condition_resolver.hpp"

namespace garlic {

class TableConditionResolver : public ConditionResolver {
    using TablePtr = std::shared_ptr<TypedTable>;
public:
    TableConditionResolver(TablePtr table, size_t init_row_id = 0)
    : table_{ table }
    , row_id_{ init_row_id }
    {}

    IntType get_int_value(ColumnType column) const override {
        return get_cell_value<IntType>(column);   
    }
    FloatType get_float_value(ColumnType column) const override { 
        return get_cell_value<FloatType>(column);   
    }
    StringViewType get_string_value(ColumnType column) const override { 
        return get_cell_value<StringViewType>(column);   
    }
    void set_row_id(size_t new_row_id) override { row_id_ = new_row_id; }

private:
    template<IsReadonlyColumnType T>
    T get_cell_value(ColumnType column) const {
        auto col_id = table_->get_column_number_by_name(column);
        return table_->get_value<T>(row_id_, col_id);
    }

    TablePtr table_;
    size_t row_id_;
};

}
