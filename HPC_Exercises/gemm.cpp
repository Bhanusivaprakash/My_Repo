#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int main() {
    const size_t rows = 3;
    const size_t columns = 8;

     vector<int> matrix_a = {
        2, 5, 1, 0, 0, 0, 0, 0,
        7, 3, 4, 0, 0, 0, 0, 0,
        6, 8, 9, 0, 0, 0, 0, 0
    };

    vector<int> matrix_b = {
        4, 2, 3, 0, 0, 0, 0, 0,
        7, 3, 4, 0, 0, 0, 0, 0,
        6, 8, 9, 0, 0, 0, 0, 0
    };

	vector<int> matrix_c(24, 0);

    auto start = chrono::high_resolution_clock::now();

	for(size_t i = 0; i < rows; i++){
		for(size_t j = 0; j < columns; j++){
			matrix_c[i] = matrix_a[i * columns + j] * matrix_b[i * columns + j];
		}
	}

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << duration.count() << " microseconds" << endl;

   	cout << "Result: " << matrix_c[0] << endl;

    return 0;
}
