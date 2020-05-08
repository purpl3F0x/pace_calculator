#pragma once

#include <algorithm>
#include <vector>
#include <numeric>


template<typename T>
std::vector<T> arange(const T start, const T stop) {
    std::vector<T> out(stop - start);
    std::iota(out.begin(), out.end(), start);
    return out;
}


template<typename T>
std::vector<T> arange(const T start, const T stop, const T step) {
    std::vector<T> out((stop - start) / step);
    std::size_t i = 0;
    for (T value = start; value < stop; value += step)
        out[i++] = (value);
    return out;
}