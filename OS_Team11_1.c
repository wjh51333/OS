// OS Team11 1번
// 2017110280 이재영
// 2017113426 우지현

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <process.h>
#include <windows.h>

#define CUSTOMERS 5
#define RESOURCES 3
#define TRY_MAX 100
#define TRUE 1
#define FALSE 0

HANDLE hMutex;
int allocation[CUSTOMERS][RESOURCES];
int maximum[CUSTOMERS][RESOURCES];
int need[CUSTOMERS][RESOURCES];
int available[RESOURCES];

int index;
int res[RESOURCES];

void init(void);
unsigned WINAPI request_resources(void *);
int safety(void);
unsigned WINAPI release_resources(void *);
void printResult(void);

int main(void)
{
	HANDLE tHandles[CUSTOMERS];
	int i, j;
	char arr[30];

	//hMutex = CreateMutex(NULL, FALSE, NULL);

	init();

	printResult();
	printf("\n");

	for (i = 0; i < CUSTOMERS; i++) {
		printf("which process? ");
		scanf("%d", &index);

		printf("request or release? ");
		scanf("%s", arr);

		printf("(A B C)? ");
		for (j = 0; j < RESOURCES; j++)
			scanf("%d", &res[j]);

		if (strcmp(arr, "request") == 0)
			tHandles[index] = (HANDLE)_beginthreadex(NULL, 0, request_resources, NULL, 0, NULL);
		else if (strcmp(arr, "release") == 0)
			tHandles[index] = (HANDLE)_beginthreadex(NULL, 0, release_resources, NULL, 0, NULL);

		WaitForSingleObject(tHandles[index], INFINITE);

		printf("\n\n");
	}

	WaitForMultipleObjects(CUSTOMERS, tHandles, TRUE, INFINITE);
	CloseHandle(hMutex);

	return 0;
}

void init(void)
{
	FILE *f;
	int i, j;

	if ((f = fopen("input1.txt", "r")) == NULL) {
		printf("Failed to open file\n");
		exit(0);
	}

	// resources
	for (i = 0; i < RESOURCES; i++)
		fscanf(f, "%d ", &available[i]);
	
	// allocation & available 
	for (i = 0; i < CUSTOMERS; i++) {
		for (j = 0; j < RESOURCES; j++) {
			fscanf(f, "%d ", &allocation[i][j]);
			available[j] -= allocation[i][j];
		}
	}

	// maximum
	for (i = 0; i < CUSTOMERS; i++)
		for (j = 0; j < RESOURCES; j++)
			fscanf(f, "%d ", &maximum[i][j]);
	
	fclose(f);

	// need
	for (i = 0; i < CUSTOMERS; i++)
		for (j = 0; j < RESOURCES; j++)
			need[i][j] = maximum[i][j] - allocation[i][j];

}

unsigned WINAPI request_resources(void *arg)
{
	int i;
	int check = 0;
	
	WaitForSingleObject(hMutex, INFINITE);

	// P[index] request (A B C)
	printf("P%d request (", index);
	for (i = 0; i < RESOURCES - 1; i++)
		printf("%d ", res[i]);
	printf("%d)\n", res[i]);

	/*step1. need[index]와 비교*/
	for (i = 0; i < RESOURCES; i++) {
		if (res[i] > need[index][i]) {
			printf("Error : Requests more than the number of resources in the system\n");
			ReleaseMutex(hMutex);
			return 0;
		}
	}

	/*step2. available과 비교*/
	for (i = 0; i < RESOURCES; i++) {
		if (res[i] > available[i]) {
			printf("Error : Lack of resources\n");
			ReleaseMutex(hMutex);
			return 0;
		}
	}

	/*step3*/
	for (i = 0; i < RESOURCES; i++) {
		available[i] -= res[i];
		allocation[index][i] += res[i];
		need[index][i] -= res[i];
	}

	// safety algorithm
	// safety() == 0이면 unsafe state
	if (!safety()) {
		printResult();

		printf("Error : unsafe state!!\n");

		for (i = 0; i < RESOURCES; i++) {
			available[i] += res[i];
			allocation[index][i] -= res[i];
			need[index][i] += res[i];
		}
	}
	else
		printf("Successfull request!!\n");
	
	ReleaseMutex(hMutex);

	return 0;
}

int safety(void)
{
	int fin[CUSTOMERS] = { 0, };
	int work[RESOURCES];
	int rpt = 0;
	int check = 0, num = 0;
	int i = 0;

	// work = available
	for (i = 0; i < RESOURCES; i++)
		work[i] = available[i];

	while (1) {
		if (rpt >= TRY_MAX)
			return 0;

		// check == 1이면 fin[i] == FALSE인 P[i]가 있다
		for (i = num; i != num; i = (i+1) % CUSTOMERS) {
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
			// need[i] > work이면 break
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


unsigned WINAPI release_resources(void *arg)
{
	int i;
	int check = 0;

	WaitForSingleObject(hMutex, INFINITE);
	
	// P[index] release (A B C)
	printf("P%d release (", index);
	for (i = 0; i < RESOURCES - 1; i++)
		printf("%d ", res[i]);
	printf("%d)\n", res[i]);

	// check == 1이면 가지고 있는 resource 개수보다 더 많은 resource release
	for (i = 0; i < RESOURCES; i++) {
		if (allocation[index][i] < res[i]) {
			check = 1;
			break;
		}
	}

	if (!check) {
		for (i = 0; i < RESOURCES; i++) {
			available[i] += res[i];
			allocation[index][i] -= res[i];
			need[index][i] += res[i];
		}

		printf("Successfull release!!\n");
	}
	else {
		printf("Error : More resources release than P%d has | P%d has (", index, index);

		for (i = 0; i < RESOURCES - 1; i++)
			printf("%d ", allocation[index][i]);
		printf("%d)\n", allocation[index][i]);
	}


	ReleaseMutex(hMutex);

	return 0;
}

void printResult() {
	int i, j;

	printf("Allocation	   MAX	          Need	        Available\n");
	for (i = 0; i < CUSTOMERS; i++) {
		for (j = 0; j < RESOURCES; j++) {
			printf("%2d ", allocation[i][j]);
		}
		printf("	");
		for (j = 0; j < RESOURCES; j++)
			printf("%2d ", maximum[i][j]);
		printf("	");
		for (j = 0; j < RESOURCES; j++)
			printf("%2d ", need[i][j]);

		if (i == 0) {
			printf("	");
			for (j = 0; j < RESOURCES; j++)
				printf("%2d ", available[j]);
		}
		printf("\n");
	}
}