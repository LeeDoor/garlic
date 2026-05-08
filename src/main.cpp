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
	    PublicColumnInfo{ String, "name", 16 },
	    PublicColumnInfo{ Int, "age", 0 },
	    PublicColumnInfo{ String, "favorite_food", 16 }
	});
    for(int i = 0; i < 7; ++i)
	table->create_empty_row();
    table->set_value(0, 0, "Alice");
    table->set_value(0, 1, 24);
    table->set_value(0, 2, "pizza");
    table->set_value(1, 0, "Bob");
    table->set_value(1, 1, 31);
    table->set_value(1, 2, "ramen");
    table->set_value(2, 0, "Clara");
    table->set_value(2, 1, 27);
    table->set_value(2, 2, "salad");
    table->set_value(3, 0, "Dmitry");
    table->set_value(3, 1, 35);
    table->set_value(3, 2, "burger");
    table->set_value(4, 0, "Eva");
    table->set_value(4, 1, 29);
    table->set_value(4, 2, "sushi");
    table->set_value(5, 0, "Farid");
    table->set_value(5, 1, 41);
    table->set_value(5, 2, "pasta");
    table->set_value(6, 0, "Greta");
    table->set_value(6, 1, 22);
    table->set_value(6, 2, "tacos");
    return table;
}

sptr<TypedTable> create_table_offices() {
    auto table = std::make_shared<TypedTable>( 
	std::initializer_list<PublicColumnInfo>{ 
	    PublicColumnInfo{ String, "city", 16 },
	    PublicColumnInfo{ Int, "floor", 0 },
	    PublicColumnInfo{ String, "team", 16 }
	});
    for(int i = 0; i < 7; ++i)
	table->create_empty_row();
    table->set_value(0, 0, "Moscow");
    table->set_value(0, 1, 3);
    table->set_value(0, 2, "backend");
    table->set_value(1, 0, "Berlin");
    table->set_value(1, 1, 5);
    table->set_value(1, 2, "frontend");
    table->set_value(2, 0, "Tokyo");
    table->set_value(2, 1, 8);
    table->set_value(2, 2, "research");
    table->set_value(3, 0, "Paris");
    table->set_value(3, 1, 2);
    table->set_value(3, 2, "hr");
    table->set_value(4, 0, "Lisbon");
    table->set_value(4, 1, 6);
    table->set_value(4, 2, "infra");
    table->set_value(5, 0, "Prague");
    table->set_value(5, 1, 4);
    table->set_value(5, 2, "qa");
    table->set_value(6, 0, "Tbilisi");
    table->set_value(6, 1, 7);
    table->set_value(6, 2, "support");
    return table;
}

sptr<TypedTable> create_table_foods() {
    auto table = std::make_shared<TypedTable>( 
	std::initializer_list<PublicColumnInfo>{ 
	    PublicColumnInfo{ String, "name", 16 },
	    PublicColumnInfo{ Int, "calories", 0 },
	    PublicColumnInfo{ String, "kind", 16 }
	});
    for(int i = 0; i < 7; ++i)
	table->create_empty_row();
    table->set_value(0, 0, "pizza");
    table->set_value(0, 1, 285);
    table->set_value(0, 2, "fastfood");
    table->set_value(1, 0, "ramen");
    table->set_value(1, 1, 436);
    table->set_value(1, 2, "soup");
    table->set_value(2, 0, "salad");
    table->set_value(2, 1, 120);
    table->set_value(2, 2, "veggie");
    table->set_value(3, 0, "burger");
    table->set_value(3, 1, 295);
    table->set_value(3, 2, "fastfood");
    table->set_value(4, 0, "sushi");
    table->set_value(4, 1, 200);
    table->set_value(4, 2, "seafood");
    table->set_value(5, 0, "pasta");
    table->set_value(5, 1, 260);
    table->set_value(5, 2, "grain");
    table->set_value(6, 0, "tacos");
    table->set_value(6, 1, 226);
    table->set_value(6, 2, "street");
    return table;
}

int main (int argc, char** argv) {
    bool debug_mode = false;
    handle_args(argc, argv, debug_mode);
    ErrorPrinter err_p;
    Database database {
	{ 
	    { "users", create_table_users() },
	    { "offices", create_table_offices() },
	    { "foods", create_table_foods() }
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
