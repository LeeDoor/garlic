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

sptr<TypedTable> create_table_users() {
    auto table = std::make_shared<TypedTable>( 
	std::initializer_list<PublicColumnInfo>{ 
	    PublicColumnInfo{ String, "name", 10 },
	    PublicColumnInfo{ Int, "age", 0 } 
	});
    table->create_empty_row();
    table->create_empty_row();
    table->set_value(0, 0, "Eblan");
    table->set_value(0, 1, 12);
    table->set_value(1, 0, "SecondMan");
    table->set_value(1, 1, 22222);
    return table;
}

int main (int argc, char** argv) {
    bool debug_mode = false;
    handle_args(argc, argv, debug_mode);
    ErrorPrinter err_p;
    Database database {
	{ 
	    { "users", create_table_users() }
	}
    };
    garlic::sql_parser::SqlRepl drv(
	    ParserEngine{ database, debug_mode },
	    QueryInput {}, 
	    err_p, 
	    SqlAstExecutor { err_p, database });
    try {
	drv.run();
    } catch (const std::logic_error& ex) {
	std::cout << "[LOGIC_ERROR] " << ex.what() << std::endl;
	return 1;
    }
}
