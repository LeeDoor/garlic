#include "dumb_table_value_gatherer.hpp"
#include "manual_io.hpp"
#include "sql_repl.hpp"

void handle_args(int argc, char** argv, bool& debug_mode) {
    for(int i = 1; i < argc; ++i) {
	char* arg = argv[i];
	if(std::strncmp("--use-cli-output", arg, 17) == 0) {
	    set_manual_IO();
	}
#ifndef NDEBUG
	else if(std::strncmp("--debug", arg, 8) == 0) {
	    debug_mode = true;
	}
#endif
	else {
	    std::cout << "Unexpected cli argument: " << arg << std::endl;
	    abort();
	}
    }
}

int main (int argc, char** argv) {
    bool debug_mode = false;
    handle_args(argc, argv, debug_mode);
    ErrorPrinter err_p;
    Database database {
	{ 
	    {
		"users", std::make_shared<TypedTable>( std::initializer_list<PublicColumnInfo>{ 
		    PublicColumnInfo{ String, "name", 10 },
		    PublicColumnInfo{ Int, "age", 0 } 
		})
	    },
	    {
		"houses", std::make_shared<TypedTable>( std::initializer_list<PublicColumnInfo>{ 
		    PublicColumnInfo{ String, "address", 10 },
		    PublicColumnInfo{ Int, "housenumber", 0 } 
		})
	    }

	}
    };
    garlic::sql_parser::SqlRepl drv(
	    debug_mode, 
	    database,
	    QueryInput {}, 
	    err_p, 
	    SqlAstExecutor { err_p, std::make_shared<DumbTableValueGatherer>() });
    try {
	drv.run();
    } catch (const std::logic_error& ex) {
	std::cout << "[LOGIC_ERROR] " << ex.what() << std::endl;
	return 1;
    }
}

