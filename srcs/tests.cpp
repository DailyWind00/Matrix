#include "Matrix.hpp"
#include "Vector.hpp"

using namespace std;

TEST_CASE("Ex01 linear_combination") {
	Vector<f32> vec1 = {1, 0, 0};
	Vector<f32> vec2 = {0, 1, 0};
	Vector<f32> vec3 = {0, 0, 1};
	
	Vector<f32> vec4 = {1, 2, 3};
	Vector<f32> vec5 = {0, 10, -100};

	CHECK(linear_combination(vector<Vector<f32>>{vec1, vec2, vec3}, vector<f32>{10.0f, -2.0f, 0.5f}) == Vector<f32>({10, -2, 0.5f}));
	CHECK(linear_combination(vector<Vector<f32>>{vec4, vec5}, vector<f32>{10.0f, -2.0f}) == Vector<f32>{10.0f, 0.0f, 230.0f});
	CHECK(linear_combination(vector<Vector<f32>>{}, vector<f32>{}) == Vector<f32>());
	CHECK_THROWS(linear_combination(vector<Vector<f32>>{vec1}, vector<f32>{}));
}