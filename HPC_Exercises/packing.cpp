#include <iostream>
#include <bitset>
#include <cstdint>
#include <vector>

using namespace std;

int main(){
    uint8_t a = 3, b = 4, result = 0, i = 0;
    vector <uint8_t> output(5, 0);

    result = (a << 4) | b;  // Packed

    result = (0b11110000) & result; // Unmask 4
    cout << bitset<8>(result) << endl;

    a = 0;

    a = (result >> 4);
    cout << static_cast<int>(a) << endl;

    return 0;
}