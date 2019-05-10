// OS Team11 1��
// 2017110280 ���翵
// 2017113426 ������

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define CUSTOMERS 5
#define RESOURCES 3

int available[RESOURCES];
int maximum[CUSTOMERS][RESOURCES];
int allocation[CUSTOMERS][RESOURCES];
int need[CUSTOMERS][RESOURCES];
int instance[RESOURCES];

int request_resources(int, int[]);
int release_resources(int, int[]);

void init();

int main(void)
{




	return 0;
}

int request_resources(int index, int *req)
{
	int i;
	int temp_allocation[3], temp_need[3], temp_available[3];

	/*step1. need[index]�� ��*/
	for (i = 0; i < RESOURCES; i++) {
		if (req[i] > need[index][i])
			return -1;// -1: step1���� ����
	}

	/*step2. available�� ��*/
	for (i = 0; i < RESOURCES; i++) {
		if (req[i] > available[i])
			return -2;// -2: step2���� ����
	}

	/*step3*/
	for (i = 0; i < RESOURCES; i++) {
		temp_available[i] = allocation[index][i]- req[i];
		temp_allocation[i] += req[i];
		temp_need[i] -= req[i];
	}

	if (safety(index, temp_available, temp_allocation, temp_need) == 1)
		for(i = 0; i < RESOURCES; i++){
			available[i] = temp_available[i];
			allocation[index][i] = temp_allocation[i];
			need[index][i] = temp_need[i];
		}
}