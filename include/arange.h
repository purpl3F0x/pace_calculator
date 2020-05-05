#pragma once

#include <algorithm>
#include <vector>


template<typename T>
std::vector<T> arange(T start, T stop, T step = 1) {
    std::vector<T> out;
    for (T value = start; value < stop; value += step)
        out.push_back(value);
    return out;
}