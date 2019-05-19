#include <stdio.h>
#include <stdlib.h>
#define FALSE 0
#define TRUE 1

typedef struct node *nodePointer;

typedef struct node {
	int vertex;
	int weight;
	nodePointer link;
};

typedef struct {
	int index;
	box* link;
}box;

typedef struct{
	box request;
	box allocate;
}waitGraph;

nodePointer *graph = NULL;
waitGraph *wait = NULL;

int *visited = NULL;
int *cycle = NULL, cnt = 0;
int next,rlast;

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

void addNode(int v, int u, int w) 
{
	nodePointer node, trail = NULL, temp = graph[v];

	node = (nodePointer)malloc(sizeof(struct node));
	node->vertex=u;
	node->weight = w;
	node->link = NULL;

	if (!temp)
		graph[v] = node;
	else {
		search(node->vertex, &trail, &temp);

		if (trail == NULL) {
			graph[v] = node;
			node->link = temp;
		}
		else {
			trail->link = node;
			node->link = temp;
		}
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

	visited = (int*)malloc((num+1)*sizeof(int));
	cycle = (int*)malloc((num+1)*sizeof(int));

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

int get_num(FILE *f)
{
	char a1,b1;
	int a2,b2,plast=0;
	int i,j;

	while(!feof(f))
	{
		fscanf(f,"%c%d %c%d ",&a1,&a2,&b1,&b2);
		if(a1=='P'){
			if(a2>plast)
				plast=a2;
		}
		else if(a1=='R')
			if(a2>rlast)
				rlast=a2;
		
		if(b1=='P'){
			if(b2>plast)
				plast=b2;
		}
		else if(b1='R')
			if(b2>rlast)
				rlast=b2;
	}

	fclose(f);
	/* wait_graph 초기화 */
	wait=(waitGraph*)malloc(sizeof(waitGraph)*(rlast+1));
	return plast;
}

int main()
{
	int num, i;
	int v, u, w;
	FILE *f;
	char u1,v1;
	int u2,v2;
	box* last,*temp;

	f = fopen("input.txt", "r");
	num=get_num(f);
	fclose(f);

	f = fopen("input.txt", "r");
	graph = (nodePointer*)malloc((num+1)*sizeof(nodePointer));
	
	for (i = 0; i <= num; i++){
		graph[i] = NULL;
	}
	/*************************이부분 다시********************************/
	while(!feof(f)){
		fscanf(f,"%c%d %c%d ",&u1,&u2,&v1,&v2);
		if(u1=='R')
		{
			if(wait[u2].allocate.index == 0){
				wait[u2].allocate.index=v2;
				wait[u2].allocate.link=NULL;
			}
			
			else{
				last=wait[u2].allocate.link;
				while(last->link!=NULL){
					last=last->link;
				}
				temp = (box*)malloc(sizeof(box));
				temp->index=v2;
				temp->link=NULL;
				last->link=temp;
			}
		}
		else if(v1='R')
		{
			if(wait[v2].allocate.index == 0){
				wait[v2].allocate.index=u2;
				wait[v2].allocate.link=NULL;
			}
			
			else{
				last=wait[v2].allocate.link;
				while(last->link!=NULL){
					last=last->link;
				}
				temp=(box*)malloc(sizeof(box));
				temp->index=u2;
				temp->link=NULL;
				last->link=temp;
			}
		}
	}
	/*************************************************************/
	for(i=1;i<=rlast;i++){
		while(wait[i].allocate.link!=NULL)
		{
			while(wait[i].request.link!=NULL)
			{
				if(wait[i].allocate.index!=0 && wait[i].request.index!=0){
					addNode(wait[i].request.index, wait[i].allocate.index, 0);
				}
			}
		}

	}

	fclose(f);

	if (!cycleCheck(num))
		printf("N\n");
	else {
		printf("P%d ", cycle[0]);

		if (cycle[1] < cycle[cnt-1]) {
			for (i = 1; i < cnt; i++)
				printf("P%d ", cycle[i]);
		}
		else {
			for (i = cnt-1; i > 0; i--)
				printf("P%d ", cycle[i]);
		}
	}

	for (i = 0; i <= num; i++)
		freeList(&graph[i]);
	free(graph);
	free(visited);
	free(cycle);

	return 0;
}