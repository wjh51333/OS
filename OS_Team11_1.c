// OS Team11 1번
// 2017110280 이재영
// 2017113426 우지현

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define CUSTOMERS 5
#define RESOURCES 3

int allocation[CUSTOMERS][RESOURCES];
int maximum[CUSTOMERS][RESOURCES];
int available[RESOURCES];
int need[CUSTOMERS][RESOURCES];

int init();
int request_resources(int, int[]);
int release_resources(int, int[]);

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

int request_resources(int customer, int request[])
{

}

int release_resources(int customer, int release[])
{

}