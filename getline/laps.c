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
	static int ids[10] = {0};  /* list of ids observed by this function */
	static int lap[10] = {0};  /* corresponding number of laps */
	static int ids_size = 10;  /* current size of the lists above */
	static int current_size = 0;  /* current number of active ids */
	int i, j, k, l, id_found;
	int inserted = 0;  /* flag for whether new number inserted before end */

    if (size == 0) {
        return;
    }

    /* Search for each id in the list */
    for (i = 0; i < (int)size; i++) {
        id_found = 0;  /* not found */
        for (j = 0; j < current_size; j++) {
            if (id[i] == ids[j]) {
                id_found = 1; /* found */
                lap[j]++; /* increment lap count */
                break;
            }
        }
		if (!id_found) {  /* add new vehicle to the list */
            printf("Car %d joined the race\n", id[i]);
            if (current_size < ids_size - 1) {  /* makes sure new car fits in memory */
                if (current_size) {  /* insert in linear order */
                    for (k = 0; k < current_size; k++) {
                        if (id[i] < ids[k]) {
                            for (l = current_size; l > k; l--) {
                                ids[l] = ids[l-1];
                                lap[l] = lap[l-1];
                            }
                            ids[l] = id[i];
                            lap[l] = 0;
                            inserted = 1;
                            break;
                        }
                    }
                    if (!inserted) {  /* insert at end */
                            ids[current_size] = id[i];
                    }
                }
                else {  /* insert when array is empty */
                    ids[0] = id[i];
                }
                current_size++;
                // ids[current_size] = id[i];
            }
            /* insert resize or new static array here for larger size*/
        }
	}

	/* Prints the race state */
	printf("Race state:\n");
	for (j = 0; j < current_size; j++) {
		printf("Car %d [%d laps]\n", ids[j], lap[j]);
	}
}
