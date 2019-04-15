// Graph ADT
// Adjacency Matrix Representation ... COMP2521 
#include "Graph.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct GraphRep {
   Vertex    nV;      // num of vertices
   int    nE;      // num of edges
   AdjList *nodes  // array of adjacent lists
} GraphRep;

static adjListNode* makeNode(Vertex v, int weight);

Graph newGraph(int noNodes) {
    // make sure input is valid
    assert(noNodes > 0);
    // allocate memory for graph
    Graph g = malloc(sizeof *g);
    assert(g != NULL);
    // set initial condition for number of nodes and edges
    g->nV = noNodes; g->nE = 0;
    // allocate memory for array of lists (adj list)
    g->nodes = malloc(noNodes * sizeof(AdjList));   
    // set every element in adj list to 0
    for (int i = 0; i < noNodes; i++) g->nodes[i] = NULL;

    return g;
}

static adjListNode* makeNode(Vertex v, int weight) {
    adjListNode* new = malloc(sizeof *new);
    new->w = v; new->weight = weight; new->next = NULL;
    return new;
}

void insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
    assert(g != NULL && src > 0 && dest > 0);
    
    AdjList connection = makeNode(dest, weight); // make new connection
    AdjList temp = g->nodes[src]; // store first connection in temp variable
    g->nodes[src] = connection; // make new connection the first connection
    connection->next = temp; // append old connections to next of new connection
    g->nE += 1; // increase number of edges by 1 
}

void removeEdge(Graph g, Vertex src, Vertex dest) {
    assert(g != NULL && src > 0 && dest > 0);
    
    AdjList curr = g->nodes[src];
    AdjList prev = NULL;
    while (curr != NULL) {
        if (curr->w == dest && prev == NULL) {
            // if you wanna delete first node
            g->nodes[src] = curr->next;
            free(curr);
        } else if (curr->w == dest) {
            // delete no first node
            prev->next = curr->next;
            free(curr);
        }
        prev = curr;
        curr = curr->next;
    }
    g->nE -= 1;
}

bool adjacent(Graph g, Vertex src, Vertex dest) {
    assert(g != NULL && src > 0 && dest > 0);
    AdjList start = g->nodes[src];
    AdjList curr = start;
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
    assert (v > 0 && g != NULL);
    return g->nodes[v];
}

AdjList inIncident(Graph g, Vertex v) {
    assert (v > 0 && g != NULL);
    
    return NULL;
}

