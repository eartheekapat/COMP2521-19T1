// Graph.c ... implementation of Graph ADT
// Written by John Shepherd, May 2013

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Graph.h"
#include "Queue.h"

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no edge)
} GraphRep;

static int makepath(int *path,int *pred, Vertex dest);
static void reverseArray(int arr[], int start, int end);
// check validity of Vertex
int validV (Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}

// make an edge
static Edge mkEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	Edge new = {v, w}; // struct assignment
	return new;
}

// insert an Edge
// - sets (v,w) and (w,v)
void insertEdge (Graph g, Vertex v, Vertex w, int wt)
{
	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0 && g->edges[w][v] != 0)
		// an edge already exists; do nothing.
		return;

	g->edges[v][w] = wt;
	g->edges[w][v] = wt;
	g->nE++;
}

// remove an Edge
// - unsets (v,w) and (w,v)
void removeEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	if (g->edges[v][w] == 0 && g->edges[w][v] == 0)
		// an edge doesn't exist; do nothing.
		return;

	g->edges[v][w] = 0;
	g->edges[w][v] = 0;
	g->nE--;
}

// create an empty graph
Graph newGraph (int nV)
{
	assert (nV > 0);

	GraphRep *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (GraphRep){ .nV = nV, .nE = 0 };

	new->edges = calloc ((size_t) nV, sizeof (int *));
	assert (new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc ((size_t) nV, sizeof (int));
		assert (new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph
void dropGraph (Graph g)
{
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++)
		free (g->edges[v]);
	free (g->edges);
	free (g);
}

// display graph, using names for vertices
void showGraph (Graph g, char **names)
{
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, names[v]);
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%d)\n", names[w], g->edges[v][w]);
			}
		}
		printf ("\n");
	}
}

// find a path between two vertices using breadth-first traversal
// only allow edges whose weight is less than "max"
int findPath (Graph g, Vertex src, Vertex dest, int max, int *path)
{
	assert (g != NULL);
	int *visited = calloc(g->nV,sizeof(int));
	int *pred = calloc(g->nV,sizeof(int));
	for(int i=0; i<g->nV; i++){
		pred[i] = -1;
	}
	Queue q = newQueue();
	visited[src] = 1;
	QueueJoin(q,src);
	
	while(!QueueIsEmpty(q)){
		Item current = QueueLeave(q);
		for (Item next = 0; next < g->nV; next++){
			//printf("Dis %d\n",(int)(g->edges[current][next]));
			if(g->edges[current][next]!=0 && (int)(g->edges[current][next]) < max){
				if(!visited[next]){
					//TO PREVENT GOING BACK
					visited[next] = 1;
					pred[next] = current;
					//printf("We come to %d via %d\n",next,current);
					//if(next == src) break;
					//printf("%d JOIN THE Q\n",next);
					QueueJoin(q,next);
				}
			}
		}
	}

	int total = makepath(path,pred,dest);
	if(total > 0 || src == dest){
		path[0] = src;
		total++;
	}
	free(visited);
	free(pred);
	dropQueue(q);
	return total;
}

static int makepath(int *path,int *pred, Vertex dest){

	if(pred[dest] == -1){
		return 0;
	}
	int a = makepath(path,pred,pred[dest])+1;
	//printf("a=%d\n",a);
	path[a] = dest;
	//printf("Path[%d] = %d\n",a,dest);
	return a;
}

static void reverseArray(int arr[], int start, int end) 
{ 
    int temp; 
    while (start < end) 
    { 
        temp = arr[start];    
        arr[start] = arr[end]; 
        arr[end] = temp; 
        start++; 
        end--; 
    }    
}      
