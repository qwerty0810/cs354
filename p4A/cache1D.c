///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2021 Deb Deppeler
// Posting or sharing this file is prohibited, including any changes/additions.
//
// We have provided comments and structure for this program to help you get
// started. Later programs will not provide the same level of commenting,
// rather you will be expected to add same level of comments to your work.
// 09/20/2021 Revised to free memory allocated in get_board_size function.
// 01/24/2022 Revised to use pointers for CLAs
//
////////////////////////////////////////////////////////////////////////////////
// This File: cache1D.c
// Other Files:cache2Drows.c, cache2Dclash.c, cache2Dcols.c
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


#include <stdio.h>

#define SIZE 100000

int arr[SIZE];

int main() {
    for (int i = 0; i < SIZE; i++) {
        arr[i] = i;
    }
    return 0;
}












