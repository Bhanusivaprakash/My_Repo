#include <iostream>
#include <vector>

using namespace std;

int main(){
    vector<vector <int>> matrix(2, vector <int> (3, 0));
    vector <int> output(6, 0);
    
    matrix = {
        {1, 4, 7},
        {3, 9, 8}
    };
    int i = 0, j = 0, k = 0;

    for(i = 0; i < 3; i++){
        for(j = 0; j < 2; j++){
            output[k] = matrix[j][i];
            k++;
        }
    }
    for(i = 0; i < 6; i++){
        cout << output[i] << ' ';
    }
}