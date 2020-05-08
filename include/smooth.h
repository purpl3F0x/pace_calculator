#pragma once

#include <algorithm>
#include <vector>


std::vector<double> highPassSmoothing(const std::vector<double> &x, const std::vector<double> &y, const double tao) {

    std::vector<double> smoothForward(x.size(), 0);
    std::vector<double> smoothBack(x.size(), 0);
    const auto size = x.size();

    std::vector<double> out = std::vector<double>(y);

    smoothForward[0] = y[0];
    smoothBack[size - 1] = y[size - 1];

    double dfTime = 0.0;

    for (auto i = 1; i < size; i++) {
        dfTime = (x[i] - x[i - 1]) / tao;
        smoothForward[i] = (smoothForward[i - 1] + dfTime * y[i]) / (1 + dfTime);
    }

    for (auto i = 2; i < size + 1; i++) {
        dfTime = (x[size - i + 1] - x[size - i]) / tao;
        smoothBack[size - i] =
            (smoothBack[size - i + 1] + dfTime * y[size - i]) / (1 + dfTime);
    }

    std::transform(smoothForward.begin() + 1, smoothForward.end(),
                   smoothBack.begin() + 1, out.begin() + 1,
                   [](const double &a, const double &b) {
                     return (a + b) / 2;
                   }
    );

    out[0] = out[1];

    return out;
}


std::vector<double> smoothElevationData(const std::vector<double> &elevation, const std::vector<double> &distance, const double coeff) {

    const auto averageDistBetween = distance.back() / distance.size();
    const auto tao = coeff * averageDistBetween;

    return highPassSmoothing(distance, elevation, tao);
}