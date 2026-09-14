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

    __m256i acc0 = _mm256_setzero_si256();
    __m256i acc1 = _mm256_setzero_si256();

    // Process 16 32-bit ints per iteration (8 per load)
    for (size_t i = 0; i + 15 < N; i += 16) {
        // Load eight 32-bit ints (256 bits) from a and b
        __m256i va0 = _mm256_loadu_si256((__m256i*)&a[i]);
        __m256i vb0 = _mm256_loadu_si256((__m256i*)&b[i]);
        __m256i va1 = _mm256_loadu_si256((__m256i*)&a[i + 8]);
        __m256i vb1 = _mm256_loadu_si256((__m256i*)&b[i + 8]);

        // Multiply even elements and widen to 64 bits
        __m256i prod_even0 = _mm256_mul_epi32(va0, vb0);
        __m256i prod_even1 = _mm256_mul_epi32(va1, vb1);

        // Shift odd elements into place and multiply to widen to 64 bits
        __m256i va0_odd = _mm256_srli_epi64(va0, 32);
        __m256i vb0_odd = _mm256_srli_epi64(vb0, 32);
        __m256i va1_odd = _mm256_srli_epi64(va1, 32);
        __m256i vb1_odd = _mm256_srli_epi64(vb1, 32);

        __m256i prod_odd0 = _mm256_mul_epi32(va0_odd, vb0_odd);
        __m256i prod_odd1 = _mm256_mul_epi32(va1_odd, vb1_odd);

        // Accumulate both even and odd 64-bit lanes
        acc0 = _mm256_add_epi64(acc0, _mm256_add_epi64(prod_even0, prod_odd0));
        acc1 = _mm256_add_epi64(acc1, _mm256_add_epi64(prod_even1, prod_odd1));
    }

    __m256i total_acc = _mm256_add_epi64(acc0, acc1);
    long long buffer[4];
    _mm256_storeu_si256((__m256i*)buffer, total_acc);
    long long result = buffer[0] + buffer[1] + buffer[2] + buffer[3];

    for (size_t i = N - (N % 16); i < N; i++) {
        result += (long long)a[i] * b[i];
    }

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << duration.count() << " microseconds" << endl;
    cout << "Result: " << result << endl;

    return 0;
}
