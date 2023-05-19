////////////////////////////////////////////////////////////////////////////////
// This File: sendsig.c
// Other Files:mySigHandler.c, divison.c
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

// Include necessary header files
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

// Function to display usage information for the program
void usage() {
    printf("Usage: sendsig <signal type> <pid>\n");
}
// Main function
int main(int argc, char *argv[]) {
    // Check if the correct number of arguments is provided
    if (argc != 3) {
        usage();
        return 1;
    }
    // Determine the signal type based on the first argument
    int signal_type;
    if (strcmp(argv[1], "-i") == 0) {
        signal_type = SIGINT;
    } else if (strcmp(argv[1], "-u") == 0) {
        signal_type = SIGUSR1;
    } else {
        usage();
        return 1;
    }
    // Convert the second argument to a process ID (pid)
    pid_t pid = atoi(argv[2]);
    // Send the specified signal to the target process
    if (kill(pid, signal_type) == -1) {
        perror("Error sending signal");
        return 1;
    }
    // Return success (0) if the signal was sent successfully
    return 0;
}
