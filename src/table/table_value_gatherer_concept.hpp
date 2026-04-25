#pragma once
#include "cell_type.hpp"

namespace garlic {

namespace details {

template<typename ColumnType>
using GatheredValueType = std::conditional_t<
    std::same_as<ColumnType, StringType>,
    StringViewType,
    ColumnType
>;

template<typename TableValueGathererT, typename StoringColumnTypeT>
concept TableValueGathererSingle = requires (TableValueGathererT table_value_gatherer) {
    requires IsStoringColumnType<StoringColumnTypeT>;
    { table_value_gatherer.template get_value<StoringColumnTypeT>(size_t{}, size_t{}) } 
	-> std::convertible_to<GatheredValueType<StoringColumnTypeT>>;
};

template<typename TableValueGathererT, typename StoringColumnT>
struct TableValueGathererAllV : 
    std::bool_constant<(TableValueGathererSingle<TableValueGathererT, StoringColumnT>)> {
	static_assert(TableValueGathererSingle<TableValueGathererT, StoringColumnT>);
    };

template<typename TableValueGathererT, typename StoringColumnTypesT>
struct TableValueGathererAll;

template<typename TableValueGathererT, typename... StoringColumnTypesT>
struct TableValueGathererAll<TableValueGathererT, TypeList<StoringColumnTypesT...>> : 
    std::bool_constant<(TableValueGathererAllV<TableValueGathererT, StoringColumnTypesT>::value && ...)> {};

} 

template<typename TableValueGathererT>
concept TableValueGathererConcept = details::TableValueGathererAll<TableValueGathererT, StoringColumnTypes>::value;


}
