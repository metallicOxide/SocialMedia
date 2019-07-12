// PQ ADT interface for Ass2 (COMP2521)
#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define HEAP_START_SIZE 4


typedef struct _DDListNode {
	ItemPQ item;
	struct _DDListNode *next;
	struct _DDListNode *prev;
} dListNode;

typedef dListNode *DListNode;

struct PQRep {
	int nitems;
	DListNode head;
	DListNode end;
};

// makes new node
static DListNode newNode (ItemPQ element);
// pops the given node from the PQ
static ItemPQ popNode(DListNode node, PQ pq);

PQ newPQ() {
	PQ new = malloc(sizeof(*new));	
	new->nitems = 0; 
	new->head = NULL;
	new->end = NULL;
	return new;
}

int PQEmpty(PQ p) {
	return ((p->nitems == 0) ? 1:0);
}

static DListNode newNode (ItemPQ element) {
	DListNode new = malloc(sizeof(*new));
	new->item = element;
	new->next = NULL;
	new->prev = NULL;
	return new;
}

void addPQ(PQ pq, ItemPQ element) {
	DListNode new = newNode(element);
	int value = element.value;

	DListNode curr = pq->head;
	DListNode temp;
	// matching key, flush element value
	while (curr != NULL) {
		if (curr->item.key == element.key) {
			updatePQ(pq, element);
			free(new);
			return;
		}
		curr = curr->next;
	}

	// if list is empty
	if (PQEmpty(pq)) {
		pq->head = new;
		pq->end = new;
		pq->nitems += 1;
		return;
	}
	// check case if element has lower value than head 
	if (value <= pq->head->item.value) {
		pq->head->prev = new;
		new->next = pq->head;
		pq->head = new;
		pq->nitems += 1;
		return;
	}

	// case where element has larger value then end
	if (value > pq->end->item.value) {
		pq->end->next = new;
		new->prev = pq->end;
		pq->end = new;
		pq->nitems += 1;
		return;
	}
	curr = pq->head;
	// cases where the head does not have lower priority
	while(curr != NULL) {
		// case where it's equal
		if (value == curr->item.value) {
			temp = curr->prev;
			curr->prev = new;
			new->next = curr;
			temp->next = new;
			new->prev = temp;
			pq->nitems += 1;
			return;
		}
		// case where it's larger
		if (value > curr->item.value && value < curr->next->item.value) {
			temp = curr->next;
			curr->next = new;
			new->prev = curr;
			temp->prev = new;
			new->next = temp;
			pq->nitems += 1;
			return;
		}
		curr = curr->next;
	}
}

ItemPQ dequeuePQ(PQ pq) {
	assert(!PQEmpty(pq));
	ItemPQ throwAway = pq->head->item;
	DListNode temp = pq->head;
	pq->head = pq->head->next;
	if (pq->head != NULL) {
		pq->head->prev = NULL;
	}
	free(temp);
	pq->nitems -= 1; 
	return throwAway;
}

static ItemPQ popNode(DListNode node, PQ pq) {
	ItemPQ temp = node->item;
	if (node == pq->head) {
		pq->head = node->next;
		pq->head->prev = NULL;
	} else if (node == pq->end) {
		pq->end = node->prev;
		pq->end->next = NULL;
	} else {
		node->prev->next = node->next;
		node->next->prev = node->prev;
	}
	pq->nitems -= 1;
	free(node);
	return temp;
}

void updatePQ(PQ pq, ItemPQ element) {
	DListNode curr = pq->head;
	while(curr != NULL) {
		if (curr->item.key == element.key) {
			curr->item.value = element.value;
			if (pq->nitems == 1) {
				return;
			}
			ItemPQ temp = popNode(curr, pq);
			addPQ(pq, temp);
			break;
		}
		curr = curr->next;
	}
}

void showPQ(PQ pq) {
	if (PQEmpty(pq)) return;
	DListNode curr = pq->head;
	while (curr->next != NULL) {
		printf("{%d, %d} -> ", curr->item.key, curr->item.value);
		curr = curr->next;
	}
	printf("{%d, %d}\n", curr->item.key, curr->item.value);
}

void  freePQ(PQ pq) {
	DListNode curr = pq->head;
	DListNode temp = NULL;
	while(curr != NULL) {
		temp = curr;
		curr = curr->next;
		free(temp);
	}
	free(pq);
}
