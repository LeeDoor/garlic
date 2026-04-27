#pragma once
#include "cell_type.hpp"

namespace garlic {

namespace details {

template<typename TableValueGathererT, typename ResultingColumnTypeT>
concept TableValueGathererSingle = requires (TableValueGathererT table_value_gatherer) {
    { table_value_gatherer.template get_value<ResultingColumnTypeT>(size_t{}, size_t{}) } 
	-> std::convertible_to<ResultingColumnTypeT>;
};

template<typename TableValueGathererT, typename ResultingColumnT>
struct TableValueGathererAllV : 
    std::bool_constant<(TableValueGathererSingle<TableValueGathererT, ResultingColumnT>)> {
	static_assert(TableValueGathererSingle<TableValueGathererT, ResultingColumnT>);
    };

template<typename TableValueGathererT, typename ResultingColumnTypesT>
struct TableValueGathererAll;

template<typename TableValueGathererT, typename... ResultingColumnTypesT>
struct TableValueGathererAll<TableValueGathererT, TypeList<ResultingColumnTypesT...>> : 
    std::bool_constant<(TableValueGathererAllV<TableValueGathererT, ResultingColumnTypesT>::value && ...)> {};

} 

template<typename TableValueGathererT>
concept TableValueGatherer = details::TableValueGathererAll<TableValueGathererT, ReadonlyColumnTypes>::value;


}
