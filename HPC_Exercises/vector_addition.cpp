#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int main() {
    const size_t N = 10000000;

    // 32-bit int: Total memory stays 80 MB
    vector<int> a(N, 2);
    vector<int> b(N, 3);

    auto start = chrono::high_resolution_clock::now();

	int sum1  = 0, sum2 = 0, total = 0;
    
    // Process 16 32-bit ints per iteration (8 per load)
    for (size_t i = 0; i < N; i++) {

    	sum1 += a[i];
    	sum2 += b[i];

    } total = sum1 + sum2;

    auto stop = chrono::high_resolution_clock::now();

    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << duration.count() << " microseconds" << endl;
    cout << "Result: " << total << endl;

    return 0;
}
