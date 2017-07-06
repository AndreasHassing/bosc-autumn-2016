/*
Main program for the virtual memory project.
Make all of your modifications to this file.
You may add or rearrange any code or data as you need.
The header files page_table.h and disk.h explain
how to use the page table and disk interfaces.
*/

#include "page_table.h"
#include "disk.h"
#include "program.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Variables for statistics
#ifdef STATS
int npage_faults;
int disk_reads;
int disk_writes;

void print_stats() {
	printf("Page faults: %d\n", npage_faults);
	printf("Disk reads: %d\n", disk_reads);
	printf("Disk writes: %d\n", disk_writes);
	printf("Disk reads+writes: %d\n\n", disk_reads+disk_writes);
}
#endif

/**
 * disk facilitates global access to read and write operations on
 * the disk, backing the virtual memory.
 */
struct disk *disk;

/**
 * frame_state contains the bits of a frame (0 if empty), and the
 * id of the page loaded into that frame.
 */
struct frame_state {
	int bits;
	int page;
};

struct frame_state *frame_states;

/**
 * Defines an interface for the frame selection algorithms to use.
 * Allows using a global variable containing the selected function,
 * and reduces the amount of switch or if/else blocks needed.
 * @see get_random_frame(struct page_table*)
 */
typedef int (*frame_select_algo_t) (struct page_table *pt);

frame_select_algo_t frame_select_algo;

/**
 * Get an empty frame index, or -1 if no such frame exists.
 * @param nframes number of available frames, empty or not.
 * @return The index of the empty frame, or -1 if no such frame exists.
 */
int get_empty_frame(const int nframes) {
	static int used_frames = 0;
	if (used_frames >= nframes) return -1;

	used_frames++;

	for (int i = 0; i < nframes; i++) {
		if (!frame_states[i].bits) return i;
	}

	return -1;
}

/**
 * Get a random frame and its corresponding page from memory.
 * @param pt the page table to select from.
 * @return the frame number to swap with.
 */
int get_random_frame(struct page_table *pt) {
	int nframes = page_table_get_nframes(pt);

	return lrand48() % nframes;
}

/**
 * Get a frame and its corresponding page from memory, using a FIFO queue approach.
 * @param pt the page table to select from.
 * @return the frame number to swap with.
 */
int get_frame_from_queue(struct page_table *pt) {
	static int current_frame = -1;
	int nframes = page_table_get_nframes(pt);

	current_frame = (current_frame + 1) % nframes;

	return current_frame;
}

/**
 * Get a frame and its corresponding page from memory, using a custom approach.
 * @param pt the page table to select from.
 * @return the frame number to swap with.
 */
int get_custom_frame(struct page_table *pt) {
	static unsigned int *page_swap_count = NULL;
	static int last_swapped_frame = -1;

	if (page_swap_count == NULL)
		page_swap_count = calloc(page_table_get_npages(pt), sizeof(int));

	int nframes = page_table_get_nframes(pt);

	int least_swapped_page = 0, least_swapped_page_frame = 0;
	for (int i = 0; i < nframes; i++) {
		// avoid page fault cycles when trying to compare two entries
		if (i == last_swapped_frame) continue;
		int swap_count = page_swap_count[frame_states[i].page];
		if (swap_count < page_swap_count[least_swapped_page]) {
			least_swapped_page = frame_states[i].page;
			least_swapped_page_frame = i;
		}
	}

	page_swap_count[least_swapped_page]++;
	last_swapped_frame = least_swapped_page_frame;

	return least_swapped_page_frame;
}

/**
 * Swap a page from disk to physical memory.
 * @param pt page table for the virtual memory.
 * @param page page number of the page to swap into memory.
 */
void swap_page(struct page_table *pt, int page) {
	int frame_to_free = frame_select_algo(pt);
	int page_to_free = frame_states[frame_to_free].page;
	int bits_on_frame = frame_states[frame_to_free].bits;

	char *physmem = page_table_get_physmem(pt);

	// dirty virtual memory in frame?
	if (bits_on_frame == (PROT_READ | PROT_WRITE)) {
		disk_write(disk, page_to_free, &physmem[frame_to_free * PAGE_SIZE]);
#ifdef STATS
		disk_writes++;
#endif
	}

	disk_read(disk, page, &physmem[frame_to_free * PAGE_SIZE]);
	page_table_set_entry(pt, page, frame_to_free, PROT_READ);
	frame_states[frame_to_free].bits = PROT_READ;
	frame_states[frame_to_free].page = page;

	page_table_set_entry(pt, page_to_free, frame_to_free, PROT_NONE);

#ifdef STATS
	disk_reads++;
#endif
}

/**
 * Handles page faults in the virtual memory. Page faults occur when
 * a page in virtual memory is not loaded into a frame in physical
 * memory, or when the protection bits do not allow the attempted operation.
 * @param pt page table for the virtual memory.
 * @param page the page involved in the page fault.
 */
void page_fault_handler(struct page_table *pt, int page)
{
	int frame, bits;
	page_table_get_entry(pt, page, &frame, &bits);

	if (bits == PROT_READ) {
		// the entry exists in physical memory, but is read-only
		page_table_set_entry(pt, page, frame, bits | PROT_WRITE);
		frame_states[frame].bits = bits | PROT_WRITE;
	} else {
		int empty_frame_index = get_empty_frame(page_table_get_nframes(pt));
		if (empty_frame_index != -1) {
			// there is an empty frame for the entry
			page_table_set_entry(pt, page, empty_frame_index, PROT_READ);
			frame_states[empty_frame_index].page = page;
			frame_states[empty_frame_index].bits = PROT_READ;
		} else {
			// there were no empty frames, find one to release via frame_select_algo
			swap_page(pt, page);
		}
	}

#ifdef STATS
	npage_faults++;
#endif
}

int main(int argc, char *argv[])
{
	if(argc!=5) {
		printf("use: virtmem <npages> <nframes> <rand|fifo|custom> <sort|scan|focus>\n");
		return 1;
	}

	int npages = atoi(argv[1]);
	int nframes = atoi(argv[2]);
	frame_states = malloc(sizeof(struct frame_state) * nframes);
	const char *select_algo = argv[3];
	const char *program = argv[4];

	disk = disk_open("myvirtualdisk",npages);
	if(!disk) {
		fprintf(stderr, "couldn't create virtual disk: %s\n", strerror(errno));
		return 1;
	}

	struct page_table *pt = page_table_create(npages, nframes, page_fault_handler);
	if(!pt) {
		fprintf(stderr, "couldn't create page table: %s\n", strerror(errno));
		return 1;
	}

	char *virtmem = page_table_get_virtmem(pt);

	if (!strcmp(select_algo, "rand")) {
		frame_select_algo = &get_random_frame;
	} else if (!strcmp(select_algo, "fifo")) {
		frame_select_algo = &get_frame_from_queue;
	} else if (!strcmp(select_algo, "custom")) {
		frame_select_algo = &get_custom_frame;
	} else {
		fprintf(stderr, "unknown selection algorithm: %s\n", select_algo);
		exit(EXIT_FAILURE);
	}

	if(!strcmp(program, "sort")) {
		sort_program(virtmem, npages*PAGE_SIZE);

	} else if(!strcmp(program, "scan")) {
		scan_program(virtmem, npages*PAGE_SIZE);

	} else if(!strcmp(program, "focus")) {
		focus_program(virtmem, npages*PAGE_SIZE);

	} else {
		fprintf(stderr, "unknown program: %s\n", program);
		exit(EXIT_FAILURE);
	}

	page_table_delete(pt);
	disk_close(disk);
	free(frame_states);

#ifdef STATS
	print_stats();
#endif

	return 0;
}
