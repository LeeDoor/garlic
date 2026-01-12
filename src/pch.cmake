add_library(pch_std INTERFACE)
target_precompile_headers(pch_std INTERFACE
    headers/std.hpp
)
