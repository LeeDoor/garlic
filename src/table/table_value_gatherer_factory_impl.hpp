#pragma once
#include "cell_type.hpp"
#include "cell_value_gatherer.hpp"

namespace garlic {
    
using ExpectedCellValueGatherer = std::expected<sptr<CellValueGatherer>, StringType>;

template<typename TableValueGathererFactoryT>
concept TableValueGathererFactoryImpl = requires(TableValueGathererFactoryT table_value_gatherer_factory) {
    { table_value_gatherer_factory.build_cell_value_gatherer(TableNameType{}) } 
	-> std::convertible_to<ExpectedCellValueGatherer>;
};

}
