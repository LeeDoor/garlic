#pragma once
#include "cell_type.hpp"
#include "cell_value_gatherer.hpp"

namespace garlic {
    
using ExpectedCellValueGatherer = ExpectedOrStr<sptr<CellValueGatherer>>;

template<typename CellValueGathererFactoryT>
concept CellValueGathererFactoryImpl = requires(CellValueGathererFactoryT cell_value_gatherer_factory) {
    { cell_value_gatherer_factory.build_cell_value_gatherer(TableNameType{}) } 
	-> std::convertible_to<ExpectedCellValueGatherer>;
};

}
