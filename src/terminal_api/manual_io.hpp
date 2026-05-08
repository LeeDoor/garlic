#pragma once

/// Returns true if both input and output streams are assotiated with the terminal.
bool is_manual_IO();
std::string_view blend_bash_color();
std::string_view accent_bash_color();
std::string_view reset_bash_color();

#ifndef NDEBUG
/// Makes the program think that it is either in terminal or in file. needed for debug reasons.
void set_manual_IO(bool value = true);
#endif
