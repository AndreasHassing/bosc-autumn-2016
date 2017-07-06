#include <stdio.h>
#include <stdlib.h>

// Compares elements from a with elements in b, and returns
// true if a[i] <= b[i] for all i = 0..n-1.
int array_lte(int* a, int* b, size_t elements) {
	for (int i = 0; i < elements; i++) {
		if (a[i] > b[i]) return 0;
	}

	return 1;
}

// Prints an array with a format string.
void print_array(char *format, int *array, size_t elements) {
	for (int i = 0; i < elements; i++) {
		printf(format, array[i]);
	}
}

// Adds each element from array b onto each element in
// array a. Array a is modified as a side-effect.
void array_add(int* a, int* b, size_t elements) {
	for (int i = 0; i < elements; i++) {
		a[i] = a[i] + b[i];
	}
}

// Subtracts each element in array b from each element
// in array a. Array a is modified as a side-effect.
void array_subtract(int* a, int* b, size_t elements) {
	for (int i = 0; i < elements; i++) {
		a[i] = a[i] - b[i];
	}
}
