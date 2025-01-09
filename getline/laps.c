#include "laps.h"
/**
 * race_state - keeps track of the number of laps made by several cars in a race
 * The number of laps of each cars listed in id must be incremented by 1.
 * Print the state of the race.
 * @id: int representing identifier of each car, pointer to int array
 * @size: number of cars, size of the array
 * 
 * Return: void
 */

void race_state(int *id, size_t size) {
	static int ids[10] = {0};
	static int lap[10] = {0};
	static int ids_size = 10;
	static int current_size = 0;
	int i, j, id_found;

	if (size == 0) {
		printf("free all");
		return;
	}

	/* Search for each id in the list */
	for (i = 0; i < (int)size; i++) {
		id_found = 0;  /* not found */
		for (j = 0; j < ids_size; j++) {
			if (id[i] == ids[j]) {
				id_found = 1; /* found */
			}
			if (id_found) {  /* increment lap count */
				lap[j]++;
				break;
			}
		}
		if (!id_found) {  /* add new vehicle to the list */
			printf("Car %d joined the race\n", id[i]);
			if (j < ids_size - 1) {  /* makes sure new car fits in memory */
				ids[current_size] = id[i];
				current_size++;
			}
			/* insert resize or new static array here for larger size*/
		}
	}

	/* Prints the race state */
	printf("Race state:\n");
	for (j = 0; j < current_size; j++) {
		printf("Car %d [%d laps]", ids[j], lap[j]);
	}
}

// car *add_car(car **head, int id, int lap) {

// }
