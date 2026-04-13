#include "manual_io.hpp"
#include "sql_repl.hpp"

void handle_args(int argc, char** argv) {
    for(int i = 0; i < argc; ++i) {
	char* arg = argv[i];
	if(std::strncmp("--use-cli-output", arg, 17)) {
	    set_manual_IO();
	}
    }
}

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

