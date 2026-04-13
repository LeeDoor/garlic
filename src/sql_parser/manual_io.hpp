#pragma once

/// Returns true if both input and output streams are assotiated with the terminal.
bool is_manual_IO();
#ifndef NDEBUG
/// Makes the program think that it is either in terminal or in file. needed for debug reasons.
void set_manual_IO(bool value = true);
#endif
