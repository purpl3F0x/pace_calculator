#pragma once

#include <algorithm>
#include <cmath>
#include <type_traits>
#include <vector>

#if (__cplusplus < 202002L)


template<typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
T lerp(T a, T b, T t) noexcept {
    return a + t * (b - a);
}


#else
using std::lerp;
#endif


template<typename T, typename = std::enable_if_t<std::is_floating_point<T>::value>>
class ListLerp {
 public:
  explicit ListLerp(const std::vector<T> &xp, const std::vector<T> &yp) : xp(xp), yp(yp) {
      b = this->xp.begin() + 1;
  }


  T interp(const T x) noexcept {
      // No extrapolation
      if (x <= xp.front()) return yp.front();
      if (x >= xp.back()) return yp.back();

      // find b positions
      while (x > *b) b++;

      // Calculate a, a_y, b_y
      const auto a = b - 1;
      const auto a_y = yp.begin() + (a - xp.begin());
      const auto b_y = yp.begin() + (b - xp.begin());

      const auto t = (x - *a) / (*b - *a);

      return lerp(*a_y, *b_y, t);
  };

 private:
  typename std::vector<T>::const_iterator b;
  const std::vector<T> &xp;
  const std::vector<T> &yp;
};


template<typename T>
std::vector<T> interp(const std::vector<T> &xp, const std::vector<T> &yp, const std::vector<T> &x) {
    std::vector<T> out(x.size());

    auto lLerp = ListLerp<T>(xp, yp);

    std::transform(x.begin(), x.end(), out.begin(),
                   [&lLerp](const auto &xi) {
                     return lLerp.interp(xi);
                   }
    );

    return out;
}
