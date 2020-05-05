#include <algorithm>
#include <vector>


template<typename T1, typename T2>
double interpolate(const std::vector<T1> &xData, const std::vector<T1> &yData, const T2 x, const bool extrapolate = false) {

    std::size_t size = xData.size();

    std::size_t i = 0;
    if (x >= xData[size - 2])
        i = size - 2;
    else
        while (x > xData[i + 1])
            i++;

    double xL = xData[i], yL = yData[i], xR = xData[i + 1], yR = yData[i + 1];
    if (!extrapolate) {
        if (x < xL)
            yR = yL;
        if (x > xR)
            yL = yR;
    }

    double dy_dx = (yR - yL) / (xR - xL); // gradient

    return yL + dy_dx * (x - xL); // linear interpolation
}


template<typename T1, typename T2>
std::vector<double>
interpolate(const std::vector<T1> &xData, const std::vector<T1> &yData, const std::vector<T2>& x, const bool extrapolate = false) {

    std::vector<double> out(x.size());
    std::transform(x.begin(), x.end(), out.begin(),
                   [&xData, &yData, &extrapolate](const T2 &v) {
                     return interpolate<T1, T2>(xData, yData, v, extrapolate);
                   }
    );

    return out;
};