#pragma once
#include <chrono>
#include <climits>
#include <ostream>
namespace application {

namespace ch = std::chrono;

// template<typename T>
// concept TimeT =
// requires(T t) {
//     { ch::duration_cast<ch::nanoseconds>(t).count() } -> std::convertible_to<unsigned long long>;
//     { T{ULLONG_MAX} } -> std::convertible_to<T>;
// };
using TimeT = ch::duration<long, std::nano>;

class Benchmark {
    static inline decltype(auto) now() { return ch::high_resolution_clock::now(); }
public:
    template<typename FuncT>
    static inline decltype(auto) measure(FuncT&& func) {
        auto start = now();
        func();
        auto end = now();
        return Timestamp{ end - start };
    }

    struct Timestamp {
        TimeT duration;
        Timestamp(TimeT time_) : duration{time_} {}
    };
};

inline std::ostream& operator<< (std::ostream& os, const Benchmark::Timestamp& timestamp) {
    auto duration = timestamp.duration;
    int presizion = 2;
    auto l = [&](auto timeobj) -> bool {
        using TimeT = decltype(timeobj);
        if(auto dur_converted = ch::duration_cast<TimeT>(duration); dur_converted.count()) {
            os << dur_converted << ' ';
            --presizion;
            if(!presizion) return true;
            duration -= dur_converted;
        }
        return !presizion;
    };

    if(l(ch::hours{}) ||
    l(ch::minutes{}) ||
    l(ch::seconds{}) ||
    l(ch::milliseconds{}) ||
    l(ch::microseconds{}) ||
    l(ch::nanoseconds{}))
        return os;
    return os;
}

}
