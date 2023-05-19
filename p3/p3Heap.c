///////////////////////////////////////////////////////////////////////////////
//
// Copyright 2020-2023 Deb Deppeler based on work by Jim Skrentny
// Posting or sharing this file is prohibited, including any changes/additions.
// Used by permission SPRING 2023, CS354-deppeler
//
///////////////////////////////////////////////////////////////////////////////
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
// Main File: p3Heap.c
// This File: p3Heap.c
// Other Files:
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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include "p3Heap.h"

/*
 * This structure serves as the header for each allocated and free block.
 * It also serves as the footer for each free block but only containing size.
 */
typedef struct blockHeader {           

	int size_status;

	/*
	 * Size of the block is always a multiple of 8.
	 * Size is stored in all block headers and in free block footers.
	 *
	 * Status is stored only in headers using the two least significant bits.
	 *   Bit0 => least significant bit, last bit
	 *   Bit0 == 0 => free block
	 *   Bit0 == 1 => allocated block
	 *
	 *   Bit1 => second last bit 
	 *   Bit1 == 0 => previous block is free
	 *   Bit1 == 1 => previous block is allocated
	 * 
	 * Start Heap: 
	 *  The blockHeader for the first block of the heap is after skip 4 bytes.
	 *  This ensures alignment requirements can be met.
	 * 
	 * End Mark: 
	 *  The end of the available memory is indicated using a size_status of 1.
	 * 
	 * Examples:
	 * 
	 * 1. Allocated block of size 24 bytes:
	 *    Allocated Block Header:
	 *      If the previous block is free      p-bit=0 size_status would be 25
	 *      If the previous block is allocated p-bit=1 size_status would be 27
	 * 
	 * 2. Free block of size 24 bytes:
	 *    Free Block Header:
	 *      If the previous block is free      p-bit=0 size_status would be 24
	 *      If the previous block is allocated p-bit=1 size_status would be 26
	 *    Free Block Footer:
	 *      size_status should be 24
	 */
} blockHeader;         

/* Global variable - DO NOT CHANGE NAME or TYPE. 
 * It must point to the first block in the heap and is set by init_heap()
 * i.e., the block at the lowest address.
 */
blockHeader *heap_start = NULL;     

/* Size of heap allocation padded to round to nearest page size.
 */
int alloc_size;

/*
 * Additional global variables may be added as needed below
 * TODO: add global variables needed by your function
 */


// Helper function to find the best-fit block
// needed_size: size of the block needed for allocation
// Returns: pointer to the best-fit block if found, NULL otherwise
blockHeader *find_best_fit(int needed_size) {
	blockHeader *current = heap_start;
	blockHeader *best_fit = NULL;
	int best_fit_size = 0;

	// Iterate through the heap to find the best-fit block
	while (current->size_status != 1) {
		int current_size = current->size_status & ~7;
		int is_free = !(current->size_status & 1);

		// Check if the current block is free and of adequate size
		if (is_free && current_size >= needed_size) {
			// Update best_fit and best_fit_size if a better fit is found
			if (!best_fit || current_size < best_fit_size) {
				best_fit = current;
				best_fit_size = current_size;
			}

			// If an exact fit is found, break the loop
			if (current_size == needed_size) {
				break;
			}
		}

		// Move to the next block in the heap
		current = (blockHeader *)((char *)current + current_size);
	}

	return best_fit;
}




/* 
 * Function for allocating 'size' bytes of heap memory.
 * Argument size: requested size for the payload
 * Returns address of allocated block (payload) on success.
 * Returns NULL on failure.
 *
 * This function must:
 * - Check size - Return NULL if size < 1 
 * - Determine block size rounding up to a multiple of 8 
 *   and possibly adding padding as a result.
 *
 * - Use BEST-FIT PLACEMENT POLICY to chose a free block
 *
 * - If the BEST-FIT block that is found is exact size match
 *   - 1. Update all heap blocks as needed for any affected blocks
 *   - 2. Return the address of the allocated block payload
 *
 * - If the BEST-FIT block that is found is large enough to split 
 *   - 1. SPLIT the free block into two valid heap blocks:
 *         1. an allocated block
 *         2. a free block
 *         NOTE: both blocks must meet heap block requirements 
 *       - Update all heap block header(s) and footer(s) 
 *              as needed for any affected blocks.
 *   - 2. Return the address of the allocated block payload
 *
 *   Return if NULL unable to find and allocate block for required size
 *
 * Note: payload address that is returned is NOT the address of the
 *       block header.  It is the address of the start of the 
 *       available memory for the requesterr.
 *
 * Tips: Be careful with pointer arithmetic and scale factors.
 */
void* balloc(int size) {     
	//TODO: Your code goes in here.

	// Return NULL if the requested size is less than 1
	if (size < 1) {
		return NULL;
	}

	// Determine block size rounding up to a multiple of 8 and possibly adding padding
	int needed_size = (size + sizeof(blockHeader) + 7) & ~7;

	// Find the best-fit block for the allocation
	blockHeader *best_fit = find_best_fit(needed_size);

	// Return NULL if no suitable block is found
	if (!best_fit) {
		return NULL;
	}

	// Calculate the size of the best-fit block and the remaining size after allocation
	int best_fit_size = best_fit->size_status & ~7;
	int remaining_size = best_fit_size - needed_size;

	// Check if the remaining size is large enough to split the block
	if (remaining_size >= (int)sizeof(blockHeader) + 8) {
		// Split the block into an allocated block and a free block
		blockHeader *next_block = (blockHeader *)((char *)best_fit + needed_size);
		next_block->size_status = remaining_size + 2;
		best_fit->size_status = needed_size + (best_fit->size_status & 3) + 1;
	} else {
		// Mark the entire block as allocated and update the status of the next block
		best_fit->size_status += 1;
		blockHeader *next_block = (blockHeader *)((char *)best_fit + best_fit_size);
		if (next_block->size_status != 1) {
			next_block->size_status += 2;
		}
	}

	// Return the address
	return (void *)(best_fit + 1);

} 

/* 
 * Function for freeing up a previously allocated block.
 * Argument ptr: address of the block to be freed up.
 * Returns 0 on success.
 * Returns -1 on failure.
 * This function should:
 * - Return -1 if ptr is NULL.
 * - Return -1 if ptr is not a multiple of 8.
 * - Return -1 if ptr is outside of the heap space.
 * - Return -1 if ptr block is already freed.
 * - Update header(s) and footer as needed.
 */                    
int bfree(void *ptr) {    
	//TODO: Your code goes in here.


	// Return -1 if the pointer is NULL.
	if (!ptr) {
		return -1;
	}

	// Check if the pointer is 8-byte aligned, return -1 if not.
	if ((size_t)ptr & 7) {
		return -1;
	}

	// Calculate the address of the block header.
	blockHeader *block = (blockHeader *)((char *)ptr - sizeof(blockHeader));

	// Define the valid heap address range.
	size_t heap_start_address = (size_t)heap_start;
	size_t heap_end_address = heap_start_address + alloc_size - sizeof(blockHeader);

	// Check if the block address is within the valid range, return -1 if not.
	if (((size_t)block < heap_start_address) || ((size_t)block >= heap_end_address)) {
		return -1;
	}

	// Check if the block is already free, return -1 if it is.
	if ((block->size_status & 1) == 0) {
		return -1;
	}	


	// Set the current block as free.
	block->size_status &= ~1;

	// Calculate the block size.
	size_t blockSize = block->size_status & ~7;

	// Calculate the address of the next block.
	blockHeader *next_block = (blockHeader *)((char *)block + blockSize);

	// Update the status of the next block if it's not the end of the heap.
	if (next_block->size_status != 1) {
		next_block->size_status &= ~2;
	}

	return 0;



	/*


	//assigning header
	blockHeader *block = (blockHeader *)((char *)ptr - sizeof(blockHeader));
	int blockSize = block->size_status % 8;
	//assingning end of heap
	//	blockHeader *end_mark = (blockHeader *)((char *)heap_start + alloc_size);

	//if block is freed
	if ((block->size_status & 1) == 0) {
	return -1;
	}
	//set a-bit to 0(frreing)
	block->size_status = block->size_status-1;  

	// Update the footer of the freed block
	blockHeader *footer = (blockHeader *)((char *)block + blockSize - sizeof(blockHeader));
	footer->size_status = blockSize;

	// Update the next block's p-bit
	blockHeader *nextBlock = (blockHeader *)((char *)block + blockSize);
	if (nextBlock->size_status != 1) {
	nextBlock->size_status = nextBlock->size_status-2;
	}
	 */
	/*
	// Check for the previous block and coalesce if it's free
	if ((block->size_status & 2) == 0) { // If the p-bit of the current block is 0 (previous block is free)
	blockHeader *prevFooter = (blockHeader *)((char *)block - sizeof(blockHeader));
	int prevSize = prevFooter->size_status;
	blockHeader *prevBlock = (blockHeader *)((char *)block - prevSize);

	// Merge blocks and update header and footer
	prevBlock->size_status += blockSize;
	footer->size_status = prevBlock->size_status;
	block = prevBlock;
	}
	// Coalesce with the next block if it's free
	if (!(nextBlock->size_status & 1)) {
	int nextBlockSize = nextBlock->size_status & ~3;
	block->size_status += nextBlockSize;

	// Update the footer of the merged block
	blockHeader *nextFooter = (blockHeader *)((char *)nextBlock + nextBlockSize - sizeof(blockHeader));
	nextFooter->size_status = block->size_status;

	// Update the next next block's p-bit
	blockHeader *nextNextBlock = (blockHeader *)((char *)nextBlock + nextBlockSize);
	if (nextNextBlock->size_status != 1) {
	nextNextBlock->size_status &= ~2;
	}
	}

	return 0;*/ 
} 

/*
 * Function for traversing heap block list and coalescing all adjacent 
 * free blocks.
 *
 * This function is used for user-called coalescing.
 * Updated header size_status and footer size_status as needed.
 */
int coalesce() {
	//TODO: Your code goes in here.



	blockHeader *current = heap_start;

	// Iterate through the entire heap.
	while (current->size_status != 1) {
		int blockSize = current->size_status & ~3;
		int isAllocated = (current->size_status & 1);

		// If the current block is not allocated, try to merge with adjacent free blocks.
		if (isAllocated == 0) {
			blockHeader *nextBlock = (blockHeader *)((char *)current + blockSize);
			int nextBlockSize;
			int nextBlockAllocated;

			// Iterate through adjacent free blocks and merge them.
			while (1) {
				nextBlockSize = nextBlock->size_status & ~3;
				nextBlockAllocated = (nextBlock->size_status & 1);

				// Break the loop if the next block is the end of the heap or allocated.
				if (nextBlock->size_status == 1 || nextBlockAllocated) {
					break;
				}

				// Update current block size and footer.
				blockSize += nextBlockSize;
				current->size_status = blockSize | (current->size_status & 2);
				blockHeader *footer = (blockHeader *)((char *)current + blockSize - sizeof(blockHeader));
				footer->size_status = blockSize;

				// Move to the next block and update its p-bit.
				nextBlock = (blockHeader *)((char *)nextBlock + nextBlockSize);
				if (nextBlock->size_status != 1) {
					nextBlock->size_status &= ~2;
				}
			}
		}

		// Move to the next block in the heap.
		current = (blockHeader *)((char *)current + blockSize);
	}

	return 0;



	/*	blockHeader *current = heap_start;

		while (current->size_status != 1) {
		int curr		newFooter->size_status = current->size_status & ~3;

	// Set next block's previous block status as free
	blockHeader *nextNextBlock = (blockHeader *)((char *)nextBlock + (nextBlock->size_status & ~3));
	nextNextBlock->size_status &= ~3;
	} else {
	current = nextBlock;
	}
	}

	return 0;
	 */
	}


/* 
 * Function used to initialize the memory allocator.
 * Intended to be called ONLY once by a program.
 * Argument sizeOfRegion: the size of the heap space to be allocated.
 * Returns 0 on success.
 * Returns -1 on failure.
 */                    
int init_heap(int sizeOfRegion) {    

	static int allocated_once = 0; //prevent multiple myInit calls

	int   pagesize; // page size
	int   padsize;  // size of padding when heap size not a multiple of page size
	void* mmap_ptr; // pointer to memory mapped area
	int   fd;

	blockHeader* end_mark;

	if (0 != allocated_once) {
		fprintf(stderr, 
				"Error:mem.c: InitHeap has allocated space during a previous call\n");
		return -1;
	}

	if (sizeOfRegion <= 0) {
		fprintf(stderr, "Error:mem.c: Requested block size is not positive\n");
		return -1;
	}

	// Get the pagesize from O.S. 
	pagesize = getpagesize();

	// Calculate padsize as the padding required to round up sizeOfRegion 
	// to a multiple of pagesize
	padsize = sizeOfRegion % pagesize;
	padsize = (pagesize - padsize) % pagesize;

	alloc_size = sizeOfRegion + padsize;

	// Using mmap to allocate memory
	fd = open("/dev/zero", O_RDWR);
	if (-1 == fd) {
		fprintf(stderr, "Error:mem.c: Cannot open /dev/zero\n");
		return -1;
	}
	mmap_ptr = mmap(NULL, alloc_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
	if (MAP_FAILED == mmap_ptr) {
		fprintf(stderr, "Error:mem.c: mmap cannot allocate space\n");
		allocated_once = 0;
		return -1;
	}

	allocated_once = 1;

	// for double word alignment and end mark
	alloc_size -= 8;

	// Initially there is only one big free block in the heap.
	// Skip first 4 bytes for double word alignment requirement.
	heap_start = (blockHeader*) mmap_ptr + 1;

	// Set the end mark
	end_mark = (blockHeader*)((void*)heap_start + alloc_size);
	end_mark->size_status = 1;

	// Set size in header
	heap_start->size_status = alloc_size;

	// Set p-bit as allocated in header
	// note a-bit left at 0 for free
	heap_start->size_status += 2;

	// Set the footer
	blockHeader *footer = (blockHeader*) ((void*)heap_start + alloc_size - 4);
	footer->size_status = alloc_size;

	return 0;
} 

/* 
 * Function can be used for DEBUGGING to help you visualize your heap structure.
 * Traverses heap blocks and prints info about each block found.
 * 
 * Prints out a list of all the blocks including this information:
 * No.      : serial number of the block 
 * Status   : free/used (allocated)
 * Prev     : status of previous block free/used (allocated)
 * t_Begin  : address of the first byte in the block (where the header starts) 
 * t_End    : address of the last byte in the block 
 * t_Size   : size of the block as stored in the block header
 */                     
void disp_heap() {     

	int    counter;
	char   status[6];
	char   p_status[6];
	char * t_begin = NULL;
	char * t_end   = NULL;
	int    t_size;

	blockHeader *current = heap_start;
	counter = 1;

	int used_size =  0;
	int free_size =  0;
	int is_used   = -1;

	fprintf(stdout, 
			"*********************************** HEAP: Block List ****************************\n");
	fprintf(stdout, "No.\tStatus\tPrev\tt_Begin\t\tt_End\t\tt_Size\n");
	fprintf(stdout, 
			"---------------------------------------------------------------------------------\n");

	while (current->size_status != 1) {
		t_begin = (char*)current;
		t_size = current->size_status;

		if (t_size & 1) {
			// LSB = 1 => used block
			strcpy(status, "alloc");
			is_used = 1;
			t_size = t_size - 1;
		} else {
			strcpy(status, "FREE ");
			is_used = 0;
		}

		if (t_size & 2) {
			strcpy(p_status, "alloc");
			t_size = t_size - 2;
		} else {
			strcpy(p_status, "FREE ");
		}

		if (is_used) 
			used_size += t_size;
		else 
			free_size += t_size;

		t_end = t_begin + t_size - 1;

		fprintf(stdout, "%d\t%s\t%s\t0x%08lx\t0x%08lx\t%4i\n", counter, status, 
				p_status, (unsigned long int)t_begin, (unsigned long int)t_end, t_size);

		current = (blockHeader*)((char*)current + t_size);
		counter = counter + 1;
	}

	fprintf(stdout, 
			"---------------------------------------------------------------------------------\n");
	fprintf(stdout, 
			"*********************************************************************************\n");
	fprintf(stdout, "Total used size = %4d\n", used_size);
	fprintf(stdout, "Total free size = %4d\n", free_size);
	fprintf(stdout, "Total size      = %4d\n", used_size + free_size);
	fprintf(stdout, 
			"*********************************************************************************\n");
	fflush(stdout);

	return;  
} 


// end p3Heap.c (Spring 2023)                                         


