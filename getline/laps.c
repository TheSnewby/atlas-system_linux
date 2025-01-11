#include "laps.h"
/**
 * add_new_vehicle - adds a new vehicle to ids and lap in linear order
 * @id: id of new vehicle
 * @current_size: current number of vehicles in ids list
 * @ids: pointer to array of ids
 * @lap: pointer to array of lap counters
 *
 * Return: void
 */
void add_new_vehicle(int id, int current_size, int *ids, int *lap)
{
	int k, l, inserted = 0;

	printf("Car %d joined the race\n", id);
	if (current_size)
	{  /* insert in linear order */
		for (k = 0; k < current_size; k++)
		{
			if (id < ids[k])
			{
				for (l = current_size; l > k; l--)
				{
					ids[l] = ids[l - 1];
					lap[l] = lap[l - 1];
				}
				ids[l] = id;
				lap[l] = 0;
				inserted = 1;
				break;
			}
		}
		if (!inserted) /* insert at end */
			ids[current_size] = id;
		inserted = 0;
	}
	else  /* insert when array is empty */
		ids[0] = id;
}

/**
 * race_state - keeps track of the number of laps made by cars in a race
 * The number of laps of each cars listed in id must be incremented by 1.
 * Print the state of the race.
 * @id: int representing identifier of each car, pointer to int array
 * @size: number of cars, size of the array
 *
 * Return: void
 */

void race_state(int *id, size_t size)
{
	static int ids[10] = {0};  /* list of ids observed by this function */
	static int lap[10] = {0};  /* corresponding number of laps */
	static int current_size;  /* current number of active ids */
	int i, j, id_found;

	if (size == 0)
		return;

	/* Search for each id in the list */
	for (i = 0; i < (int)size; i++)
	{
		id_found = 0;  /* not found */
		for (j = 0; j < current_size; j++)
		{
			if (id[i] == ids[j])
			{
				id_found = 1; /* found */
				lap[j]++; /* increment lap count */
				break;
			}
		}
		if (!id_found)
		{
			add_new_vehicle(id[i], current_size, ids, lap);
			current_size++;
		}
	}

	/* Prints the race state */
	printf("Race state:\n");
	for (j = 0; j < current_size; j++)
		printf("Car %d [%d laps]\n", ids[j], lap[j]);
}
