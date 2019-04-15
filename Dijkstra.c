// Dijkstra ADT interface for Ass2 (COMP2521)
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

#define INF -1
static ItemPQ newItem(int key, int value);
static PredNode *makeNode(int v);
static void freeList (PredNode *target);

ShortestPaths dijkstra(Graph g, Vertex v) {
	ShortestPaths paths;
	PQ pq = newPQ();
	paths.noNodes = numVerticies(g);
	paths.src = v;
	paths.dist = malloc(sizeof(int) * paths.noNodes);
	paths.pred = malloc(sizeof(struct PredNode) * paths.noNodes);

	// Initalize distances of all vertices as infinite and pred as NULL
	for (int i = 0; i < paths.noNodes; i++)  {
		paths.dist[i] = INF;
		paths.pred[i] = NULL;
	}

	// add source into pq with cost as 0
	ItemPQ item = newItem(v, 0);
	addPQ(pq, item);
	// set distance from source to source as 0
	paths.dist[paths.src] = 0;
	while (!PQEmpty(pq)) {
		// pop edge from queue
		ItemPQ popEdge = dequeuePQ(pq);
		AdjList out = outIncident(g, popEdge.key);

		// printf("popped edge: %d\n", popEdge.key);

		// for each edge going out from popped edge
		while (out != NULL) {

			// printf("out pointer: %p\t", out);
			// printf("out w: %d\t", out->w);
			// printf("out weight: %d\t distance from out: %d\n", out->weight, paths.dist[out->w]);
			
			// if distance not updated yet, set distance as 
			// distance from source to vertex and predecessor as 
			// source. Then add edge to pq 
			if (paths.dist[out->w] == INF || paths.dist[out->w] > out->weight + paths.dist[popEdge.key]) {
				paths.dist[out->w] = out->weight + paths.dist[popEdge.key];
				// printf("new dist %d\n", paths.dist[out->w]);
				if (paths.pred[out->w] == NULL) {
					if (out->w == 11) {
						// printf("adding new predecessors %d\n", popEdge.key);
						// printf("distance from src to 11 is: %d\n", paths.dist[11]);
					}
					paths.pred[out->w] = makeNode(popEdge.key);
				} else {
					// printf("updating predecessors from %d to %d\n", paths.pred[out->w]->v, popEdge.key);
					// printf("distance from src to 11 is: %d\n", paths.dist[11]);
					freeList(paths.pred[out->w]);
					paths.pred[out->w] = makeNode(popEdge.key);
				}
				ItemPQ temp;
				temp.key = out->w;
				temp.value = paths.dist[out->w];
				// printf("adding temp key: %d \t temp value: %d\n", temp.key, temp.value);
				addPQ(pq, temp);
				// showPQ(pq);
			} else if (paths.dist[out->w] == out->weight + paths.dist[popEdge.key] && popEdge.key != paths.pred[out->w]->v) {
				int seen = 0;
				if (out->w == 11) {
					// printf("paths.dist[out-w]: %d\t out->weight: %d\t paths.dist[popEdge.key]: %d\n", paths.dist[out->w], out->weight, paths.dist[popEdge.key]);
					// printf("equal distance, adding in %d\n", popEdge.key);

				}
				PredNode *curr = paths.pred[out->w];
				while(curr != NULL) {
					if (curr->v == popEdge.key) seen = 1;
					if (curr->next == NULL && seen == 0) curr->next = makeNode(popEdge.key);
					curr = curr->next;
				}
			}
			out = out->next;
		}
	}
	// set the distance of inifinite to 0 at end of Dijkstras
	for (int i = 0; i < paths.noNodes; i++) {
		if (paths.pred[i] == NULL && paths.dist[i] == INF) {
			paths.dist[i] = 0;
		}
	}
	freePQ(pq);
	return paths;
}

static PredNode *makeNode(int v) {
	PredNode *new = malloc(sizeof(struct PredNode));
	new->v = v;
	new->next = NULL;
	return new;
}

static void freeList (PredNode *target) {
	PredNode *curr = target;
	PredNode *temp = curr;
	while(curr != NULL) {
		curr = curr->next;
		free(temp);
		temp = curr;
	}
}
ItemPQ newItem(int key, int value) {
	ItemPQ item;
	item.key = key;
	item.value = value;
	return item;
}
void showShortestPaths(ShortestPaths paths) {

}


void  freeShortestPaths(ShortestPaths paths) {
	free(paths.dist);
	for (int i = 0; i < paths.noNodes; i++) freeList(paths.pred[i]);
}
