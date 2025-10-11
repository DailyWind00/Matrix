# pragma once

# include "Vector.hpp"
# include "Matrix.hpp"

/**
 * @brief Computes the linear combination of given vectors and scalars.
 * @details This mean that the function computes the sum of each vector multiplied by its corresponding scalar.
 * @tparam T The type of the elements in the vectors and scalars.
 * @param vectors The vectors to combine.
 * @param scalars The scalars to multiply each vector by.
 * @return Vector<T> The resulting vector from the linear combination.
 * @throw std::invalid_argument If the sizes of the vectors and scalars lists do not match, or if the vectors are not of the same size.
 * @note Time complexity : O(n)
 * @note Space complexity : O(n)
 * @note Allowed math functions : fma
 */
template<typename T>
Vector<T> linear_combination(std::initializer_list<Vector<T>> vectors, std::initializer_list<T> scalars) {
    if (vectors.size() != scalars.size())
        throw std::invalid_argument("Vectors and scalars lists must be of the same size.");

    if (vectors.size() == 0)
        return Vector<T>();

    auto vecIt = vectors.begin();
    auto scaIt = scalars.begin();

    Vector<T> result(vecIt->size());

    while (vecIt != vectors.end() && scaIt != scalars.end()) {
        if (vecIt->size() != result.size())
            throw std::invalid_argument("All vectors must be of the same size.");

        for (size_t j = 0; j < result.size(); ++j)
            result[j] = std::fma(*scaIt, (*vecIt)[j], result[j]);

        ++vecIt;
        ++scaIt;
    }

    return result;
}

/**
 * @brief Performs linear interpolation between two vectors.
 * @details The function computes a point along the line connecting vectors u and v, based on the interpolation factor t.
 *          When t = 0, the result is u; when t = 1, the result is v; for values between 0 and 1, the result is a blend of u and v.
 * @tparam T The type of the elements in the vectors and the interpolation factor.
 * @param u The first vector.
 * @param v The second vector.
 * @param t The interpolation factor.
 * @return Vector<T> The interpolated vector.
 * @throw std::invalid_argument If the vectors are not of the same size.
 * @note Time complexity : O(n)
 * @note Space complexity : O(n)
 * @note Allowed math functions : fma
 * 
 * @see https://en.wikipedia.org/wiki/Linear_interpolation
 */
template<typename T>
Vector<T> lerp(const Vector<T>& u, const Vector<T>& v, const T& t) {
	if (u.size() != v.size())
		throw std::invalid_argument("Both vectors must be of the same size.");

	Vector<T> result(u.size());

	for (size_t i = 0; i < u.size(); i++)
		result[i] = std::fma(t, v[i] - u[i], u[i]);

	return result;
}

template<typename T>
auto angle_cos(const Vector<T>& u, const Vector<T>& v) {
	if (u.size() != v.size())
		throw std::invalid_argument("Both vectors must be of the same size.");

	using R = TO_REAL<T>;
	R uNorm = u.norm();
	R vNorm = v.norm();

	if (uNorm == R(0) || vNorm == R(0))
		throw std::logic_error("Cannot compute angle with zero-length vector.");

	return u.dot(v) / (uNorm * vNorm);
}