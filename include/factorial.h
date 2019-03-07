#pragma once

#include <boost/multiprecision/gmp.hpp>
#include <boost/math/special_functions/factorials.hpp>

int my_factorial(int i)
{
      int ret = 1;
      for (int j = 1; j <= i; j++)
      {
            ret *= j;
      }

      return ret;
}

template <typename T>
auto factorial(int i) -> T
{
      if constexpr (std::is_same<int, T>::value)
            return my_factorial(i);
      else
            return static_cast<T>(boost::math::factorial<boost::multiprecision::mpf_float_100>(i));
}
