#include <stdio.h>

void main(){
	int a[2][2] = {{2,3},{5,1}}, b[2][2], i, j;

	for(i = 0; i < 2; i++){
		for(j = 0; j < 2; j++){
			b[i][j] = a[i][j];
		}
	}

	for(i = 0; i < 2; i++){
		for(j = 0; j < 2; j++){
			printf("%d %d %d\n", i, j, b[i][j]);
		}
	}
}
