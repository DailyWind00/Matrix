#pragma once

# define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
# define GREEN "\033[1;32m"
# define RED   "\033[1;31m"
# define RESET "\033[0m"

# include <vector>
# include <iostream>
# include <cmath>
# include <complex>
# include <type_traits>
# include "doctest.h"

# define IS_ARITHMETIC(T) (std::is_arithmetic_v<T>)
# define IS_COMPLEX(T) (std::is_same_v<T, std::complex<float>> || std::is_same_v<T, std::complex<double>> || std::is_same_v<T, std::complex<long double>>)

// Helper to get the real type from a possibly complex type
template<typename T> struct to_real { using type = T; };
template<typename T> struct to_real<std::complex<T>> { using type = T; };
template<typename T> using TO_REAL = typename to_real<T>::type;

using f32 = float;
using c32 = std::complex<float>;

// forward declarations
template<typename T> class Vector;
template<typename T> class Matrix;