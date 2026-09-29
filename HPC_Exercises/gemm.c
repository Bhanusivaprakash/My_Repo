#include <stdio.h>

void main(){
	int a[4] = {1,2,3,4}, b[4] = {5,6,7,8}, c[4] = {0,0,0,0}, i = 0, k = 0, j = 0, N = 0;

/*	while(i < 4 && j < 4){
		for(k = 0; k < 2; k++){
			//printf("%d %d\n", i*2, k%2);
			c[j] = a[i * 2] * b[k % 2];
			j++;
		}	
		i++;
	}

	i = 0; j = 0; k = 0;
	while(i < 4 && j < 4){
		for(k = 0; k < 2; k++){
			c[j] += a[(i*2)+1] * b[(k % 2) + 2];
			j++;
		}
		i++;
	}
*/

	while(N < 2){
		i = 0; j = 0; k = 0;
		while(i < 4 && j < 4){
			for(k = 0; k < 2; k++){
				//printf("%d %d\n", i*2, k%2);
				c[j] += a[(i * 2)+N] * b[(k % 2) + 2*N];
				j++;
			}	
			i++;
		}	
		N++;
	}


	for(k = 0; k < 4; k++){
		printf("%d\n", c[k]);	
	}
}
