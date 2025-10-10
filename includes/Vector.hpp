#pragma once

# include "config.hpp"

/**
 * @brief Represents a mathematical vector of n dimensions.
 * 
 * @tparam T The type of the elements in the vector.
 */
template<typename T>
class Vector {
	protected:
		std::vector<T> data;

	public:
		Vector() = default;
		Vector(const size_t& size) : data(size) {}
		Vector(std::initializer_list<T> list) : data(list) {}
		Vector(const std::vector<T>& other) : data(other) {}

		# pragma region Operators

		/**
		 * @brief Adds two vectors.
		 * @param other The other vector to add.
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

		# pragma endregion

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

/**
 * @brief Computes the linear combination of given vectors and scalars.
 * @details This mean that the function computes the sum of each vector multiplied by its corresponding scalar.
 * @tparam T The type of the elements in the vectors and scalars.
 * @param vectors The vectors to combine.
 * @param scalars The scalars to multiply each vector by.
 * @return Vector<T> The resulting vector from the linear combination.
 * @note Time complexity : O(n)
 * @note Space complexity : O(n)
 * @note Allowed math functions : fma
 */
template<typename T>
Vector<T> linear_combination(const std::vector<Vector<T>>& vectors, const std::vector<T>& scalars) {
	if (vectors.size() != scalars.size())
		throw std::invalid_argument("Vectors and scalars lists must be of the same size.");

	if (vectors.empty())
		return Vector<T>();

	Vector<T> result(vectors[0].size());

	for (size_t i = 0; i < vectors.size(); i++) {
		if (vectors[i].size() != result.size())
			throw std::invalid_argument("All vectors must be of the same size.");

		for (size_t j = 0; j < vectors[0].size(); j++)
			result[j] = std::fma(scalars[i], vectors[i][j], result[j]);
	}

	return result;
}