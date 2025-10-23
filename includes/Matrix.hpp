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
		Matrix() = default;
		Matrix(const T& value) { // Identity matrix
			data = {{ value, 0, 0, 0 },
					{ 0, value, 0, 0 },
					{ 0, 0, value, 0 },
					{ 0, 0, 0, value }};
		}
		Matrix(std::initializer_list<Vector<T>> rows) {
			if (!rows.size()) return;

			size_t c = rows.begin()->size(); // Columns size
			size_t r = rows.size();          // Rows size

			data.assign(c, Vector<T>(r));

			size_t i = 0;
			for (const Vector<T>& row : rows) {
				if (row.size() != c)
					throw std::invalid_argument("All rows must have the same size.");

				for (size_t j = 0; j < c; j++)
					data[j][i] = row[j];
					
				i++;
			}
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
		Vector<T> mul_vec(const Vector<T>& other) const {
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

		/**
		 * @brief Multiplies the matrix by another matrix.
		 * @details The multiplication is performed using the standard matrix multiplication algorithm.
		 * @param other The other matrix to multiply.
		 * @return Matrix<T> The resulting matrix.
		 * @throw std::invalid_argument If the matrix columns do not match the other matrix rows.
		 * @note Time complexity : O(m*n*p) matrix A rows * matrix A cols * matrix B cols
		 * @note Space complexity : O(m*p) matrix A rows * matrix B cols
		 * @note Allowed math functions : fma
		 * 
		 * @see https://en.wikipedia.org/wiki/Matrix_multiplication_algorithm
		 * @see https://matrix.reshish.com/matrix-multiplication/
		 */
		Matrix<T> mul_mat(const Matrix<T>& other) {
			if (cols() != other.rows())
				throw std::invalid_argument("Matrix A columns must match Matrix B rows");

			Matrix<T> result(rows(), other.cols());

			for (size_t c = 0; c < other.cols(); c++) {
				for (size_t r = 0; r < rows(); r++) {
					for (size_t k = 0; k < cols(); k++) {
						if constexpr (IS_ARITHMETIC(T))
							result[c][r] = std::fma(data[k][r], other[c][k], result[c][r]);
						else
							result[c][r] += data[k][r] * other[c][k];
					}
				}
			}

			return result;
		}

		/**
		 * @brief Computes the trace of the matrix.
		 * @details The trace is defined as the sum of the elements on the main diagonal (from the top left to the bottom right) of a square matrix.
		 * @return T The trace of the matrix.
		 * @throw std::invalid_argument If the matrix is not square.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(1)
		 * @note Allowed math functions : None
		 */
		T trace() const {
			if (!is_square())
				throw std::invalid_argument("Trace can only be computed on square matrix");

			T result = T(0);

			for (size_t i = 0; i < cols(); i++)
				result += data[i][i];

			return result;
		}

		/**
		 * @brief Transposes the matrix.
		 * @details The transpose of a matrix is obtained by swapping its rows with its columns.
		 * @return Matrix<T> The transposed matrix.
		 * @note Time complexity : O(m*n) matrix rows * matrix cols
		 * @note Space complexity : O(m*n) matrix rows * matrix cols
		 * @note Allowed math functions : None
		 */
		Matrix<T> transpose() {
			Matrix<T> result(rows(), cols());

			for (size_t c = 0; c < cols(); c++)
				for (size_t r = 0; r < rows(); r++)
					result[r][c] = data[c][r];

			return result;
		}

		/**
		 * @brief Converts the matrix to its Row Echelon Form (REF).
		 * @details The REF of a matrix is a form where all nonzero rows are above any rows of all zeros,
		 *          and the leading coefficient (the first nonzero number from the left, also called the pivot) of a nonzero row is always strictly to the right of
		 *          the leading coefficient of the previous row. Additionally, all entries in a column below a leading coefficient are zeros.
		 * @return Matrix<T> The Row Echelon Form of the matrix.
		 * @note Time complexity : O(m*n*min(m, n)) matrix rows * matrix cols * min(matrix rows, matrix cols)
		 * @note Space complexity : O(m*n) matrix rows * matrix cols
		 * @note Allowed math functions : None
		 */
		Matrix<T> row_echelon() const {
			Matrix<T> result(*this);
			size_t lead = 0; // Index of current leading column
			
			for (size_t r = 0; r < rows(); r++) {
				if (lead >= cols())
					break;

				// Find the pivot row
				size_t i = r;
				while (result[lead][i] == T(0)) {
					if (++i == rows()) {
						i = r;
						lead++;
						if (lead == cols())
							return result;
					}
				}
				
				// Swap the current row with the pivot row
				if (i != r) {
					for (size_t k = 0; k < cols(); k++)
						std::swap(result[k][r], result[k][i - 1]);
				}

				// Normalize the pivot row
				T pivot = result[lead][r];
				if (pivot != T(0)) {
					for (size_t k = 0; k < cols(); k++)
						result[k][r] /= pivot;
				}

				// Eliminate all rows below the pivot
				for (size_t j = r + 1; j < rows(); j++) {
					T factor = result[lead][j];
					for (size_t k = 0; k < cols(); k++)
						result[k][j] -= factor * result[k][r];
				}

				lead++;
			}

			// Eliminate above (back substitution)
			for (ssize_t r = rows() - 1; r >= 0; r--) {
				// Find pivot column in row r
				size_t pivot_col = 0;

				while (pivot_col < cols() && result[pivot_col][r] == T(0))
					++pivot_col;

				if (pivot_col == cols())
					continue;

				// Eliminate above
				for (ssize_t i = r - 1; i >= 0; --i) {
					T factor = result[pivot_col][i];

					for (size_t k = 0; k < cols(); ++k)
						result[k][i] -= factor * result[k][r];
				}
			}

			return result;
		}

		/**
		 * @brief Computes the determinant of the matrix.
		 * @return T The determinant of the matrix.
		 * @throw std::invalid_argument If the matrix is not square.
		 * @note Time complexity : O(n^3) using Gaussian elimination
		 * @note Space complexity : O(n^2) matrix rows * matrix cols
		 * @note Allowed math functions : None
		 */
		T determinant() const {
			if (!is_square())
				throw std::invalid_argument("Determinant can only be computed on square matrix");

			if (rows() == 1) return data[0][0];
   			if (rows() == 2) return data[0][0]*data[1][1] - data[1][0]*data[0][1];

			Matrix<T> tmp = *this;
			int swaps = 0;
			
			// Basic Gaussian elimination
			for (size_t i = 0; i < rows(); ++i) {
				if (tmp.data[i][i] == T(0)) {
					// find row to swap
					for (size_t j = i+1; j < rows(); ++j) {
						if (tmp.data[j][i] != T(0)) {
							std::swap(tmp.data[i], tmp.data[j]);
							swaps++;
							break;
						}
					}
				}
				// eliminate below
				for (size_t j = i+1; j < rows(); ++j) {
					T factor = tmp.data[j][i] / tmp.data[i][i];
					for (size_t k = i; k < rows(); ++k)
						if constexpr (IS_ARITHMETIC(T))
							tmp.data[j][k] = std::fma(-factor, tmp.data[i][k], tmp.data[j][k]);
						else
							tmp.data[j][k] -= factor * tmp.data[i][k];
				}
			}

			T det = (swaps % 2 == 0) ? 1 : -1;
			for (size_t i = 0; i < rows(); ++i)
				det *= tmp.data[i][i];

			return det;
		}

		/**
		 * @brief Computes the inverse of the matrix.
		 * @details The inverse matrix is compute using the Gauss-Jordan elimination method.
		 * @throw std::invalid_argument If the matrix is not square.
		 * @throw std::logic_error If the matrix is singular and cannot be inverted.
		 * @return Matrix<T> The inverse of the matrix.
		 * @note Time complexity : O(n^3)
		 * @note Space complexity : O(n^2) matrix rows * matrix cols
		 * @note Allowed math functions : None
		 */
		Matrix<T> inverse() const {
			if (!is_square())
				throw std::invalid_argument("Inverse can only be computed on square matrix.");

			if (determinant() == T(0))
				throw std::logic_error("Matrix is singular and cannot be inverted.");

			size_t n = rows();

			// Augment A with identity matrix [A | I]
			Matrix<T> identity(n, n);
			for (size_t i = 0; i < n; ++i)
				identity[i][i] = T(1);

			Matrix<T> augmented = *this | identity;

			// Perform Gauss–Jordan elimination
			for (size_t i = 0; i < n; ++i) {
				// Find pivot
				size_t pivot_row = i;
				for (size_t j = i; j < n; ++j)
					if (this->abs(augmented[i][j]) > this->abs(augmented[i][pivot_row]))
						pivot_row = j;

				if (pivot_row != i)
					for (size_t c = 0; c < augmented.cols(); ++c)
						std::swap(augmented[c][i], augmented[c][pivot_row]);

				// Normalize pivot row
				T pivot = augmented[i][i];
				if (pivot == T(0))
					throw std::logic_error("Matrix is singular during elimination.");

				for (size_t c = 0; c < augmented.cols(); ++c)
					augmented[c][i] /= pivot;

				// Eliminate all other rows
				for (size_t r = 0; r < n; ++r) {
					if (r == i) continue;
					T factor = augmented[i][r];
					for (size_t c = 0; c < augmented.cols(); ++c)
						augmented[c][r] -= factor * augmented[c][i];
				}
			}

			// Extract inverse (right half)
			Matrix<T> inverse(n, n);
			for (size_t c = 0; c < n; ++c)
				for (size_t r = 0; r < n; ++r)
					inverse[c][r] = augmented[c + n][r];

			return inverse;
		}

		/**
		 * @brief Computes the rank of the matrix.
		 * @details The rank is defined as the maximum number of linearly independent row or column vectors in the matrix.
		 *          It is computed by transforming the matrix to its Row Echelon Form (REF) and counting the number of non-zero rows.
		 * @return size_t The rank of the matrix.
		 * @note Time complexity : O(m*n*min(m, n)) matrix rows * matrix cols * min(matrix rows, matrix cols)
		 * @note Space complexity : O(m*n) matrix rows * matrix cols
		 * @note Allowed math functions : None
		 * 
		 * @see https://en.wikipedia.org/wiki/Rank_(linear_algebra)
		 */
		size_t rank() const {
			Matrix<T> ref = row_echelon();
			size_t result = 0;

			for (size_t r = 0; r < ref.rows(); ++r) {
				bool non_zero_row = false;

				for (size_t c = 0; c < ref.cols(); ++c) {
					if (ref[c][r] != T(0)) {
						non_zero_row = true;
						break;
					}
				}

				if (non_zero_row) result++;
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

		bool operator==(const Matrix<T>& other) const {
			if (rows() != other.rows() || cols() != other.cols())
				return false;

			using R = TO_REAL<T>;
			const R eps = R(1e-5); // Tolerance for floating-point comparison
			
			for (size_t c = 0; c < cols(); ++c)
				for (size_t r = 0; r < rows(); ++r)
					if (this->abs(data[c][r] - other[c][r]) > eps)
						return false;

			return true;
		}

		/**
		 * @brief Concatenates two matrices horizontally (side-by-side).
		 * @param other The other matrix to concatenate.
		 * @return Matrix<T> The resulting concatenated matrix.
		 * @throw std::invalid_argument If the matrices do not have the same number of cols.
		 * @note Time complexity : O(n)
		 * @note Space complexity : O(n)
		 * @note Allowed math functions : None
		 */
		Matrix<T> operator|(const Matrix<T>& other) const {
			if (rows() != other.rows())
				throw std::invalid_argument("Matrices must have the same number of rows for horizontal concatenation.");

			Matrix<T> result(cols() + other.cols(), rows());

			for (size_t c = 0; c < cols(); ++c)
				for (size_t r = 0; r < rows(); ++r)
					result[c][r] = data[c][r];

			for (size_t c = 0; c < other.cols(); ++c)
				for (size_t r = 0; r < other.rows(); ++r)
					result[c + cols()][r] = other[c][r];

			return result;
		}

		# pragma endregion
};

template<typename T>
std::ostream& operator<<(std::ostream& os, const Matrix<T>& mat) {
	os << "{";
	for (size_t r = 0; r < mat.rows(); ++r) {
		if (r) os << ", ";
		os << "[";
		for (size_t c = 0; c < mat.cols(); ++c) {
			if (c) os << ", ";
			os << mat[c][r];
		}
		os << "]";
	}
	os << "}";
	return os;
}
