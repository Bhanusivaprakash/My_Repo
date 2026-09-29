#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

int main(){
    vector<vector <uint8_t>> input(4, vector <uint8_t>(4, 0));
    vector<vector <int8_t>> kernel(3, vector <int8_t>(3, 0));
    vector<vector <int16_t>> output(2, vector <int16_t>(2, 0));

    int i = 0, j = 0, k = 0, l = 0, acc = 0, m = 0, n = 0;

    input = {
        {1, 4, 3, 2},
        {2, 2, 1, 6},
        {1, 4, 4, 3},
        {6, 5, 1, 2}
    }; 
    
    kernel = {
        {-1, 0, 1},
        {-1, 0, 1},
        {-1, 0, 1}
    };

    for(i = 0; i < 3; i++){
        for(j = 0; j < 3; j++){
            acc += input[i][j] * kernel[k][l];
            l++;
        }
        l = 0;
        k++;
    }
    output[0][0] = acc;

    acc = 0; k = 0; l = 0;

    for(i = 0; i < 3; i++){
        for(j = 1; j < 4; j++){
            acc += input[i][j] * kernel[k][l];
            l++;
        }
        l = 0;
        k++;
    }
    output[0][1] = acc;

    acc = 0; k = 0; l = 0;

    for(i = 1; i < 4; i++){
        for(j = 0; j < 3; j++){
            acc += input[i][j] * kernel[k][l];
            l++;
        }
        l = 0;
        k++;
    }
    output[1][0] = acc;

    acc = 0; k = 0; l = 0;

    for(i = 1; i < 4; i++){
        for(j = 1; j < 4; j++){
            acc += input[i][j] * kernel[k][l];
            l++;
        }
        l = 0;
        k++;
    }
    output[1][1] = acc;

    cout << acc << endl;
    return 0;

}