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

	// Bonus : Complex numbers

	Vector<c32> cvec1 = { {1,1}, {2,2}, {3,3} };
	Vector<c32> cvec2 = { {4,-1}, {5,-2}, {6,-3} };
	Matrix<c32> cmat1 = { {{1,1}, {4,4}}, {{2,2}, {5,5}}, {{3,3}, {6,6}} };
	Matrix<c32> cmat2 = { {{7,-1}, {10,-1}}, {{8,-2}, {11,-2}}, {{9,-3}, {12,-3}} };

	cvec1.add(cvec2);
	CHECK(cvec1 == Vector<c32>({ {5,0}, {7,0}, {9,0} }));
	cvec1.sub(cvec2);
	CHECK(cvec1 == Vector<c32>({ {1,1}, {2,2}, {3,3} }));
	cvec1.scl(c32(2,0));
	CHECK(cvec1 == Vector<c32>({ {2,2}, {4,4}, {6,6} }));

	cmat1.add(cmat2);
	CHECK(cmat1 == Matrix<c32>({ {{8,0}, {14,3}}, {{10,0}, {16,3}}, {{12,0}, {18,3}} }));
	cmat1.sub(cmat2);
	CHECK(cmat1 == Matrix<c32>({ {{1,1}, {4,4}}, {{2,2}, {5,5}}, {{3,3}, {6,6}} }));
	cmat1.scl(c32(2,0));
	CHECK(cmat1 == Matrix<c32>({ {{2,2}, {8,8}}, {{4,4}, {10,10}}, {{6,6}, {12,12}} }));
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

	// Bonus : Complex numbers

	Vector<c32> cvec1 = {{1,1}, {0,1}, {1,0}};
	Vector<c32> cvec2 = {{-1,2}, {2,-1}, {0,3}};

	CHECK(linear_combination({cvec1, cvec2}, {c32(2,0), c32(-1,1)}) ==
	      Vector<c32>({
	          cvec1[0]*c32(2,0) + cvec2[0]*c32(-1,1),
	          cvec1[1]*c32(2,0) + cvec2[1]*c32(-1,1),
	          cvec1[2]*c32(2,0) + cvec2[2]*c32(-1,1)
	      }));

	CHECK_THROWS(linear_combination({cvec1}, {})); // Mismatched sizes
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

	// Bonus : Complex numbers
	
	Vector<c32> cu = {{0,0}, {1,1}, {2,0}};
	Vector<c32> cv = {{10,0}, {11,1}, {12,0}};

	CHECK(lerp(cu, cv, 0.0f) == Vector<c32>({{0,0}, {1,1}, {2,0}}));
	CHECK(lerp(cu, cv, 1.0f) == Vector<c32>({{10,0}, {11,1}, {12,0}}));
	CHECK(lerp(cu, cv, 0.5f) == Vector<c32>({{5,0}, {6,1}, {7,0}}));
	CHECK(lerp(cu, cv, 0.25f) == Vector<c32>({{2.5f,0}, {3.5f,1}, {4.5f,0}}));
	CHECK(lerp(cu, cv, 0.75f) == Vector<c32>({{7.5f,0}, {8.5f,1}, {9.5f,0}}));
	CHECK_THROWS(lerp(cu, Vector<c32>({{1,1}, {2,2}}), 0.5f)); // Mismatched sizes
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
	
	// Bonus: Complex numbers

	Vector<c32> cvec1 = {{1,1}, {0,1}, {1,0}};
	Vector<c32> cvec2 = {{-1,2}, {2,-1}, {0,3}};

	c32 expected = cvec1[0]*std::conj(cvec2[0]) + 
	                cvec1[1]*std::conj(cvec2[1]) + 
	                cvec1[2]*std::conj(cvec2[2]);

	CHECK(cvec1.dot(cvec2) == expected);
	CHECK_THROWS(cvec1.dot(Vector<c32>{{1,0},{2,0}})); // Mismatched sizes
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

	// Bonus: Complex vectors

	Vector<c32> cvec1 = {{3, 4}, {1, -1}};
	Vector<c32> cvec2 = {{-1, 2}, {0, -3}, {2, 2}};

	CHECK(cvec1.norm_1() == std::abs(cvec1[0]) + std::abs(cvec1[1]));
	CHECK(cvec1.norm() == std::sqrt(std::norm(cvec1[0]) + std::norm(cvec1[1])));
	CHECK(cvec1.norm_inf() == std::max(std::abs(cvec1[0]), std::abs(cvec1[1])));

	CHECK(cvec2.norm_1() == std::abs(cvec2[0]) + std::abs(cvec2[1]) + std::abs(cvec2[2]));
	CHECK(cvec2.norm() == std::sqrt(std::norm(cvec2[0]) + std::norm(cvec2[1]) + std::norm(cvec2[2])));
	CHECK(cvec2.norm_inf() == std::max({std::abs(cvec2[0]), std::abs(cvec2[1]), std::abs(cvec2[2])}));
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

	// Bonus: Complex vectors

	Vector<c32> cvec1 = {{1, 1}, {0, 1}, {1, 0}};
	Vector<c32> cvec2 = {{-1, 2}, {2, -1}, {0, 3}};

	CHECK(angle_cos(cvec1, cvec1) == 1.0f); // Same complex vector
	CHECK_THROWS(angle_cos(cvec1, Vector<c32>({{0,0},{0,0},{0,0}}))); // Zero-length vector
	CHECK_THROWS(angle_cos(cvec1, Vector<c32>({{1,0},{2,0}}))); // Mismatched sizes
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

	// Bonus: Complex vectors

	Vector<c32> cvec1 = {{1,1}, {0,1}, {1,0}};
	Vector<c32> cvec2 = {{-1,2}, {2,-1}, {0,3}};
	Vector<c32> expected = {
		cvec1[1]*cvec2[2] - cvec1[2]*cvec2[1],
		cvec1[2]*cvec2[0] - cvec1[0]*cvec2[2],
		cvec1[0]*cvec2[1] - cvec1[1]*cvec2[0]
	};
	CHECK(cross_product(cvec1, cvec2) == expected);
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

	// Bonus: Complex numbers

	Matrix<c32> cmat1 = {{{1,1}, {2,0}}, {{0,1}, {1,2}}};
	Matrix<c32> cmat2 = {{{-1,0}, {0,1}}, {{1,1}, {2,-1}}};
	Vector<c32> cvec1 = {{1,0}, {0,1}};
	
	CHECK(cmat1.mul_vec(cvec1) == Vector<c32>({cmat1[0][0]*cvec1[0] + cmat1[1][0]*cvec1[1], cmat1[0][1]*cvec1[0] + cmat1[1][1]*cvec1[1]}));

	CHECK(cmat1.mul_mat(cmat2) == Matrix<c32>({
		{cmat1[0][0]*cmat2[0][0] + cmat1[1][0]*cmat2[0][1], cmat1[0][1]*cmat2[0][0] + cmat1[1][1]*cmat2[0][1]},
		{cmat1[0][0]*cmat2[1][0] + cmat1[1][0]*cmat2[1][1], cmat1[0][1]*cmat2[1][0] + cmat1[1][1]*cmat2[1][1]}
	}));
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

TEST_CASE("Ex11 determinant") {
	Matrix<f32> mat1 = {{1, -1}, {-1, 1}};
	Matrix<f32> mat2 = {{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};
	Matrix<f32> mat3 = {{8, 5, -2}, {4, 7, 20}, {7, 6, 1}};
	Matrix<f32> mat4 = {
		{ 8,   5, -2, 4 },
		{ 4, 2.5, 20, 4 },
		{ 8,   5,  1, 4 },
		{28,  -4, 17, 1 }
	};
	Matrix<f32> mat5 = {{5}};
	Matrix<f32> mat6 = {{1, 2, 3}, {4, 5, 6}};

	CHECK(mat1.determinant() == 0.0f);
	CHECK(mat2.determinant() == 8.0f);
	CHECK(doctest::Approx(mat3.determinant()) == -174.0f); // Need Approx due to floating-point arithmetic
	CHECK(doctest::Approx(mat4.determinant()) == 1032.0f); // Need Approx due to floating-point arithmetic
	CHECK(mat5.determinant() == 5.0f);  // Single element
	CHECK_THROWS(mat6.determinant());   // Non-square matrix
}

TEST_CASE("Ex12 inverse") {
	Matrix<f32> mat1 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	Matrix<f32> mat2 = {{2, 0, 0}, {0, 2, 0}, {0, 0, 2}};
	Matrix<f32> mat3 = {{8, 5, -2}, {4, 7, 20}, {7, 6, 1}};
	Matrix<f32> non_square = {{1, 2, 3}, {4, 5, 6}};
	Matrix<f32> singular = {{1, 2}, {2, 4}};
	
	CHECK(mat1.inverse() == mat1); // Identity matrix
	CHECK(mat2.inverse() == Matrix<f32>({{0.5f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f}, {0.0f, 0.0f, 0.5f}}));
	CHECK(mat3.inverse() == Matrix<f32>({
		{0.649425287f, 0.097701149f, -0.655172414f},
		{-0.781609195f, -0.126436782f, 0.965517241f},
		{0.143678161f, 0.074712644f, -0.206896552f}
	}));
	CHECK_THROWS(non_square.inverse()); // Non-square matrix
	CHECK_THROWS(singular.inverse()); // Singular matrix (determinant = 0)
}

TEST_CASE("Ex13 rank") {
	Matrix<f32> mat1 = {{1, 0, 0}, {0, 1, 0}, {0, 0, 1}};
	Matrix<f32> mat2 = {
		{ 1, 2, 0, 0},
		{ 2, 4, 0, 0},
		{-1, 2, 1, 1}
	};
	Matrix<f32> mat3 = {
		{ 8,  5, -2},
		{ 4,  7, 20},
		{ 7,  6,  1},
		{21, 18,  7}
	};

	CHECK(mat1.rank() == 3);
	CHECK(mat2.rank() == 2);
	CHECK(mat3.rank() == 3);
}

TEST_CASE("Bonus Ex14 projection") {
	Matrix<f32> mat = projection(90.0f, 1.0f, 0.1f, 100.0f);

	ofstream file("display_linux/proj");
	CHECK(file.is_open());

	for (size_t r = 0; r < mat.rows(); ++r) {
		for (size_t c = 0; c < mat.cols(); ++c) {
			file << mat[c][r] << (c + 1 == mat.cols() ? "\n" : ", ");
		}
	}
}