#pragma once
#include "cell_value_gatherer.hpp"

namespace garlic {

class CellValueGathererMock : public CellValueGatherer {
public:
    MOCK_METHOD(bool, is_table_empty, (), (const, override));
    MOCK_METHOD(sptr<CellValue>, get_table_value, (const std::string& column_name), (override));
    MOCK_METHOD(bool, jump_to_next_row, (), (override));

};

}
