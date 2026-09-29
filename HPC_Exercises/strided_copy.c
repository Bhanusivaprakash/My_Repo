#include <stdio.h>

void main(){
	int a[4] = {3,1,7,9}, b[2], i, stride = 2;

	for(i = 0; i < 2; i++){
		b[i] = a[i * stride];
		printf("%d ", b[i]);
	}
}
