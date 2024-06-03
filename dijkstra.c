#include <stdio.h>
#include <stdlib.h>
#include "dynarray.h"
#include "pq.h"

#define DATA_FILE "airports.dat"
#define START_NODE 0

// a struct to represent one step in a path, which includes 
//destination node, cost, and previous node
struct Path {
    int node;
    int prev;
    int cost;
};

// Function to print a 2D array with 10 elements per row 
void print_matrix(int **matrix, int n_nodes, int n_edges) {
    int width = 3;
    int newLine = 0;
    for (int i = 0; i < n_nodes; i++) {
        if (newLine == 10) {
            printf("\n");
            newLine = 0;
        }
        for (int j = 0; j < n_nodes; j++) {
            printf(" %*d ", width, matrix[i][j]); //looked up stackoverflow how to space evenly (that is the %*d thing)
            newLine++;
        }
    }
    printf("\n");
    return;
}

int main(int argc, char const *argv[]) {
    /*
	 * Open file and read the first two int: num of nodes, num of edges
	 */
	int n_nodes, n_edges;
	FILE* file = fopen(DATA_FILE, "r");
	fscanf(file, " %d %d ", &n_nodes, &n_edges);

	// Create 2d (10x10) array reresentin a matrix for node costs
    int **matrix = malloc(n_nodes * sizeof(int*));
    for (int i = 0; i < n_nodes; i++) {
        matrix[i] = malloc(n_nodes * sizeof(int)); 
    }
	
    // Read edges from file and store them in the cost matrix
    for (int i = 0; i < n_edges; i++) {
        int begNode, endNode, cost;
        fscanf(file, "%d %d %d", &begNode, &endNode, &cost);
        matrix[begNode][endNode] = cost;
    }

	// doone with file
    fclose(file);

    // make sure matrxi is filled correctly
    // print_matrix(matrix, n_nodes, n_edges);

    //Initialize paths to all nodes with infinite cost and no previous node
    struct Path* paths = malloc(sizeof(struct Path) * n_nodes);
    for (int i = 0; i < n_nodes; i++) {
        paths[i].node = i; // node # (0-9)
        paths[i].cost = 2147483647; // infinity
        paths[i].prev = -1; // undefined
    }

    // Create priority queue
    struct pq* pq = pq_create();

    // Set the start node cost, and previous to 0 and insert it into pq
	struct Path* start = malloc(sizeof(struct Path));
	start->cost = 0;
	start->node = 0;
	start->prev = 0; // I figured prev would be -1 because it has no previous, but assignment wants it to print 0
    pq_insert(pq, start, 0);

    // Set up is done, start algorithm pseudocode given in assignment pdf
	// while pq is empty esentially means until every node has been checked
    while (!pq_isempty(pq)) {
		// Retrieve current node we are looking at and will look at its connecting edges/paths
        struct Path* curr = pq_remove_first(pq); 
        int currNode = curr->node;

        // If current node hasn't been visited set paths[currNode] to current
        if (paths[currNode].cost == 2147483647) {
            paths[currNode].cost = curr->cost;
            paths[currNode].prev = curr->prev;
        }

        // Checking for and adding neighbors
        for (int j = 0; j < n_nodes; j++) {
            if (matrix[currNode][j] > 0) {
                int edgeCost = matrix[currNode][j];
                int newCost = curr->cost + edgeCost;

                // Check if the new path is shorter otherwise do nothing
                if (newCost < paths[j].cost) {
                    paths[j].cost = newCost;
                    paths[j].prev = currNode;

                    // Add neighbor to pq
                    struct Path* neighborPath = malloc(sizeof(struct Path));
                    neighborPath->cost = newCost;
                    neighborPath->node = j;
                    neighborPath->prev = currNode;
                    pq_insert(pq, neighborPath, newCost);
                }
            }
        }
        free(curr); 
    }

    // Print paths of each node with lowest cost
    for (int i = 0; i < n_nodes; i++) {
        printf("Node %d: Cost = %d, Previous = %d\n", paths[i].node, paths[i].cost, paths[i].prev);
    }

    // Free memory
    free(matrix);
	free(paths);
    pq_free(pq);
    

    return 0;
}
