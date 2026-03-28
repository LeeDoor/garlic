#include "driver.hpp"

int main (int argc, char** ) {
    bool debug_mode = argc >= 2;
    garlic::sql_parser::driver drv(debug_mode);
    try {
	return drv.parse();
    } catch (const std::logic_error& ex) {
	std::cout << "[LOGIC_ERROR] " << ex.what() << std::endl;
    }
}

