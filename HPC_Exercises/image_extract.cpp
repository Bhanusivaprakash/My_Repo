#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>  // Required for fixed-width integer types
#include <math.h>

using namespace std;

int main(){

	vector<vector<int>> image_data(426, vector<int>(640, 0));
	vector<uint8_t> pixels((640 * 426), 0);
	int i = 0, j = 0, k = 0;

	ifstream file("C:/Users/bhanu/Downloads/sample.pgm", ios::binary);

	int ch = 0;
	
	if(!file.is_open()){
		cerr << "Error: Could not open the file.\n";
		return 1;
	}

	// 2. Ignore the 3 header lines
    file.ignore(256, '\n'); // P5
    file.ignore(256, '\n'); // 640 426
    file.ignore(256, '\n'); // 255
	            
	for(i = 0; i < (640*426); i++){
		ch = file.get();
		pixels[i] = static_cast<uint8_t>(ch);
	}
	cout << "P2\n640 426\n255\n";
	// Write pixels[426*640] into image_data[426][640];
	for(i = 0; i < 426; i++){
		for(j = 0; j < 640; j++){
			image_data[i][j] = (pixels[k] / 32);
			k++;
		}
	}

	for(i = 0; i < 426; i++){
		for(j = 0; j < 640; j++){
			cout << image_data[i][j] << ' '; // tab-separated;
		}
		cout << "\n";
	}

	return 0;
}
