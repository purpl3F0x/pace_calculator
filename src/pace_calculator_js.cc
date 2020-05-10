

#include <string>

#include "arange.h"
#include "elevation_adj.h"
#include "gradient.h"
#include "interpolate.h"
#include "linspace.h"
#include "smooth.h"
#include "tcx.h"

#include <chrono>


# ifdef __EMSCRIPTEN__

#include <emscripten/bind.h>

# endif //__EMSCRIPTEN__


struct Result {
    # ifdef __EMSCRIPTEN__
    emscripten::val pace = emscripten::val::null();                 // Uint32Array (to be :D )
    emscripten::val elevation = emscripten::val::null();            // Float64Array
    emscripten::val elevationSmoothed = emscripten::val::null();    // Float64Array
    emscripten::val gradient = emscripten::val::null();             // Float64Array
    # endif //__EMSCRIPTEN__


    std::vector<uint32_t> pace_v;
    std::vector<double> elevation_v;
    std::vector<double> elevationSmoothed_v;
    std::vector<double> gradient_v;

    unsigned avg_pace = 0;
    unsigned distance = 0;
};


Result analyse(const std::string &s, const unsigned target_time) {
    Result out;

    const auto tcx = parseString2TCX(s);
    if (tcx.size() < 2) return out;

    auto arranged = arange<double>(tcx.DistanceMeters.front(), tcx.DistanceMeters.back());
    out.elevation_v = interp(tcx.DistanceMeters, tcx.AltitudeMeters, arranged);
    out.elevationSmoothed_v = smoothElevationData(out.elevation_v, linspace<std::size_t>(
        0, out.elevation_v.size(), out.elevation_v.size()), 2500);
    out.gradient_v = gradient(out.elevationSmoothed_v);
    out.pace_v = adjusted_pace(out.gradient_v, target_time, (target_time / tcx.DistanceMeters.back() * 1000));

    # ifdef __EMSCRIPTEN__
    out.elevation = emscripten::val(emscripten::typed_memory_view(out.elevation_v.size(), out.elevation_v.data()));
    out.elevationSmoothed =
        emscripten::val(emscripten::typed_memory_view(out.elevationSmoothed_v.size(), out.elevationSmoothed_v.data()));
    out.gradient = emscripten::val(emscripten::typed_memory_view(out.gradient_v.size(), out.gradient_v.data()));
    out.pace = emscripten::val(emscripten::typed_memory_view(out.pace_v.size(), out.pace_v.data()));
    # endif //__EMSCRIPTEN__

    // must check with real average
    out.avg_pace = unsigned(target_time / tcx.DistanceMeters.back() * 1000.);

    return out;
}


# ifdef __EMSCRIPTEN__
EMSCRIPTEN_BINDINGS(pace_calculator_js) {
    function("analyse", &analyse, emscripten::allow_raw_pointers());

    emscripten::class_<Result>("Result")
        .constructor<>()
        .property("pace", &Result::pace)
        .property("elevation", &Result::elevation)
        .property("elevationSmoothed", &Result::elevationSmoothed)
        .property("gradient", &Result::gradient)
        .property("avg_pace", &Result::avg_pace)
        .property("distance", &Result::distance);

}


# endif //__EMSCRIPTEN__
