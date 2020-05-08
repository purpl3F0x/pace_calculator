
#pragma once

#include <vector>


/**
 * @brief Return evenly spaced numbers over a specified interval. Assumes num_in > 2, And start < end
 * @tparam T
 * @param start_in  The starting value of the sequence.
 * @param end_in    The end value of the sequence.
 * @param num_in    Number of samples to generate
 * @return
 */
template<typename T>
std::vector<double> linspace(const double start, const double end, const std::size_t num_in) {
    std::vector<double> out(num_in);

    const auto num = static_cast<double>(num_in);
    const auto delta = static_cast<double>((end - start) / (num - 1));

    for (std::size_t i = 0; i < num - 1; ++i)
        out[i] = (start + delta * i);

    out.back() = end;

    return out;
}