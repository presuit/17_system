#include <stdio.h>
#include <stdlib.h>

int main(){
	//set variables
	typedef int** intPtr;
	intPtr arr;
	int row, column;
	int Ui = 0;
	int Uj = 0;

	//input row, column values by user.
	printf( "Please Enter 2 dimentional array's row and column values/\n" );
	scanf( "%d%d", &row, &column );
	if( row <= 0 || column <= 0 ){
		printf("Error : Wrong values about row and column\n");
		return -1;
	}

	//allocation
	arr = (intPtr)malloc( row * sizeof(int*) );
	for(int i = 0; i < row; i++){
		arr[i] = (int*)malloc( column * sizeof(int) );
	}

	//set all arr's element's values 1
	for(int i = 0; i < row; i++){
		for(int j = 0; j < column; j++){
			arr[i][j] = 1;
		}
	}
	
	while(1){
		//print state
		for(int i = 0; i < row; i++){
			for(int j = 0; j < column; j++){
				if( arr[i][j] > 0 ){
					printf("%c", '+');	
				}
				printf("%d ", arr[i][j]);
			}
			printf("\n");
		}
		//add values to targeted element.
		printf( "Input row, column values to change.\n" );
		scanf( "%d%d", &Ui, &Uj );
		//check value
		if( Ui >= 0 && Ui < row && Uj >= 0 && Uj < column ){
			arr[Ui][Uj] += 1;
			if( arr[Ui][Uj] > 2 ){
				arr[Ui][Uj] = -2;
			}
		}
		else {
		//Thing is wrong. should kill our program.
			for(int i = 0; i < row; i++){
				free(arr[i]);
			}
			free(arr);
			break;
		}
	}
	return 0;
}
