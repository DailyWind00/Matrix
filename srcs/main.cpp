#include "Matrix.hpp"
#include "Vector.hpp"

using namespace std;

int main() {
	try // The main function is used for tests, and may change overtime
	{
		Vector<f32> vec{1, 2, 3, 4};

		cout << "Vector: " << vec << "\n";

		Matrix<f32> mat = vec.reshape(2, 2);

		cout << "Reshaped Matrix (2x2):\n" << mat << "\n";

		vec.add(Vector{4.0f, 3.0f, 2.0f, 1.0f});

		cout << "Add : " << vec << "\n";

		Matrix<f32> mat2({
			{1.0f, 2.0f, 3.0f, 4.0f},
			{5.0f, 6.0f, 7.0f, 8.0f},
			{9.0f, 10.0f, 11.0f, 12.0f}
		});

		cout << "Matrix 2:\n" << mat2 << "\n";
		cout << "Matrix 2 shape: (" << mat2.shape().first << ", " << mat2.shape().second << ")\n";
		cout << "Matrix 2 is square: " << (mat2.is_square() ? "true" : "false") << "\n";
		cout << "Matrix 2 flattened: " << mat2.flatten() << "\n";

		mat2.scl(2);

		cout << "Matrix 2 scaled by 2:\n" << mat2 << "\n";
	}
	catch(const std::exception& e)
	{
		std::cerr << "\033[1;31m" << "Critical error : " << e.what() << "\033[0m" << '\n';
	}
}