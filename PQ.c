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

static DListNode newNode (ItemPQ element);
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

	// if list is empty
	if (PQEmpty(pq)) {
		pq->head = new;
		pq->end = new;
		pq->nitems += 1;
		// printf("first element inserted\n");
		return;
	}
	// check case if element has lower value than head 
	if (value <= pq->head->item.value) {
		pq->head->prev = new;
		new->next = pq->head;
		pq->head = new;
		pq->nitems += 1;
		// printf("lower than head\n");
		return;
	}

	// case where element has larger value then end
	if (value > pq->end->item.value) {
		pq->end->next = new;
		new->prev = pq->end;
		pq->end = new;
		pq->nitems += 1;
		// printf("larger than end\n");
		return;
	}

	// cases where the head does not have lower priority
	DListNode curr = pq->head;
	DListNode temp;
	while(curr != NULL) {
		// matching key, flush element value
		if (curr->item.key == element.key) {
			curr->item.value = element.value;
			// printf("value update\n");
			return;
		}
		if (value == curr->item.value) {
			// printf("reeet\n");
			temp = curr->prev;
			curr->prev = new;
			new->next = curr;
			temp->next = new;
			new->prev = temp;
			pq->nitems += 1;
			return;
		}
		if (value > curr->item.value) {
			// printf("lol");
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
	ItemPQ throwAway = pq->head->item;
	DListNode temp = pq->head;
	// printf("pointer of temp: %p\n", temp);
	// printf("key of temp: %d\n", temp->item.key);
	pq->head = pq->head->next;
	// printf("pointer of head->next: %p\n", pq->head);
	if (pq->head != NULL) pq->head->prev = NULL;
	free(temp);
	pq->nitems -= 1; 
	return throwAway;
}

static ItemPQ popNode(DListNode node, PQ pq) {
	// printf("popping node\n");
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
			ItemPQ temp = popNode(curr, pq);
			// printf("%d", temp.key);
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
