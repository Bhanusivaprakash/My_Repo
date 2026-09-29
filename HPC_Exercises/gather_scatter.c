#include <stdio.h>

void main(){
	int a[5] = {40,15,8,3,10}, b[5] = {0,0,0,0,0}, idx[5] = {4,0,2,1,3}, option, i = 0;

	printf("1.Gather, 2.Scatter: \n");
	scanf("%d", &option);

	if(option == 1){
		for(i = 0; i < 5; i++){
			b[i] = a[idx[i]];
			printf("%d ", b[i]);
		}
	}

	else if(option == 2){
		for(i = 0; i < 5; i++){
			b[idx[i]] = a[i];
		}
		for(i = 0; i < 5; i++){
			printf("%d ", b[i]);
		}
	}
}
