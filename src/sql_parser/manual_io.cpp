#include "manual_io.hpp"

#ifdef _WIN32
static bool IS_MANUAL_IO = _isatty(_fileno(stdin)) && _isatty(_fileno(stdout));
#else
static bool IS_MANUAL_IO = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
#endif

bool is_manual_IO() { return IS_MANUAL_IO; }
void set_manual_IO(bool value) { IS_MANUAL_IO = value; }

