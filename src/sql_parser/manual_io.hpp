#pragma once

#ifdef _WIN32
inline bool is_manual_IO() {
    return _isatty(_fileno(stdin)) && _isatty(_fileno(stdout));
}
#else
inline bool is_manual_IO() {
    return isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
}
#endif
