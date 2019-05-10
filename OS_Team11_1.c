// OS Team11 1번
// 2017110280 이재영
// 2017113426 우지현

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define CUSTOMERS 5
#define RESOURCES 3
#define TRY_MAX 10
#define TRUE 1
#define FALSE 0

int allocation[CUSTOMERS][RESOURCES];
int maximum[CUSTOMERS][RESOURCES];
int available[RESOURCES];
int need[CUSTOMERS][RESOURCES];
int instance[RESOURCES];

int init(void);
int request_resources(int, int[]);
int safety(void);
int release_resources(int, int[]);

void init();

int main(void)
{
	int cstm;
	int arr[RESOURCES];
	int i = 0;

	init();

	return 0;
}

int init(void)
{
	FILE *f;
	int i, j;

	f = fopen("input.txt", "r");

	for (i = 0; i < RESOURCES; i++)
		fscanf(f, "%d", available[i]);

	for (i = 0; i < CUSTOMERS; i++) {
		for (j = 0; j < RESOURCES; j++) {
			fscanf(f, "&%d", allocation[i][j]);
			available[j] -= allocation[i][j];
		}
	}

	for (i = 0; i < CUSTOMERS; i++)
		for (j = 0; j < RESOURCES; j++)
			fscanf(f, "&%d", maximum[i][j]);
	
	fclose(f);

	for (i = 0; i < CUSTOMERS; i++)
		for (j = 0; j < RESOURCES; j++)
			need[i][j] = maximum[i][j] - allocation[i][j];

}

int request_resources(int index, int req[])
{
	int i;
	int temp_allocation[3], temp_need[3], temp_available[3];

	/*step1. need[index]와 비교*/
	for (i = 0; i < RESOURCES; i++) {
		if (req[i] > need[index][i])
			return -1;// -1: step1에서 실패
	}

	/*step2. available과 비교*/
	for (i = 0; i < RESOURCES; i++) {
		if (req[i] > available[i])
			return -2;// -2: step2에서 실패
	}

	/*step3*/
	for (i = 0; i < RESOURCES; i++) {
		available[i] = allocation[index][i] - req[i];
		allocation[index][i] += req[i];
		need[index][i] -= req[i];
	}

	if (!safety()) {
		for (i = 0; i < RESOURCES; i++) {
			available[i] += req[i];
			allocation[index][i] -= req[i];
			need[index][i] += req[i];
		}
	}
}

int safety(void)
{
	int fin[CUSTOMERS] = { 0, };
	int work[RESOURCES];
	int rpt = 0;
	int check = 0, num = 0;
	int i = 0;

	for (i = 0; i < RESOURCES; i++)
		work[i] = available[i];

	while (1) {
		if (rpt >= TRY_MAX)
			return 0;

		for (i = num; i < CUSTOMERS; i = (i+1) % CUSTOMERS) {
			if (fin[i] == FALSE) {
				num = i; check = 1;
				break;
			}
			else
				check = 0;
		}

		// fin[i] == true for all i
		if (!check)
			return 1;

		for (i = 0; i < RESOURCES; i++) {
			if (need[num][i] > work[i]) {
				check = 0;
				break;
			}
			else
				check = 1;
		}

		if (check) {
			for (i = 0; i < RESOURCES; i++)
				work[i] += allocation[num][i];

			fin[num] = TRUE;
		}

		num = (num + 1) % CUSTOMERS;
		rpt++;
	}

}

int release_resources(int customer, int release[])
{

=======
	return 0;
}