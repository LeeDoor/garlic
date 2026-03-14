#include <iostream>
#include "driver.hpp"

int main (int argc, char** ) {
    bool debug_mode = argc >= 2;
    driver drv(debug_mode);
    return drv.parse();
}

