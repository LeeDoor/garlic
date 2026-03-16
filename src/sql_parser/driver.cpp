#include "driver.hpp"
#include "parser.tab.hpp"

namespace garlic::sql_parser {

driver::driver(bool debug_mode)
: debug_mode_{ debug_mode }
{}

int driver::parse () {
    yy::parser parse (*this);
    parse.set_debug_level (debug_mode_);
    scan_begin();
    int res = -1;
    try {
        res = parse ();
    } catch (const std::runtime_error& ex) {
        std::cerr << ex.what() << std::endl;
    }
    scan_end ();
    return res;
}

}
