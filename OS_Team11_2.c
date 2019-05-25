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
int next[100], index = 0;
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

void addNode(nodePointer *list, int u)
{
	nodePointer node, trail = NULL, temp = *list;

	node = (nodePointer)malloc(sizeof(struct node));
	node->vertex = u;
	node->link = NULL;

	if (!(*list)) {
		*list = node;
	}
	else {
		search(node->vertex, &trail, &temp);

		if (trail == NULL) {
			*list = node;
			node->link = temp;
		}
		else {
			trail->link = node;
			node->link = temp;
		}
	}
}

void printResult(void)
{
	nodePointer temp;
	int i;

	// request edge
	printf("Adjacency list of RAG\n");
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
	printf("\n\n");

	// wait-for graph
	printf("Adjacency list of wait-for graph\n");
	for (i = 1; i <= pnum; i++) {
		temp = graph[i];

		printf("[P%d] ", i);
		while (temp != NULL) {
			printf("P%d ", temp->vertex);
			temp = temp->link;
		}
		printf("\n");
	}
	printf("\n");
}

int isCyclic(int v, int parent, int *i)
{
	nodePointer w;

	visited[v] = TRUE;                

	for (w = graph[v]; w; w = w->link) {
		if (index > *i && w->vertex == next[*i]) {
			(*i)++;
			
			if (graph[(w->link)->vertex]->vertex == next[*i])
				next[index++] = next[*i];

			continue;
		}

		if (!visited[w->vertex]) {
			if (isCyclic(w->vertex, v, &(*i))) {
				cycle[cnt++] = w->vertex;
				visited[v] = FALSE;

				if (w->link != NULL)
					next[index++] = w->vertex;

				return TRUE;
			}
		}
		else if (w->vertex != parent) {
			cycle[cnt++] = w->vertex;
			visited[v] = FALSE;

			if (w->link != NULL)
				next[index++] = w->vertex;

			return TRUE;
		}
	}
	
	return FALSE;
}

void cycleCheck(int num)
{
	int i, j;
	int check = 0; // cycle 개수
	int now = 0;

	visited = (int*)malloc((num + 1) * sizeof(int));
	cycle = (int*)malloc((num + 1) * sizeof(int));

	for (i = 0; i <= num; i++)
		visited[i] = cycle[i] = 0;

	for (i = 0; i < 100; i++)
		next[i] = 0;

	printf("cycle\n");

	for (i = 1; i <= num; i++) {
		if (!visited[i]) {
			if (isCyclic(i, -1, &now)) {
				if (cnt < 2)
					continue;

				check++;
				printf("P%d ", cycle[0]);

				if (cycle[1] < cycle[cnt - 1]) {
					for (j = 1; j < cnt; j++)
						printf("P%d ", cycle[j]);
				}
				else {
					for (j = cnt - 1; j > 0; j--)
						printf("P%d ", cycle[j]);
				}
				printf("\n");

				if (now <= index)
					i--;
			}
			else {
				visited[i] = TRUE;
				next[index++] = i;

				for (j = i+1; j <= num; j++)
					visited[j] = FALSE;
			}
			cnt = 0;
		}
	}

	if (check == 0)
		printf("No cycle!!\n");
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

		fscanf(f, "%c%d %c%d ", &u.ar, &u.vertex, &v.ar, &v.vertex);

		if (u.ar == 'R') {
			// allocation edge R[u.vertex] -> P[v.vertex]
			addNode(&alloc[u.vertex], v.vertex);
		}
		else {
			// request edge P[u.vertex] -> R[v.vertex]
			addNode(&request[u.vertex], v.vertex);
		}
	}

	fclose(f);

	// make wait-for graph
	for (i = 1; i <= pnum; i++) {
		reqNext = request[i];

		while (reqNext != NULL) {
			allocNext = alloc[reqNext->vertex];

			while (allocNext != NULL) {
				addNode(&graph[i], allocNext->vertex);
				allocNext = allocNext->link;
			}
			reqNext = reqNext->link;
		}
	}

	printResult();

	cycleCheck(pnum);

	for (i = 0; i <= pnum; i++)
		freeList(&graph[i]);
	free(graph);
	free(visited);
	free(cycle);
	free(next);

	return 0;
}