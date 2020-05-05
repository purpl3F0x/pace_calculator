

#include <string>
#include <iostream>

#include "arange.h"
#include "elevation_adj.h"
#include "gradient.h"
#include "interpolate.h"
#include "linspace.h"
#include "smooth.h"
#include "tcx.h"


# ifdef __EMSCRIPTEN__
#include <emscripten/bind.h>

using namespace emscripten;
# endif //__EMSCRIPTEN__


std::vector<uint32_t> analyse_(const std::string &s, const unsigned target_time) {
    auto tcx = parseString2TCX(s);

    if (tcx.size() < 2) return {};

    auto ySampled = interpolate<double>(tcx.DistanceMeters,
                                        tcx.AltitudeMeters,
                                        arange<int>(tcx.DistanceMeters.front(), tcx.DistanceMeters.back())
    );

    auto ySmoothed = smoothElevationData(ySampled, linspace<std::size_t>(0, ySampled.size(), ySampled.size()), 2500);

    auto altitude_gradient = gradient<>(ySmoothed);

    for (auto &i: ySmoothed) std::cout << i << " ";
    std::cout << "\n";


    auto adj_pace = adjusted_pace(altitude_gradient, target_time, (target_time / tcx.DistanceMeters.back() * 1000));

    return adj_pace;
}


# ifdef __EMSCRIPTEN__
emscripten::val analyse(const std::string &s, const unsigned target_time) {
    val heapU8 = val::module_property("HEAPU8");
    val asmJSHeapBuffer = heapU8["buffer"];

    auto res = analyse_(s, target_time);

    return val::global("Uint32Array").new_(asmJSHeapBuffer, reinterpret_cast<uintptr_t>(res.data()), res.size());
}




EMSCRIPTEN_BINDINGS(pace_calculator_js) {
    function("analyse", &analyse, allow_raw_pointers());
}


# endif //__EMSCRIPTEN__
