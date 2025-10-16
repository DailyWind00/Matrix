#include "Matrix.hpp"
#include "Vector.hpp"
#include "functions.hpp"

using namespace std;

TEST_CASE("Ex00 Vector & Matrix add/sub/scl") {
	Vector<f32> vec1 = {1, 2, 3};
	Vector<f32> vec2 = {4, 5, 6};
	Matrix<f32> mat1 = {{1, 4}, {2, 5}, {3, 6}};
	Matrix<f32> mat2 = {{7, 10}, {8, 11}, {9, 12}};

	vec1.add(vec2);
	CHECK(vec1 == Vector<f32>({5, 7, 9}));
	CHECK_THROWS(vec1.add(Vector<f32>({1, 2}))); // Mismatched sizes
	vec1.sub(vec2);
	CHECK(vec1 == Vector<f32>({1, 2, 3}));
	CHECK_THROWS(vec1.sub(Vector<f32>({1, 2}))); // Mismatched sizes
	vec1.scl(2.0f);
	CHECK(vec1 == Vector<f32>({2, 4, 6}));

	mat1.add(mat2);
	CHECK(mat1 == Matrix<f32>({{8, 14}, {10, 16}, {12, 18}}));
	CHECK_THROWS(mat1.add(Matrix<f32>({{1, 3}, {2, 4}}))); // Mismatched shapes
	mat1.sub(mat2);
	CHECK(mat1 == Matrix<f32>({{1, 4}, {2, 5}, {3, 6}}));
	CHECK_THROWS(mat1.sub(Matrix<f32>({{1, 3}, {2, 4}}))); // Mismatched shapes
	mat1.scl(2.0f);
	CHECK(mat1 == Matrix<f32>({{2, 8}, {4, 10}, {6, 12}}));
}

TEST_CASE("Ex01 linear combination") {
	Vector<f32> vec1 = {1, 0, 0};
	Vector<f32> vec2 = {0, 1, 0};
	Vector<f32> vec3 = {0, 0, 1};

	Vector<f32> vec4 = {1, 2, 3};
	Vector<f32> vec5 = {0, 10, -100};

	Vector<f32> vec6 = {1, 2};

	CHECK(linear_combination({vec1, vec2, vec3}, {10.0f, -2.0f, 0.5f}) == Vector<f32>({10, -2, 0.5f}));
	CHECK(linear_combination({vec4, vec5}, {10.0f, -2.0f}) == Vector<f32>{10.0f, 0.0f, 230.0f});
	CHECK(linear_combination<f32>({}, {}) == Vector<f32>{});
	CHECK_THROWS(linear_combination({vec1}, {})); // Mismatched sizes
	CHECK_THROWS(linear_combination({vec1, vec6}, {1.0f, 2.0f})); // Mismatched vector sizes
}

TEST_CASE("Ex02 lerp") {
	Vector<f32> u = {0, 0, 0};
	Vector<f32> v = {10, 10, 10};

	CHECK(lerp(u, v, 0.0f) == Vector<f32>({0, 0, 0}));
	CHECK(lerp(u, v, 1.0f) == Vector<f32>({10, 10, 10}));
	CHECK(lerp(u, v, 0.5f) == Vector<f32>({5, 5, 5}));
	CHECK(lerp(u, v, 0.25f) == Vector<f32>({2.5f, 2.5f, 2.5f}));
	CHECK(lerp(u, v, 0.75f) == Vector<f32>({7.5f, 7.5f, 7.5f}));
	CHECK_THROWS(lerp(u, Vector<f32>({1, 2}), 0.5f)); // Mismatched sizes
}

TEST_CASE("Ex03 dot product") {
	Vector<f32> vec1 = {1, 2, 3};
	Vector<f32> vec2 = {4, 5, 6};
	Vector<f32> vec3 = {0, 0, 0};
	Vector<f32> vec4 = {1, 2};

	CHECK(vec1.dot(vec2) == 32.0f); // 1*4 + 2*5 + 3*6 = 32
	CHECK(vec1.dot(vec3) == 0.0f);  // Dot product with zero vector
	CHECK(vec4.dot(vec4) == 5.0f);  // 1*1 + 2*2 = 5
	CHECK_THROWS(vec1.dot(vec4));   // Mismatched sizes
}

TEST_CASE("Ex04 norms") {
	Vector<f32> vec1 = {3, 4};
	Vector<f32> vec2 = {-1, -2, -3};
	Vector<f32> vec3 = {0, 0, 0};
	Vector<f32> vec4 = {1, -1, 1, -1};

	CHECK(vec1.norm_1() == 7.0f);             // |3| + |4| = 7
	CHECK(vec1.norm() == 5.0f);               // sqrt(3² + 4²) = 5
	CHECK(vec1.norm_inf() == 4.0f);           // max(|3|, |4|) = 4

	CHECK(vec2.norm_1() == 6.0f);             // |-1| + |-2| + |-3| = 6
	CHECK(vec2.norm() == f32(std::sqrt(14))); // sqrt(1² + 2² + 3²) = sqrt(14) in float
	CHECK(vec2.norm_inf() == 3.0f);           // max(|-1|, |-2|, |-3|) = 3

	CHECK(vec3.norm_1() == 0.0f);             // Zero vector
	CHECK(vec3.norm() == 0.0f);
	CHECK(vec3.norm_inf() == 0.0f);

	CHECK(vec4.norm_1() == 4.0f);             // |1| + |-1| + |1| + |-1| = 4
	CHECK(vec4.norm() == 2.0f);               // sqrt(1² + (-1)² + 1² + (-1)²) = sqrt(4) = 2
	CHECK(vec4.norm_inf() == 1.0f);           // max(|1|, |-1|) = 1
}

TEST_CASE("Ex05 cosine") {
	Vector<f32> vec1 = {1, 0, 0};
	Vector<f32> vec2 = {0, 1, 0};
	Vector<f32> vec3 = {1, 1, 0};
	Vector<f32> vec4 = {-1, -1, 0};
	Vector<f32> vec5 = {1, 2, 3};
	Vector<f32> vec6 = {4, 5, 6};

	CHECK(angle_cos(vec1, vec1) == 1.0f);   // Same direction
	CHECK(angle_cos(vec1, vec2) == 0.0f);   // Perpendicular
	CHECK(angle_cos(vec3, vec4) == doctest::Approx(-1.0f));  // Opposite direction (not exactly -1 due to floating-point precision)
	CHECK(angle_cos(vec5, vec6) == 0.9746318f); // General case
	CHECK_THROWS(angle_cos(vec1, Vector<f32>({1, 2}))); // Mismatched sizes
	CHECK_THROWS(angle_cos(vec1, Vector<f32>({0, 0, 0}))); // Zero-length vector
}

TEST_CASE("Ex06 cross product") {
	Vector<f32> vec1 = {1, 0, 0};
	Vector<f32> vec2 = {0, 1, 0};
	Vector<f32> vec3 = {0, 0, 1};
	Vector<f32> vec4 = {1, 2, 3};
	Vector<f32> vec5 = {4, 5, 6};

	CHECK(cross_product(vec1, vec2) == vec3); // Standard basis vectors
	CHECK(cross_product(vec2, vec3) == vec1);
	CHECK(cross_product(vec3, vec1) == vec2);
	CHECK(cross_product(vec4, vec5) == Vector<f32>({-3, 6, -3})); // General case
	CHECK_THROWS(cross_product(vec1, Vector<f32>({1, 2}))); // Mismatched sizes
	CHECK_THROWS(cross_product(Vector<f32>({1, 2}), Vector<f32>({1, 2}))); // Not 3D vectors
}

TEST_CASE("Ex07 linear map & Matrix multiplication") {
	Matrix<f32> mat1 = {{1, 4}, {2, 5}, {3, 6}};
	Vector<f32> vec1 = {7, 8, 9};
	Matrix<f32> mat2 = {{7, 9, 11}, {8, 10, 12}};
	Matrix<f32> mat3 = {{1, 3}, {2, 4}};
	Matrix<f32> mat4 = {{5, 7}, {6, 8}};

	CHECK(mat1.mul_vec(vec1) == Vector<f32>({50, 122}));
	CHECK_THROWS(mat1.mul_vec(Vector<f32>({1, 2}))); // Mismatched sizes

	CHECK(mat2.mul_mat(mat1) == Matrix<f32>({{58, 139}, {64, 154}}));
	CHECK(mat1.mul_mat(mat2) == Matrix<f32>({{39, 49, 59}, {54, 68, 82}, {69, 87, 105}}));
	CHECK(mat3.mul_mat(mat4) == Matrix<f32>({{23, 31}, {34, 46}}));
	CHECK_THROWS(mat3.mul_mat(mat1)); // Mismatched shapes
}

TEST_CASE("Ex08 trace") {
	Matrix<f32> mat1 = {{1, 4, 7}, {2, 5, 8}, {3, 6, 9}};
	Matrix<f32> mat2 = {{1, 3}, {2, 4}};
	Matrix<f32> mat3 = {{1, 4, 7}, {2, 5, 8}};
	Matrix<f32> mat4 = {{5}};

	CHECK(mat1.trace() == 15.0f); // 1 + 5 + 9
	CHECK(mat2.trace() == 5.0f);  // 1 + 4
	CHECK(mat4.trace() == 5.0f);  // Single element
	CHECK_THROWS(mat3.trace());   // Non-square matrix
}

TEST_CASE("Ex09 transpose") {
	Matrix<f32> mat1 = {{1, 4}, {2, 5}, {3, 6}};
	Matrix<f32> mat2 = {{1, 3, 5}, {2, 4, 6}};
	Matrix<f32> mat3 = {{1}};
	Matrix<f32> mat4 = {};

	CHECK(mat1.transpose() == Matrix<f32>({{1, 2, 3}, {4, 5, 6}}));   // 3x2 to 2x3
	CHECK(mat2.transpose() == Matrix<f32>({{1, 2}, {3, 4}, {5, 6}})); // 2x3 to 3x2
	CHECK(mat3.transpose() == Matrix<f32>({{1}}));                    // 1x1 remains the same
	CHECK(mat4.transpose() == Matrix<f32>());                         // Empty matrix remains empty
}

TEST_CASE("Ex10 row echelon form") {
	Matrix<f32> mat1 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	Matrix<f32> mat2 = {{1, 2}, {3, 4}};
	Matrix<f32> mat3 = {{1, 2}, {2, 4}};
	Matrix<f32> mat4 = {
		{8,   5, -2, 4, 28},
		{4, 2.5, 20, 4, -4},
		{8,   5,  1, 4, 17}
	};
	Matrix<f32> mat5 = {{0}};
	Matrix<f32> mat6 = {};

	CHECK(mat1.row_echelon() == mat1); // Identity matrix
	CHECK(mat2.row_echelon() == Matrix<f32>({{1, 0}, {0, 1}}));
	CHECK(mat3.row_echelon() == Matrix<f32>({{1, 2}, {0, 0}}));
	CHECK(mat4.row_echelon() == Matrix<f32>({
		{1.0f, 0.625f, 0.0f, 0.0f, -12.1666667f},
		{0.0f, 0.0f, 1.0f, 0.0f, -3.6666667f},
		{0.0f, 0.0f, 0.0f, 1.0f, 29.5f}
	}));
	CHECK(mat5.row_echelon() == mat5); // Single zero element
	CHECK(mat6.row_echelon() == mat6); // Empty matrix
}