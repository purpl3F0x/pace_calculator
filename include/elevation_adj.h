
#pragma once

#include <cmath>
#include <vector>
#include <algorithm>


inline std::vector<double> gradientEffort(const std::vector<double> &elevation) {
    std::vector<double> out(elevation.size());

    std::transform(elevation.begin(), elevation.end(), out.begin(),
                   [](const auto &m) {
                     return 0.1707 + 0.5656 * m + 3.2209 * m * m -
                         0.32118 * m * m * m - 4.3635 * m * m * m * m;
                   });

    return out;
}


inline std::vector<uint32_t> adjusted_pace(const std::vector<double> &data, const unsigned duration, const double avg_pace) {
    std::vector<uint32_t> out(data.size());

    auto effort = gradientEffort(data);
    const double mult_ = avg_pace / 0.1707;

//    const auto k = 4.446 * 1e-5;

    std::transform(
        effort.begin(), effort.end(), out.begin(),
//        [&k, &duration](const auto &x) { return x * (1 + k * duration); });
        [&mult_](const auto &x) { return (uint32_t) std::rint(x * mult_); });

    return out;
};
