#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>

using namespace std;

int main() {
    const size_t rows = 10000;
    const size_t columns = 10000;

    vector<int> matrix(rows * columns, 2);	// Matrix declaration
    vector<int> x(columns, 7);				// Vector to be multiplied with
    vector<int> y(rows, 0);					// Result

    auto start = chrono::high_resolution_clock::now();

	for(size_t i = 0; i < rows; i++){
		for(size_t j = 0; j < columns; j++){
			y[i] += matrix[i * columns + j] * x[j];
		}
	}

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << duration.count() << " microseconds" << endl;

   	cout << "Y: " << y[0] << endl;

    return 0;
}
