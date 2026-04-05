#pragma once
#include <bits/stdc++.h>
namespace garlic {

using Byte = unsigned char;
using ByteVector = std::vector<Byte>;
using ByteSpan = std::span<const Byte>;

template<typename T, typename Deleter = std::default_delete<T>>
using uptr = std::unique_ptr<T, Deleter>;

template<typename T>
using sptr = std::shared_ptr<T>;

}
