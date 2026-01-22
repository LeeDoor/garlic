#include "table.hpp"

int main() {
    using namespace garlic;
    Table table {
        { Int, "Id", 0 },
        { String, "Name", 10 }
    };

    table.create_empty_row();
    table.set_value(0, 0, INT_MAX + 1);
    table.set_value(0, 1, "Ivanov");
    std::cout << "[0;0]: " << table.get_value<int>(0, 0) << std::endl;
    std::cout << "[0;1]: " << table.get_value<std::string>(0, 1) << std::endl;

}
