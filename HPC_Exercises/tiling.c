#include <stdio.h>

void main(){
	int a[10] = {1,4,2,5,3,7,6,8,9,11}, i, chunk = 0, sum = 0;

	while(chunk < 10){
		for(i = chunk; i < chunk + 2; i++){
			sum += a[i];
		}
		chunk += 2;
	}
	printf("%d ", sum);
	
}
