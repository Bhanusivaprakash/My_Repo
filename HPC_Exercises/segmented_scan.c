#include <stdio.h>

void main(){
	int a[6] = {3,1,5,7,8,9}, b[6] = {0,0,0,0,0,0}, i = 0, j = 0;

	// First segment: inclusive scan
	while(i < 3){
		for(j = 0; j <= i; j++){
			b[i] += a[j];
		}
		i++;
	}

	// Second segment: exclusive scan
	b[i] = 0;
	i = 4;
	while(i < 6){
		j = 3;
		while(j < i){
			b[i] += a[j];
			j++; 
		}
		i++;
	}

	for(i = 0; i < 6; i++){
		printf("%d ", b[i]);
	}
}
