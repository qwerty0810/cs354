////////////////////////////////////////////////////////////////////////////////
// This File: divison.c
// Other Files:mySigHandler.c, sendsig.c
// Semester: CS 354 Spring 2023
// Instructor: deppeler
//
// Author: Gurusharan Kunusoth
// Email: kunusoth@wisc.edu
// CS Login: gurusharan
// GG#: 17
// (See https://canvas.wisc.edu/groups for your GG number)
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
// Fully acknowledge and credit all sources of help,
// including family, friencs, classmates, tutors,
// Peer Mentors, TAs, and Instructor.
//
// Persons: Identify persons by name, relationship to you, and email.
// Describe in detail the the ideas and help they provided.
//
// Online sources: Avoid web searches to solve your problems, but if you do
// search, be sure to include Web URLs and description of
// of any information you find.
////////////////////////////////////////////////////////////////////////////////

//Include necessary header files
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
// Global variable to store the number of successful operations
int successful_operations = 0;
// Function to handle the SIGFPE signal (division by zero)
void sigfpe_handler(int signum) {
    printf("Error: a division by 0 operation was attempted.\n");
    printf("Total number of operations completed successfully: %d\n", successful_operations);
    printf("The program will be terminated.\n");
    exit(0);
}
// Function to handle the SIGINT signal (Ctrl-C)
void sigint_handler(int signum) {
    printf("\nTotal number of operations completed successfully: %d\n", successful_operations);
    printf("The program will be terminated.\n");
    exit(0);
}
//Main function
int main() {
    struct sigaction sa_fpe;
    memset(&sa_fpe, 0, sizeof(sa_fpe));
    sa_fpe.sa_handler = sigfpe_handler;
    sigaction(SIGFPE, &sa_fpe, NULL); //Set up SIGFPE handler

    struct sigaction sa_int;
    memset(&sa_int, 0, sizeof(sa_int));
    sa_int.sa_handler = sigint_handler;
    sigaction(SIGINT, &sa_int, NULL);  //Set up SIGINT handler
    // Main loop to perform integer division operations
    while (1) {
        int int1, int2, quotient, remainder;
        char buffer[100];

        printf("Enter first integer: ");
        fgets(buffer, sizeof(buffer), stdin);
        int1 = atoi(buffer);

        printf("Enter second integer: ");
        fgets(buffer, sizeof(buffer), stdin);
        int2 = atoi(buffer);
        // Check for division by zero
        if (int2 == 0) {
            raise(SIGFPE);
        } else {
            quotient = int1 / int2;
            remainder = int1 % int2;
            printf("%d / %d is %d with a remainder of %d\n", int1, int2, quotient, remainder);
            successful_operations++; // Increment successful operations count
        }
    }

    return 0;
}
