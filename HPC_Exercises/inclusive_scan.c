#include <stdio.h>

void main(){
	int a[4] = {1,2,3,4}, b[4] = {0,0,0,0}, i = 0, j = 0;

	for(i = 0; i < 4; i++){
		for(j = 0; j <= i; j++){
			b[i] += a[j];
		}
		printf("%d ", b[i]);
	}
}
