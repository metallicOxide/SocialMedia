// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

// struct to store information for closeness
typedef struct _closenessInfo{
	double n;
	double sum;
}closenessInfo;

static int countNodes (AdjList target);
static closenessInfo getClosenessInfo (Graph g, int target);
static NodeValues getBetweenInfo (ShortestPaths *path, NodeValues nodes);
static double inList (PredNode *list, int target);
static int listSize (PredNode *list);

static int countNodes (AdjList target) {
	AdjList curr = target;
	int count = 0;
	while(curr != NULL) {
		count ++;
		curr = curr->next;
	}
	return count;
}
NodeValues outDegreeCentrality(Graph g){
	NodeValues out;
	out.noNodes = numVerticies(g);
	out.values = calloc(out.noNodes, sizeof(double));
	for (int i = 0; i < out.noNodes; i++) {
		out.values[i] = countNodes(outIncident(g, i));
	}
	return out;
}
NodeValues inDegreeCentrality(Graph g){
	NodeValues in;
	in.noNodes = numVerticies(g);
	in.values = calloc(in.noNodes, sizeof(double));
	for (int i = 0; i < in.noNodes; i++) {
		in.values[i] = countNodes(inIncident(g, i));
	}
	return in;
}
NodeValues degreeCentrality(Graph g) {
	NodeValues value;
	value.noNodes = numVerticies(g);
	value.values = calloc(value.noNodes, sizeof(double));
	for (int i = 0; i < value.noNodes; i++) {
		value.values[i] = countNodes(inIncident(g, i)) + countNodes(outIncident(g, i));
	}
	return value;
}

NodeValues closenessCentrality(Graph g){
	NodeValues closeness;
	closeness.noNodes = numVerticies(g);
	closeness.values = calloc(closeness.noNodes, sizeof(double));
	for (int i = 0; i < closeness.noNodes; i++) {
		closenessInfo info = getClosenessInfo(g, i);
		// factors to consider, would weights ever be decimal?
		double factor = (info.n-1)*(info.n-1)/(closeness.noNodes-1);
		factor = factor/info.sum;
		closeness.values[i] = factor;
	}
	return closeness;
}

NodeValues betweennessCentrality(Graph g){
	NodeValues between;
	between.noNodes = numVerticies(g);
	between.values = calloc(between.noNodes, sizeof(double));
	ShortestPaths *path = malloc(numVerticies(g) * sizeof(struct ShortestPaths));
	double total , num , dem;
	total = num = dem = 0;
	for (int i = 0; i < between.noNodes; i++) path[i] = dijkstra(g, i);
	

	return between;
	// NodeValues throwAway = {0};
	// return throwAway;
}

NodeValues betweennessCentralityNormalised(Graph g){
	NodeValues throwAway = {0};
	return throwAway;
}

void showNodeValues(NodeValues values){
	for (int i = 0; i < values.noNodes; i++) {
		printf("%d: %lf\n", i, values.values[i]);
	}
}

void freeNodeValues(NodeValues values){
	free(values.values);
}

static closenessInfo getClosenessInfo (Graph g, int target) {
	ShortestPaths path = dijkstra(g, target);
	closenessInfo info;
	int sum = 0;
	// connection to itself
	int n = 1;
	for (int i = 0; i < path.noNodes; i++) {
		sum += path.dist[i];
		if (path.dist[i] != 0) n++;
	}
	info.n = n;
	info.sum = (sum == 0) ? 1 : sum;
	// printf("sum: %lf\n", sum);
	freeShortestPaths(path);
	// printf("sum: %lf\n", sum);
	return info;
}

static NodeValues getBetweenInfo (ShortestPaths *path) {
	NodeValues nodes;
	nodes.noNodes = path->noNodes;
	nodes.values = malloc(sizeof(double) * nodes.noNodes);
	for (int i = 0; i < nodes.noNodes; i++) {
		ShortestPaths currPath = path[i];

	}
	return nodes;
}

static NodeValues 






















static double inList (PredNode *list, int target) {
	// printf("is it in list?\n");
	PredNode *curr = list;
	while (curr != NULL) {
		if (curr->v == target) return 1;
		curr = curr->next;
	}
	return 0;
}

static int listSize (PredNode *list) {
	// printf("calculating list size\n");
	int size = 0;
	PredNode *curr = list;
	while (curr != NULL) {
		size++;
		curr = curr->next;
	}
	return size;
}
