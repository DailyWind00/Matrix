#include "Matrix.hpp"
#include "Vector.hpp"

using namespace std;

TEST_CASE("Ex00 Vector & Matrix add/sub/scl") {
	Vector<f32> vec1 = {1, 2, 3};
	Vector<f32> vec2 = {4, 5, 6};
	Matrix<f32> mat1 = {{1, 2, 3}, {4, 5, 6}};
	Matrix<f32> mat2 = {{7, 8, 9}, {10, 11, 12}};

	vec1.add(vec2);
	CHECK(vec1 == Vector<f32>({5, 7, 9}));
	CHECK_THROWS(vec1.add(Vector<f32>({1, 2}))); // Mismatched sizes
	vec1.sub(vec2);
	CHECK(vec1 == Vector<f32>({1, 2, 3}));
	CHECK_THROWS(vec1.sub(Vector<f32>({1, 2}))); // Mismatched sizes
	vec1.scl(2.0f);
	CHECK(vec1 == Vector<f32>({2, 4, 6}));

	mat1.add(mat2);
	CHECK(mat1 == Matrix<f32>({{8, 10, 12}, {14, 16, 18}}));
	CHECK_THROWS(mat1.add(Matrix<f32>({{1, 2}, {3, 4}}))); // Mismatched shapes
	mat1.sub(mat2);
	CHECK(mat1 == Matrix<f32>({{1, 2, 3}, {4, 5, 6}}));
	CHECK_THROWS(mat1.sub(Matrix<f32>({{1, 2}, {3, 4}}))); // Mismatched shapes
	mat1.scl(2.0f);
	CHECK(mat1 == Matrix<f32>({{2, 4, 6}, {8, 10, 12}}));
}

TEST_CASE("Ex01 linear_combination") {
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