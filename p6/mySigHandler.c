////////////////////////////////////////////////////////////////////////////////
// This File: mySigHandler.c
// Other Files: division.c, sendsig.c
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
#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
// Global variable to store SIGUSR1 signal count
volatile int sigusr1_count = 0;
// Function to handle the SIGALRM signal
void alarm_handler(int signum) {
    pid_t pid = getpid();  //Get the process ID
    time_t current_time = time(NULL);  //get the current time
    if (current_time == (time_t)-1) {
        perror("time");
        exit(EXIT_FAILURE);
    }
    char *time_str = ctime(&current_time);  //Convert time to string format
    if (time_str == NULL) {
        perror("ctime");
        exit(EXIT_FAILURE);
    }
    time_str[strlen(time_str) - 1] = '\0';
    printf("PID: %d CURRENT TIME: %s\n", pid, time_str);
    alarm(4);  //Set next alarm
}
// Function to handle the SIGUSR1 signal
void sigusr1_handler(int signum) {
    sigusr1_count++;  //Increment the SIGUSR1 signal count
    printf("SIGUSR1 handled and counted!\n");
}
// Function to handle the SIGINT signal (Ctrl-C)
void sigint_handler(int signum) {
    printf("SIGINT handled.\n");
    printf("SIGUSR1 was handled %d times. Exiting now.\n", sigusr1_count);
    exit(0);
}
//Main function
int main() {
    struct sigaction act;

    // Set up alarm handler
    memset(&act, 0, sizeof(act));
    act.sa_handler = alarm_handler;
    if (sigaction(SIGALRM, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }
    alarm(4); //Set initial alarm

    // Set up SIGUSR1 handler
    memset(&act, 0, sizeof(act));
    act.sa_handler = sigusr1_handler;
    if (sigaction(SIGUSR1, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    // Set up SIGINT handler
    memset(&act, 0, sizeof(act));
    act.sa_handler = sigint_handler;
    if (sigaction(SIGINT, &act, NULL) < 0) {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    printf("PID and time print every 4 seconds.\n");
    printf("Type Ctrl-C to end the program.\n");

    //Main loop
    while (1) {
    }

    return 0;
}

