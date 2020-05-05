
#pragma once

#include <vector>


template<typename T>
std::vector<double> linspace(T start_in, T end_in, std::size_t num_in) {

    std::vector<double> out;

    const auto start = static_cast<double>(start_in);
    const auto end = static_cast<double>(end_in);
    const auto num = static_cast<double>(num_in);

    if (num == 0) { return out; }

    if (num == 1) {
        out.push_back(start);
        return out;
    }

    double delta = (end - start) / (num - 1);

    for (std::size_t i = 0; i < num - 1; ++i) {
        out.push_back(start + delta * i);
    }
    out.push_back(end);

    return out;
}