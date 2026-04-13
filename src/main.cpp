#include "sql_repl.hpp"

int main (int argc, char** ) {
    bool debug_mode = argc >= 2;
    garlic::sql_parser::SqlRepl drv(debug_mode);
    try {
	drv.parse();
    } catch (const std::logic_error& ex) {
	std::cout << "[LOGIC_ERROR] " << ex.what() << std::endl;
	return 1;
    }
}

