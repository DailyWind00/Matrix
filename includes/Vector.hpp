#pragma once

# include "config.hpp"

/**
 * @brief Represents a mathematical vector of n dimensions.
 * @tparam T The type of the elements in the vector.
 */
template<typename T>
class Vector {
	protected:
		std::vector<T> data;

		/**
		 * @brief Computes the absolute value of a number.
		 * @details Works for both real and complex numbers.
		 * @param v The value to compute the absolute value for.
		 * @return The absolute value of v, as a real number.
		 * @throw std::invalid_argument If the type T is neither arithmetic nor complex.
		 * @note Time complexity : O(1)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : fma, pow
		 */
		inline auto abs(const T& v) const {
			using R = TO_REAL<T>;
			
			if constexpr (IS_ARITHMETIC(T))
				return (v < R(0)) ? -v : v;
			else if constexpr (IS_COMPLEX(T))
				return std::pow(std::fma(v.real(), v.real(), v.imag() * v.imag()), R(0.5)); // sqrt(real² + imag²)
			else
				throw std::invalid_argument("Cannot compute abs with the given type.");
		}

	public:
		Vector() = default;
		Vector(const size_t& size) : data(size) {}
		Vector(std::initializer_list<T> list) : data(list) {}
		Vector(const std::vector<T>& other) : data(other) {}

		/**
		 * @brief Adds two vectors.
		 * @param other The other vector to add.
		 * @throw std::invalid_argument If the vectors are not of the same size.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : None
		 */
		void add(const Vector<T>& other) {
			if (size() != other.size())
				throw std::invalid_argument("Vectors must have the same size");

			for (size_t i = 0; i < size(); ++i)
				data[i] += other.data[i];
		}

		/**
		 * @brief Substract two vectors.
		 * @param other The other vector to subtract.
		 * @throw std::invalid_argument If the vectors are not of the same size.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : None
		 */
		void sub(const Vector<T>& other) {
			if (size() != other.size())
				throw std::invalid_argument("Vectors must have the same size");

			for (size_t i = 0; i < size(); ++i)
				data[i] -= other.data[i];
		}

		/**
		 * @brief Scale the vector by a scalar.
		 * @param scalar The scalar to scale the vector by.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : None
		 */
		void scl(const T& scalar) {
			for (T& i : data)
				i *= scalar;
		}

		/**
		 * @brief Divide the vector by a scalar.
		 * @param scalar The scalar to divide the vector by.
		 * @throw std::logic_error If the scalar is zero.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : None
		 */
		void div(const T& scalar) {
			if (scalar == T(0))
				throw std::logic_error("Division by zero is not allowed.");

			for (T& i : data)
				i /= scalar;
		}

		/**
		 * @brief Computes the dot product of two vectors.
		 * @details Work with both real and complex numbers.
		 * @param other The other vector to compute the dot product with.
		 * @return The dot product result.
		 * @throw std::invalid_argument If the vectors are not of the same size.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : fma
		 */
		auto dot(const Vector<T>& other) const {
			if (size() != other.size())
				throw std::invalid_argument("Vectors must have the same size");

			using R = TO_REAL<T>;
			R result = R(0);

			for (size_t i = 0; i < size(); i++) {
				if constexpr (IS_ARITHMETIC(T)) {
					// Real numbers: use FMA for precision
					result = std::fma(data[i], other[i], result);
				}
				else if constexpr (IS_COMPLEX(T)) {
					// Complex numbers: conjugate the first operand, no fma
					result += std::conj(data[i]) * other[i];
				}
				else
					// Fallback: generic multiply-add
					result += data[i] * other[i];
			}
				
			return result;
		}

		/**
		 * @brief Compute the Taxicab norm (L1 norm) of the vector.
		 * @details The L1 norm is the sum of the absolute values of the vector's elements.
		 * @return The L1 norm of the vector.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : fma, pow
		 */
		auto norm_1() const {
			using R = TO_REAL<T>;
			R result = R(0);

			for (const T& i : data)
				result += this->abs(i);
	
			return result;
		}

		/**
		 * @brief Compute the Euclidean norm (L2 norm) of the vector.
		 * @details The L2 norm is the square root of the sum of the squares of the vector's elements.
		 * @return The L2 norm of the vector.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : fma, pow
		 */
		auto norm() const {
			using R = TO_REAL<T>;
			R result = R(0);

			for (const T& i : data) {
				const R a = this->abs(i); // Avoid doing fma with complex numbers
				result = std::fma(a, a, result);
			}

			return std::pow(result, R(0.5)); // = sqrt()
		}

		/**
		 * @brief Compute the Infinity norm (L∞ norm) of the vector.
		 * @details The L∞ norm is the maximum absolute value of the vector's elements.
		 * @return The L∞ norm of the vector.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : fma, pow
		 */
		auto norm_inf() const {
			using R = TO_REAL<T>;
			R result = R(0);

			for (const T& i : data)
				result = std::max(result, this->abs(i));

			return result;
		}

		# pragma region Utils

		/**
		 * @brief Returns the number of elements in the vector.
		 * @return size_t The size of the vector.
		 */
		inline size_t size() const { return data.size(); }

		/**
		 * @brief Reshapes the vector into a matrix.
		 * @param rows The number of rows in the resulting matrix.
		 * @param cols The number of columns in the resulting matrix.
		 * @return Matrix<T> The reshaped matrix.
		 * @throw std::invalid_argument If the product of rows and cols does not equal the size of the vector.
		 */
		Matrix<T> reshape(size_t rows, size_t cols) const {
			if (rows * cols != size())
				throw std::invalid_argument("Reshape dimensions do not match vector size.");

			std::vector<Vector<T>> mat;
			for (size_t i = 0; i < rows; ++i) {
				Vector<T> row;

				for (size_t j = 0; j < cols; ++j)
					row.data.push_back(data[i * cols + j]);

				mat.push_back(row);
			}

			return Matrix<T>(mat);
		}

		T& operator[](size_t index) { return data[index]; }
		const T& operator[](size_t index) const { return data[index]; }
		bool operator==(const Vector<T>& other) const { return data == other.data; }

		# pragma endregion
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Vector<T>& vec) {
	os << "[";
	for (size_t i = 0; i < vec.size(); ++i)
		os << vec[i] << (i + 1 < vec.size() ? ", " : "");
	os << "]";
	return os;
}