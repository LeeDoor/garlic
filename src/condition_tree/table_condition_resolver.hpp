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

    IntType get_int_value(size_t column) const override {
        return get_cell_value<IntType>(column);   
    }
    FloatType get_float_value(size_t column) const override { 
        return get_cell_value<FloatType>(column);   
    }
    StringViewType get_string_value(size_t column) const override { 
        return get_cell_value<StringViewType>(column);   
    }
    void set_row_id(size_t new_row_id) override { row_id_ = new_row_id; }

private:
    template<IsReadonlyColumnType T>
    T get_cell_value(size_t column) const {
        return table_->get_value<T>(row_id_, column);
    }

    TablePtr table_;
    size_t row_id_;
};

}
