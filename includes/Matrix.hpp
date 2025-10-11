#pragma once

# include "config.hpp"

/**
 * @brief Represents a mathematical matrix of m x n dimensions.
 * @tparam T The type of the elements in the matrix.
 * @note The matrix is stored in column-major order.
 */
template<typename T>
class Matrix {
	protected:
		std::vector<Vector<T>> data;

	public:
		Matrix() = default;
		Matrix(const T& value) { // Identity matrix
			data = {{ value, 0, 0, 0 },
					{ 0, value, 0, 0 },
					{ 0, 0, value, 0 },
					{ 0, 0, 0, value }};
		}
		Matrix(std::initializer_list<Vector<T>> init) {
			for (const auto& row : init)
				data.emplace_back(row);
		}
		Matrix(std::vector<Vector<T>> other) : data(other) {}

		/**
		 * @brief Adds two matrices.
		 * @param other The other matrix to add.
		 * @throw std::invalid_argument If the matrices are not of the same shape.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : None
		 */
		void add(const Matrix<T>& other) {
			if (shape() != other.shape())
				throw std::invalid_argument("Matrices must have the same shape.");

			for (size_t i = 0; i < rows(); ++i)
				data[i].add(other[i]); // Reuse Vector's add method
		}

		/**
		 * @brief Substract two matrices.
		 * @param other The other matrix to subtract.
		 * @throw std::invalid_argument If the matrices are not of the same shape.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : None
		 */
		void sub(const Matrix<T>& other) {
			if (shape() != other.shape())
				throw std::invalid_argument("Matrices must have the same shape.");

			for (size_t i = 0; i < rows(); ++i)
				data[i].sub(other[i]); // Reuse Vector's sub method
		}

		/**
		 * @brief Scale the matrix by a scalar.
		 * @param scalar The scalar to scale the matrix by.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : None
		 */
		void scl(const T& scalar) {
			for (Vector<T>& row : data)
				row.scl(scalar); // Reuse Vector's scl method
		}

		# pragma region Utils

		/**
		 * @brief Returns the number of rows in the matrix.
		 * @return size_t The number of rows.
		 */
		inline size_t rows() const { return data.size(); }

		/**
		 * @brief Returns the number of columns in the matrix.
		 * @return size_t The number of columns.
		 */
		inline size_t cols() const { return data.empty() ? 0 : data[0].size(); }

		/**
		 * @brief Returns the shape of the matrix as a pair (rows, cols).
		 * @return std::pair<size_t, size_t> The shape of the matrix.
		 */
		inline std::pair<size_t, size_t> shape() const { return { rows(), cols() }; }

		/**
		 * @brief Flattens the matrix into a single vector.
		 * @return Vector<T> The flattened vector.
		 */
		Vector<T> flatten() const {
			std::vector<T> vec;
			vec.reserve(rows() * cols());

			for (const Vector<T>& row : data)
				for (size_t j = 0; j < row.size(); ++j)
					vec.push_back(row[j]);
					
			return Vector<T>(vec);
		}

		/**
		 * @brief Checks if the matrix is square (rows == cols).
		 * @return true If the matrix is square, false Otherwise.
		 */
		inline bool is_square() const { return rows() == cols(); }

		Vector<T>& operator[](size_t index) { return data[index]; }
		const Vector<T>& operator[](size_t index) const { return data[index]; }
		bool operator==(const Matrix<T>& other) const { return data == other.data; }

		# pragma endregion
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat) {
	os << "{";
	for (size_t i = 0; i < mat.rows(); ++i) {
		if (i) os << " ";
		os << mat[i];
		if (i + 1 < mat.rows()) os << "\n";
	}
	os << "}";
	return os;
}
