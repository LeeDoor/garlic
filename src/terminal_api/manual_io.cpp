#include "manual_io.hpp"

namespace garlic::terminal_api {

#ifdef _WIN32
static bool IS_MANUAL_IO = _isatty(_fileno(stdin)) && _isatty(_fileno(stdout));
#else
static bool IS_MANUAL_IO = isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
#endif

inline constexpr const char ERROR_HIGHLIGHT_BASH_COLOR[] = "\033[38;5;9m";
inline constexpr const char ACCENT_BASH_COLOR[] = "\033[7m";
inline constexpr const char BLEND_BASH_COLOR[] = "\033[0m";
inline constexpr const char RESET_BASH_COLOR[] = "\033[0m";

bool is_manual_IO() { return IS_MANUAL_IO; }

std::string_view error_highlight_bash_color() {
	if (is_manual_IO())
		return ERROR_HIGHLIGHT_BASH_COLOR;
	return "";
}

std::string_view blend_bash_color() {
	if (is_manual_IO())
		return BLEND_BASH_COLOR;
	return "";
}
std::string_view accent_bash_color() {
	if (is_manual_IO())
		return ACCENT_BASH_COLOR;
	return "";
}
std::string_view reset_bash_color() {
	if (is_manual_IO())
		return RESET_BASH_COLOR;
	return "";
}

#ifndef NDEBUG
void set_manual_IO(bool value) { IS_MANUAL_IO = value; }
#endif

} // namespace garlic::terminal_api
