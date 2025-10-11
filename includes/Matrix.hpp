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
			for (const auto& col : init)
				data.emplace_back(col);
		}
		Matrix(std::vector<Vector<T>> other) : data(other) {}
		Matrix(const size_t& cols, const size_t& rows) : data(cols, Vector<T>(rows)) {}

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

			for (size_t i = 0; i < cols(); ++i)
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

			for (size_t i = 0; i < cols(); ++i)
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
			for (Vector<T>& col : data)
				col.scl(scalar); // Reuse Vector's scl method
		}

		/**
		 * @brief Multiplies the matrix by a vector.
		 * @param other The vector to multiply.
		 * @return Vector<T> The resulting vector.
		 * @throw std::invalid_argument If the matrix rows do not match the vector size.
		 * @note Time complexity : O(m*n) matrix rows * matrix cols
		 * @note Space complexity : O(n)
		 * @note Allowed math functions : fma
		 */
		Vector<T> mul_vec(const Vector<T>& other) {
			if (rows() != other.size())
				throw std::invalid_argument("Matrix rows must match vector size");

			Vector<T> result(cols());

			for (size_t c = 0; c < cols(); c++) {
				for (size_t r = 0; r < rows(); r++) {
					if constexpr (IS_ARITHMETIC(T))
						result[c] = std::fma(data[c][r], other[r], result[c]);
					else
						result[c] += data[c][r] * other[r];
				}
			}

			return result;
		}

		Matrix<T> mul_mat(const Matrix<T>& other) {
			if (cols() != other.rows())
				throw std::invalid_argument("Matrix A columns must match Matrix B rows");

			Matrix<T> result(cols(), other.rows());

			for (size_t c = 0; c < cols(); c++) {
				for (size_t r = 0; r < other.rows(); r++) {
					if constexpr (IS_ARITHMETIC(T))
						result[c][r] = std::fma(data[c][r], other[r][c], result[c][r]);
					else
						result[c][r] += data[c][r] * other[r][c];
				}
			}

			return result;
		}

		# pragma region Utils

		/**
		 * @brief Returns the number of rows in the matrix.
		 * @return size_t The number of rows.
		 */
		inline size_t rows() const { return data.empty() ? 0 : data[0].size(); }

		/**
		 * @brief Returns the number of columns in the matrix.
		 * @return size_t The number of columns.
		 */
		inline size_t cols() const { return data.size(); }

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

			for (const Vector<T>& col : data)
				for (size_t j = 0; j < col.size(); ++j)
					vec.push_back(col[j]);

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
		if (i) os << ", ";
		os << mat[i];
	}
	os << "}";
	return os;
}
