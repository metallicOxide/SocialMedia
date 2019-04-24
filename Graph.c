// Graph ADT interface for Ass2 (COMP2521)
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "Graph.h"

typedef struct GraphRep {
   Vertex    nV;      // num of vertices
   int    nE;      // num of edges
   AdjList *nodesOut;  // array of out going nodes
   AdjList *nodesIn;  // array of in comming
} GraphRep;

// helper function to make connection
static adjListNode* makeConnec(Vertex v, int weight);

// helper to free list
static void freeAdjList(adjListNode *target);

Graph newGraph(int noNodes) {
    // make sure input is valid
    assert(noNodes > 0);
    // allocate memory for graph
    Graph g = malloc(sizeof(struct GraphRep));
    assert(g != NULL);
    // set initial condition for number of nodes and edges
    g->nV = noNodes; g->nE = 0;
    // allocate memory for array of lists (adj list)
    g->nodesOut = malloc(noNodes * sizeof(struct _adjListNode));
    g->nodesIn = malloc(noNodes * sizeof(struct _adjListNode));      
    // set every element in adj list to 0
    for (int i = 0; i < noNodes; i++) {
        g->nodesOut[i] = NULL;
        g->nodesIn[i] = NULL;
    }

    return g;
}

// helper function to make connection
static adjListNode* makeConnec(Vertex v, int weight) {
    adjListNode* new = malloc(sizeof(struct _adjListNode));
    new->w = v; new->weight = weight; new->next = NULL;
    return new;
}

void insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
    assert(g != NULL && src >= 0 && dest >= 0);
    
    AdjList connectionOut = makeConnec(dest, weight); // make new connection
    AdjList temp = g->nodesOut[src]; // store first connection in temp variable
    g->nodesOut[src] = connectionOut; // make new connection the first connection
    connectionOut->next = temp; // append old connections to next of new connection

    AdjList connectionIn = makeConnec(src, weight);
    temp = g->nodesIn[dest];
    g->nodesIn[dest] = connectionIn;
    connectionIn->next = temp;

    g->nE += 1; // increase number of edges by 1 
}

void removeEdge(Graph g, Vertex src, Vertex dest) {
    assert(g != NULL && src >= 0 && dest >= 0);
    
    // for outgoing
    AdjList curr = g->nodesOut[src];
    AdjList prev = NULL;
    while (curr != NULL) {
        // case for Out going nodes
        // node being deleted is first node
        if (curr->w == dest && prev == NULL) {
            g->nodesOut[src] = curr->next;
            // printf("%d\n", curr->w);
            free(curr);
            break;
        } else if (curr->w == dest) {
            // delete no first node
            // printf("%d\n",curr->w);
            prev->next = curr->next;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    // for incoming
    curr = g->nodesIn[dest];
    prev = NULL;
    while (curr != NULL) {
        // case for Out going nodes
        // node being deleted is first node
        if (curr->w == src && prev == NULL) {
            g->nodesIn[dest] = curr->next;
            free(curr);
            break;
        } else if (curr->w == src) {
            // delete no first node
            prev->next = curr->next;
            free(curr);
            break;
        }
        prev = curr;
        curr = curr->next;
    }

    g->nE -= 1;
}

bool adjacent(Graph g, Vertex src, Vertex dest) {
    assert(g != NULL && src >= 0 && dest >= 0);
    AdjList curr = g->nodesOut[src];
    // see if it's in the list or nah
    while (curr != NULL) {
        if (curr->w == dest) return true;
        curr = curr->next;
    }
    return false;
}

int numVerticies(Graph g) {
    assert(g != NULL);
    return g->nV;
}

AdjList outIncident(Graph g, Vertex v) {
    assert (v >= 0 && g != NULL);
    return g->nodesOut[v];
}

AdjList inIncident(Graph g, Vertex v) {
    assert (v >= 0 && g != NULL);
    return g->nodesIn[v];
}

void  showGraph(Graph g) {


}

// helper function to free adjLists for a given node
static void freeAdjList(adjListNode *target) {
	// if input is null, we don't have to do shit
	if (target == NULL) return;
	adjListNode *start = target;
	adjListNode *nxt = target->next;
	while(start != NULL){
		free(start);
		start = nxt;
        if (nxt != NULL) nxt = nxt->next;
	}
}
void  freeGraph(Graph g) {
	// free lists within nodes array
	for (int i = 0; i < g->nV; i++) {
        freeAdjList(g->nodesOut[i]);
        freeAdjList(g->nodesIn[i]);
    }
    // printf("finished freeing\n");
	// free memory allocated for nodes
	free(g->nodesOut);
    free(g->nodesIn);
	free(g);
}
