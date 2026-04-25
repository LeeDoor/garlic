#pragma once
#include "cell_type.hpp"
#include "cell_value_gatherer.hpp"

namespace garlic {
    
using ExpectedTableValueGatherer = std::expected<sptr<CellValueGatherer>, StringType>;

template<typename TableValueGathererFactoryT>
concept TableValueGathererFactoryImpl = requires(TableValueGathererFactoryT table_value_gatherer_factory) {
    { table_value_gatherer_factory.build_table_value_gatherer(TableNameType{}) } 
	-> std::convertible_to<ExpectedTableValueGatherer>;
};

}
