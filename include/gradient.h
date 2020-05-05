
#pragma once

#include <vector>
#include <algorithm>


template<typename T, typename R = double>
std::vector<R> gradient(const std::vector<T> &y) {
    std::vector<R> out(y.size());

    switch (y.size()) {
        case 0:
        case 1:
            return out;
        case 2:
            out[0] = out[1] - out[0];
            break;
        default:
            out[0] = y[1] - y[0];
            out[y.size() - 1] = y[y.size() - 1] - y[y.size() - 2];
    }


    for (int i = 1; i < y.size() - 1; i++) {
        out[i] = 0.5 * (y[i + 1] - y[i - 1]);
    }


    return out;
}
