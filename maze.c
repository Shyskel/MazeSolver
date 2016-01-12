// Name: Tyler Gauntlett
// NID: ty340586
// Date: 10/6/2015
// Assignment: Maze.c
// Program description: The objective is to move from S to ~ in as few
// moves as possible, using only - as spaces.

#include <stdio.h>
#include <stdlib.h>

const char VALIDSPOT = '-';
const char INVALIDSPOT = 'X';
const char START = 'S';
const char GOAL = '~';
const int UNUSED = -1;

// Typedef each struct with N for new

// Stores X/Y cordinates in a single struct.
typedef struct XYCord {
	int x;
	int y;
} XYCordN;

// 
typedef struct node {
	struct XYCord* location;
	struct node* next;
} nodeN;

// Stores our queue.
typedef struct queue {
	struct node* front;
	struct node* back;
} queueN;

const int directionSize = 4;
const int  DX[4] = { 1, 0, -1, 0 };
const int DY[4] = { 0, -1, 0, 1 };

int breathsFirstSearch(char** charMaze, int** intMaze, int rows, int cols, XYCordN* location);
char** makeMaze(int rows, int cols);
int** makeUsedMaze(int rows, int cols, char** charMaze, XYCordN* getS);

// Function prototypes containing structs must
// go UNDER the structs defining them.
void init(queueN* qPtr);
int enqueue(queueN* qPtr, int x, int y);
XYCordN* dequeue(queueN* qPtr);
int empty(queueN* qPtr);

void freeCharMaze(int rows, char** charMaze);
void freeIntMaze(int rows, int** maze);

void printMazeInt(int rows, int cols, int** maze);
void printMazeChar(int rows, int cols, char** maze);


int main(){

	// Initialize all variables used in main.
	int cases, rows, cols;

	// Takes in number of cases outside the loop it creates.
	scanf("%i", &cases);

	// Loops 
	while (cases > 0){

		// Initializes the boards and the struct variable
		// that finds the S location.
		XYCordN* getS = (struct XYCord*)malloc(sizeof(struct XYCord));
		char** mazeBoard;
		int** usedBoard;
		int search;

		// Take in rows/colums input.
		scanf("%i", &rows);
		scanf("%i", &cols);

		// Initialize indiviual char and used boards. Char
		// board will hold the chars taken from input. Used
		// board will keep track of which spaces have been
		// already visted.
		mazeBoard = makeMaze(rows, cols);
		usedBoard = makeUsedMaze(rows, cols, mazeBoard, getS);

		// Stores the value of the shortest path out of the maze.
		search = breathsFirstSearch(mazeBoard, usedBoard, rows, cols, getS);

		// Prints the shortest way out of the maze.
		printf("%d\n", search);

		// Frees the dynamically allocated 2D arrays.
		freeCharMaze(rows, mazeBoard);
		freeIntMaze(rows, usedBoard);
		free(getS);

		// Decrement cases before processing the next set of input.
		cases--;
	}

	return 0;
}

// Algorithm to find the quickest path out of a maze using x/y coordinates.
int breathsFirstSearch(char** charMaze, int** intMaze, int rows, int cols, XYCordN* location){

	// Stores the x/y location of S into
	// int variables.
	int curX = location->x;
	int curY = location->y;

	// Initialize spaces 
	int spacesToEscape = intMaze[location->x][location->y];
	queueN* MyQueuePtr = (queueN*)malloc(sizeof(queueN));


	// Initialize the queue.
	init(MyQueuePtr);

	// Queues the x/y location of S.
	enqueue(MyQueuePtr, location->x, location->y);

	// Loop that 
	while (!empty(MyQueuePtr)){

		// Takes the struct that was dequeued and stores the x and y
		// values respectively.
		XYCordN* tempCord = dequeue(MyQueuePtr);
		curX = tempCord->x;
		curY = tempCord->y;
		free(tempCord);

		// Change return variable to the current valid
		// x/y spaces. This spaces contains a non negative number
		// corrisponding to the number of spaces since the S.
		int spacesToEscape = intMaze[curX][curY];
		int i;

		// Compares current x/y location to the ~ char.
		if (charMaze[curX][curY] == GOAL){

			// Empty the queue after finding the ~ char.
			while (!empty(MyQueuePtr)) dequeue(MyQueuePtr);

			return spacesToEscape;
		}

		// Check the up, down, left, and right directions. Also checks if its not an X, is a -, and is a postive space.
		for (i = 0; i < directionSize; i++){
			if ((curX > 0) && (curY > 0) && (charMaze[curX + DX[i]][curY + DY[i]]) != INVALIDSPOT && (intMaze[curX + DX[i]][curY + DY[i]] == UNUSED)){
				intMaze[curX + DX[i]][curY + DY[i]] = spacesToEscape + 1;
				enqueue(MyQueuePtr, curX + DX[i], curY + DY[i]);
			}
		}


	}
	free(MyQueuePtr);

	// Returns -1 if no pathways were found.
	return -1;
}

// Reads in input values and puts them in a 2D array
char** makeMaze(int rows, int cols){

	// Initialize variables.
	char** maze = malloc(sizeof(char*)*rows);
	int i;

	for (i = 0; i < rows; i++){
		// Makes space for the null char.
		maze[i] = malloc(sizeof(char)*(cols + 1));

		// Reads in each row of strings.
		scanf("%s", maze[i]);

	}

	return maze;
}

int** makeUsedMaze(int rows, int cols, char** charMaze, XYCordN* getS) {

	// Initialize variables.
	int** usedMaze = malloc(sizeof(int*)*rows);
	int i, j;

	// Fills the 2D usedMaze with -1.
	for (i = 0; i < rows; i++){
		usedMaze[i] = malloc(sizeof(int)*(cols));
		for (j = 0; j < cols; j++){
			// -1 values represent spaces that 
			// have not been accessed yet.
			usedMaze[i][j] = -1;
		}
	}


	// Searches for the S char, then marks that index with 0 and
	// returns the x y cordinates to the getS variable in main.
	for (i = 0; i < rows; i++) {
		for (j = 0; j < (cols + 1); j++) {
			if (charMaze[i][j] == 'S'){
				// Makes the starting location with a 0 on the used board.
				usedMaze[i][j] = 0;
				// Stores x/y cordinates into the global getS variable.
				getS->x = i;
				getS->y = j;
			}
		}
	}

	return usedMaze;

}


// Initialize queue.
void init(queueN* qPtr){

	qPtr->front = NULL;
	qPtr->back = NULL;

}

// Queue the next item at the back of the queue list.
int enqueue(queueN* qPtr, int x, int y){

	// Initialize a temp variable used to hold
	// elements in the queue.
	nodeN* temp = (nodeN*)malloc(sizeof(nodeN));

	// Initialize a new struct of x/y datat type.
	XYCordN* newPt = malloc(sizeof(XYCordN));

	newPt->x = x;
	newPt->y = y;

	// Ensures that the malloc function was successful.
	if (temp != NULL) {

		// Set up our node to enqueue into the back of the queue.
		temp->location = newPt;
		temp->next = NULL;

		// If the queue is NOT empty, we must set the old "last" node to point
		// to this newly created node.
		if (qPtr->back != NULL)
			qPtr->back->next = temp;

		// Now, we must reset the back of the queue to our newly created node.
		qPtr->back = temp;

		// If the queue was previously empty we must ALSO set the front of the
		// queue.
		if (qPtr->front == NULL)
			qPtr->front = temp;

		// Signifies a successful operation.
		return 1;
	}

	else
		return 0;
}

// Removes the item from the front of the queue list.
XYCordN* dequeue(queueN* qPtr) {

	// Initialize a temp variable used to hold
	// elements in the queue.
	nodeN* tmp = malloc(sizeof(nodeN));

	// Check the empty case.
	if (qPtr->front == NULL)
		return NULL;

	// Set up a temporary pointer to use to free the memory for this node.
	tmp = qPtr->front;

	// Make front point to the next node in the queue.
	qPtr->front = qPtr->front->next;

	// Sets the return value to the current location.
	XYCordN* returnStruct = tmp->location;

	// If deleting this node makes the queue empty, we have to change the back
	// pointer also!
	if (qPtr->front == NULL)
		qPtr->back = NULL;


	// Free our memory.
	free(tmp);

	// Returns the X/Y struct that was dequeued.
	return returnStruct;
}

// Returns if the queue is empty or not.
int empty(queueN* qPtr) {
	return qPtr->front == NULL;
}

// Frees the maze 2D array holding the char values.
void freeCharMaze(int rows, char** charMaze){

	int i;

	for (i = 0; i < rows; i++){
		free(charMaze[i]);
	}

	free(charMaze);
}

// Frees the used 2D array holding the int values.
void freeIntMaze(int rows, int** intMaze){

	int i;

	for (i = 0; i < rows; i++){
		free(intMaze[i]);
	}

	free(intMaze);
}