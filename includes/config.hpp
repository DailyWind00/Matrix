#pragma once

# define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
# define GREEN "\033[1;32m"
# define RED   "\033[1;31m"
# define RESET "\033[0m"

# include <vector>
# include <iostream>
# include <cmath>
# include "doctest.h"


using f32 = float;

// forward declarations
template<typename T> class Vector;
template<typename T> class Matrix;