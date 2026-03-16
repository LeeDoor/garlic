#include "driver.hpp"

int main (int argc, char** ) {
    bool debug_mode = argc >= 2;
    garlic::sql_parser::driver drv(debug_mode);
    return drv.parse();
}

