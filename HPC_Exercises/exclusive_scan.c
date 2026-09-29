#include <stdio.h>

void main(){
		int a[4] = {1,2,3,4}, b[4] = {0,0,0,0}, i = 0, j = 0;

		b[i] = 0;
		
		for(i = 1; i < 4; i++){
			for(j = 0; j < i; j++){
					b[i] += a[j];	
			}
		}
		for(i = 0; i < 4; i++){
			printf("%d ", b[i]);
		}
}
