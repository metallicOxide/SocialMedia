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
		// for each edge going out from popped edge
		while (out != NULL) {
			// if distance not updated yet, set distance as 
			// distance from source to vertex and predecessor as 
			// source. Then add edge to pq 
			if (paths.dist[out->w] == INF || paths.dist[out->w] > out->weight + paths.dist[popEdge.key]) {
				paths.dist[out->w] = out->weight + paths.dist[popEdge.key];
				if (paths.pred[out->w] == NULL) {
					paths.pred[out->w] = makeNode(popEdge.key);
				} else {
					freeList(paths.pred[out->w]);
					paths.pred[out->w] = makeNode(popEdge.key);
				}
				ItemPQ temp;
				temp.key = out->w;
				temp.value = paths.dist[out->w];
				addPQ(pq, temp);
			} else if (paths.dist[out->w] == out->weight + paths.dist[popEdge.key] && popEdge.key != paths.pred[out->w]->v) {
				int seen = 0;
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
	printf("src is: %d\t numNodes: %d\n", paths.src, paths.noNodes);
	printf("distances: \n");
	for (int i = 0; i < paths.noNodes; i++) {
		printf("[%d]: %d\n", i, paths.dist[i]);
	}
	printf("preds: \n");
	for (int j = 0; j < paths.noNodes; j++) {
		printf("[%d]: ", j);
		PredNode *curr = paths.pred[j];
		while (curr != NULL) {
			printf("%d -> ", curr->v);
			curr = curr->next;
		} 
		printf("NULL\n");
	}
}


void  freeShortestPaths(ShortestPaths paths) {
	free(paths.dist);
	for (int i = 0; i < paths.noNodes; i++) freeList(paths.pred[i]);
	free(paths.pred);
}
