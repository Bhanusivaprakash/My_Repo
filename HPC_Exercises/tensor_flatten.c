#include <stdio.h>

void main(){
	int matrix[2][3][4] = {
	    {
	        {1, 2, 3, 4},
	        {5, 6, 7, 8},
	        {9, 11, 9, 13}
	    },
	    {
	        {13, 14, 15, 16},
	        {17, 18, 1, 2},
	        {21, 20, 23, 24}
	    }
	};
	int matrix2[6][4];

	int tensor[24], i = 0, j = 0, k = 0, l = 0;

	for(i = 0; i < 2; i++){
		for(j = 0; j < 3; j++){
			for(k = 0; k < 4; k++){
				tensor[l] = matrix[i][j][k];
				matrix[i][j][k] = 0;
				l++;
			}
		}
	}

/*	l = 0;
	for(l = 0; l < 24; l++){
		printf("%d ", tensor[l]);
	}
*/
	// Reshape
	l = 0;
	for(i = 0; i < 6; i++){
		for(j = 0; j < 4; j++){
			matrix2[i][j] = tensor[l];
			printf("%d ", matrix2[i][j]);
			l++;
		}
	}
}
