#pragma once
#include "table_value_gatherer.hpp"

namespace garlic {

class TableValueGathererMock : public TableValueGatherer {
public:
    MOCK_METHOD(sptr<CellValue>, get_table_value, (const std::string& column_name), (override));

};

}
