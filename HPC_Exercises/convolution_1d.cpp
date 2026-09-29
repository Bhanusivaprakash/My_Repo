#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

int main(){
    vector <uint8_t> input(5, 0);
    vector <uint8_t> kernel(3, 0);
    vector <uint16_t> output(3, 0);

    int i = 0, j = 0, k = 0, acc = 0;

    input = {1,2,3,4,5}; kernel = {1,2,1};

    while(i < 3){
        for(j = i; j < i+3; j++){
            acc += input[j] * kernel[k];
            output[i] = acc;
            k++;
        }
        k = 0;
        acc = 0;
        cout << endl;
        i++;
    }

    for(i = 0; i < 3; i++){
        cout << output[i] << ' ';
    }

    return 0;

}