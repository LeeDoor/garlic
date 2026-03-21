#pragma once

namespace garlic {
class CellValue;

class TableValueGatherer {
public:
    virtual ~TableValueGatherer() = default;

    virtual sptr<CellValue> get_table_value(const std::string& column_name) = 0;
};

}
