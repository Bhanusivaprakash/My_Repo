#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>

using namespace std;

int main() {
    const size_t rows = 3;
    const size_t columns = 8;

    vector<int> matrix_a = {
        2, 5, 1, 0, 0, 0, 0, 0,
        7, 3, 4, 0, 0, 0, 0, 0,
        6, 8, 9, 0, 0, 0, 0, 0
    };                                      // 3x3 Matrix

    vector<int> matrix_b = {
        4, 2, 3, 0, 0, 0, 0, 0,
        7, 3, 4, 0, 0, 0, 0, 0,
        6, 8, 9, 0, 0, 0, 0, 0
    };                                      // 3x3 Matrix

    auto start = chrono::high_resolution_clock::now();

    __m256i acc_mat = _mm256_setzero_si256();

    for(size_t i = 0; i < rows; i++){

        for(size_t j = 0; j < columns; j += 8){
        	__m256i mat_a = _mm256_loadu_si256((__m256i*)&matrix_a[i * columns + j]);
            __m256i mat_b = _mm256_loadu_si256((__m256i*)&matrix_b[i * columns + j]);

            acc_mat = _mm256_add_epi32(
                acc_mat,
                _mm256_mullo_epi32(mat_a, mat_b)
            );
        }

    }

    int32_t buffer[8];
    _mm256_storeu_si256((__m256i*)buffer, acc_mat);

    int32_t result = buffer[0] + buffer[1] + buffer[2] + buffer[3]
                   + buffer[4] + buffer[5] + buffer[6] + buffer[7];

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);

    cout << duration.count() << " microseconds" << endl;
    cout << "result: " << buffer[3] << endl;

    return 0;
}
