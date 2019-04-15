// Graph ADT interface for Ass2 (COMP2521)
#include "CentralityMeasures.h"
#include "Dijkstra.h"
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

static int countNodes (AdjList target);

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
	out.values = malloc(sizeof(double) * out.noNodes);
	for (int i = 0; i < out.noNodes; i++) {
		out.values[i] = countNodes(outIncident(g, i));
	}
	return out;
}
NodeValues inDegreeCentrality(Graph g){
	NodeValues in;
	in.noNodes = numVerticies(g);
	in.values = malloc(sizeof(double) * in.noNodes);
	for (int i = 0; i < in.noNodes; i++) {
		in.values[i] = countNodes(inIncident(g, i));
	}
	return in;
}
NodeValues degreeCentrality(Graph g) {
	NodeValues throwAway = {0};
	return throwAway;
}

NodeValues closenessCentrality(Graph g){
	NodeValues throwAway = {0};
	return throwAway;
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

}

void freeNodeValues(NodeValues values){

}
