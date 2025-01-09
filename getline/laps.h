#ifndef laps_h
#define laps_h
#include <stdio.h>
// #include <stdlib.h>

/**
 * car - singly linked list of cars
 * @id: id of car
 * @lap: lap number
 * @next: points to the next car/node
 */
typedef struct car {
	int id;
	int lap;
	struct car *next;
} car;

void race_state(int *id, size_t size);

#endif
