#include <iostream>
#include <vector>
#include <cstdint>

using namespace std;

int main(){
    vector <uint16_t> a(4, 0);
    vector <uint16_t> b(4, 0);
    vector <uint16_t> c(4, 0);
    vector <uint16_t> output(12, 0);

    int i = 0, k = 0;

    cout << "start" << endl;

    a = {10,20,30,40};
    b = {1,2,3,4};
    c = {100,200,300,400};

    for(i = 0; i < 4; i++){
        output[i * 3] = a[i]; 
        output[(i * 3) + 1] = b[i];
        output[(i * 3) + 2] = c[i];
    }

    for(i = 0; i < 4; i++){
        a[i] = 0;
        b[i] = 0;
        c[i] = 0;
    }

    for(i = 0; i < 12; i++){
        cout << output[i] << " ";
    }
    cout << endl;

    for(i = 0; i < 4; i++){
        a[i] = output[i * 3];
        b[i] = output[(i * 3) + 1];
        c[i] = output[(i * 3) + 2];
    }
    for(i = 0; i < 4; i++){
        cout << c[i] << " ";
    }

    return 0;
}