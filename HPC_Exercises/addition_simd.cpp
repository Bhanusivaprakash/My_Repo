#include <iostream>
#include <vector>
#include <chrono>
#include <immintrin.h>

using namespace std;

int main() {
    const size_t N = 10000000;

    // 32-bit int: Total memory stays 80 MB
    vector<int> a(N, 2);
    vector<int> b(N, 3);

    auto start = chrono::high_resolution_clock::now();

    __m256i acc = _mm256_setzero_si256();
    
    // Process 16 32-bit ints per iteration (8 per load)
    for (size_t i = 0; i + 7 < N; i += 8) {
        __m256i va = _mm256_loadu_si256((__m256i*)&a[i]);
        __m256i vb = _mm256_loadu_si256((__m256i*)&b[i]);

        acc = _mm256_add_epi32(acc, _mm256_add_epi32(va, vb));
    }

    int buffer[8];
    _mm256_storeu_si256((__m256i*)buffer, acc);
    long long result = buffer[0] + buffer[1] + buffer[2] + buffer[3] + buffer[4] + buffer[5] + buffer[6] + buffer[7];

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << duration.count() << " microseconds" << endl;
    cout << "Result: " << result << endl;

    return 0;
}
