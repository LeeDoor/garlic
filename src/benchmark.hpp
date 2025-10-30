#pragma once
#include <chrono>
#include <climits>
#include <ostream>
namespace application {

namespace ch = std::chrono;

template<typename T>
concept TimeT =
requires(T t) {
    { ch::duration_cast<ch::nanoseconds>(t).count() } -> std::convertible_to<unsigned long long>;
    { T{ULLONG_MAX} } -> std::convertible_to<T>;
};

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

    template<TimeT DurationType>
    struct Timestamp {
        DurationType duration;
        Timestamp(DurationType time_) : duration{time_} {}
    };
};

template<TimeT DurationType>
std::ostream& operator<< (std::ostream& os, const Benchmark::Timestamp<DurationType>& timestamp) {
    auto duration = timestamp.duration;
    os << "{" << duration_cast<ch::milliseconds>(duration) << "} is ";
    int presizion = 2;
#define DECREMENT_TYPE(TimeT) \
    if(auto dur_converted = ch::duration_cast<TimeT>(duration); dur_converted.count()) { \
        os << dur_converted << ' '; \
        --presizion; \
        if(!presizion) return os; \
        duration -= dur_converted; \
    }
    DECREMENT_TYPE(ch::hours);
    DECREMENT_TYPE(ch::minutes);
    DECREMENT_TYPE(ch::seconds);
    DECREMENT_TYPE(ch::milliseconds);
    DECREMENT_TYPE(ch::microseconds);
    DECREMENT_TYPE(ch::nanoseconds);
    return os;
}

}
