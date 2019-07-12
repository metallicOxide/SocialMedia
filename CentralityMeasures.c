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

// note: the betweeness algorithm works like this:
// adjusted Prednode -> predNode where everything in predecessor array
// is traced back and replaced so that predecessor array is comprised of 
// source and target nodes:
// ie 
// source = 3, target = 1
// [4]: 1->3->NULL
// [0]: 1->3->4->NULL
// adustPred:
// [0]: 1->3->1->3->NULl

// from that point you can simply count number of targets in your list/number of paths in list
// to get the number of path blocked if you remove target
// If target has multiple predecessors, then you have to do:
// number of factor in list * number of predecessor of factor/ number of elements in list
// where number of elements in list is 
// (number of non-target nodes + nnumber of factors in list * number of precessors of factor) 

// structure to store adjustedPrednode 
typedef struct _adjustedPrednode {
	PredNode *nodes;
} adjPrednode;


// count number of nodes in Adjlist
static int countNodes (AdjList target);
// fills closeness info datastructure
static closenessInfo getClosenessInfo (Graph g, int target);
// count number of node in predNode list
static double listSize (PredNode *target);
// count number of paths blocked if target vertex is removed
static double countPath (ShortestPaths path, int target);
// returns 1*factor if predNode list contains target, 0 if it does not
static double pathConains (PredNode *list, int target, double factor);
// counts number of total paths spanning from target
static double totalPaths (PredNode *list, int target, double factor);
// adjusts predNode array so only source and target is in the array
static PredNode *adjustPred (int target, int src, ShortestPaths path, PredNode *pre);
// makes a new predNode with vertex v
static PredNode *makeNode(int v);
// finds predNode with v == target, appends dest list on node with v == target
static PredNode *appendList (PredNode *src, PredNode *dest, int target);
// returns a copy of source list
static PredNode *copyList (PredNode *src);
// frees adjacent list
static void freeList (PredNode *target);
// frees adjusted Prednodes
static void freeAdjPrednode (adjPrednode *target, int size);

// helper functions to help with debugging, commented out to 
// prevent errors

// static void printPredNodes (adjPrednode *target, int size);
// static void printPredlist (PredNode *target);

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
		double factor = (info.n-1)*(info.n-1)/(closeness.noNodes-1);
		factor = factor/info.sum;
		closeness.values[i] = factor;
	}
	return closeness;
}

NodeValues betweennessCentrality(Graph g){
	NodeValues betweeness;
	betweeness.noNodes = numVerticies(g);
	betweeness.values = calloc(betweeness.noNodes, sizeof(double));
	int i, j;
	for (i=0; i < betweeness.noNodes; i++) {
		double total = 0;
		for( j=0; j <betweeness.noNodes; j++) {
			// we don't need to count paths spanning from 
			// target node 
			if (j == i) j++;
			if (j == betweeness.noNodes) break;
			ShortestPaths path = dijkstra(g, j);
			total += countPath(path, i);
			freeShortestPaths(path);
		}
		betweeness.values[i] = total;
	}
	return betweeness;
}

NodeValues betweennessCentralityNormalised(Graph g){
	NodeValues normBetweeness = betweennessCentrality(g);
	double dem = (numVerticies(g)-1) * (numVerticies(g)-2);
	double factor = 1/dem;
	for (int i = 0; i < normBetweeness.noNodes; i++) {
		normBetweeness.values[i] = normBetweeness.values[i]*factor;
	}
	return normBetweeness;
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
	freeShortestPaths(path);
	return info;
}

static double countPath(ShortestPaths path, int target) {
	int i;
	// allocating necessary space
	adjPrednode *adjustedPath = malloc(sizeof(struct _adjustedPrednode) * path.noNodes); 
	// copying preds from dijkstra to adjusted preds
	for (i = 0; i < path.noNodes; i++) {
		adjustedPath[i].nodes = copyList(path.pred[i]);
	}

	// adjusting adjustedpreds so that it only contains source or target
	int src = path.src;
	for (i = 0; i < path.noNodes; i++) {
		if (i == target || i == src) i++;
		if (i != path.noNodes) adjustedPath[i].nodes = adjustPred (target, src, path, adjustedPath[i].nodes);
	}
	PredNode *temp = adjustPred(target, src, path, copyList(path.pred[target]));
	double factor =  listSize(temp);
	double totalPath = 0;
	// double pathContains = 0;
	double sum = 0;

	// counting the number of paths avaliable
	for (i = 0; i < path.noNodes; i++) {
		if (i == src) i++;
		if (i == path.noNodes) break;
		totalPath = totalPaths(adjustedPath[i].nodes, target, factor);
		if (totalPath != 0) {
			sum += pathConains(adjustedPath[i].nodes, target, factor) / totalPath;
		}
	}
	freeList(temp);
	freeAdjPrednode(adjustedPath, path.noNodes);
	return sum;
}

// recursively calls upon itself to make sure that every element in node is adjusted
static PredNode *adjustPred (int target, int src, ShortestPaths path, PredNode *pre) {
	if (pre == NULL) return NULL;
	PredNode *curr = pre;

	while (curr != NULL) {
		if (curr->v != src && curr->v != target) {
			pre = appendList(path.pred[curr->v], pre, curr->v);
			// start from beginning again to see if there is anymore
			// that needs adjusting
			pre = adjustPred(target, src, path, pre);
			curr = pre;
		}
		curr = curr->next;
	}
	return pre;
}

static double totalPaths (PredNode *list, int target, double factor) {
	double sum = 0;
	PredNode *curr = list;
	while (curr != NULL) {
		if (curr->v == target) {
			sum += factor;
		} else {
			sum += 1;
		}
		curr = curr->next;
	}
	return sum;
}

static double pathConains (PredNode *list, int target, double factor) {
	double sum = 0;
	PredNode *curr = list;
	while (curr != NULL) {
		if (curr->v == target) {
			sum += factor;
		}
		curr = curr->next;
	}
	return sum;
}

static double listSize (PredNode *target) {
	double sum = 0;
	PredNode *curr = target;
	while (curr != NULL) {
		sum++;
		curr = curr->next;
	}
	return sum;
}

static PredNode *makeNode(int v) {
	PredNode *new = malloc(sizeof(struct PredNode));
	new->v = v;
	new->next = NULL;
	return new;
}

static PredNode *copyList (PredNode *src) {
	if (src == NULL) return NULL;
	PredNode *result = makeNode(src->v);
	PredNode *temp = result;
	PredNode *curr = src->next;
	while (curr != NULL) {
		temp->next = makeNode(curr->v);
		temp = temp->next;
		curr = curr->next;
	}
	return result;
}

static PredNode *appendList (PredNode *src, PredNode *dest, int target) {
	if (dest == NULL) return NULL;
	if (src == NULL) return NULL;
	PredNode *curr = dest;
	PredNode *prev = NULL;
	while (curr->v != target) {
		prev = curr;
		curr = curr->next;
		if (curr == NULL) return dest;
	}
	// temp is beginning of copied list
	PredNode *temp = copyList(src);
	PredNode *end = temp;
	// find end of copied list
	while (end->next != NULL) end = end->next;
	end->next = curr->next;
	if (prev == NULL) {
		dest = temp;
	} else {
		prev->next = temp;
	}
	free(curr);
	return dest;
}

static void freeList (PredNode *target) {
	if (target == NULL) return;
	PredNode *start = target;
	PredNode *nxt = target->next;
	while(start != NULL){
		free(start);
		start = nxt;
        if (nxt != NULL) nxt = nxt->next;
	}
}

static void freeAdjPrednode (adjPrednode *target, int size) {
	for (int i = 0; i < size; i++) {
		freeList(target[i].nodes);
	}
	free(target);
}


// static void printPredNodes (adjPrednode *target, int size) {
// 	for(int i = 0; i < size; i++) {
// 		PredNode *curr = target[i].nodes;
// 		if (curr == NULL) continue;
// 		printf("%d :", i);

// 		printPredlist(curr);
// 	}
// }

// static void printPredlist (PredNode *target) {
// 	PredNode *curr = target;
// 	while (curr != NULL) {
// 		printf("[%d]->", curr->v);
// 		curr = curr->next;
// 	}
// 	printf("NULL\n");
// }
