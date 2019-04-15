// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

typedef struct _closenessInfo{
	int n;
	double sum;
}closenessInfo;

static int countNodes (AdjList target);
static closenessInfo getInfo (Graph g, int target);

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
		closenessInfo info = getInfo(g, i);
		double top = (info.n-1)*(info.n-1);
		double bot = (closeness.noNodes - 1);
		double factor = top/bot;
		factor = factor/info.sum;
		closeness.values[i] = factor;
	}
	return closeness;
}

NodeValues betweennessCentrality(Graph g){
	NodeValues throwAway = {0};
	return throwAway;
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

static closenessInfo getInfo (Graph g, int target) {
	ShortestPaths path = dijkstra(g, target);
	closenessInfo info;
	double sum = 0;
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
