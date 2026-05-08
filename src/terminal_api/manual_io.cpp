#include "manual_io.hpp"

#ifdef _WIN32
static bool IS_MANUAL_IO = _isatty(_fileno(stdin)) && _isatty(_fileno(stdout));
#else
static bool IS_MANUAL_IO = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
#endif

static constexpr std::string ACCENT_BASH_COLOR = "\033[7m";
static constexpr std::string BLEND_BASH_COLOR = "\033[0m";
static constexpr std::string RESET_BASH_COLOR = "\033[0m";

bool is_manual_IO() { return IS_MANUAL_IO; }
std::string_view blend_bash_color() {
    if(is_manual_IO())
	return BLEND_BASH_COLOR;
    return "";
}
std::string_view accent_bash_color() {
    if(is_manual_IO())
	return ACCENT_BASH_COLOR;
    return "";
}
std::string_view reset_bash_color() {
    if(is_manual_IO())
	return RESET_BASH_COLOR;
    return "";
}

#ifndef NDEBUG
void set_manual_IO(bool value) { IS_MANUAL_IO = value; }
#endif
