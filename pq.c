/*
 * In this file, you will write the structures and functions needed to
 * implement a priority queue.  Feel free to implement any helper functions
 * you need in this file to implement a priority queue.  Make sure to add your
 * name and @oregonstate.edu email address below:
 *
 * Name: Cole Seifert	
 * Email: seiferco@oregonstate.edu
 */

#include <stdlib.h>
#include <stdio.h>

#include "pq.h"
#include "dynarray.h"

/*
 * This is the structure that represents a priority queue.  You must define
 * this struct to contain the data needed to implement a priority queue.
 */
struct pq {
	struct dynarray* da;
};

// This stucture represents what values will be stores in the dynamic array (i.e. a dynamic array will consist of a bunch of nodes)
struct node {
	int priority;
	void* value;
};


/*
 * This function should allocate and initialize an empty priority queue and
 * return a pointer to it.
 */
struct pq* pq_create() {
	struct pq* pq = malloc(sizeof(struct pq));
	pq->da = dynarray_create();
	return pq;
}

struct node* node_create(void* value, int priority) {
	struct node *newNode = malloc(sizeof(struct node));
	newNode->priority = priority;
	newNode->value = value;
	return newNode;
}


/*
 * This function should free the memory allocated to a given priority queue.
 * Note that this function SHOULD NOT free the individual elements stored in
 * the priority queue. That is the responsibility of the caller. However, you
 * do need to free the memory that holds each element stored in the priority 
 * queue. 
 *
 * Params:
 *   pq - the priority queue to be destroyed.  May not be NULL.
 */
void pq_free(struct pq* pq) {
	for(int i = 0; i < dynarray_size(pq->da); i++){
		free(dynarray_get(pq->da, i));
	}
	dynarray_free(pq->da);
	free(pq);
	return;
}


/*
 * This function should return 1 if the specified priority queue is empty and
 * 0 otherwise.
 *
 * Params:
 *   pq - the priority queue whose emptiness is to be checked.  May not be
 *     NULL.
 *
 * Return:
 *   Should return 1 if pq is empty and 0 otherwise.
 */
int pq_isempty(struct pq* pq) {
	if(dynarray_size(pq->da) == 0){
		return 1;
	}
	return 0;
}


/*
 * This function should insert a given element into a priority queue with a
 * specified priority value.  Note that in this implementation, LOWER priority
 * values are assigned to elements with HIGHER priority.  In other words, the
 * element in the priority queue with the LOWEST priority value should be the
 * FIRST one returned.
 *
 * Params:
 *   pq - the priority queue into which to insert an element.  May not be
 *     NULL.
 *   value - the value to be inserted into pq.
 *   priority - the priority value to be assigned to the newly-inserted
 *     element.  Note that in this implementation, LOWER priority values
 *     should correspond to elements with HIGHER priority.  In other words,
 *     the element in the priority queue with the LOWEST priority value should
 *     be the FIRST one returned.
 */
void pq_insert(struct pq* pq, void* value, int priority) {
	struct node* newNode = malloc(sizeof(struct node));
  	newNode->value = value;
  	newNode->priority = priority;

	// insert new node to the end of the array
	dynarray_insert(pq->da, newNode);

	// keep track of the newNode's index, parent Node's Index and retrieve the parent node
	int newNodeIdx = dynarray_size(pq->da) - 1; 
	int parentIdx = ((newNodeIdx - 1) / 2); 
	struct node* parentNode = (struct node*)dynarray_get(pq->da, parentIdx);
	
	while(newNode->priority < parentNode->priority && newNodeIdx > -1){

		// Swap parent and child
		dynarray_set(pq->da, parentIdx, newNode);
		dynarray_set(pq->da, newNodeIdx, parentNode);


		// Update indexes since we swapped
		newNodeIdx = parentIdx;
		parentIdx = ((newNodeIdx - 1) / 2);
		parentNode = (struct node*)dynarray_get(pq->da, parentIdx); 

	}

	return;
}




/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_first(struct pq* pq) {
	struct node* n = (struct node*)dynarray_get(pq->da, 0);
	void* firstPriorityVal = n->value;
	return firstPriorityVal;
}


/*
 * This function should return the priority value of the first item in a
 * priority queue, i.e. the item with LOWEST priority value.
 *
 * Params:
 *   pq - the priority queue from which to fetch a priority value.  May not be
 *     NULL or empty.
 *
 * Return:
 *   Should return the priority value of the first item in pq, i.e. the item
 *   with LOWEST priority value.
 */
int pq_first_priority(struct pq* pq) {
	struct node* n = (struct node*)dynarray_get(pq->da, 0); // first value in the array should be the highest priority if insertion was corrrect
	int firstPriorityLvl = n->priority; // grab the pririty level of the first element in the heap/array
	return firstPriorityLvl;
}


/*
 * This function should return the value of the first item in a priority
 * queue, i.e. the item with LOWEST priority value, and then remove that item
 * from the queue.
 *
 * Params:
 *   pq - the priority queue from which to remove a value.  May not be NULL or
 *     empty.
 *
 * Return:
 *   Should return the value of the first item in pq, i.e. the item with
 *   LOWEST priority value.
 */
void* pq_remove_first(struct pq* pq) {
	if(pq->da == NULL){
		return NULL;
	}																																								

	// get root node for returning the value we removed
	struct node* rootNode = (struct node*)dynarray_get(pq->da, 0); // Get the first node in the queue(aka highest priority node)
	void* rootNodeVal = rootNode->value; 

	struct node* lastAdded = (struct node*)dynarray_get_last(pq->da);

	// We know heap is implemented correctly if last added node's priority prints 51
	// according to gdb, the last inserted andom number is 51
	//printf("Last added node: %d\n",lastAdded->priority);

	// set da[0] to da[size - 1] (first node to the last added node)
	dynarray_set(pq->da, 0, lastAdded);


	// free last node in the array
	int lastIndex = dynarray_size(pq->da) - 1;
	dynarray_set(pq->da, lastIndex, NULL);

	// // decrement size of da
	dynarray_decrement_size(pq->da);
	int size = dynarray_size(pq->da);


	perc_down2(pq, size, 0); 
	
	free(rootNode);
	
	return rootNodeVal;
}

void perc_down2(struct pq* pq, int size, int rootIdx){
	int smallestIdx = rootIdx;
	int leftChildIdx = 2*rootIdx + 1;
	int rightChildIdx = 2*rootIdx + 2;

	struct node* smallest = dynarray_get(pq->da, rootIdx);
	struct node* leftChild = NULL;
	struct node* rightChild = NULL;

	// printf("root val: %d\n", root->priority);
	// printf("leftChild val: %d\n", leftChild->priority);
	// printf("rightChild val: %d\n", rightChild->priority);

	// if left child is smaller than smallest indicator, set smallest indicator to lefts index
	if(leftChildIdx < size){	// make sure left child exists
		leftChild = dynarray_get(pq->da, leftChildIdx);
		if(leftChild->priority < smallest->priority){
			smallestIdx = leftChildIdx;
			smallest = dynarray_get(pq->da, smallestIdx);
		}
	}

	// if right child is smaller than smallest indicator, set smallest indicator to rights index
	if(rightChildIdx < size){	// Make sure right child exists
		rightChild = dynarray_get(pq->da, rightChildIdx);
		if(rightChild->priority < smallest->priority){
			smallestIdx = rightChildIdx;
		}
	}

	

	if(smallestIdx != rootIdx){
	// Swap the nodes in the array
	struct node* temp = dynarray_get(pq->da, rootIdx);
	dynarray_set(pq->da, rootIdx, dynarray_get(pq->da, smallestIdx));
	dynarray_set(pq->da, smallestIdx, temp);

	// Continue percolating down the swapped child node
	perc_down2(pq, size, smallestIdx);

	}

}



