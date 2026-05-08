#include <stdio.h>

int main(void) {
	int list[10] = { 10,20,40,50,60,70 };
	int i, move, size = 6;
	printf("\n 삽입 전 선형 리스트 : ");
	for (i = 0; i < size; i++) printf("%3d", list[i]);
	printf("\n원소의 개수 : %d \n", size);

	move = insertelement(list, size, 30);

	printf("삽입 후 선형 리스트:");
	for (i = 0; i <= size; i++) printf("%3d", list[i]);
	printf("\n원소의 개수 : %d", ++size);
	printf("\n자리 이동 횟수:%d\n", move);

	move = deleteelement(list, size, 30);
	printf("\n삭제 후 선형 리스트:");
	for (i = 0; i < size - 1; i++) printf(" %d", list[i]);
	printf("\n원소의개수 : %d", --size);
	printf("\n자리이동횟수 :%d\n", move);


	getchar();
	return 0;
}