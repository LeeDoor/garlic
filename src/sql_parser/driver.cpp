#include "driver.hpp"
#include "parser.tab.hpp"

driver::driver(bool debug_mode)
: debug_mode_{ debug_mode }
{}

int driver::parse () {
    yy::parser parse (*this);
    parse.set_debug_level (debug_mode_);
    scan_begin();
    int res = parse ();
    scan_end ();
    return res;
}
