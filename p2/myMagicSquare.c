///////////////////////////////////////////////////////////////////////////////
// Copyright 2020 Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission, CS 354 Spring 2022, Deb Deppeler
////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2021 Deb Deppeler
// Posting or sharing this file is prohibited, including any changes/additions.
//
// We have provided comments and structure for this program to help you get 
// started.  Later programs will not provide the same level of commenting,
// rather you will be expected to add same level of comments to your work.
// 09/20/2021 Revised to free memory allocated in get_board_size function.
// 01/24/2022 Revised to use pointers for CLAs
//
////////////////////////////////////////////////////////////////////////////////
// Main File:        myMagicSquare.c
// This File:        myMagicSquare.c
// Other Files:      
// Semester:         CS 354 Spring 2023
// Instructor:       deppeler
//
// Author:           Gurusharan Kunusoth
// Email:            kunusoth@wisc.edu
// CS Login:         gurusharan
// GG#:              17
//                   (See https://canvas.wisc.edu/groups for your GG number)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                   Fully acknowledge and credit all sources of help,
//                   including family, friencs, classmates, tutors,
//                   Peer Mentors, TAs, and Instructor.
//
// Persons:          Identify persons by name, relationship to you, and email.
//                   Describe in detail the the ideas and help they provided.
//
// Online sources:   Avoid web searches to solve your problems, but if you do
//                   search, be sure to include Web URLs and description of
//                   of any information you find.
//                   https://www.youtube.com/watch?v=GYBr8n-_Rq4
//                   https://www.youtube.com/watch?v=D13dFLpHFJg&t=435s
////////////////////////////////////////////////////////////////////////////////
// add your own File Header information here (as provided in p2A or Commenting guide

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure that represents a magic square
typedef struct {
	int size;           // dimension of the square
	int **magic_square; // pointer to heap allocated magic square
} MagicSquare;

/* TODO:
 * Prompts the user for the magic square's size, reads it,
 * checks if it's an odd number >= 3 (if not display the required
 * error message and exit), and returns the valid number.
 */
int getSize() {
	int size;
	printf("Enter magic square's size (odd integer >=3)\n");
	//scan input
	scanf("%d",&size);
	//prints if user input is even
	if(size % 2==0){
		printf("Magic square's size should be odd.\n");
		exit(1);
	}
	//prints if user input is less than 3
	if(size<3){
		printf("Magic square's size should be >=3\n");
		exit(1);
	}
	return size;   
} 

/* TODO:
 * Makes a magic square of size n using the 
 * Siamese magic square algorithm or alternate from assignment 
 * or another valid alorithm that produces a magic square.
 *
 * n is the number of rows and columns
 *
 * returns a pointer to the completed MagicSquare struct.
 */
MagicSquare *generateMagicSquare(int n) {


	//allocate heap memory for a 2D square
	int **m;
	m = malloc(sizeof(int *) * n);

	//allocating memory
	for (int i = 0; i < n; i++){        
		*(m+i) = malloc(sizeof(int) * n);        
		//if allocation fails
		if(*(m+i) == NULL){
			printf("Allocation Failed.");
			exit(1);
		}
	}

	//puts  to 0
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			*(*(m+i)+j) = 0;
		}
	}

	//initializse positions
	int r=n-1;
	int c=n/2;	
	//variables for new positions
	int row;
	int col;

	//next positions
	for(int i=1; i<((n*n)+1); i++){
		//starts from 1
		*(*(m+r)+c)=i;
		//algorithm for the next positions
		row=(r-1 +n)%n;
		col=(c+2) %n;
		//if square is fille
		if(*(*(m+row)+col)!=0){
			//fills the square beloq the original one
			c=col;
			r=(r+1)%n;

		}
		else{
			r=row;
			c=col;
		}
	} 
	//struct
	MagicSquare *ms = malloc(sizeof(MagicSquare));
	ms-> size=n;
	ms-> magic_square =m;
	if(ms == NULL){
		printf("Allocation Failed.");
	}


	return ms;    
}
/* Opens a new file (or overwrites the existing file)
 * and writes the square in the specified format.
 *
 * magic_square the magic square to write to a file
 * filename the name of the output file
 */
void fileOutputMagicSquare(MagicSquare *magic_square, char *filename) {
	//openeing file
	FILE *fp = fopen(filename, "w");
	if (fp == NULL) {
		printf("Error!! File cannot be opened!\n");
		exit(1);
	}

	//printing
	int size=magic_square->size;
	int **m = magic_square->magic_square;
	fprintf(fp, "%d\n", size);


	for(int i=0; i< size; i++){
		for(int j=0; j< size; j++){
			fprintf(fp, "%d", *(*(m+i)+j));			
		}
		if (j != size-1) {
				fprintf(fp, ",");
			}


fprintf(fp, "\n");
	}
	fclose(fp);




}


/* TODO:
 * Generates a magic square of the user specified size and
 * outputs the square to the output filename.
 * 
 * Add description of required CLAs here
 */
int main(int argc, char **argv) {
	// TODO: Check input arguments to get output filename
	if(argc != 2){
		printf("Usage: ./myMagicSquare <output_filename>\n");
		exit(1);
	}

	// TODO: Get magic square's size from user
	int size = getSize();

	// TODO: Generate the magic square by correctly interpreting 
	//       the algorithm(s) in the write-up or by writing or your own.  
	//       You must confirm that your program produces a 
	//       Magic Sqare as described in the linked Wikipedia page.
	MagicSquare *ms = generateMagicSquare(size);

	// TODO: Output the magic square
	fileOutputMagicSquare(ms, *(argv+1));
	for(int i=0; i<size; i++){
		free(*(ms->magic_square + i));
	}
	free(ms -> magic_square);
	free(ms);
	ms =NULL;
	return 0;
} 

