// OS Team11 2번
// 2017110280 이재영
// 2017113426 우지현

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1

typedef struct node *nodePointer;
typedef struct node {
	int vertex;
	nodePointer link;
};

typedef struct {
	char ar;
	int vertex;
} type;

nodePointer *graph = NULL;
nodePointer *alloc = NULL;
nodePointer *request = NULL;

int *visited = NULL;
int *cycle = NULL, cnt = 0;
int pnum, rnum;

void get_size(void)
{
	FILE *f;
	type temp[2];
	int i;

	f = fopen("input.txt", "r");

	pnum = rnum = 0;
	while (!feof(f))
	{
		for (i = 0; i < 2; i++) {
			fscanf(f, "%c%d", &temp[i].ar, &temp[i].vertex);

			if (temp[i].ar == 'P') {
				if (temp[i].vertex > pnum)
					pnum = temp[i].vertex;
			}
			else if (temp[i].ar == 'R')
				if (temp[i].vertex > rnum)
					rnum = temp[i].vertex;
		}
	}

	fclose(f);

	// resource allocation graph
	alloc = (nodePointer*)malloc((rnum + 1)*sizeof(nodePointer));
	request = (nodePointer*)malloc((pnum + 1)*sizeof(nodePointer));
	graph = (nodePointer*)malloc((pnum + 1) * sizeof(nodePointer));

	for (i = 0; i <= rnum; i++)
		alloc[i] = NULL;

	for (i = 0; i <= pnum; i++) {
		request[i] = NULL;
		graph[i] = NULL;
	}
}

void search(int u, nodePointer *trail, nodePointer *temp)
{
	while ((*temp) != NULL) {
		if ((*temp)->vertex >= u)
			break;
		if ((*temp)->vertex < u) {
			*trail = *temp;
			*temp = (*temp)->link;
		}
	}
}

void addNode(nodePointer *temp, int v, int u)
{
	nodePointer node, trail = NULL;

	node = (nodePointer)malloc(sizeof(struct node));
	node->vertex = u;
	node->link = NULL;

	if (!temp[v]) {
		temp[v] = node;
	}
	else {
		search(node->vertex, &trail, &temp[v]);

		if (trail == NULL) {
			temp[v] = node;
			node->link = temp[v];
		}
		else {
			trail->link = node;
			node->link = temp[v];
		}
	}
}

void printResult(void)
{
	nodePointer temp;
	int i;

	// request edge
	for (i = 1; i <= pnum; i++) {
		temp = request[i];

		printf("[P%d] ", i);
		while (temp != NULL) {
			printf("R%d ", temp->vertex);
			temp = temp->link;
		}
		printf("\n");
	}

	// allocation edge
	for (i = 1; i <= rnum; i++) {
		temp = alloc[i];

		printf("[R%d] ", i);
		while (temp != NULL) {
			printf("P%d ", temp->vertex);
			temp = temp->link;
		}
		printf("\n");
	}

	// wait-for graph
	for (i = 1; i <= pnum; i++) {
		temp = graph[i];

		printf("[P%d] ", i);
		while (temp != NULL) {
			printf("P%d ", temp->vertex);
			temp = temp->link;
		}
		printf("\n");
	}
}

int isCyclic(int v, int parent)
{
	nodePointer w;

	visited[v] = TRUE;

	for (w = graph[v]; w; w = w->link) {
		if (!visited[w->vertex]) {
			if (isCyclic(w->vertex, v)) {
				cycle[cnt++] = w->vertex;
				return TRUE;
			}
		}
		else if (w->vertex != parent) {
			cycle[cnt++] = w->vertex;
			return TRUE;
		}
	}

	return FALSE;
}

int cycleCheck(int num)
{
	int i;

	visited = (int*)malloc((num + 1) * sizeof(int));
	cycle = (int*)malloc((num + 1) * sizeof(int));

	for (i = 0; i <= num; i++)
		visited[i] = cycle[i] = 0;

	for (i = 1; i <= num; i++)
		if (!visited[i])
			if (isCyclic(i, -1))
				return TRUE;

	return FALSE;
}

void freeList(nodePointer *ptr)
{
	nodePointer temp;

	while (*ptr) {
		temp = *ptr;
		*ptr = (*ptr)->link;
		free(temp);
	}
}

int main()
{
	FILE *f;
	type u, v;
	nodePointer reqNext = NULL, allocNext = NULL;
	int i;

	get_size();

	f = fopen("input.txt", "r");

	while (!feof(f)) {

		fscanf(f, "%c%d %c%d", &u.ar, &u.vertex, &v.ar, &v.vertex);

		if (u.ar == 'R') {
			// allocation edge R[u.vertex] -> P[v.vertex]
			addNode(alloc, u.vertex, v.vertex);
		}
		else {
			// request edge P[u.vertex] -> R[v.vertex]
			addNode(request, u.vertex, v.vertex);
		}
	}

	fclose(f);

	for (i = 1; i <= pnum; i++) {
		reqNext = request[i];

		while (request != NULL) {
			allocNext = alloc[reqNext->vertex];
			while (allocNext != NULL)
			{
				addNode(graph, i, allocNext->vertex);
				allocNext = allocNext->link;
			}
			reqNext = reqNext->link;
		}
	}

	printResult();

	/*
	if (!cycleCheck(pnum))
		printf("N\n");
	else {
		printf("P%d ", cycle[0]);

		if (cycle[1] < cycle[cnt - 1]) {
			for (i = 1; i < cnt; i++)
				printf("P%d ", cycle[i]);
		}
		else {
			for (i = cnt - 1; i > 0; i--)
				printf("P%d ", cycle[i]);
		}
	}*/


	/*for (i = 0; i <= pnum; i++)
		freeList(&graph[i]);
	free(graph);
	free(visited);
	free(cycle);*/

	return 0;
}