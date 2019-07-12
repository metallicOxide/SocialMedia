/*Lance-Williams Algorithm for Agglomerative Clustering
  Written by 
  COMP2521 2019 T1
*/
#include <stdlib.h>
#include <stdio.h>
#include "LanceWilliamsHAC.h"
#include "Graph.h"
#define numVertices numVerticies
#define NONE -2

typedef struct _node {
	int vertex;
	struct _node *next;
} node;

typedef struct _pairs {
    int src;
    int dest;
    double distance;
}Pairs;

typedef struct _pairsList {
	int size;
	Pairs *pair;
}pairsList;

typedef struct _cluster {
	Dendrogram dendro;
	node *vertList;
	struct _cluster *next; 
} clusterNode;

typedef struct _clusterList {
	clusterNode *head;
} clusterList;

static Dendrogram SingleLink (Graph g);
static Dendrogram CompleteLink (Graph g);
static Pairs *sort (int size, Pairs *array);
static Pairs *reverseSort (int size, Pairs *array);
static Dendrogram newDendrogram (int v);
static node *makeNode (int v);
static int inNodeList (node *list, int target);
static clusterNode *makeClusterNode (int v);
static clusterList makeClusterList (int numVert);
// mode 0 for singlelink, 1 for complete link
static pairsList getDistances (Graph g, int mode);
static pairsList singleinsertPair (pairsList list, adjListNode *curr, int src);
static pairsList completeinsertPair (pairsList list, adjListNode *curr, int src);
// static void printpairsList (pairsList list);
// static void printpairs(Pairs *list, int size);
static Dendrogram mergeDendo (Dendrogram src, Dendrogram dest, clusterNode *head);
static void mergeCluster(clusterNode *src, clusterNode *dest, clusterNode *head);
static clusterNode *deleteCluster(clusterNode *head, clusterNode *target);
// static void printNodeList (node *head);
// static void printClusterList (clusterNode *head);


/* 
 * Finds Dendrogram using Lance-Williams algorithm (as discussed in the specs) 
   for the given graph g and the specified method for agglomerative clustering.
   Method value 1 represents 'Single linkage', and 2 represents 'Complete linkage'.
   For this assignment, you only need to implement the above two methods. 

   The function returns 'Dendrogram' structure (binary tree) with the required information.
 * 
 */
Dendrogram LanceWilliamsHAC(Graph g, int method) {
	if (method == 1) {
		return SingleLink(g);
	} else if (method == 2) {
		return CompleteLink(g);
	} else {
		return NULL;
	}
}

static Dendrogram SingleLink (Graph g) {
	// allocate vertcies as individual 'clusters'
	clusterList clist = makeClusterList(numVertices(g));
	// get distances between all vertices (finds smallest connection between 1->2 and 2->1 etc)
	pairsList plist = getDistances(g, 0);
	// sort them in accending order
	
	// printClusterList(clist.head);
	plist.pair = sort(plist.size, plist.pair);
	for (int i = 0; i < plist.size; i++) {
		Pairs target = plist.pair[i];
		clusterNode *curr = clist.head;
		clusterNode *source = NULL;
		clusterNode *destination = NULL;
		while (curr != NULL) {
			// if source is in cluster list and destination node is not in cluster list
			if (inNodeList(curr->vertList, target.src) && 
				!inNodeList(curr->vertList, target.dest)) {
				// printf("found source: %d\n", curr->vertList->vertex);
				source = curr;
			} else if (inNodeList(curr->vertList, target.dest) && 
				!inNodeList(curr->vertList, target.src)) {
				// printf("found dest: %d\n", curr->vertList->vertex);
				destination = curr;
			}
			curr = curr->next;
		}
		if (source != NULL && destination != NULL) {
			mergeCluster(source, destination, clist.head);
			// printf("merged cluster\n");
			clist.head = deleteCluster(clist.head, destination);
			// printf("deleted cluster lol\n");
		}
		// printf("sdad");

		// printClusterList(clist.head);
		// printf("lol");

		if (clist.head->next == NULL) i = plist.size;
	}
	Dendrogram result = clist.head->dendro;
	free(plist.pair);
	clusterNode *curr = clist.head;
	while(curr != NULL) {
		node *currnode = curr->vertList;
		while (currnode != NULL) {
			node *temp = currnode->next;
			free(currnode);
			currnode = temp;
		}
		curr = curr->next;
	}
	free(clist.head);
	return result;
}

static Dendrogram CompleteLink (Graph g) {
	clusterList clist = makeClusterList(numVertices(g));
	printf("%p", clist.head);
	pairsList plist = getDistances(g, 1);
	plist.pair = reverseSort(plist.size, plist.pair);
	return NULL;
}

static Pairs *sort (int size, Pairs *array) {
    Pairs temp;
	for (int i = 0; i < size; i++) {
		for (int j = size-1; j > i; j--) {
			if (array[j].distance < array[j-1].distance) {
				temp = array[j];
				array[j] = array[j-1];
				array[j-1] = temp;
			}
		}
	}
    return array;
}

static Pairs *reverseSort (int size, Pairs *array) {
    Pairs temp;
	for (int i = 0; i < size; i++) {
		for (int j = size-1; j > i; j--) {
			if (array[j].distance > array[j-1].distance) {
				temp = array[j];
				array[j] = array[j-1];
				array[j-1] = temp;
			}
		}
	}
    return array;
}

static Dendrogram newDendrogram (int v) {
    Dendrogram dendro = malloc(sizeof(struct DNode));
    dendro->left = NULL;
    dendro->right = NULL;
    dendro->vertex = v;
    return dendro;
} 

static node *makeNode (int v) {
	node *new = malloc(sizeof(struct _node));
	new->vertex = v;
	new->next = NULL;
	return new;
}

static int inNodeList (node *list, int target) {
	node *curr = list;
	while (curr != NULL) {
		if (curr->vertex == target) return 1;
		curr = curr->next;
	}
	return 0;
}

static clusterNode *makeClusterNode (int v) {
	clusterNode *new = malloc(sizeof(struct _cluster));
	new->dendro = newDendrogram(v);
	new->vertList = makeNode(v);
	new->next = NULL;
	return new;
}

static clusterList makeClusterList (int numVert) {
	clusterList new;
	new.head = makeClusterNode(0);
	clusterNode *curr = new.head;

	for (int i = 1; i < numVert; i++) {
		curr->next = makeClusterNode(i);
		curr = curr->next;
	}
	return new;
}

static pairsList getDistances (Graph g, int mode) {
	// numVertices * numVertices (max number of edges in graph V*(V-1)/2)
	// lets assume graph is dense af, so upperbound V^2
	pairsList distlist;
	distlist.size = 0;
	distlist.pair = malloc(sizeof(struct _pairs) * (numVertices(g) * numVertices(g)));
	for (int i = 0; i < numVertices(g); i++) {
		adjListNode	*curr = outIncident(g, i); 
		while (curr != NULL) {
			if (mode == 0) {
				distlist = singleinsertPair(distlist, curr, i);
			} else if (mode == 1) {
				distlist = completeinsertPair(distlist, curr, i);
			}
			curr = curr->next;
		}
	}
	return distlist;
}

static pairsList singleinsertPair (pairsList list, adjListNode *curr, int src) {
	double dist = 1/((double)curr->weight);
	for (int j = 0; j < list.size; j++) {
		if (src == list.pair[j].dest && curr->w == list.pair[j].src) {
			if (dist < list.pair[j].distance) {
				// only update the distance because src and dest doesn't matter
				list.pair[j].distance = dist;
				list.size += 1;
			}
			// if it's larger or equal, we don't want to add it to end of array either so
			// we return
			return list;
		}
	}
	// not in list, add new connection to end of list
	list.pair[list.size].src = src;
	list.pair[list.size].dest = curr->w;
	list.pair[list.size].distance = dist;
	list.size += 1;
	return list;
}

static pairsList completeinsertPair (pairsList list, adjListNode *curr, int src) {
	double dist = 1/((double)curr->weight);
	for (int j = 0; j < list.size; j++) {
		if (src == list.pair[j].dest && curr->w == list.pair[j].src) {
			// if it is equal, then we don't really wanna do anything
			if (dist > list.pair[j].distance) {
				// only update the distance because src and dest doesn't matter
				list.pair[j].distance = dist;
				list.size += 1;
			}
			// if it's smaller to equal, we don't want to add it to end of array either so
			// we return
			return list;
		}
	}
	// not in list, add new connection to end of list
	list.pair[list.size].src = src;
	list.pair[list.size].dest = curr->w;
	list.pair[list.size].distance = dist;
	list.size += 1;
	return list;
}

static Dendrogram mergeDendo (Dendrogram src, Dendrogram dest, clusterNode *head) {
	// printf("merging dendogram\n");
	Dendrogram newDendro = malloc(sizeof(struct DNode));
	newDendro->vertex = NONE;
	newDendro->left = src;
	newDendro->right = dest;
	// printf("newDendro->left %p\t newDendro->right %p\n",newDendro->left, newDendro->right);
	return newDendro;
}

static void mergeCluster(clusterNode *src, clusterNode *dest, clusterNode *head) {
	// printf("src: %p", src);
	src->dendro = mergeDendo(src->dendro, dest->dendro, head);
	// printf("src->dendro: %p\n", src->dendro);
	node *curr = src->vertList;
	while (curr->next != NULL) {
		curr = curr->next;
	} 
	curr->next = dest->vertList;
}

static clusterNode *deleteCluster(clusterNode *head, clusterNode *target) {
	if (head == target) {
		// printf("head is target\n");
		clusterNode *temp = head->next;
		free(head);
		return temp;
	}
	clusterNode *curr = head->next;
	clusterNode *prev = head;
	while (curr != NULL) {
		if (curr == target) {
			prev->next = curr->next;
			free(curr);
			return head;
		}
		prev = curr;
		curr = curr->next;
	}
	return head;
}

// static void printpairsList (pairsList list) {
// 	for (int i = 0; i < list.size; i++) {
// 		printf("{%d -> %d : %f} , ", list.pair[i].src, list.pair[i].dest, list.pair[i].distance);
// 	}
// 	printf("\n");
// }

// // static void printpairs(Pairs *list, int size) {
// // 	for (int i = 0; i < size; i++) {
// // 		printf("{%d -> %d : %f} , ", list[i].src, list[i].dest, list[i].distance);
// // 	}
// // 	printf("\n");
// // }

// static void printNodeList (node *head) {
// 	node *curr = head;
// 	printf("{ ");
// 	while (curr != NULL) {
// 		printf("%d ", curr->vertex);
// 		curr = curr->next;
// 	}
// 	printf("}");
// }
// static void printClusterList (clusterNode *head) {
// 	clusterNode *curr = head;
// 	while (curr != NULL) {
// 		printf(" [");
// 		printNodeList(curr->vertList);
// 		printf(" dendo:%p left node:%p right node:%p", curr->dendro, curr->dendro->left, curr->dendro->right);
// 		printf("] ->");
// 		curr = curr->next;
// 	}
// 	printf("\n");
// }


void freeDendrogram(Dendrogram d) {
	if (d != NULL) {
		freeDendrogram(d->left);
		freeDendrogram(d->right);
		free(d);
	}
	return;
}
