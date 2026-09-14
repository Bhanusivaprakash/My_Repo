#include <iostream>
#include <vector>
#include <chrono>

using namespace std;

int main(){
	vector<int> a(10000000, 2);
	vector<int> b(10000000, 3);
	long long result = 0, acc0 = 0, acc1 = 0, acc2 = 0, acc3 = 0;

	auto start = chrono::high_resolution_clock::now();

	for(size_t i = 0; i+3 < a.size(); i+=4){
		acc0 += (long long)a[i]*b[i];
		acc1 += (long long)a[i+1]*b[i+1];
		acc2 += (long long)a[i+2]*b[i+2];
		acc3 += (long long)a[i+3]*b[i+3];
	}
	result = acc0 + acc1 + acc2 + acc3;

	auto stop = chrono::high_resolution_clock::now();

	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	cout << duration.count() << " microseconds" << endl;
cout << "Result: " << result << endl; // <-- Forces the compiler to keep the loop!	
	return 0;
}
